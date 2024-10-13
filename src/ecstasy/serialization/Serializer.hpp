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

#include <fstream>
#include <sstream>
#include <unordered_map>

#include "ecstasy/resources/entity/RegistryEntity.hpp"
#include "ecstasy/serialization/EntityComponentSerializer.hpp"
#include "ecstasy/serialization/ISerializer.hpp"
#include "util/serialization/foreach.hpp"
#include "ecstasy/serialization/concepts/has_extraction_operator.hpp"
#include "ecstasy/serialization/concepts/has_insertion_operator.hpp"
#include "ecstasy/serialization/traits/can_load_type.hpp"
#include "ecstasy/serialization/traits/can_save_type.hpp"
#include "ecstasy/serialization/traits/can_update_type.hpp"

#define __CONCATENATE_DETAIL(x, y)      x##y
#define __CONCATENATE(x, y)             __CONCATENATE_DETAIL(x, y)
#define _REGISTER_SERIALIZABLES_AGAIN() _REGISTER_SERIALIZABLES_HELPER
#define _REGISTER_SERIALIZABLES_HELPER(COMPONENT, a1, ...) \
    REGISTER_SERIALIZABLE(COMPONENT, a1)                   \
    __VA_OPT__(_REGISTER_SERIALIZABLES_AGAIN PARENS(COMPONENT, __VA_ARGS__))

///
/// @brief Register a component to a serializer.
///
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @since 1.0.0 (2024-10-04)
///
#define REGISTER_SERIALIZABLE(COMPONENT, SERIALIZER)                                  \
    static bool __CONCATENATE(registered_, __CONCATENATE(COMPONENT, _##SERIALIZER)) = \
        SERIALIZER::registerComponent<COMPONENT>(#COMPONENT);

///
/// @brief Register a component to multiple serializers.
///
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @since 1.0.0 (2024-10-04)
///
#define REGISTER_SERIALIZABLES(COMPONENT, a1, ...) EXPAND(_REGISTER_SERIALIZABLES_HELPER(COMPONENT, a1, __VA_ARGS__))

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

        /// @copydoc ISerializer::importFile
        void importFile(const std::filesystem::path &filename) override
        {
            std::ifstream fstream(filename);

            importStream(fstream);
        }

        /// @copydoc ISerializer::importBytes
        void importBytes(const std::string &content) override
        {
            std::stringstream stream(content);

            importStream(stream);
        }

        /// @copydoc ISerializer::exportFile
        void exportFile(const std::filesystem::path &filename) const override
        {
            std::ofstream fstream(filename);

            exportStream(fstream);
        }

        /// @copydoc ISerializer::exportBytes
        std::string exportBytes() const override
        {
            std::stringstream stream;

            exportStream(stream);
            return stream.str();
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
        /// @brief Save all registered components of an entity to the serializer.
        ///
        /// @note See @ref registerComponent for registering components.
        ///
        /// @param[in] entity Entity to save.
        ///
        /// @return S& Reference to @b this for chain calls.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-04)
        ///
        S &saveEntity(RegistryEntity &entity)
        {
            beforeSaveEntity(entity);
            auto storages = entity.getRegistry().getEntityStorages(entity);

            for (IStorage &storage : storages) {
                std::size_t hash = storage.getComponentTypeInfos().hash_code();

                if (this->hasEntityComponentSerializer(hash)) {
                    this->getEntityComponentSerializer(hash).save(*this, storage, entity);
                }
            }
            afterSaveEntity(entity);
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
        /// @brief Load an entity component from the serializer.
        ///
        /// @param[in] registry Registry to load the component to.
        ///
        /// @return RegistryEntity Loaded entity.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-04)
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
        /// @brief Update all registered components of an entity from the serializer.
        ///
        /// @note Missing components are loaded, existing components are updated.
        ///
        /// @param[in] entity Entity to update.
        ///
        /// @return S& Reference to @b this for chain calls.
        ///
        /// @throws std::out_of_range If an entity component is not registered in the serializer.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-04)
        ///
        S &updateEntity(RegistryEntity &entity)
        {
            beforeUpdateEntity(entity);
            std::size_t component_hash = loadComponentHash();

            while (component_hash != 0) {
                if (this->hasEntityComponentSerializer(component_hash)) {
                    IEntityComponentSerializer &component = this->getEntityComponentSerializer(component_hash);
                    IStorage &storage =
                        entity.getRegistry().getStorages().get(std::type_index(component.getStorageTypeInfo()));

                    component.load(*this, storage, entity);
                } else {
                    throw std::out_of_range("Component with hash " + std::to_string(component_hash)
                        + " not registered. Use "
                          "registerComponent to register components.");
                }
                component_hash = loadComponentHash();
            }
            afterUpdateEntity(entity);
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

        ///
        /// @brief Register a component to this serializer type.
        ///
        /// This is required for calls to saveEntity calls without explicit component types.
        ///
        /// @tparam C Component type to register.
        ///
        /// @param[in] name Name of the component type.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-04)
        ///
        template <typename C>
        static bool registerComponent(std::string_view name)
        {
            size_t hash = typeid(C).hash_code();

            if (getRegisteredComponents().contains(hash))
                return false;
            getRegisteredComponents()[hash] = std::make_unique<EntityComponentSerializer<C, S>>(name);
            return true;
        }

        ///
        /// @brief Check if a component type is registered to this serializer.
        ///
        /// @param[in] hash Hash of the component type.
        ///
        /// @return bool True if the component type is registered, false otherwise.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-04)
        ///
        static bool hasEntityComponentSerializer(std::size_t hash)
        {
            return getRegisteredComponents().contains(hash);
        }

        ///
        /// @brief Get the Entity Component Serializer for a component type.
        ///
        /// @param[in] hash Hash of the component type.
        ///
        /// @return IEntityComponentSerializer& Reference to the entity component serializer.
        ///
        /// @throw std::out_of_range If the component type is not registered.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-04)
        ///
        static IEntityComponentSerializer &getEntityComponentSerializer(std::size_t hash)
        {
            return *getRegisteredComponents().at(hash);
        }

        ///
        /// @brief Try to get the Entity Component Serializer for a component type.
        ///
        /// @param[in] hash Hash of the component type.
        ///
        /// @return std::optional<std::reference_wrapper<IEntityComponentSerializer>> Optional reference to the entity
        /// component serializer.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-13)
        ///
        static std::optional<std::reference_wrapper<IEntityComponentSerializer>> tryGetEntityComponentSerializer(
            std::size_t hash)
        {
            auto it = getRegisteredComponents().find(hash);

            if (it != getRegisteredComponents().end())
                return std::ref(*it->second);
            return std::nullopt;
        }

        ///
        /// @brief Get the Entity Component Serializer from a component type name.
        ///
        /// @param[in] name Name of the component type.
        ///
        /// @return IEntityComponentSerializer& Reference to the entity component serializer.
        ///
        /// @throw std::out_of_range If the component type is not registered.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-11)
        ///
        static IEntityComponentSerializer &getEntityComponentSerializer(std::string_view name)
        {
            for (auto &[hash, serializer] : getRegisteredComponents()) {
                if (serializer->getTypeName() == name)
                    return *serializer;
            }
            throw std::out_of_range("Component with name " + std::string(name) + " not registered.");
        }

      protected:
        ///
        /// @brief Get a reference to the Registered Components map.
        ///
        /// @return std::unordered_map<std::size_t, std::unique_ptr<IEntityComponentSerializer>>& Reference to the
        /// registered components map.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-10)
        ///
        static std::unordered_map<std::size_t, std::unique_ptr<IEntityComponentSerializer>> &getRegisteredComponents()
        {
            static std::unordered_map<std::size_t, std::unique_ptr<IEntityComponentSerializer>> registeredComponents;
            return registeredComponents;
        }

        ///
        /// @brief Load the hash of the next component type from the stream.
        ///
        /// @note Return 0 if there is no more components to load.
        ///
        /// @return std::size_t Hash of the component type.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-04)
        ///
        virtual std::size_t loadComponentHash() = 0;

        ///
        /// @brief Optional method triggered at the start of @ref saveEntity.
        ///
        /// @param[in] entity Entity being saved.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-11)
        ///
        virtual void beforeSaveEntity(RegistryEntity &entity)
        {
            static_cast<void>(entity);
        }

        ///
        /// @brief Optional method triggered at the end of @ref saveEntity.
        ///
        /// @param[in] entity Entity being saved.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-11)
        ///
        virtual void afterSaveEntity(RegistryEntity &entity)
        {
            static_cast<void>(entity);
        }

        ///
        /// @brief Optional method triggered at the start of @ref updateEntity.
        ///
        /// @param[in] entity Entity being updated.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-11)
        ///
        virtual void beforeUpdateEntity(RegistryEntity &entity)
        {
            static_cast<void>(entity);
        }

        ///
        /// @brief Optional method triggered at the end of @ref updateEntity.
        ///
        /// @param[in] entity Entity being updated.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-11)
        ///
        virtual void afterUpdateEntity(RegistryEntity &entity)
        {
            static_cast<void>(entity);
        }
    };

} // namespace ecstasy::serialization

#endif /* !ECSTASY_SERIALIZATION_SERIALIZER_HPP_ */
