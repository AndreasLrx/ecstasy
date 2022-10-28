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
    /// @internal
    /// @brief Performs a left outer join between two parameters packs.
    ///
    /// @tparam Rights @ref Traits containing the rights types.
    /// @tparam IsInner Whether @p Left is contained in the Lefts and in the Rights types.
    /// @tparam Left Evaluated type (from the Lefts types).
    /// @tparam OuterLefts Evaluated types which are in Lefts but not in Rights.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-28)
    ///
    template <typename Rights, bool IsInner, typename Left, typename... OuterLefts>
    struct _left_outer_join {
        using type = Traits<>;
    };

    /// @internal
    /// @brief Performs a left outer join between two parameters packs.
    ///
    /// @tparam Rights... @ref Rights types.
    /// @tparam IsInner Whether @p Left is contained in the Lefts and in the Rights types.
    /// @tparam Left Evaluated type (from the Lefts types).
    /// @tparam OuterLefts Evaluated types which are in Lefts but not in Rights.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-28)
    ///
    template <typename... Rights, bool IsInner, typename Left, typename... OuterLefts>
    struct _left_outer_join<Traits<Rights...>, IsInner, Left, OuterLefts...> {
        ///
        /// @brief Resulting type of the outer join.
        ///
        /// @tparam LeftsUntreated Empty parameters pack.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-28)
        ///
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

        ///
        /// @brief Resulting type of the outer join.
        ///
        /// @tparam NextLeft Next evaluated type from the Lefts types.
        /// @tparam LeftsUntreated Following types to be evaluated (from the Lefts types).
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-28)
        ///
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

    ///
    /// @brief Performs a left outer join on two parameter pack types (returns types in Lefts but no in Rights).
    ///
    /// @tparam typename Lefts types.
    /// @tparam typename Rights types.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-29)
    ///
    template <typename, typename>
    struct left_outer_join {
        using type = Traits<>;
    };

    ///
    /// @brief Performs a left outer join on two parameter pack types wrapped in @ref Traits types (returns types in
    /// Lefts but not in Rights).
    ///
    /// @tparam Left First left type.
    /// @tparam Lefts Other Lefts types.
    /// @tparam Rights Rights types.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-29)
    ///
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

    ///
    /// @brief Performs a left outer join on two parameter pack types wrapped in tuple types (return types in lefts but
    /// not in Rights).
    ///
    /// @tparam Lefts Left types.
    /// @tparam Rights Right types.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-29)
    ///
    template <typename... Lefts, typename... Rights>
    struct left_outer_join<std::tuple<Lefts...>, std::tuple<Rights...>> {
        using type = typename left_outer_join<Traits<Lefts...>, Traits<Rights...>>::type;
    };

    ///
    /// @brief Helper for left_outer_join<...>::type.
    ///
    /// @tparam Lefts @ref Traits or @ref std::tuple containing the left types.
    /// @tparam Rights @ref Traits or @ref std::tuple containing the right types.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-29)
    ///
    template <typename Lefts, typename Rights>
    using left_outer_join_t = left_outer_join<Lefts, Rights>::type;

    ///
    /// @brief Performs a right outer join on two parameter pack types wrapped in tuple types (return types in rights
    /// but not in lefts).
    ///
    /// @tparam Lefts Left types.
    /// @tparam Rights Right types.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-29)
    ///
    template <typename Lefts, typename Rights>
    using right_outer_join = left_outer_join<Rights, Lefts>;

    ///
    /// @brief Helper for right_outer_join<...>::type.
    ///
    /// @tparam Lefts @ref Traits or @ref std::tuple containing the left types.
    /// @tparam Rights @ref Traits or @ref std::tuple containing the right types.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-29)
    ///
    template <typename Lefts, typename Rights>
    using right_outer_join_t = right_outer_join<Lefts, Rights>::type;
} // namespace util::meta

#endif /* !UTIL_META_OUTER_JOIN_HPP_ */
