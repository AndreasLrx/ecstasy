///
/// @file outer_join.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-10-28
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef UTIL_META_OUTER_JOIN_HPP_
#define UTIL_META_OUTER_JOIN_HPP_

#include "Traits.hpp"

namespace util::meta
{
    template <typename Rights, bool IsInner, typename Left, typename... OuterLefts>
    struct _left_outer_join {
        using type = Traits<>;
    };

    template <typename... Rights, bool IsInner, typename Left, typename... OuterLefts>
    struct _left_outer_join<Traits<Rights...>, IsInner, Left, OuterLefts...> {
        template <typename... LeftsUntreated>
        struct result {
            // clang-format off
                using type = std::conditional_t<
                    IsInner,
                    /// Last left element is in inner part, skip it.
                    Traits<OuterLefts...>,
                    /// Last left element is in outer part, keep it.
                    Traits<OuterLefts..., Left>
                >;
            // clang-format on
        };

        /// @copydoc missings_tuple
        template <typename NextLeft, typename... LeftsUntreated>
        struct result<NextLeft, LeftsUntreated...> {
            // clang-format off
                using type = std::conditional_t<
                    IsInner, 
                    /// Left element is in inner part, skip it.
                    typename _left_outer_join<
                        Traits<Rights...>,                          // Rights
                        util::meta::contains<NextLeft, Rights...>,  // IsInner
                        NextLeft,                                   // Left
                        OuterLefts...                               // OuterLefts
                    >::template result<LeftsUntreated...>::type,
                    /// Left element is in outer part, keep it.
                    typename _left_outer_join<
                        Traits<Rights...>,                          // Rights
                        util::meta::contains<NextLeft, Rights...>,  // IsInner
                        NextLeft,                                   // Left
                        OuterLefts..., Left                         // OuterLefts
                    >::template result<LeftsUntreated...>::type
                >;
            // clang-format on
        };
    };

    template <typename, typename>
    struct left_outer_join {
        using type = Traits<>;
    };

    template <typename Left, typename... Lefts, typename... Rights>
    struct left_outer_join<Traits<Left, Lefts...>, Traits<Rights...>> {
        // clang-format off
        using type = 
            _left_outer_join<
                Traits<Rights...>,              // Rights
                contains<Left, Rights...>,      // IsInner
                Left                            // Left
            >::template result<Lefts...>::type;
        // clang-format on
    };

    template <typename... Lefts, typename... Rights>
    struct left_outer_join<std::tuple<Lefts...>, std::tuple<Rights...>> {
        using type = typename left_outer_join<Traits<Lefts...>, Traits<Rights...>>::type;
    };

    template <typename Lefts, typename Rights>
    using left_outer_join_t = left_outer_join<Lefts, Rights>::type;

    template <typename Lefts, typename Rights>
    using right_outer_join = left_outer_join<Rights, Lefts>;

    template <typename Lefts, typename Rights>
    using right_outer_join_t = right_outer_join<Lefts, Rights>::type;
} // namespace util::meta

#endif /* !UTIL_META_OUTER_JOIN_HPP_ */
