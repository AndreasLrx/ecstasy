///
/// @file IEntityComponentSerializer.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief File containing the IEntityComponentSerializer class, used to serialize entity components from an IStorage
/// instance.
/// @version 1.0.0
/// @date 2024-10-03
///
/// @copyright Copyright (c) ECSTASY 2024
///
///

#ifndef ECSTASY_SERIALIZATION_ICOMPONESERIALIZERTTI_HPP_
#define ECSTASY_SERIALIZATION_ICOMPONESERIALIZERTTI_HPP_

#include <functional>
#include <typeindex>

namespace ecstasy
{
    class IStorage;
    class RegistryEntity;

    namespace serialization
    {
        class ISerializer;

        ///
        /// @brief Type erased interface for serializing entity components.
        ///
        /// This interface is used to serialize entity components from IStorage/ISerializer instances when we don't know
        /// the type of the component/serializer.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-04)
        ///
        class IEntityComponentSerializer {
          public:
            ///
            /// @brief Destroy the IEntityComponentSerializer
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2024-10-04)
            ///
            virtual ~IEntityComponentSerializer() noexcept = default;

            ///
            /// @brief Save the component to the serializer.
            ///
            /// @param[in] serializer Reference to the serializer to save the component to.
            /// @param[in] storage Const reference to the storage containing the component.
            /// @param[in] entity Const reference to the entity associated with the component.
            ///
            /// @return ISerializer& Reference to the serializer for chaining.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2024-10-04)
            ///
            virtual ISerializer &save(
                ISerializer &serializer, const IStorage &storage, const RegistryEntity &entity) const = 0;

            ///
            /// @brief Load an entity component from the serializer.
            ///
            /// @param[in] serializer Reference to the serializer to load the component from.
            /// @param[in] storage Reference to the storage to load the component to.
            /// @param[in] entity Reference to the entity to load/update the component to.
            ///
            /// @return ISerializer& Reference to the serializer for chaining.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2024-10-04)
            ///
            virtual ISerializer &load(ISerializer &serializer, IStorage &storage, RegistryEntity &entity) const = 0;

            ///
            /// @brief Get the type info of the component storage.
            ///
            /// @return const std::type_info& Type info of the component storage.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2024-10-04)
            ///
            [[nodiscard]] virtual const std::type_info &getStorageTypeInfo() const noexcept = 0;

            ///
            /// @brief Get the type info of the component.
            ///
            /// @return const std::type_info& Type info of the component.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2024-10-11)
            ///
            [[nodiscard]] virtual const std::type_info &getComponentTypeInfo() const noexcept = 0;

            ////
            /// @brief Get the Component Type Name
            ///
            /// @note Return the explicit name of the component if any and fallback on the type name.
            ///
            /// @return std::string_view Name of the component type.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2024-10-11)
            ///
            [[nodiscard]] virtual std::string_view getTypeName() const noexcept = 0;

          private:
            /// @brief Name of the component type.
            std::string_view _name;
        };

    } // namespace serialization
} // namespace ecstasy

#endif /* !ECSTASY_SERIALIZATION_ICOMPONESERIALIZERTTI_HPP_ */
