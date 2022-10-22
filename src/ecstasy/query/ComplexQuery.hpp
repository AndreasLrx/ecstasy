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

#include <stddef.h>
#include <tuple>

#include "Query.hpp"
#include "QueryConcepts.hpp"

namespace ecstasy
{

    template <Queryable... Selects>
    struct Select {
        using SelectedTuple = std::tuple<Selects &...>;

        template <bool ContainsPivot, typename T, Queryable Pivot, Queryable... Lefts>
        struct Isolate {
            constexpr static T value(Lefts &...lefts, Pivot &pivot);
        };

        template <typename T, Queryable Pivot, Queryable... Lefts>
        struct Isolate<true, T, Pivot, Lefts...> {
            constexpr static T value(Lefts &...lefts, Pivot &pivot)
            {
                return std::tie(std::forward<Lefts &>(lefts)..., pivot);
            }
        };

        template <typename T, Queryable Pivot, Queryable... Lefts>
        struct Isolate<false, T, Pivot, Lefts...> {
            constexpr static T value(Lefts &...lefts, Pivot &pivot)
            {
                (void)pivot;
                return std::tie(std::forward<Lefts &>(lefts)...);
            }
        };

        template <bool ContainsPivot, typename T, Queryable Pivot, Queryable... Lefts>
        struct IsolateStorages {
            constexpr static T value(Lefts &...lefts, Pivot &pivot);

            template <Queryable NextPivot>
            constexpr static T value(Lefts &...lefts, Pivot &pivot, NextPivot &nextPivot);

            template <Queryable NextPivot, Queryable... Rights>
            constexpr static T value(Lefts &...lefts, Pivot &pivot, NextPivot &nextPivot, Rights &...rights);
        };

        template <typename T, Queryable Pivot, Queryable... Lefts>
        struct IsolateStorages<true, T, Pivot, Lefts...> {
            template <Queryable NextPivot>
            constexpr static T value(Lefts &...lefts, Pivot &pivot, NextPivot &nextPivot)
            {
                return Isolate<contains<NextPivot, Selects...>(), T, NextPivot, Lefts..., Pivot>::value(
                    std::forward<Lefts &>(lefts)..., pivot, nextPivot);
            }

            template <Queryable NextPivot, Queryable... Rights>
            constexpr static T value(Lefts &...lefts, Pivot &pivot, NextPivot &nextPivot, Rights &...rights)
            {
                return IsolateStorages<contains<NextPivot, Selects...>(), T, NextPivot, Lefts..., Pivot>::value(
                    std::forward<Lefts &>(lefts)..., pivot, nextPivot, std::forward<Rights &>(rights)...);
            }
        };

        template <typename T, Queryable Pivot, Queryable... Lefts>
        struct IsolateStorages<false, T, Pivot, Lefts...> {
            template <Queryable NextPivot>
            constexpr static T value(Lefts &...lefts, Pivot &pivot, NextPivot &nextPivot)
            {
                (void)pivot;
                return Isolate<contains<NextPivot, Selects...>(), T, NextPivot, Lefts...>::value(
                    std::forward<Lefts &>(lefts)..., nextPivot);
            }

            template <Queryable NextPivot, Queryable... Rights>
            constexpr static T value(Lefts &...lefts, Pivot &pivot, NextPivot &nextPivot, Rights &...rights)
            {
                (void)pivot;
                return IsolateStorages<contains<NextPivot, Selects...>(), T, NextPivot, Lefts...>::value(
                    std::forward<Lefts &>(lefts)..., nextPivot, std::forward<Rights &>(rights)...);
            }
        };

        template <Queryable... Storages>
        constexpr static SelectedTuple isolateStorages(Storages &...storages)
        {
            return IsolateStorages<contains<std::tuple_element_t<0, std::tuple<Storages...>>, Selects...>(),
                SelectedTuple,
                std::tuple_element_t<0, std::tuple<Storages...>>>::value(std::forward<Storages &>(storages)...);
        }

        template <Queryable FirstWhere, Queryable... Wheres>
        static Query<Selects...> where(FirstWhere &firstWhere, Wheres &...wheres)
        {
            util::BitSet mask = (util::BitSet(firstWhere.getMask()) &= ... &= wheres.getMask());

            return Query<Selects...>(mask, isolateStorages(firstWhere, wheres...));
        }
    };

} // namespace ecstasy

#endif /* !ECSTASY_QUERY_COMPLEXQUERY_HPP_ */
