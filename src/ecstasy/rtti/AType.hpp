///
/// @file AType.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief File containing the AType interface for cross-platform type information.
/// @version 1.0.0
/// @date 2024-10-24
///
/// @copyright Copyright (c) ECSTASY 2024
///
///

#ifndef ECSTASY_RTTI_ATYPE_HPP_
#define ECSTASY_RTTI_ATYPE_HPP_

#include <optional>
#include <typeindex>
#include <string_view>
#include <type_traits>
#include <unordered_map>

#include "ecstasy/serialization/ISerializer.hpp"

namespace ecstasy
{
    namespace serialization
    {
        class IEntityComponentSerializer;
    }

    namespace rtti
    {
        ///
        /// @brief Type erased interface for cross-platform type information.
        ///
        /// The cross-platform hash is computed by hashing the name string, which must be the same on all
        /// platforms.
        ///
        /// @note The associated type will be referred as @b T in the documentation.
        ///
        /// @warning If you don't set an explicit name, it will fallback on the type name, which is @b not guaranteed to
        /// be cross-platform.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-24)
        ///
        class AType {
          public:
            ///
            /// @brief Destroy the AType instance.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2024-10-24)
            ///
            virtual ~AType() noexcept = default;

            ///
            /// @brief Get the type info of @b T storage type, as find by @ref getStorageType<T>().
            ///
            /// @return const std::type_info& Type info of @b T storage type.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2024-10-24)
            ///
            [[nodiscard]] virtual const std::type_info &getStorageTypeInfo() const noexcept = 0;

            ///
            /// @brief Get the type info of @b T.
            ///
            /// @return const std::type_info& Type info of @b T.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2024-10-24)
            ///
            [[nodiscard]] virtual const std::type_info &getTypeInfo() const noexcept = 0;

            ///
            /// @brief Get the cross-platform hash of the type name.
            ///
            /// @note Expected to be the same on all platforms.
            ///
            /// @return size_t Cross-platform hash of the type name.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2024-10-24)
            ///
            [[nodiscard]] virtual size_t getHash() const noexcept = 0;

            ///
            /// @brief Get the name of @b T.
            ///
            /// @note Expected to be the same on all platforms.
            ///
            /// @return std::string_view Name of @b T.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2024-10-24)
            ///
            [[nodiscard]] virtual std::string_view getTypeName() const noexcept = 0;

            ///
            /// @brief Compare two AType instances by their hash.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2024-10-24)
            ///
            [[nodiscard]] std::strong_ordering operator<=>(const AType &rhs) const noexcept;

            ///
            /// @brief Compare if two AType instances are equal by their hash.
            ///
            /// @note This is required because <=> is not enough for unordered containers 🤡.
            ///
            /// @param[in] rhs AType instance to compare with.
            ///
            /// @return bool True if the AType instances are equal, false otherwise.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2024-10-24)
            ///
            [[nodiscard]] bool operator==(const AType &rhs) const noexcept;

            ///
            /// @brief Compare if an AType instance is matching a type info.
            ///
            /// @param[in] rhs Type info to compare with.
            ///
            /// @return bool True if the AType instance is matching the type info, false otherwise.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2024-10-24)
            ///
            [[nodiscard]] bool operator==(const std::type_info &rhs) const noexcept;

            ///
            /// @brief Compare if an AType instance is matching a type name.
            ///
            /// @param[in] rhs Type name to compare with.
            ///
            /// @return bool True if the AType instance is matching the type name, false otherwise.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2024-10-24)
            ///
            [[nodiscard]] bool operator==(const std::string_view &rhs) const noexcept;

            ///
            /// @brief Check if a serializer is registered for the component type.
            ///
            /// @tparam Serializer Type of the serializer to check.
            ///
            /// @return bool Whether the serializer is registered or not.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2024-10-31)
            ///
            template <std::derived_from<ecstasy::serialization::ISerializer> Serializer>
            [[nodiscard]] bool hasSerializer() const noexcept
            {
                return _serializers.contains(std::type_index(typeid(Serializer)));
            }

            ///
            /// @brief Get a reference to the entity component serializer for the given serializer type.
            ///
            /// @tparam Serializer Type of the serializer to use.
            ///
            /// @return ecstasy::serialization::IEntityComponentSerializer& Reference to the serializer.
            ///
            /// @throw std::out_of_range If the serializer is not registered.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2024-10-31)
            ///
            template <std::derived_from<ecstasy::serialization::ISerializer> Serializer>
            [[nodiscard]] ecstasy::serialization::IEntityComponentSerializer &getSerializer() const noexcept
            {
                return *_serializers.at(std::type_index(typeid(Serializer)));
            }

            ///
            /// @brief Try to get a reference to the entity component serializer for the given serializer type.
            ///
            /// @tparam Serializer Type of the serializer to use.
            ///
            /// @return std::optional<std::reference_wrapper<ecstasy::serialization::IEntityComponentSerializer>>
            /// Reference to the serializer if found, std::nullopt otherwise.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2024-10-31)
            ///
            template <std::derived_from<ecstasy::serialization::ISerializer> Serializer>
            [[nodiscard]] std::optional<std::reference_wrapper<ecstasy::serialization::IEntityComponentSerializer>>
            tryGetSerializer() const noexcept
            {
                auto res = _serializers.find(std::type_index(typeid(Serializer)));
                if (res != _serializers.end())
                    return std::ref(*res->second);
                return std::nullopt;
            }

          protected:
            /// @brief Hash of the type name.
            std::unordered_map<std::type_index, std::unique_ptr<ecstasy::serialization::IEntityComponentSerializer>>
                _serializers;
        };

        ///
        /// @brief Concept to check if a type is comparable with AType.
        ///
        /// @tparam T Type to check.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-24)
        ///
        template <typename T>
        concept is_comparable_with_atype = requires(AType &a, T t) {
            {
                a == t
            } -> std::convertible_to<bool>; // Ensure a == t returns a boolean-like result
        };

    } // namespace rtti
} // namespace ecstasy

#endif /* !ECSTASY_RTTI_ATYPE_HPP_ */
