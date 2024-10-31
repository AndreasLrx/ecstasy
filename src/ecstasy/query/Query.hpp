///
/// @file Query.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief Query components presents in all given queryables.
/// @version 1.0.0
/// @date 2024-10-17
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_QUERY_QUERY_HPP_
#define ECSTASY_QUERY_QUERY_HPP_

#include <algorithm>
#include <functional>
#include <thread>

#include "concepts/Condition.hpp"
#include "concepts/Queryable.hpp"
#include "concepts/QueryableNeedAdjust.hpp"
#include "conditions/Condition.hpp"
#include "util/BitSet.hpp"
#include "util/meta/Traits.hpp"
#include "util/meta/index.hpp"

namespace ecstasy::query
{
    class Entities;
    class IStorage;

    // clang-format off
    ///
    /// @brief Query components presents in all given queryables.
    ///
    /// @note See @ref ecstasy::query::modifier for advanced queries using queryable modifiers.
    ///
    /// @tparam First First storage class.
    /// @tparam Others All other storage classes.
    /// @tparam AutoLock Whether the query must be thread safe or not (locking any @ref ecstasy::thread::Lockable "Lockable" queryable).
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-20)
    ///
    // clang-format on
    template <typename Storages, typename Conditions = void, bool AutoLock = false>
    class QueryImplementation {};

    // clang-format off
    ///
    /// @brief @ref QueryImplementation specialization.
    ///
    /// @tparam First First queryable type.
    /// @tparam Others All other queryable types.
    /// @tparam Conditions All query conditions.
    /// @tparam AutoLock Whether the query must be thread safe or not (locking any @ref ecstasy::thread::Lockable "Lockable" queryable).
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-10-17)
    ///
    // clang-format on
    template <Queryable First, Queryable... Others, typename... Conditions, bool AutoLock>
    class QueryImplementation<util::meta::Traits<First, Others...>, util::meta::Traits<Conditions...>, AutoLock> {
      public:
        /// @brief QueryableObject constraint.
        using QueryData = std::tuple<queryable_data_t<First>, queryable_data_t<Others>...>;
        using Queryables =
            std::tuple<queryable_qualifiers_t<First, AutoLock>, queryable_qualifiers_t<Others, AutoLock>...>;

        ///
        /// @brief Query iterator.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-20)
        ///
        class Iterator {
          public:
            using iterator_category = std::input_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = QueryData;
            using pointer = value_type *;
            using reference = value_type &;

            ///////////////////////////////////////////////////////////////////////////////////////////////////////////
            // All iterators must be constructible, copy-constructible, copy-assignable, destructible and swappable.///
            ///////////////////////////////////////////////////////////////////////////////////////////////////////////

            ///
            /// @brief Construct a new uninitialized Iterator.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-10-20)
            ///
            explicit Iterator() noexcept
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
            explicit Iterator(util::BitSet const &mask, Queryables const &storages, std::size_t pos) noexcept(
                sizeof...(Conditions) == 0)
                : _mask(std::cref(mask)), _storages(std::cref(storages)), _pos(pos)
            {
                if constexpr (sizeof...(Conditions) != 0)
                    applyConditions();
            }

            ///
            /// @brief Default copy operator.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-10-20)
            ///
            Iterator(Iterator const &) noexcept = default;

            ///
            /// @brief Default assignment operator.
            ///
            /// @return Iterator& @b this.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-10-20)
            ///
            [[nodiscard]] Iterator &operator=(Iterator const &) noexcept = default;

            ///
            /// @brief Default move constructor.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-10-20)
            ///
            Iterator(Iterator &&) noexcept = default;

            ///
            /// @brief Default move assignment operator.
            ///
            /// @return Iterator& @b this.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-10-20)
            ///
            [[nodiscard]] Iterator &operator=(Iterator &&) noexcept = default;

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
            [[nodiscard]] constexpr bool operator==(Iterator const &other) const noexcept
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
            [[nodiscard]] constexpr bool operator!=(Iterator const &other) const noexcept
            {
                return this->_pos != other._pos;
            }

            ///
            /// @brief Fetch the components corresponding to the entity at the current position.
            ///
            /// @return @p value_type tuple containing component references.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-10-20)
            ///
            [[nodiscard]] value_type operator*() const
            {
                return this->get_components(std::make_index_sequence<(sizeof...(Others)) + 1>());
            }

            ///
            /// @brief Fetch the components corresponding to the entity at the current position.
            ///
            /// @return @p value_type tuple containing component references.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-10-20)
            ///
            [[nodiscard]] value_type operator->() const
            {
                return *this;
            }

            ///
            /// @brief Increments the iterator in place.
            ///
            /// @warning It is undefined behavior to increment the iterator past the end sentinel
            ///
            /// @return Iterator& @b this.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-10-20)
            ///
            Iterator &operator++()
            {
                this->_pos = this->_mask.get().firstSet(this->_pos + 1);
                if constexpr (sizeof...(Conditions) != 0)
                    applyConditions();
                return *this;
            }

            ///
            /// @brief Copies the iterator and increments the copy, please use pre-incrementation instead.
            ///
            /// @warning It is undefined behavior to increment the iterator past the end sentinel.
            /// @warning This creates a copy of the iterator!
            ///
            /// @return Iterator The incremented copy.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-10-20)
            ///
            [[nodiscard]] Iterator operator++(int)
            {
                Iterator result = *this;

                return ++result;
            }

            ///
            /// @brief Get the iterator Position in the query mask.
            ///
            /// @return size_t Iterator position.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-12-20)
            ///
            [[nodiscard]] constexpr size_t getPosition() const noexcept
            {
                return _pos;
            }

          private:
            /// @brief Mask of the query.
            std::reference_wrapper<const util::BitSet> _mask;
            /// @brief Storages of the query.
            std::reference_wrapper<const Queryables> _storages;
            /// @brief Current position in the mask.
            std::size_t _pos;

            ///
            /// @brief Evaluate a constant condition.
            ///
            /// @tparam Condition Condition to check.
            ///
            /// @return bool Whether the condition is valid.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2024-10-17)
            ///
            template <QConditionConst Condition>
            bool checkCondition() const
            {
                return Condition::test();
            }

            ///
            /// @brief Evaluate a condition with a left operand and a constant right operand.
            ///
            /// @tparam Condition Condition to check.
            ///
            /// @return bool Whether the condition is valid.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2024-10-17)
            ///
            template <QConditionLeft Condition>
            bool checkCondition() const
            {
                static_assert(
                    std::disjunction_v<
                        std::is_same<typename Condition::Left, std::remove_reference_t<queryable_data_t<Others>>>...,
                        std::is_same<typename Condition::Left, std::remove_reference_t<queryable_data_t<First>>>>,
                    "Condition left operand must be included in the queryable types.");
                auto &storage = std::get<
                    util::meta::index_v<typename Condition::Left, std::remove_reference_t<queryable_data_t<First>>,
                        std::remove_reference_t<queryable_data_t<Others>>...>>(_storages.get());

                return Condition::test(getQueryableData(storage, _pos));
            }

            ///
            /// @brief Evaluate a condition with a constant left operand and a right operand.
            ///
            /// @tparam Condition Condition to check.
            ///
            /// @return bool Whether the condition is valid.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2024-10-17)
            ///
            template <QConditionRight Condition>
            bool checkCondition() const
            {
                static_assert(
                    std::disjunction_v<
                        std::is_same<typename Condition::Right, std::remove_reference_t<queryable_data_t<Others>>>...,
                        std::is_same<typename Condition::Right, std::remove_reference_t<queryable_data_t<First>>>>,
                    "Condition right operand must be included in the queryable types.");
                auto &storage = std::get<
                    util::meta::index_v<typename Condition::Right, std::remove_reference_t<queryable_data_t<First>>,
                        std::remove_reference_t<queryable_data_t<Others>>...>>(_storages.get());

                return Condition::test(getQueryableData(storage, _pos));
            }

            ///
            /// @brief Evaluate a condition with two runtime operands.
            ///
            /// @tparam Condition Condition to check.
            ///
            /// @return bool Whether the condition is valid.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2024-10-17)
            ///
            template <QConditionLeftRight Condition>
            bool checkCondition() const
            {
                static_assert(
                    std::disjunction_v<
                        std::is_same<typename Condition::Left, std::remove_reference_t<queryable_data_t<Others>>>...,
                        std::is_same<typename Condition::Left, std::remove_reference_t<queryable_data_t<First>>>>,
                    "Condition left operand must be included in the queryable types.");
                static_assert(
                    std::disjunction_v<
                        std::is_same<typename Condition::Right, std::remove_reference_t<queryable_data_t<Others>>>...,
                        std::is_same<typename Condition::Right, std::remove_reference_t<queryable_data_t<First>>>>,
                    "Condition right operand must be included in the queryable types.");
                auto &storageLeft = std::get<
                    util::meta::index_v<typename Condition::Left, std::remove_reference_t<queryable_data_t<First>>,
                        std::remove_reference_t<queryable_data_t<Others>>...>>(_storages.get());
                auto &storageRight = std::get<
                    util::meta::index_v<typename Condition::Right, std::remove_reference_t<queryable_data_t<First>>,
                        std::remove_reference_t<queryable_data_t<Others>>...>>(_storages.get());

                return Condition::test(getQueryableData(storageLeft, _pos), getQueryableData(storageRight, _pos));
            }

            ///
            /// @brief Apply all conditions to the current entity.
            /// This function will increment the iterator until all conditions are met.
            ///
            /// @note This function is only called if conditions are present.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2024-10-17)
            ///
            inline void applyConditions()
            {
                if constexpr (sizeof...(Conditions) != 0) {
                    while (this->_pos != this->_mask.get().size() - 1 && !(true & ... & checkCondition<Conditions>()))
                        ++*this;
                }
            }

            ///
            /// @brief Get the components from the storages tuple.
            ///
            /// @tparam Indices Represent all the indices to fetch in the @p _storages attribute.
            ///
            /// @return @p value_type tuple containing component references.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-10-20)
            ///
            template <size_t... Indices>
            [[nodiscard]] value_type get_components(std::index_sequence<Indices...>) const
            {
                return {getQueryableData(std::get<Indices>(_storages.get()), _pos)...};
            }
        };

        ///
        /// @brief Construct a new Query from a bitmask already computed and a storages list.
        ///
        /// @param[in] mask mask matching a precomputed query.
        /// @param[in] storages storages containing requested data.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-22)
        ///
        QueryImplementation(util::BitSet &mask, const Queryables &storages) : _mask(mask), _storages(storages)
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
        QueryImplementation(First &first, Others &...others) : _storages(first, others...)
        {
            /// Adjusts the masks only if required
            if constexpr (is_queryable_with_adjust_v<First>
                || std::disjunction_v<is_queryable_with_adjust<Others>...>) {
                size_t maxSize = std::max({getQueryableMask(first).size(), getQueryableMask(others).size()...});

                adjustMask(first, maxSize);
                (adjustMask(others, maxSize), ...);
            }
            this->_mask = (util::BitSet(getQueryableMask(first)) &= ... &= getQueryableMask(others));

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
        [[nodiscard]] Iterator begin() const noexcept
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
        [[nodiscard]] Iterator end() const noexcept
        {
            return Iterator(this->_mask, this->_storages, this->_mask.size() - 1);
        }

        ///
        /// @brief Get the Query Mask. All bit set means an entity match the chained request.
        ///
        /// @note @ref ecstasy::query::QueryableObject constraint.
        ///
        /// @return const util::BitSet& Query BitMask.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-20)
        ///
        [[nodiscard]] constexpr const util::BitSet &getMask() const noexcept
        {
            return _mask;
        }

        ///
        /// @brief Query the components associated to the given entity.
        ///
        /// @note @ref ecstasy::query::QueryableObject constraint.
        ///
        /// @param[in] index Index of the entity.
        ///
        /// @return QueryData tuple containing component references.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-20)
        ///
        [[nodiscard]] QueryData getQueryData(std::size_t index) const
        {
            return get_components(index, std::make_index_sequence<(sizeof...(Others)) + 1>());
        }

        ///
        /// @brief Split the query in multiple batch of @p batchSize matching entities. Each batch is executed in a
        /// separate thread.
        ///
        /// @warning This doesn't ensure thread safety if you modify the same value from different entities (therefore
        /// different threads).
        /// @note Using batch query doesn't mean performance will be better, it depends on the @p batchSize and the
        /// complexity of the @p fct
        ///
        /// @tparam F Type of the function applied on each matching entity.
        ///
        /// @param[in] batchSize Max size of each batch.
        /// @param[in] fct Function applied on each matching entity.
        /// @param[in] wait Whether this method must be synchronous (wait all threads to terminate) or not.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-01-08)
        ///
        template <typename F>
        void splitThreads(std::size_t batchSize, F &&fct, bool wait = true)
        {
            Iterator end = this->end();
            Iterator current = this->begin();
            Iterator currentBatchStart = current;
            size_t currentBatchSize = 0;
            std::vector<std::thread> threads;

            while (current != end) {
                ++currentBatchSize;
                // Find the next element (which may be the end sentinel)
                ++current;
                // Batch full, we start a new thread and reset the current batch informations
                if (currentBatchSize == batchSize) {
                    threads.emplace_back(processBatch<F>, currentBatchStart, current, fct);
                    currentBatchSize = 0;
                    currentBatchStart = current;
                }
            }

            if (currentBatchSize > 0)
                threads.emplace_back(processBatch<F>, currentBatchStart, current, fct);

            if (wait)
                for (auto &thread : threads)
                    thread.join();
        }

      private:
        /// @brief Mask of the query.
        util::BitSet _mask;
        /// @brief Storages of the query.
        Queryables _storages;
        /// @brief Begin position of the query.
        size_t _begin;

        ///
        /// @brief Adjust the mask of a queryable to the given size.
        ///
        /// @tparam F Type of the function applied on each matching entity.
        ///
        /// @param[in] start Iterator to the first entity.
        /// @param[in] end Iterator to the last entity.
        /// @param[in] fct Function applied on each matching entity.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-17)
        ///
        template <typename F>
        static void processBatch(Iterator start, Iterator end, F &&fct) noexcept(noexcept(fct(*start)))
        {
            for (auto i = start; i != end; ++i)
                fct(*i);
        }

        ///
        /// @brief Get the components from the storages tuple.
        ///
        /// @tparam Indices Represent all the indices to fetch in the @p _storages attribute.
        ///
        /// @return @ref QueryData tuple containing component references.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-20)
        ///
        template <size_t... Indices>
        QueryData get_components(std::size_t index, std::index_sequence<Indices...>) const
        {
            return {getQueryableData(std::get<Indices>(_storages), index)...};
        }
    };

    ///
    /// @brief More high-level query class, wrapping the @ref QueryImplementation.
    ///
    /// @tparam First First queryable type.
    /// @tparam Others All other queryable types.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-10-17)
    ///
    template <Queryable First, Queryable... Others>
    class Query : public QueryImplementation<util::meta::Traits<First, Others...>, util::meta::Traits<>> {
      public:
        ///
        /// @brief Construct a new Query from a bitmask already computed and a storages list.
        ///
        /// @param[in] mask mask matching a precomputed query.
        /// @param[in] storages storages containing requested data.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-17)
        ///
        Query(util::BitSet &mask, const std::tuple<First &, Others &...> &storages)
            : QueryImplementation<util::meta::Traits<First, Others...>, util::meta::Traits<>>(mask, storages)
        {
        }

        ///
        /// @brief Construct a new Query from a list of storages. The mask is computed from the storages.
        ///
        /// @param[in] first First storage.
        /// @param[in] others All other storages.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-17)
        ///
        Query(First &first, Others &...others)
            : QueryImplementation<util::meta::Traits<First, Others...>, util::meta::Traits<>>(
                first, std::forward<Others &>(others)...)
        {
        }
    };

    ///
    /// @brief More high-level query class, wrapping the @ref QueryImplementation and locking all @ref
    /// ecstasy::thread::Lockable "Lockable" queryables.
    ///
    /// @tparam Qs All queryable types.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-10-17)
    ///
    template <Queryable... Qs>
    class ThreadSafeQuery : public QueryImplementation<util::meta::Traits<Qs...>, util::meta::Traits<>, true> {
      public:
        ///
        /// @brief Construct a new Thread Safe Query from a bitmask already computed and a storages list.
        ///
        /// @param[in] mask mask matching a precomputed query.
        /// @param[in] storages storages containing requested data.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-17)
        ///
        ThreadSafeQuery(util::BitSet &mask, const std::tuple<Qs &...> &storages)
            : QueryImplementation<util::meta::Traits<Qs...>, util::meta::Traits<>, true>(mask, storages)
        {
        }

        ///
        /// @brief Construct a new Thread Safe Query from a list of storages. The mask is computed from the storages.
        ///
        /// @param[in] others All other storages.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-17)
        ///
        ThreadSafeQuery(Qs &...others)
            : QueryImplementation<util::meta::Traits<Qs...>, util::meta::Traits<>, true>(std::forward<Qs &>(others)...)
        {
        }
    };

} // namespace ecstasy::query

#endif /* !QUERY_HPP_ */
