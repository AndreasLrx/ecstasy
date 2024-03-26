///
/// @file Modifier.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-22
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_QUERY_CONCEPTS_MODIFIER_HPP_
#define ECSTASY_QUERY_CONCEPTS_MODIFIER_HPP_

#include "Queryable.hpp"
#include "ecstasy/query/modifiers/Modifier.hpp"

namespace ecstasy::query
{
    ///
    /// @brief Defines a query modifier type.
    ///
    /// @tparam M Evaluated type.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-11-22)
    ///
    template <typename M>
    concept Modifier = requires(M &modifier) {
        /// Data stored in the queryable.
        requires Queryable<M>;

        /// Modifiers must inherit from the @ref ecstasy::query::modifier::Modifier .
        requires std::derived_from<M, ecstasy::query::modifier::Modifier>;

        /// Modifier operands
        typename M::Operands;
    };
} // namespace ecstasy::query

#endif /* !ECSTASY_QUERY_CONCEPTS_MODIFIER_HPP_ */
