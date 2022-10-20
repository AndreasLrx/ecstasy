/*
** EPITECH PROJECT, 2022
** ecstasy
** File description:
** Query
*/

#ifndef ECSTASY_QUERY_QUERY_HPP_
#define ECSTASY_QUERY_QUERY_HPP_

#include <functional>
#include "QueryConcepts.hpp"
#include "util/BitSet.hpp"

namespace ecstasy
{
    class Entities;
    class IStorage;

    template <Queryable First, Queryable... Others>
    class Query {
      public:
        class Iterator {
          public:
            using iterator_category = std::input_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = std::tuple<typename First::QueryData, typename Others::QueryData...>;
            using pointer = value_type *;
            using reference = value_type &;

            // All iterators must be constructible, copy-constructible, copy-assignable, destructible and swappable.
            explicit Iterator()
            {
            }

            explicit Iterator(
                util::BitSet const &mask, std::tuple<First &, Others &...> const &storages, std::size_t pos)
                : _mask(std::cref(mask)), _storages(std::cref(storages)), _pos(pos)
            {
            }

            Iterator(Iterator const &) = default;

            Iterator &operator=(Iterator const &) = default;

            Iterator(Iterator &&) = default;
            Iterator &operator=(Iterator &&other) = default;

            /// @note It is Undefined Behavior to compare two iterators that do not belong to the same query.
            ///
            /// @returns Whether the two iterators are equal.
            constexpr bool operator==(Iterator const &other) const
            {
                return this->_pos == other._pos;
            }

            /// @note It is Undefined Behavior to compare two iterators that do not belong to the same container.
            ///
            /// @returns Whether the two iterators are not equal.
            constexpr bool operator!=(Iterator const &other) const
            {
                return this->_pos != other._pos;
            }

            /// @returns The components and/or resources corresponding the entity at the current position.
            value_type operator*() const
            {
                return this->get_components(std::make_index_sequence<(sizeof...(Others)) + 1>());
            }

            /// @returns The components and/or resources corresponding the entity at the current position.
            value_type operator->() const
            {
                return *this;
            }

            /// Increments the iterator.
            ///
            /// @note It is Undefined Behavior to increment the iterator past the end sentinel.
            ///
            /// @returns A reference to this iterator.
            Iterator &operator++()
            {
                this->_pos = this->_mask.get().firstSet(this->_pos + 1);
                return *this;
            }

            /// Copies the iterator and increments the copy, please use pre-incrementation instead.
            ///
            /// @note It is Undefined Behavior to increment the iterator past the end sentinel.
            /// @note This creates a copy of the iterator!
            ///
            /// @returns The incremented copy.
            Iterator operator++(int)
            {
                Iterator result = *this;

                ++result->_pos;
                return result;
            }

          private:
            std::reference_wrapper<const util::BitSet> _mask;
            std::reference_wrapper<const std::tuple<First &, Others &...>> _storages;
            std::size_t _pos;

            template <size_t... Indices>
            value_type get_components(std::index_sequence<Indices...>) const
            {
                return {std::get<Indices>(_storages.get()).getQueryData(_pos)...};
            }
        };

        Query(First &first, Others &...others) : _storages(first, others...)
        {
            this->_mask = (util::BitSet(first.getMask()) &= ... &= others.getMask());

            // push a sentinel bit at the end.
            this->_mask.push(true);
            this->_begin = this->_mask.firstSet();
        }

        /// @returns A @ref std::forward_iterator to the first available entity, or a value that is equal to @ref end()
        /// if not found.
        Iterator begin() const noexcept
        {
            return Iterator(this->_mask, this->_storages, this->_begin);
        }

        /// @returns An iterator sentinel value, do not deference it.
        Iterator end() const noexcept
        {
            return Iterator(this->_mask, this->_storages, this->_mask.size() - 1);
        }

        ///
        /// @brief Get the Query Mask. All bit set means an entity match the chained request.
        ///
        /// @return const util::BitSet& Query BitMask.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-20)
        ///
        constexpr const util::BitSet &getMask() const
        {
            return _mask;
        }

      private:
        util::BitSet _mask;
        std::tuple<First &, Others &...> _storages;
        size_t _begin;
    };
} // namespace ecstasy

#endif /* !QUERY_HPP_ */
