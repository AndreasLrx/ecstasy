///
/// @file EntityComponentSerializer.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2024-10-03
///
/// @copyright Copyright (c) ECSTASY 2024
///
///

#ifndef ECSTASY_COMPONENT_COMPONENT_SERIALIZER_HPP_
#define ECSTASY_COMPONENT_COMPONENT_SERIALIZER_HPP_

#include "ecstasy/serialization/IEntityComponentSerializer.hpp"

namespace ecstasy::serialization
{
    ///
    /// @brief Entity component serializer class bound to a specific component and a serializer type.
    ///
    /// The class instance doesn't contains any data except its vtable.
    ///
    /// @tparam Component Type of the component to serialize.
    /// @tparam Serializer Type of the serializer to use to serialize the component.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-10-04)
    ///
    template <typename Component, std::derived_from<ISerializer> Serializer>
    class EntityComponentSerializer : public IEntityComponentSerializer {
      public:
        /// Type of the storage used to store the component.
        using StorageType = getStorageType<Component>;

        ///
        /// @brief Construct a new Component Rtti
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-04)
        ///
        EntityComponentSerializer() : IEntityComponentSerializer()
        {
        }

        ///
        /// @brief Destroy the Component Rtti
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-04)
        ///
        ~EntityComponentSerializer() override = default;

        /// @copydoc IEntityComponentSerializer::save
        ISerializer &save(
            ISerializer &serializer, const IStorage &storage, const RegistryEntity &entity) const override final
        {
            return dynamic_cast<Serializer &>(serializer)
                .template saveEntityComponent<Component>(
                    dynamic_cast<const StorageType &>(storage).at(entity.getIndex()));
        }

        /// @copydoc IEntityComponentSerializer::load
        ISerializer &load(ISerializer &serializer, IStorage &storage, RegistryEntity &entity) const override final
        {
            if (!storage.contains(entity.getIndex())) {
                dynamic_cast<StorageType &>(storage).insert(
                    entity.getIndex(), dynamic_cast<Serializer &>(serializer).template load<Component>());
                return serializer;
            } else
                return dynamic_cast<Serializer &>(serializer)
                    .template update<Component>(dynamic_cast<StorageType &>(storage).at(entity.getIndex()));
        }

        /// @copydoc IEntityComponentSerializer::getStorageTypeIndex
        std::type_index getStorageTypeIndex() const override final
        {
            return std::type_index(typeid(StorageType));
        }
    };

} // namespace ecstasy::serialization

#endif /* !ECSTASY_COMPONENT_COMPONENT_SERIALIZER_HPP_ */
