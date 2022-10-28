///
/// @file Select.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-10-21
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef ECSTASY_QUERY_SELECT_HPP_
#define ECSTASY_QUERY_SELECT_HPP_

#include <algorithm>
#include <stddef.h>
#include <tuple>

#include "Query.hpp"
#include "concepts/util.hpp"

namespace ecstasy::query
{
    ///
    /// @brief Select functions using templates at its maximum. Most of them are constexpr and needs a lot of
    /// compile time but are really fast at run time.
    ///
    /// @tparam SelectedQueryables Queryables to keep in the resulting query.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-22)
    ///
    template <Queryable... SelectedQueryables>
    struct Select {
      public:
        /// @brief Resulting selected @ref Queryables tuple
        using SelectedTuple = std::tuple<SelectedQueryables &...>;

      private:
        /// @internal
        /// @brief Test if the @ref Queryable type Q is in the @ref SelectedQueryables types.
        ///
        /// @tparam Q Evaluated type.
        ///
        /// @return constexpr bool Whether the type Q is in the @ref SelectedQueryables types.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-25)
        ///
        template <Queryable Q>
        constexpr static bool isQueryableSelected()
        {
            return contains<Q, SelectedQueryables...>();
        }

        /// @internal
        /// @brief Finalize the queryables selection.
        ///
        /// @tparam Qs Selected queryables types found in the where clause.
        ///
        /// @param[in] queryables selected queryables found in the where clause.
        ///
        /// @return constexpr SelectedTuple selected queryables tuple.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-28)
        ///
        template <Queryable... Qs>
        constexpr SelectedTuple tieQueryables(Qs... queryables)
        {
            static_assert(type_set_eq_v<std::tuple<SelectedQueryables...>, std::tuple<Qs...>>,
                "Missing queryables in where clause");
            static_assert(std::is_same_v<std::tuple<SelectedQueryables...>, std::tuple<Qs...>>,
                "Queryables have not the same order in the select and the where clauses");
            return std::tie(std::forward<Qs &>(queryables)...);
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
            /// @brief Primary template to finalize the queryables selection.
            ///
            /// @param[in] lefts Already accepted queryables.
            /// @param[in] pivot Last queryable, will be accepted if @p ContainsPivot is true.
            ///
            /// @return constexpr SelectedTuple selected queryables.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-10-22)
            ///
            constexpr static SelectedTuple value(Lefts &...lefts, Pivot &pivot);

            ///
            /// @brief Primary template recursively called until there is no @p nextPivot. It calls itself with:
            /// - @p ContainsPivot to to true if @p nextPivot is in the @ref SelectedQueryables types.
            /// - @p lefts to the current @p lefts with @p pivot (only if @p ContainsPivot is currently true).
            /// - @p pivot to current @p nextPivot.
            /// - @p nextPivot and @p rights are set or not depending on the queryable lefts in the current @p rights.
            /// If @p rights is empty, it will call the @ref value() without @p nextPivot value to finalize the result.
            ///
            /// @tparam NextPivot Next Pivot type.
            /// @tparam Rights Remaining @ref Queryable types.
            ///
            /// @param[in] lefts Already accepted queryables.
            /// @param[in] pivot Inspected queryable, will be accepted if @p ContainsPivot is true.
            /// @param[in] nextPivot Following queryable, will be use to set the recursive @p ContainsPivot value.
            /// @param[in] rights Remaining queryables, first one will become the @p nextPivot of the recursive call.
            ///
            /// @return constexpr SelectedTuple selected queryables.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-10-22)
            ///
            template <Queryable NextPivot, Queryable... Rights>
            constexpr static SelectedTuple value(
                Lefts &...lefts, Pivot &pivot, NextPivot &nextPivot, Rights &...rights);
        };

        /// @internal
        /// @brief Specialization of @ref FilterQueryables with @p ContainsPivot true, ie the pivot is selected in the
        /// resulting tuple.
        ///
        /// @tparam Pivot Current Queryable Type.
        /// @tparam Lefts All 'accepted' Queryables Types (they all belongs to @ref SelectedTuple types).
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-22)
        ///
        template <Queryable Pivot, Queryable... Lefts>
        struct FilterQueryables<true, Pivot, Lefts...> {
            ///
            /// @brief Test if the @ref Queryable type Q must be kept in the resulting queryables.
            ///
            /// @tparam Q Evaluated type.
            ///
            /// @return constexpr bool Whether the type Q is in the @ref SelectedQueryables types and not already
            /// selected (in the Lefts types).
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-10-25)
            ///
            template <Queryable Q>
            constexpr static bool isQueryableRequired()
            {
                return isQueryableSelected<Q>() && !contains<Q, Lefts...>();
            }

            constexpr static SelectedTuple value(Lefts &...lefts, Pivot &pivot)
            {
                return tieQueryables(std::forward<Lefts &>(lefts)..., pivot);
            }

            template <Queryable NextPivot, Queryable... Rights>
            constexpr static SelectedTuple value(Lefts &...lefts, Pivot &pivot, NextPivot &nextPivot, Rights &...rights)
            {
                return FilterQueryables<isQueryableRequired<NextPivot>(), NextPivot, Lefts..., Pivot>::value(
                    std::forward<Lefts &>(lefts)..., pivot, nextPivot, std::forward<Rights &>(rights)...);
            }
        };

        /// @internal
        /// @brief Specialization of @ref FilterQueryables with @p ContainsPivot false, ie the pivot is not selected in
        /// the resulting tuple.
        ///
        /// @tparam Pivot Current Queryable Type.
        /// @tparam Lefts All 'accepted' Queryables Types (they all belongs to @ref SelectedTuple types).
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-22)
        ///
        template <Queryable Pivot, Queryable... Lefts>
        struct FilterQueryables<false, Pivot, Lefts...> {
            ///
            /// @brief Test if the @ref Queryable type Q must be kept in the resulting queryables.
            ///
            /// @tparam Q Evaluated type.
            ///
            /// @return constexpr bool Whether the type Q is in the @ref SelectedQueryables types and not already
            /// selected (in the Lefts types).
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-10-25)
            ///
            template <Queryable Q>
            constexpr static bool isQueryableRequired()
            {
                return isQueryableSelected<Q>() && !contains<Q, Lefts...>();
            }

            constexpr static SelectedTuple value(Lefts &...lefts, Pivot &pivot)
            {
                (void)pivot;
                return tieQueryables(std::forward<Lefts &>(lefts)...);
            }

            template <Queryable NextPivot, Queryable... Rights>
            constexpr static SelectedTuple value(Lefts &...lefts, Pivot &pivot, NextPivot &nextPivot, Rights &...rights)
            {
                (void)pivot;
                return FilterQueryables<isQueryableSelected<NextPivot>(), NextPivot, Lefts...>::value(
                    std::forward<Lefts &>(lefts)..., nextPivot, std::forward<Rights &>(rights)...);
            }
        };

        ///
        /// @brief Isolate the given queryables to keep only the @ref Selected ones.
        ///
        /// @tparam Queryables given queryables types.
        ///
        /// @param[in] queryables given queryables.
        ///
        /// @return constexpr SelectedTuple selected queryables.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-22)
        ///
        template <Queryable... Queryables>
        constexpr static SelectedTuple filterQueryables(Queryables &...queryables)
        {
            return FilterQueryables<isQueryableSelected<first_type_t<Queryables...>>(),
                first_type_t<Queryables...>>::value(std::forward<Queryables &>(queryables)...);
        }

      public:
        ///
        /// @brief Execute a AND query with all the given Queryables but returns only the one in @ref Selected.
        ///
        /// @warning All queryables specified in @ref Selected @b must be passed as parameters in the same order
        /// (independing from the one non selected). For example: Selected<A, B, C> must have at least an instance of
        /// type A, B and C in the parameters in the same order: where<A, U, V, B, C, X, Z> (which leads to where<A, B,
        /// C> if we omit non selected queryables)
        ///
        /// @tparam FirstWhere First @ref Queryable type.
        /// @tparam Wheres Others @ref Queryable types.
        ///
        /// @param[in] firstWhere first @ref Queryable instance.
        /// @param[in] wheres others @ref Queryable instances.
        ///
        /// @return Query<SelectedQueryables...> Resulting query that can be iterated.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-22)
        ///
        template <Queryable FirstWhere, Queryable... Wheres>
        static Query<SelectedQueryables...> where(FirstWhere &firstWhere, Wheres &...wheres)
        {
            size_t maxSize = std::max({firstWhere.getMask().size(), wheres.getMask().size()...});

            adjustMask(firstWhere, maxSize);
            (adjustMask(wheres, maxSize), ...);

            util::BitSet mask = (util::BitSet(firstWhere.getMask()) &= ... &= wheres.getMask());

            return Query<SelectedQueryables...>(mask, filterQueryables(firstWhere, wheres...));
        }
    };

} // namespace ecstasy::query

#endif /* !ECSTASY_QUERY_SELECT_HPP_ */
