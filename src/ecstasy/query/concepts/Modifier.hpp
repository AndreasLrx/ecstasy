///
/// @file Modifier.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-22
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef ECSTASY_QUERY_CONCEPTS_MODIFIER_HPP_
#define ECSTASY_QUERY_CONCEPTS_MODIFIER_HPP_

#include "Queryable.hpp"

namespace ecstasy::query
{
    template <typename M>
    concept Modifier = requires(M &modifier)
    {
        /// Data stored in the queryable.
        requires Queryable<M>;

        typename M::Operands;
    };
} // namespace ecstasy::query

#endif /* !ECSTASY_QUERY_CONCEPTS_MODIFIER_HPP_ */
