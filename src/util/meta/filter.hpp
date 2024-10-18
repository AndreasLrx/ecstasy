///
/// @file filter.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief Filter types from a list.
/// @version 1.0.0
/// @date 2022-12-15
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef UTIL_META_FILTER_HPP_
#define UTIL_META_FILTER_HPP_

#include "Traits.hpp"

namespace util::meta
{
    /// @internal
    /// @brief Internal @ref filter struct.
    ///
    /// @note Resulting type is a @ref util::meta::Traits containing the types from @p Untreated for which
    /// Filter<T>::value was true.
    ///
    /// @tparam Treated @ref Must be a @ref util::meta::Traits of types already filtered.
    /// @tparam Filter Filter type, must contains a boolean value element (as in @ref std::true_type and @ref
    /// std::false_type).
    /// @tparam Untreated Types to filter.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-12-16)
    ///
    template <typename Treated, template <typename> typename Filter, typename... Untreated>
    struct _filter {
        using type = Traits<>;
    };

    /// @copydoc _filter
    /// @note End condition, no more types to filter.
    template <typename... Treated, template <typename> typename Filter>
    struct _filter<Traits<Treated...>, Filter> {
        using type = Traits<Treated...>;
    };

    /// @copydoc _filter
    /// @note Loop condition, treat type @p Current.
    template <typename... Treated, template <typename> typename Filter, typename Current, typename... Untreated>
    struct _filter<Traits<Treated...>, Filter, Current, Untreated...> {
        using type = typename _filter<
            std::conditional_t<Filter<Current>::value, Traits<Treated..., Current>, Traits<Treated...>>, Filter,
            Untreated...>::type;
    };

    ///
    /// @brief Filter the types in @p Ts keeping only those for which Filter<T>::value was true.
    ///
    /// @note Resulting type is a @ref util::meta::Traits containing the types from @p Untreated for which
    /// Filter<T>::value was true.
    ///
    /// @tparam Filter Filter type, must contains a boolean value element (as in @ref std::true_type and @ref
    /// std::false_type).
    /// @tparam Ts Types to filter
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-12-16)
    ///
    template <template <typename> typename Filter, typename... Ts>
    struct filter {
        using type = typename _filter<Traits<>, Filter, Ts...>::type;
    };

    ///
    /// @brief Helper for filter<...>::type.
    ///
    /// @tparam Filter filter.
    /// @tparam Ts Types to filter.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-29)
    ///
    template <template <typename> typename Filter, typename... Ts>
    using filter_t = typename filter<Filter, Ts...>::type;

} // namespace util::meta

#endif /* !UTIL_META_FILTER_HPP_ */
