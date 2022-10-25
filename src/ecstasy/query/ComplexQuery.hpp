///
/// @file ComplexQuery.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-10-21
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef ECSTASY_QUERY_COMPLEXQUERY_HPP_
#define ECSTASY_QUERY_COMPLEXQUERY_HPP_

#include <algorithm>
#include <stddef.h>
#include <tuple>

#include "Query.hpp"
#include "QueryConcepts.hpp"

namespace ecstasy
{
    ///
    /// @brief Complex query functions using templates at its maximum. Most of them are constexpr and needs a lot of
    /// compile time but are really fast at run time.
    ///
    /// @tparam Selects Queryables to keep in the resulting query.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-22)
    ///
    template <Queryable... Selects>
    struct Select {
      public:
        /// @brief Resulting selected @ref Queryables tuple
        using SelectedTuple = std::tuple<Selects &...>;

      private:
        /// @internal
        /// @brief Primary template to isolate required storages (the one in Selects) from the queried ones (where).
        ///
        /// @tparam ContainsPivot Whether or not the current Queryable (called pivot) must be selected.
        /// @tparam Pivot Current Queryable Type.
        /// @tparam Lefts All 'accepted' Queryables Types (they all belongs to @ref SelectedTuple types).
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-22)
        ///
        template <bool ContainsPivot, Queryable Pivot, Queryable... Lefts>
        struct IsolateStorages {
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
            /// - @p ContainsPivot to to true if @p nextPivot is in the @ref Selects types.
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
        /// @brief Specialization of @ref IsolateStorage with @p ContainsPivot true, ie the pivot is selected in the
        /// resulting tuple.
        ///
        /// @tparam Pivot Current Queryable Type.
        /// @tparam Lefts All 'accepted' Queryables Types (they all belongs to @ref SelectedTuple types).
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-22)
        ///
        template <Queryable Pivot, Queryable... Lefts>
        struct IsolateStorages<true, Pivot, Lefts...> {
            constexpr static SelectedTuple value(Lefts &...lefts, Pivot &pivot)
            {
                return std::tie(std::forward<Lefts &>(lefts)..., pivot);
            }

            template <Queryable NextPivot, Queryable... Rights>
            constexpr static SelectedTuple value(Lefts &...lefts, Pivot &pivot, NextPivot &nextPivot, Rights &...rights)
            {
                return IsolateStorages<contains<NextPivot, Selects...>(), NextPivot, Lefts..., Pivot>::value(
                    std::forward<Lefts &>(lefts)..., pivot, nextPivot, std::forward<Rights &>(rights)...);
            }
        };

        /// @internal
        /// @brief Specialization of @ref IsolateStorage with @p ContainsPivot false, ie the pivot is not selected in
        /// the resulting tuple.
        ///
        /// @tparam Pivot Current Queryable Type.
        /// @tparam Lefts All 'accepted' Queryables Types (they all belongs to @ref SelectedTuple types).
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-22)
        ///
        template <Queryable Pivot, Queryable... Lefts>
        struct IsolateStorages<false, Pivot, Lefts...> {
            constexpr static SelectedTuple value(Lefts &...lefts, Pivot &pivot)
            {
                (void)pivot;
                return std::tie(std::forward<Lefts &>(lefts)...);
            }

            template <Queryable NextPivot, Queryable... Rights>
            constexpr static SelectedTuple value(Lefts &...lefts, Pivot &pivot, NextPivot &nextPivot, Rights &...rights)
            {
                (void)pivot;
                return IsolateStorages<contains<NextPivot, Selects...>(), NextPivot, Lefts...>::value(
                    std::forward<Lefts &>(lefts)..., nextPivot, std::forward<Rights &>(rights)...);
            }
        };

        ///
        /// @brief Isolate the given queryables to keep only the @ref Selected ones.
        ///
        /// @tparam Storages given queryables types.
        ///
        /// @param[in] storages given queryables.
        ///
        /// @return constexpr SelectedTuple selected queryables.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-22)
        ///
        template <Queryable... Storages>
        constexpr static SelectedTuple isolateStorages(Storages &...storages)
        {
            return IsolateStorages<contains<first_type_t<Storages...>, Selects...>(), first_type_t<Storages...>>::value(
                std::forward<Storages &>(storages)...);
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
        /// @return Query<Selects...> Resulting query that can be iterated.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-22)
        ///
        template <Queryable FirstWhere, Queryable... Wheres>
        static Query<Selects...> where(FirstWhere &firstWhere, Wheres &...wheres)
        {
            size_t maxSize = std::max({firstWhere.getMask().size(), wheres.getMask().size()...});

            adjustMask(firstWhere, maxSize);
            (adjustMask(wheres, maxSize), ...);

            util::BitSet mask = (util::BitSet(firstWhere.getMask()) &= ... &= wheres.getMask());

            return Query<Selects...>(mask, isolateStorages(firstWhere, wheres...));
        }
    };

} // namespace ecstasy

#endif /* !ECSTASY_QUERY_COMPLEXQUERY_HPP_ */
