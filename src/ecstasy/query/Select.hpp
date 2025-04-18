///
/// @file Select.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief Advanced @ref ecstasy::query::Query "Query". It allows to select which data must be kept from a query
/// request.
/// @version 1.0.0
/// @date 2022-10-21
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_QUERY_SELECT_HPP_
#define ECSTASY_QUERY_SELECT_HPP_

#include <algorithm>
#include <stddef.h>
#include <tuple>

#include "Query.hpp"
#include "util/meta/Traits.hpp"
#include "util/meta/contains.hpp"
#include "util/meta/type_set_eq.hpp"

namespace ecstasy::query
{
    ///
    /// @brief Advanced @ref ecstasy::query::Query "Query". It allows to select which data must be kept from a query
    /// request.
    ///
    /// @note Cannot do AutoLock at this scope because of variable lifetimes (see where method).  For thread safety,
    /// queryables lock must be done before calling the where method.
    ///
    /// @tparam SelectedQueryables Queryables to keep in the resulting query.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-22)
    ///
    template <Queryable... SelectedQueryables>
    struct Select {
      public:
        /// @brief Resulting selected @ref ecstasy::query::Queryable "Queryable" tuple
        using SelectedTuple = std::tuple<SelectedQueryables &...>;

      private:
        /// @internal
        /// @brief Test if the @ref Queryable type Q is in the @ref SelectedQueryables types.
        ///
        /// @tparam Q Evaluated type.
        ///
        /// @return bool Whether the type Q is in the @ref SelectedQueryables types.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-25)
        ///
        template <Queryable Q>
        [[nodiscard]] constexpr static bool isQueryableSelected() noexcept
        {
            return util::meta::contains<Q, SelectedQueryables...>;
        }

        /// @internal
        /// @brief Sort the queryable following the @ref SelectedTuple types order and tie them.
        ///
        /// @tparam Valids Selected queryable types.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-28)
        ///
        template <typename... Valids>
        struct SorteredTie {
            ///
            /// @brief Final condition: all queryables are in the good order so tie them and return the tuple.
            ///
            /// @param[in] valids valid sorted queryables.
            ///
            /// @return SelectedTuple selected queryables.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-10-28)
            ///
            [[nodiscard]] static constexpr SelectedTuple sort(Valids &...valids) noexcept
            {
                return std::tie(std::forward<Valids &>(valids)...);
            }

            ///
            /// @brief Recursively sort the queryables.
            ///
            /// @tparam Q Current evaluated queryable type.
            /// @tparam Qs Queryable types to sort after.
            ///
            /// @param[in] valids already sorted queryables.
            /// @param[in] current evaluated queryable.
            /// @param[in] lefts queryable to sort after @p current.
            ///
            /// @return SelectedTuple selected queryables.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-10-28)
            ///
            template <Queryable Q, Queryable... Qs>
            [[nodiscard]] static constexpr SelectedTuple sort(Valids &...valids, Q &current, Qs &...lefts) noexcept
            {
                if constexpr (std::is_same_v<
                                  typename util::meta::Traits<SelectedQueryables...>::template Nth<sizeof...(Valids)>,
                                  Q>)
                    return SorteredTie<Valids..., Q>::sort(
                        std::forward<Valids &>(valids)..., current, std::forward<Qs &>(lefts)...);
                else
                    return SorteredTie<Valids...>::sort(
                        std::forward<Valids &>(valids)..., std::forward<Qs &>(lefts)..., current);
            }
        };

        /// @internal
        /// @brief Finalize the queryables selection.
        ///
        /// @tparam Qs Selected queryables types found in the where clause.
        ///
        /// @param[in] queryables selected queryables found in the where clause.
        ///
        /// @return SelectedTuple selected queryables tuple.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-28)
        ///
        template <Queryable... Qs>
        [[nodiscard]] constexpr static SelectedTuple tieQueryables(Qs &...queryables) noexcept
        {
            static_assert(util::meta::type_set_eq_v<std::tuple<SelectedQueryables...>, std::tuple<Qs...>>,
                "Missing queryables in where clause");
            if constexpr (std::is_same_v<std::tuple<SelectedQueryables...>, std::tuple<Qs...>>)
                return std::tie(std::forward<Qs &>(queryables)...);
            else
                return SorteredTie<>::template sort(std::forward<Qs &>(queryables)...);
        }

        /// @internal
        /// @brief Primary template to filter the queryables keeping only the one in SelectedQueryables.
        ///
        /// @tparam ContainsPivot Whether or not the current Queryable (called pivot) must be selected.
        /// @tparam Pivot Current Queryable Type.
        /// @tparam Lefts All 'accepted' Queryables Types (they all belongs to @ref SelectedTuple types).
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-22)
        ///
        template <bool ContainsPivot, Queryable Pivot, Queryable... Lefts>
        struct FilterQueryables {
            ///
            /// @brief Test if the @ref ecstasy::query::Queryable "Queryable" type Q must be kept in the resulting
            /// queryables.
            ///
            /// @tparam Q Evaluated type.
            ///
            /// @return bool Whether the type Q is in the @p SelectedQueryables types and not already
            /// selected (in the Lefts types).
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-10-25)
            ///
            template <Queryable Q>
            [[nodiscard]] constexpr static bool isQueryableRequired() noexcept
            {
                return isQueryableSelected<Q>() && !util::meta::contains<Q, Lefts...>;
            }

            ///
            /// @brief Primary template to finalize the queryables selection.
            ///
            /// @param[in] lefts Already accepted queryables.
            /// @param[in] pivot Last queryable, will be accepted if @p ContainsPivot is true.
            ///
            /// @return SelectedTuple selected queryables.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-10-22)
            ///
            [[nodiscard]] constexpr static SelectedTuple value(Lefts &...lefts, Pivot &pivot) noexcept
            {
                if constexpr (ContainsPivot)
                    return tieQueryables(std::forward<Lefts &>(lefts)..., pivot);
                else {
                    (void)pivot;
                    return tieQueryables(std::forward<Lefts &>(lefts)...);
                }
            }

            ///
            /// @brief Primary template recursively called until there is no @p nextPivot. It calls itself with:
            /// - @p ContainsPivot to to true if @p nextPivot is in the @p SelectedQueryables types.
            /// - @p lefts to the current @p lefts with @p pivot (only if @p ContainsPivot is currently true).
            /// - @p pivot to current @p nextPivot.
            /// - @p nextPivot and @p rights are set or not depending on the queryable lefts in the current @p rights.
            /// If @p rights is empty, it will call the @ref value() without @p nextPivot value to finalize the result.
            ///
            /// @tparam NextPivot Next Pivot type.
            /// @tparam Rights Remaining @ref ecstasy::query::Queryable "Queryable" types.
            ///
            /// @param[in] lefts Already accepted queryables.
            /// @param[in] pivot Inspected queryable, will be accepted if @p ContainsPivot is true.
            /// @param[in] nextPivot Following queryable, will be use to set the recursive @p ContainsPivot value.
            /// @param[in] rights Remaining queryables, first one will become the @p nextPivot of the recursive call.
            ///
            /// @return SelectedTuple selected queryables.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-10-22)
            ///
            template <Queryable NextPivot, Queryable... Rights>
            [[nodiscard]] constexpr static SelectedTuple value(
                Lefts &...lefts, Pivot &pivot, NextPivot &nextPivot, Rights &...rights) noexcept
            {
                if constexpr (ContainsPivot)
                    return FilterQueryables<isQueryableRequired<NextPivot>(), NextPivot, Lefts..., Pivot>::value(
                        std::forward<Lefts &>(lefts)..., pivot, nextPivot, std::forward<Rights &>(rights)...);
                else {
                    (void)pivot;
                    return FilterQueryables<isQueryableSelected<NextPivot>(), NextPivot, Lefts...>::value(
                        std::forward<Lefts &>(lefts)..., nextPivot, std::forward<Rights &>(rights)...);
                }
            }
        };

        ///
        /// @brief Isolate the given queryables to keep only the @p SelectedQueryables ones.
        ///
        /// @tparam Queryables given queryables types.
        ///
        /// @param[in] queryables given queryables.
        ///
        /// @return SelectedTuple selected queryables.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-22)
        ///
        template <Queryable... Queryables>
        [[nodiscard]] constexpr static SelectedTuple filterQueryables(Queryables &...queryables) noexcept
        {
            return FilterQueryables<isQueryableSelected<typename util::meta::Traits<Queryables...>::First>(),
                typename util::meta::Traits<Queryables...>::First>::value(std::forward<Queryables &>(queryables)...);
        }

      public:
        ///
        /// @brief Execute a AND query with all the given Queryables but returns only the one in @p SelectedQueryables.
        ///
        /// @warning All queryables specified in @p SelectedQueryables @b must be passed as parameters.
        ///
        /// @tparam Conditions @ref util::meta::Traits of multiple @ref ecstasy::query::Condition "Condition".
        /// @tparam FirstWhere First @ref ecstasy::query::Queryable "Queryable" type.
        /// @tparam Wheres Others @ref ecstasy::query::Queryable "Queryable" types.
        ///
        /// @param[in] firstWhere first @ref ecstasy::query::Queryable "Queryable" instance.
        /// @param[in] wheres others @ref ecstasy::query::Queryable "Queryable" instances.
        ///
        /// @return QueryImplementation<util::meta::Traits<SelectedQueryables...>, Conditions> Resulting query that can
        /// be iterated.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-22)
        ///
        template <typename Conditions = util::meta::Traits<>, Queryable FirstWhere, Queryable... Wheres>
        static QueryImplementation<util::meta::Traits<SelectedQueryables...>, Conditions> where(
            FirstWhere &firstWhere, Wheres &...wheres)
        {
            /// Adjusts the masks only if required
            if constexpr (is_queryable_with_adjust_v<FirstWhere>
                || std::disjunction_v<is_queryable_with_adjust<Wheres>...>) {
                size_t maxSize = std::max({getQueryableMask(firstWhere).size(), getQueryableMask(wheres).size()...});

                adjustMask(firstWhere, maxSize);
                (adjustMask(wheres, maxSize), ...);
            }

            util::BitSet mask = (util::BitSet(getQueryableMask(firstWhere)) &= ... &= getQueryableMask(wheres));

            return QueryImplementation<util::meta::Traits<SelectedQueryables...>, Conditions>(
                mask, filterQueryables(firstWhere, wheres...));
        }
    };

} // namespace ecstasy::query

#endif /* !ECSTASY_QUERY_SELECT_HPP_ */
