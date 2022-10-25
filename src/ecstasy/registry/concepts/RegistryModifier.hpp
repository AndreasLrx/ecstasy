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
#include "ecstasy/query/modifiers/ModifiersList.hpp"

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

    ///
    /// @brief Apply a unary modifier to a queryable, this version does nothing but returning the queryable unchanged.
    ///
    /// @tparam C Component type.
    /// @tparam Q Queryable type.
    ///
    /// @param[in] queryable queryable value.
    /// @param[in] allocator allocator for modifiers (not used there).
    ///
    /// @return Q& @p queryable.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-24)
    ///
    template <typename C, query::Queryable Q>
    Q &applyUnaryModifier(Q &queryable, query::modifier::ModifiersList &allocator)
    {
        (void)allocator;
        return queryable;
    }

    ///
    /// @brief Returns @p queryable wrapped in the given modifier allocated in the @p allocator.
    ///
    /// @tparam M Modifier Type.
    /// @tparam Q Wrapped queryable type.
    ///
    /// @param[in] queryable queryable value.
    /// @param[in] allocator Allocator for the newly created modifier.
    ///
    /// @return M::Modifier& newly created modifier.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-24)
    ///
    template <RegistryModifier M, query::Queryable Q>
    M::Modifier &applyUnaryModifier(Q &queryable, query::modifier::ModifiersList &allocator)
    {
        return allocator.instanciateModifier<typename M::Modifier>(queryable);
    }

} // namespace ecstasy

#endif /* !ECSTASY_REGISTRY_CONCEPTS_REGISTRYMODIFIER_HPP_ */
