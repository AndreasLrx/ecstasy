///
/// @file apply.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-12-15
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef UTIL_META_APPLY_HPP_
#define UTIL_META_APPLY_HPP_

#include "Traits.hpp"

namespace util::meta
{
    ///
    /// @brief Apply a modification on all types in @p Ts.
    ///
    /// @note Result is a structure with a @b type alias defined to a @ref util::meta::Traits containing the modified
    /// types.
    ///
    /// @tparam Functor Templated type to apply on @p Ts types. (Ex: @ref std::reference_wrapper, or @ref
    /// std::remove_reference)
    /// @tparam Ts Types to modify.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-12-16)
    ///
    template <template <typename> typename Functor, typename... Ts>
    struct apply {
        using type = Traits<Functor<Ts>...>;
    };

    /// @copydoc apply
    template <template <typename> typename Functor, typename... Ts>
    struct apply<Functor, Traits<Ts...>> {
        using type = Traits<Functor<Ts>...>;
    };

    ///
    /// @brief Helper for apply<...>::type.
    ///
    /// @tparam Functor functor.
    /// @tparam Ts Types to modify.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-29)
    ///
    template <template <typename> typename Functor, typename... Ts>
    using apply_t = typename apply<Functor, Ts...>::type;
} // namespace util::meta

#endif /* !UTIL_META_APPLY_HPP_ */
