///
/// @file TypeRegistry.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2024-10-24
///
/// @copyright Copyright (c) ECSTASY 2024
///
///

#ifndef ECSTASY_RTTI_TYPEREGISTRY_HPP_
#define ECSTASY_RTTI_TYPEREGISTRY_HPP_

#include <algorithm>
#include <functional>
#include <optional>

#include "Type.hpp"
#include "util/serialization/foreach.hpp"

#define CONCAT_IMPL(x, y) x##y
#define CONCAT(x, y)      CONCAT_IMPL(x, y)

///
/// @brief Register a component type.
///
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @since 1.0.0 (2024-10-04)
///
#define REGISTER_TYPE(TYPE)                        \
    static bool CONCAT(registered_, __COUNTER__) = \
        reinterpret_cast<bool &>(ecstasy::rtti::TypeRegistry::getInstance().registerType<TYPE>(#TYPE));

///
/// @brief Register multiple component types.
///
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @since 1.0.0 (2024-10-04)
///
#define REGISTER_TYPES(TYPE, ...) FOR_EACH(REGISTER_TYPE, TYPE, __VA_ARGS__)

namespace ecstasy::rtti
{

    ///
    /// @brief Type registry class to store types in cross-platform way.
    ///
    /// @note The class is a singleton and should be accessed through the @ref getInstance() method.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-10-24)
    ///
    class TypeRegistry {
      public:
        /// @brief Predicate type for custom type search.
        using Predicate = std::function<bool(const std::pair<const std::size_t, std::unique_ptr<AType>> &)>;
        /// @brief Alias to an optional reference to a type (to be less verbose).
        using OptionalATypeReference = std::optional<std::reference_wrapper<AType>>;

        ///
        /// @brief Get the Instance object
        ///
        /// @return TypeRegistry& The instance of the TypeRegistry.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-24)
        ///
        [[nodiscard]] static TypeRegistry &getInstance() noexcept;

        ///
        /// @brief Register a type in the registry.
        ///
        /// @tparam T Type to register.
        ///
        /// @param[in] name Name of the type.
        ///
        /// @return bool True if the type was successfully registered, false if it was already registered.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-24)
        ///
        template <typename T>
        AType &registerType(std::string_view name)
        {
            size_t hash = std::hash<std::string_view>{}(name);
            OptionalATypeReference type = find(hash);

            if (type.has_value())
                return type.value();
            else
                return *_types.emplace(hash, std::make_unique<Type<T>>(name)).first->second;
        }

        ///
        /// @brief Check if a type is registered. This calls @ref find() and checks if the result is not std::nullopt.
        ///
        /// @tparam T Type to check.
        ///
        /// @param[in] target Type to check.
        ///
        /// @return bool True if the type is registered, false otherwise.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-24)
        ///
        template <is_comparable_with_atype T>
        [[nodiscard]] bool has(const T &target) const noexcept
        {
            return find(target).has_value();
        }

        /// @copydoc TypeRegistry::has()
        [[nodiscard]] bool has(size_t name_hash) const noexcept;

        ///
        /// @brief Check if a type is registered.
        ///
        /// @tparam T Type to check.
        ///
        /// @return bool True if the type is registered, false otherwise.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-24)
        ///
        template <typename T>
        [[nodiscard]] bool has() const noexcept
        {
            return has<std::type_info>(typeid(T));
        }

        ///
        /// @brief Calls @ref std::find_if on the types map using the given predicate.
        ///
        /// @param[in] p Predicate to use for the search.
        ///
        /// @return OptionalATypeReference Optional reference to the type if found, std::nullopt otherwise.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-24)
        ///
        [[nodiscard]] OptionalATypeReference findIf(const Predicate &p) const noexcept;

        ///
        /// @brief Get a reference to the AType instance matching the predicate.
        ///
        /// @param[in] p Predicate to use for the search.
        ///
        /// @return AType& Reference to the type.
        ///
        /// @throw std::out_of_range If the type is not found.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-24)
        ///
        [[nodiscard]] AType &getIf(const Predicate &p) const;

        ///
        /// @brief Search for a registered type.
        ///
        /// @note This method call @ref std::find on the internal types map.
        ///
        /// @tparam T Type to check. It must be equality comparable with AType.
        ///
        /// @param[in] target Value to compare the types with.
        ///
        /// @return OptionalATypeReference& Optional reference to the type if found, std::nullopt otherwise.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-24)
        ///
        template <is_comparable_with_atype T>
        [[nodiscard]] OptionalATypeReference find(const T &target) const noexcept
        {
            auto result = std::find_if(_types.begin(), _types.end(), [&target](const auto &pair) {
                return *pair.second == target;
            });

            if (result == _types.end())
                return std::nullopt;
            return std::ref(*result->second);
        }

        /// @copydoc TypeRegistry::find()
        [[nodiscard]] OptionalATypeReference find(std::size_t name_hash) const noexcept;

        ///
        /// @brief Search for a registered type.
        ///
        /// @tparam T Type to search.
        ///
        /// @return OptionalATypeReference Optional reference to the type if found, std::nullopt otherwise.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-24)
        ///
        template <typename T>
        [[nodiscard]] OptionalATypeReference find() const noexcept
        {
            return find<std::type_info>(typeid(T));
        }

        ///
        /// @brief Get a reference to the AType instance matching the target.
        ///
        /// @tparam T Type to check. It must be equality comparable with AType.
        ///
        /// @param[in] target Value to compare the types with.
        ///
        /// @return AType& Reference to the type.
        ///
        /// @throw std::out_of_range If the type is not found.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-24)
        ///
        template <is_comparable_with_atype T>
        [[nodiscard]] AType &get(const T &target) const
        {
            auto result = find(target);

            if (result.has_value())
                return result.value();
            throw std::out_of_range("Type not found.");
        }

        /// @copydoc TypeRegistry::get()
        [[nodiscard]] AType &get(std::size_t name_hash) const;

        ///
        /// @brief Get a reference to the AType instance matching the target.
        ///
        /// @tparam T Type to search.
        ///
        /// @return AType& Reference to the type.
        ///
        /// @throw std::out_of_range If the type is not found.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-24)
        ///
        template <typename T>
        [[nodiscard]] AType &get() const
        {
            return get<std::type_info>(typeid(T));
        }

      private:
        /// @brief Map of types indexed by their (cross-platform) hash code.
        std::unordered_map<std::size_t, std::unique_ptr<AType>> _types;

        ///
        /// @brief Construct a new Type Registry instance.
        ///
        /// @note The constructor is private to prevent instantiation.
        ///
        TypeRegistry() noexcept = default;

        ///
        /// @brief Destroy the Type Registry instance.
        ///
        /// @note The destructor is private to prevent deletion.
        ///
        ~TypeRegistry() noexcept = default;
    };

} // namespace ecstasy::rtti

#endif /* !ECSTASY_RTTI_TYPEREGISTRY_HPP_ */
