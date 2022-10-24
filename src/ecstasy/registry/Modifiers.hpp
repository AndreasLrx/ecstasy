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

#ifndef ECSTASY_REGISTRY_MODIFIERS_HPP_
#define ECSTASY_REGISTRY_MODIFIERS_HPP_

#include "ecstasy/query/ModifiersList.hpp"
#include "ecstasy/query/NotModifier.hpp"
#include "ecstasy/query/QueryConcepts.hpp"
#include "ecstasy/storage/IStorage.hpp"
#include "ecstasy/storage/StorageConcepts.hpp"

namespace util
{
    class BitSet;
}

namespace ecstasy
{
    class ModifiersList;

    ///
    /// @brief Placeholder for @ref Not query modifier.
    ///
    /// @tparam C Component type.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-24)
    ///
    template <typename C>
    struct RegistryNot {
        using QueryData = C;
        using Modifier = Not<getStorageType<C>>;
    };

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

    ///
    /// @brief Primary get the component type of the given type. For a component it return the component itself, for a
    /// @ref QueryDataHolder it returns the internal @b QueryData type.
    ///
    /// @tparam T Evaluated type.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-24)
    ///
    template <typename T>
    struct component_type;

    /// @copydoc component_type
    template <typename T>
    struct component_type {
        using type = T;
    };

    /// @copydoc component_type
    template <QueryDataHolder Q>
    struct component_type<Q> {
        using type = typename Q::QueryData;
    };

    ///
    /// @brief Helper for component_type<T>::type.
    ///
    /// @tparam T Evaluated type.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-24)
    ///
    template <typename T>
    using component_type_t = typename component_type<T>::type;

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
        Queryable<typename M::Modifier>;
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
    template <typename C, Queryable Q>
    Q &applyUnaryModifier(Q &queryable, ModifiersList &allocator)
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
    template <RegistryModifier M, Queryable Q>
    M::Modifier &applyUnaryModifier(Q &queryable, ModifiersList &allocator)
    {
        return allocator.instanciateModifier<typename M::Modifier>(queryable);
    }

} // namespace ecstasy

#endif /* !MODIFIERS_HPP_ */
