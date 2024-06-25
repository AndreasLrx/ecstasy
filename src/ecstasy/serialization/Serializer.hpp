///
/// @file Serializer.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2024-04-29
///
/// @copyright Copyright (c) ECSTASY 2024
///
///

#ifndef ECSTASY_SERIALIZATION_SERIALIZER_HPP_
#define ECSTASY_SERIALIZATION_SERIALIZER_HPP_

#include "ecstasy/resources/entity/RegistryEntity.hpp"
#include "ecstasy/serialization/ISerializer.hpp"
#include "ecstasy/serialization/concepts/has_extraction_operator.hpp"
#include "ecstasy/serialization/concepts/has_insertion_operator.hpp"
#include "ecstasy/serialization/traits/can_load_type.hpp"
#include "ecstasy/serialization/traits/can_save_type.hpp"
#include "ecstasy/serialization/traits/can_update_type.hpp"

namespace ecstasy::serialization
{

    ///
    /// @brief Serializer class.
    ///
    /// @tparam S Child serializer type. This is required to call the sub serializer class methods because template and
    /// virtual methods cannot be mixed.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-04-30)
    ///
    template <typename S>
    class Serializer : public ISerializer {
      protected:
        /// @brief Parent serializer type, used for inheriting classes.
        using Parent = Serializer<S>;

      public:
        ///
        /// @brief Check if a type is constructible from the serializer.
        ///
        /// @note The type must be constructible from a non const reference to the inner serializer.
        ///
        /// @tparam U Type to check.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-04-30)
        ///
        template <typename U>
        static constexpr bool is_constructible = std::is_constructible_v<U, std::add_lvalue_reference_t<S>>;

        ///
        /// @brief Construct a new Serializer.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-04-30)
        ///
        Serializer() = default;

        ///
        /// @brief Destroy the Serializer
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-06-11)
        ///
        ~Serializer() override = default;

        ///
        /// @brief Get a reference to the inner serializer.
        ///
        /// @return constexpr S& Reference to the inner serializer.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-04-30)
        ///
        constexpr S &inner()
        {
            return reinterpret_cast<S &>(*this);
        }

        ///
        /// @brief Save an object to the serializer.
        ///
        /// @tparam U Type of the object to save.
        ///
        /// @param[in] object Object to save.
        ///
        /// @return S& Reference to @b this for chain calls.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-04-30)
        ///
        template <typename U>
            requires concepts::has_extraction_operator<S, U> || traits::has_save_impl_for_type_v<S, U>
        S &save(const U &object)
        {
            if constexpr (traits::has_save_impl_for_type_v<S, U>)
                return inner().saveImpl(object);
            else
                return object >> inner();
        }

        ///
        /// @brief Save an entity component to the serializer. This includes the component type before the component
        /// data.
        ///
        /// @tparam C Component type.
        ///
        /// @param[in] component Component to save.
        ///
        /// @return S& Reference to @b this for chain calls.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-06-11)
        ///
        template <typename C>
        S &saveEntityComponent(const C &component)
        {
            return inner() << typeid(C) << component;
        }

        ///
        /// @brief Save an entity to the serializer with explicit components.
        ///
        /// @tparam Cs Components to save.
        ///
        /// @param[in] entity Entity to save.
        ///
        /// @return S& Reference to @b this for chain calls.
        ///
        /// @throws std::out_of_range If the entity does not have one of the components.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-06-11)
        ///
        template <typename... Cs>
        S &saveEntity(const RegistryEntity &entity)
        {
            S &s = inner();
            (saveEntityComponent(entity.get<Cs>()), ...);
            return s;
        }

        ///
        /// @brief Save an entity to the serializer.
        ///
        /// @warning This will try to save all the components of the entity, but not the entity ID.
        ///
        /// @param[in] entity Entity to save.
        ///
        /// @return S& Reference to @b this for chain calls.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-06-11)
        ///
        S &saveEntity(RegistryEntity &entity)
        {
            auto storages = entity.getRegistry().getEntityStorages(entity);

            for (auto &storage : storages) {
                // We send the typeid of the serializer before loosing the type information (since storage.serialize
                // takes an ISerializer)
                storage.get().save(*this, typeid(S), entity.getIndex());
            }
            return inner();
        }

        ///
        /// @brief Load an object from the serializer.
        ///
        /// @note This construct a new object and consume the associated data from the serializer.
        ///
        /// @tparam U Type of the object to load.
        ///
        /// @return U Loaded object.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-04-30)
        ///
        template <typename U>
            requires is_constructible<U> || (std::is_default_constructible_v<U> && traits::can_update_type_v<S, U>)
            || traits::has_load_impl_for_type_v<S, U>
        U load()
        {
            if constexpr (traits::has_load_impl_for_type_v<S, U>)
                return inner().template loadImpl<U>();
            else if constexpr (is_constructible<U>) {
                return U(inner());
            } else {
                U object;
                inner().update(object);
                return object;
            }
        }

        ///
        /// @brief Load an entity from the serializer.
        ///
        /// @param[in] registry Registry to load the entity into.
        ///
        /// @return RegistryEntity Loaded entity.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-06-25)
        ///
        RegistryEntity loadEntity(Registry &registry)
        {
            RegistryEntity entity(registry.entityBuilder().build(), registry);

            updateEntity(entity);
            return entity;
        }

        ///
        /// @brief Update an existing object from the serializer.
        ///
        /// @note If the object is fundamental, it will use the assignment operator, otherwise the << operator is
        /// expected to be implemented in U.
        ///
        /// @tparam U Type of the object to update.
        ///
        /// @param[in] object Existing object to update.
        ///
        /// @return S& Reference to @b this for chain calls.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-04-30)
        ///
        template <typename U>
            requires traits::has_update_impl_for_type_v<S, U> || std::is_fundamental_v<U>
            || concepts::has_insertion_operator<S, U>
        S &update(U &object)
        {
            if constexpr (traits::has_update_impl_for_type_v<S, U>)
                return inner().updateImpl(object);
            else if constexpr (std::is_fundamental_v<U>)
                object = inner().template load<U>();
            else
                object << inner();
            return inner();
        }

        ///
        /// @brief Update an entity component from the serializer.
        ///
        /// @param[in] entity Entity to update.
        ///
        /// @return S& Reference to @b this for chain calls.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-06-25)
        ///
        S &updateEntity(RegistryEntity &entity)
        {
            std::size_t component_hash = load<std::size_t>();
            auto &storages = entity.getRegistry().getStorages().getInner();

            for (const auto &pair : storages) {
                if (pair.second->getComponentTypeInfos().hash_code() == component_hash) {
                    pair.second->load(*this, typeid(S), entity.getIndex());
                }
            }
            return inner();
        }

        ///
        /// @brief Operator overload to simplify the save method.
        ///
        /// @tparam U Type of the object to save.
        ///
        /// @param[in] object Object to save.
        ///
        /// @return S& Reference to @b this for chain calls.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-04-30)
        ///
        template <typename U>
        S &operator<<(const U &object)
        {
            return inner().save(object);
        }

        ///
        /// @brief Operator overload to simplify the update method.
        ///
        /// @tparam U Type of the object to update.
        ///
        /// @param[in] object  Object to update.
        ///
        /// @return S& Reference to @b this for chain calls.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-04-30)
        ///
        template <typename U>
        S &operator>>(U &object)
        {
            return inner().update(object);
        }
    };
} // namespace ecstasy::serialization

#endif /* !ECSTASY_SERIALIZATION_SERIALIZER_HPP_ */
