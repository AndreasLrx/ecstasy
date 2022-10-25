///
/// @file QueryDataHolder.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-10-25
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef ECSTASY_QUERY_CONCEPTS_QUERYDATAHOLDER_HPP_
#define ECSTASY_QUERY_CONCEPTS_QUERYDATAHOLDER_HPP_

#include <concepts>

namespace ecstasy::query
{
    ///
    /// @brief Concept matching all type defining a @b QueryData type.
    ///
    /// @tparam T Type of the element evaluated.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-24)
    ///
    template <typename T>
    concept QueryDataHolder = requires()
    {
        typename T::QueryData;
    };
} // namespace ecstasy::query

#endif /* !ECSTASY_QUERY_CONCEPTS_QUERYDATAHOLDER_HPP_ */
