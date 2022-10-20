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

    class Query {
      public:
        ///
        /// @brief
        ///
        /// @tparam First
        /// @tparam Others
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-20)
        ///
        template <Queryable First, Queryable... Others>
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

            Iterator(Iterator<First, Others...> const &) = default;

            Iterator<First, Others...> &operator=(Iterator<First, Others...> const &) = default;

            Iterator(Iterator<First, Others...> &&) = default;
            Iterator<First, Others...> &operator=(Iterator<First, Others...> &&other) = default;

            /// @note It is Undefined Behavior to compare two iterators that do not belong to the same query.
            ///
            /// @returns Whether the two iterators are equal.
            constexpr bool operator==(Iterator<First, Others...> const &other) const
            {
                return this->_pos == other._pos;
            }

            /// @note It is Undefined Behavior to compare two iterators that do not belong to the same container.
            ///
            /// @returns Whether the two iterators are not equal.
            constexpr bool operator!=(Iterator<First, Others...> const &other) const
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
            Iterator<First, Others...> &operator++()
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
            Iterator<First, Others...> operator++(int)
            {
                Iterator<First, Others...> result = *this;

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

        ///
        /// @brief Construct a new Query initialized with the entities alive.
        ///
        /// @param[in] entities Entities manager.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-20)
        ///
        Query(Entities &entities);

        ///
        /// @brief Perform a AND request using the given component storage.
        ///
        /// @param[in] storage Component storage.
        ///
        /// @return Query& @b this.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-20)
        ///
        Query &where(IStorage &storage);

        template <Queryable First, Queryable... Others>
        Iterator<First, Others...> fetch(First &first, Others &...others)
        {
            util::BitSet mask = _mask;

            // push a sentinel bit at the end.
            mask.push(true);
            return Iterator<First, Others...>(
                mask, std::tuple<First &, Others &...>(first, others...), mask.firstSet());
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
    };
} // namespace ecstasy

#endif /* !QUERY_HPP_ */
