///
/// @file Serializer.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief Abstract serializer parent class.
/// @version 1.0.0
/// @date 2024-04-29
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
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

#include "ecstasy/rtti/TypeRegistry.hpp"

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
#define REGISTER_SERIALIZABLE(COMPONENT, SERIALIZER)                \
    static bool __CONCATENATE(registered_component_, __COUNTER__) = \
        reinterpret_cast<bool &>(SERIALIZER::registerComponent<COMPONENT>(#COMPONENT));

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
        [[nodiscard]] std::string exportBytes() const override
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
                auto optional_type = ecstasy::rtti::TypeRegistry::getInstance().find(storage.getComponentTypeInfos());

                if (optional_type.has_value()) {
                    auto serializer = optional_type->get().tryGetSerializer<S>();

                    if (serializer.has_value())
                        serializer->get().save(*this, storage, entity);
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
        [[nodiscard]] U load()
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
            else {
                if constexpr (std::is_fundamental_v<U>)
                    object = inner().template load<U>();
                else
                    object << inner();
                return inner();
            }
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
            OptionalEntityComponentSerializer component = loadComponentSerializer();

            while (component.has_value()) {
                rtti::AType &type = rtti::TypeRegistry::getInstance().get(component->get().getType());
                IStorage &storage = entity.getRegistry().getStorages().get(std::type_index(type.getStorageTypeInfo()));

                component->get().load(*this, storage, entity);

                component = loadComponentSerializer();
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
        /// @return IEntityComponentSerializer& Reference to the registered serializer.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-04)
        ///
        template <typename C>
        static IEntityComponentSerializer &registerComponent(std::string_view name)
        {
            rtti::AType &type = rtti::TypeRegistry::getInstance().registerType<C>(name);

            return dynamic_cast<rtti::Type<C> &>(type).template registerSerializer<S>();
        }

        ///
        /// @brief Get the registered components of the serializer.
        ///
        /// @tparam T Type of the component to get.
        ///
        /// @return bool True if the component is registered, false otherwise.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-25)
        ///
        template <typename T>
        [[nodiscard]] static bool hasEntityComponentSerializer()
        {
            auto atype = rtti::TypeRegistry::getInstance().find<T>();

            if (atype.has_value())
                return atype->get().template hasSerializer<S>();
            return false;
        }

      protected:
        using OptionalEntityComponentSerializer = std::optional<std::reference_wrapper<IEntityComponentSerializer>>;

        ///
        /// @brief Load the next component serializer from the stream.
        ///
        /// @note Return @ref std::nullopt if no more components are available.
        ///
        /// @return OptionalEntityComponentSerializer Optional reference to the next component serializer.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-04)
        ///
        [[nodiscard]] virtual OptionalEntityComponentSerializer loadComponentSerializer() = 0;

        ///
        /// @brief Optional method triggered at the start of @ref saveEntity.
        ///
        /// @param[in] entity Entity being saved.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-11)
        ///
        virtual void beforeSaveEntity([[maybe_unused]] RegistryEntity &entity)
        {
        }

        ///
        /// @brief Optional method triggered at the end of @ref saveEntity.
        ///
        /// @param[in] entity Entity being saved.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-11)
        ///
        virtual void afterSaveEntity([[maybe_unused]] RegistryEntity &entity)
        {
        }

        ///
        /// @brief Optional method triggered at the start of @ref updateEntity.
        ///
        /// @param[in] entity Entity being updated.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-11)
        ///
        virtual void beforeUpdateEntity([[maybe_unused]] RegistryEntity &entity)
        {
        }

        ///
        /// @brief Optional method triggered at the end of @ref updateEntity.
        ///
        /// @param[in] entity Entity being updated.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-11)
        ///
        virtual void afterUpdateEntity([[maybe_unused]] RegistryEntity &entity)
        {
        }
    };

} // namespace ecstasy::serialization

#endif /* !ECSTASY_SERIALIZATION_SERIALIZER_HPP_ */
