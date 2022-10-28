///
/// @file util.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-10-25
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef ECSTASY_QUERY_CONCEPTS_UTIL_HPP_
#define ECSTASY_QUERY_CONCEPTS_UTIL_HPP_

#include <concepts>
#include <cstddef>

#include "util/meta/contains.hpp"

namespace ecstasy::query
{

    ///
    /// @brief Fetch the type of a parameter pack types from its index.
    ///
    /// @note The @b type using is the @p Index'th types in @p Types.
    ///
    /// @tparam Index Index of the type to fetch.
    /// @tparam Types Types to search in.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-25)
    ///
    template <size_t Index, typename... Types>
    struct type_at_index {
        using type = std::tuple_element_t<Index, std::tuple<Types...>>;
    };

    ///
    /// @brief Fetch the type of a parameter pack types from its index.
    ///
    /// @note The result is the @p Index'th types in @p Types.
    ///
    /// @tparam Index Index of the type to fetch.
    /// @tparam Types Types to search in.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-25)
    ///
    template <size_t Index, typename... Types>
    using type_at_index_t = typename type_at_index<Index, Types...>::type;

    ///
    /// @brief Extract the first type from @p Types.
    ///
    /// @tparam Types Types to search in.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-25)
    ///
    template <typename... Types>
    using first_type_t = type_at_index_t<0, Types...>;

} // namespace ecstasy::query

#endif /* !ECSTASY_QUERY_CONCEPTS_UTIL_HPP_ */
