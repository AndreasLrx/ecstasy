/*
** EPITECH PROJECT, 2022
** ecstasy
** File description:
** Query
*/

#ifndef ECSTASY_QUERY_QUERY_HPP_
#define ECSTASY_QUERY_QUERY_HPP_

#include <algorithm>
#include <functional>

#include "concepts/Queryable.hpp"
#include "concepts/QueryableNeedAdjust.hpp"
#include "util/BitSet.hpp"

namespace ecstasy::query
{
    class Entities;
    class IStorage;

    ///
    /// @brief Query components from multiple storage following logical rules.
    ///
    /// @note At the moment, it only supports and clauses (CompA && CompB). It will have to be reworked to support
    /// logical conditions.
    ///
    /// @tparam First First storage class.
    /// @tparam Others All other storage classes.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-20)
    ///
    template <Queryable First, Queryable... Others>
    class Query {
      public:
        ///
        /// @brief Query Iterator.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-20)
        ///
        class Iterator {
          public:
            using iterator_category = std::input_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = std::tuple<typename First::QueryData, typename Others::QueryData...>;
            using pointer = value_type *;
            using reference = value_type &;

            ///////////////////////////////////////////////////////////////////////////////////////////////////////////
            // All iterators must be constructible, copy-constructible, copy-assignable, destructible and swappable.///
            ///////////////////////////////////////////////////////////////////////////////////////////////////////////

            ///
            /// @brief Construct a new uniitialized Iterator.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-10-20)
            ///
            explicit Iterator()
            {
            }

            ///
            /// @brief Construct a new Iterator.
            ///
            /// @param[in] mask query mask, all bit set to true correspond to an entity matching the requirements.
            /// @param[in] storages components storages, used to retrieve iterator content.
            /// @param[in] pos Current position, must be on a bit set to true in the @p mask.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-10-20)
            ///
            explicit Iterator(
                util::BitSet const &mask, std::tuple<First &, Others &...> const &storages, std::size_t pos)
                : _mask(std::cref(mask)), _storages(std::cref(storages)), _pos(pos)
            {
            }

            ///
            /// @brief Default copy operator.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-10-20)
            ///
            Iterator(Iterator const &) = default;

            ///
            /// @brief Default assignment operator.
            ///
            /// @return Iterator& @b this.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-10-20)
            ///
            Iterator &operator=(Iterator const &) = default;

            ///
            /// @brief Default move constructor.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-10-20)
            ///
            Iterator(Iterator &&) = default;

            ///
            /// @brief Default move assignment operator.
            ///
            /// @return Iterator& @b this.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-10-20)
            ///
            Iterator &operator=(Iterator &&) = default;

            ///
            /// @brief Compare two iterators from the same @ref Query.
            ///
            /// @warning It is undefined behavior to compare two iterators that do not belong to the same query.
            ///
            /// @param[in] other iterator to compare.
            ///
            /// @return bool Whether the two iterators are equals.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-10-20)
            ///
            constexpr bool operator==(Iterator const &other) const
            {
                return this->_pos == other._pos;
            }

            ///
            /// @brief Compare two iterators from the same @ref Query.
            ///
            /// @warning It is undefined behavior to compare two iterators that do not belong to the same query.
            ///
            /// @param[in] other iterator to compare.
            ///
            /// @return bool Whether the two iterators are different.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-10-20)
            ///
            constexpr bool operator!=(Iterator const &other) const
            {
                return this->_pos != other._pos;
            }

            ///
            /// @brief Fetch the components corresponding to the entity at the current position.
            ///
            /// @return value_type tuple containing component references.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-10-20)
            ///
            value_type operator*() const
            {
                return this->get_components(std::make_index_sequence<(sizeof...(Others)) + 1>());
            }

            ///
            /// @brief Fetch the components corresponding to the entity at the current position.
            ///
            /// @return value_type tuple containing component references.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-10-20)
            ///
            value_type operator->() const
            {
                return *this;
            }

            ///
            /// @brief Increments the iterator in place.
            ///
            /// @warning It is undefined behavior to increment the iterator past the end sentinel ( @ref Query::end() ).
            ///
            /// @return Iterator& @b this.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-10-20)
            ///
            Iterator &operator++()
            {
                this->_pos = this->_mask.get().firstSet(this->_pos + 1);
                return *this;
            }

            ///
            /// @brief Copies the iterator and increments the copy, please use pre-incrementation instead.
            ///
            /// @warning It is undefined behavior to increment the iterator past the end sentinel ( @ref Query::end() ).
            /// @warning This creates a copy of the iterator!
            ///
            /// @return Iterator The incremented copy.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-10-20)
            ///
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

            ///
            /// @brief Get the components from the storages tuple.
            ///
            /// @tparam Indices Represent all the indices to fetch in the @p _storages attribute.
            ///
            /// @return value_type tuple containing component references.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-10-20)
            ///
            template <size_t... Indices>
            value_type get_components(std::index_sequence<Indices...>) const
            {
                return {std::get<Indices>(_storages.get()).getQueryData(_pos)...};
            }
        };

        ///
        /// @brief Construct a new Query from a bitmask already computed and a storages list.
        ///
        /// @param[in] mask mask matching a predefined query.
        /// @param[in] storages storages containing requested data.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-22)
        ///
        Query(util::BitSet &mask, const std::tuple<First &, Others &...> &storages) : _mask(mask), _storages(storages)
        {
            // push a sentinel bit at the end.
            this->_mask.push(true);
            this->_begin = this->_mask.firstSet();
        }

        ///
        /// @brief Construct a new Query trying to match multiple storages on one entity.
        ///
        /// @param[in] first First storage.
        /// @param[in] others All other storages.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-20)
        ///
        Query(First &first, Others &...others) : _storages(first, others...)
        {
            /// Adjusts the masks only if required
            if constexpr (is_queryable_with_adjust_v<
                              First> || std::disjunction_v<is_queryable_with_adjust<Others>...>) {
                size_t maxSize = std::max({first.getMask().size(), others.getMask().size()...});

                adjustMask(first, maxSize);
                (adjustMask(others, maxSize), ...);
            }
            this->_mask = (util::BitSet(first.getMask()) &= ... &= others.getMask());

            // push a sentinel bit at the end.
            this->_mask.push(true);
            this->_begin = this->_mask.firstSet();
        }

        ///
        /// @brief Get the start operator of the valid entities.
        ///
        /// @return Iterator A @ref std::forward_iterator to the first available entity, or a value that is equal to
        /// @ref end() if not found.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-20)
        ///
        Iterator begin() const noexcept
        {
            return Iterator(this->_mask, this->_storages, this->_begin);
        }

        ///
        /// @brief Get the end operator of the valid entities.
        ///
        /// @return Iterator An iterator sentinel value, do not deference it.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-20)
        ///
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
} // namespace ecstasy::query

#endif /* !QUERY_HPP_ */
