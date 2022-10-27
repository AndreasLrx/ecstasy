///
/// @file Modifiers.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-10-24
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef ECSTASY_REGISTRY_CONCEPTS_REGISTRYMODIFIER_HPP_
#define ECSTASY_REGISTRY_CONCEPTS_REGISTRYMODIFIER_HPP_

#include "ecstasy/query/concepts/Queryable.hpp"
#include "ecstasy/query/modifiers/Modifier.hpp"
#include "util/Allocator.hpp"

namespace ecstasy
{
    ///
    /// @brief Registry Modifier concept, which are placeholders for the actual query modifiers.
    ///
    /// @tparam M Registry Modifier Type.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-24)
    ///
    template <typename M>
    concept RegistryModifier = requires()
    {
        typename M::QueryData;
        typename M::Modifier;
        typename M::Modifier::Internal;
        query::Queryable<typename M::Modifier>;
    };

} // namespace ecstasy

#endif /* !ECSTASY_REGISTRY_CONCEPTS_REGISTRYMODIFIER_HPP_ */
