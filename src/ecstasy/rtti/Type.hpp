///
/// @file Type.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief File containing the Type class (AType implementation) for cross-platform type information.
/// @version 1.0.0
/// @date 2024-10-24
///
/// @copyright Copyright (c) ECSTASY 2024
///
///

#ifndef ECSTASY_RTTI_TYPE_HPP_
#define ECSTASY_RTTI_TYPE_HPP_

#include <typeindex>
#include <string_view>

#include "ecstasy/rtti/AType.hpp"
#include "ecstasy/storages/StorageConcepts.hpp"

namespace ecstasy
{
    namespace rtti
    {
        ///
        /// @brief Type erased interface for cross-platform type information.
        ///
        /// @tparam T Type to get the type information from.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-24)
        ///
        template <typename T>
        class Type : public AType {
          public:
            /// Type of the storage used to store the component.
            using StorageType = getStorageType<T>;

            ///
            /// @brief Construct a new Type instance.
            ///
            /// @warning The name will fallback on the type name, which is @b not guaranteed to be cross-platform.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2024-10-24)
            ///
            Type() noexcept : AType()
            {
                setTypeName(typeid(T).name());
            }

            ///
            /// @brief Construct a new Type instance with a custom name.
            ///
            /// @warning The name life time must be greater than the Type instance life time, as it is stored as a
            /// reference.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2024-10-24)
            ///
            Type(std::string_view name) noexcept : AType()
            {
                setTypeName(name);
            }

            /// @copydoc AType::~AType
            ~Type() noexcept override final = default;

            /// @copydoc AType::getStorageTypeInfo
            [[nodiscard]] const std::type_info &getStorageTypeInfo() const noexcept override final
            {
                return typeid(StorageType);
            }

            /// @copydoc AType::getTypeInfo
            [[nodiscard]] const std::type_info &getTypeInfo() const noexcept override final
            {
                return typeid(T);
            }

            /// @copydoc AType::getHash
            [[nodiscard]] size_t getHash() const noexcept override final
            {
                return _hash;
            }

            /// @copydoc AType::getTypeName
            [[nodiscard]] std::string_view getTypeName() const noexcept override final
            {
                return _name;
            }

          private:
            /// @brief Name of the component type.
            std::string_view _name;
            /// @brief Hash of the component type name. Should be cross-platform.
            std::size_t _hash;

            ///
            /// @brief Set the Type name. This will also update the hash.
            ///
            /// @param[in] name New name of the Type.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2024-10-24)
            ///
            void setTypeName(std::string_view name) noexcept
            {
                _name = name;
                _hash = std::hash<std::string_view>{}(_name);
            }
        };

    } // namespace rtti
} // namespace ecstasy

#endif /* !ECSTASY_RTTI_TYPE_HPP_ */
