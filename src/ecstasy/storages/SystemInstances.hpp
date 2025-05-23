///
/// @file Instances.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief Set of instances inheriting from the @b Base type.
/// @version 1.0.0
/// @date 2022-10-17
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_STORAGE_SYSTEM_INSTANCES_HPP_
#define ECSTASY_STORAGE_SYSTEM_INSTANCES_HPP_

#include <concepts>
#include <map>
#include <memory>
#include <stdexcept>
#include <typeindex>

namespace ecstasy
{
    class ISystem;

    ///
    /// @brief Set of instances inheriting from the @b Base type.
    ///
    /// @tparam Base base type for all instances.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-17)
    ///
    class SystemInstances {
      private:
        /// @brief The internal map key type.
        using key_type = std::pair<std::type_index, size_t>;

        /// @internal
        /// @brief Comparer for the internal map.
        ///
        /// @author Andréas Leroux
        /// @since 1.0.0 (2024-10-17)
        ///
        class Comparer {
          public:
            using is_transparent = std::true_type;

            ///
            /// @brief Construct a new Comparer object
            ///
            /// @author Andréas Leroux
            /// @since 1.0.0 (2024-10-17)
            ///
            Comparer() = default;

            ///
            /// @brief Compare a key with a type index.
            ///
            /// @param a Left operand.
            /// @param b Right operand.
            ///
            /// @return Whether @b a is less than @b b.
            ///
            /// @author Andréas Leroux
            /// @since 1.0.0 (2024-10-17)
            ///
            bool operator()(const key_type &a, const std::type_index &b) const
            {
                return a.first < b;
            }

            ///
            /// @brief Compare a type index with a key.
            ///
            /// @param a Left operand.
            /// @param b Right operand.
            ///
            /// @return Whether @b a is less than @b b.
            ///
            /// @author Andréas Leroux
            /// @since 1.0.0 (2024-10-17)
            ///
            bool operator()(const std::type_index &a, const key_type &b) const
            {
                return a < b.first;
            }

            ///
            /// @brief Compare two keys.
            ///
            /// @param a Left operand.
            /// @param b Right operand.
            ///
            /// @return Whether @b a is less than @b b.
            ///
            /// @author Andréas Leroux
            /// @since 1.0.0 (2024-10-17)
            ///
            bool operator()(const key_type &a, const key_type &b) const
            {
                if (a.second == b.second)
                    return a.first < b.first;
                else
                    return a.second < b.second;
            }
        };

      public:
        ///
        /// @brief Construct a new SystemInstances storage.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-17)
        ///
        SystemInstances() = default;

        ///
        /// @brief Destroy the SystemInstances storage. All elements are freed.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-17)
        ///
        ~SystemInstances() = default;

        ///
        /// @brief SystemInstances aren't copy constructible.
        ///
        /// @param[in] other instance to copy.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-17)
        ///
        SystemInstances(const SystemInstances &other) = delete;

        ///
        /// @brief Emplace a new instance of type @b Derived in the storage.
        ///
        /// @tparam Derived The type of the instance to create. It must inherit from @b Base.
        /// @tparam Args The arguments to pass to the constructor of @b Derived.
        ///
        /// @param[in] args The arguments to pass to the constructor of @b Derived.
        ///
        /// @return Derived& A reference to the newly created object.
        ///
        /// @throw std::logic_error If an instance of type @b Derived was already set.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-17)
        ///
        template <std::derived_from<ISystem> Derived, size_t Priority = 0, typename... Args>
        Derived &emplace(Args &&...args)
        {
            std::type_index id(typeid(Derived));

            if (_instances.contains(id)) [[unlikely]]
                throw std::logic_error("Duplicate instance");

            return *dynamic_cast<Derived *>(_instances
                                                .emplace(std::make_pair(std::make_pair(id, Priority),
                                                    std::make_unique<Derived, Args...>(std::forward<Args>(args)...)))
                                                .first->second.get());
        }

        ///
        /// @brief Get a const reference to the contained instance identified by @b type.
        ///
        /// @param[in] type the instance type index.
        ///
        /// @return const ISystem& A reference to the instance of @b ISystem contained within.
        ///
        /// @throw std::logic_error If no instance matching @b type is in the set.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-17)
        ///
        [[nodiscard]] const ISystem &get(const std::type_index &type) const
        {
            return *getPtr(type);
        }

        ///
        /// @brief Get a const reference to the contained instance of @b Derived.
        ///
        /// @tparam Derived Type of the instance to get, must inherit @b ISystem.
        ///
        /// @return const Derived& A reference to the instance of @b Derived contained within.
        ///
        /// @throw std::logic_error If no instance of @b Derived is in the set.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-17)
        ///
        template <std::derived_from<ISystem> Derived>
        [[nodiscard]] const Derived &get() const
        {
            return *getPtr<Derived>();
        }

        ///
        /// @brief Get a reference to the contained instance identified by @b type.
        ///
        /// @param[in] type the instance type index.
        ///
        /// @return ISystem& A reference to the instance of @b ISystem contained within.
        ///
        /// @throw std::logic_error If no instance matching @b type is in the set.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-17)
        ///
        [[nodiscard]] ISystem &get(const std::type_index &type)
        {
            return *getPtr(type);
        }

        ///
        /// @brief Get a reference to the contained instance of @b Derived.
        ///
        /// @tparam Derived Type of the instance to get, must inherit @b ISystem.
        ///
        /// @return Derived& A reference to the instance of @b Derived contained within.
        ///
        /// @throw std::logic_error If no instance of @b Derived is in the set.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-17)
        ///
        template <std::derived_from<ISystem> Derived>
        [[nodiscard]] Derived &get()
        {
            return *getPtr<Derived>();
        }

        ///
        /// @brief Get a pointer to the contained instance identified by @b type.
        ///
        /// @param[in] type the instance type index.
        ///
        /// @return ISystem* A pointer to the instance of @b ISystem contained within.
        ///
        /// @throw std::logic_error If no instance matching @b type is in the set.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-17)
        ///
        [[nodiscard]] ISystem *getPtr(const std::type_index &type) const
        {
            const auto &valueIt = _instances.find(type);

            if (valueIt == _instances.end()) [[unlikely]]
                throw std::logic_error("Instance not found");
            return valueIt->second.get();
        }

        ///
        /// @brief Get a pointer to the contained instance of @b Derived.
        ///
        /// @tparam Derived Type of the instance to get, must inherit @b ISystem.
        ///
        /// @return Derived* A pointer to the instance of @b Derived contained within.
        ///
        /// @throw std::logic_error If no instance of @b Derived is in the set.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-17)
        ///
        template <std::derived_from<ISystem> Derived>
        [[nodiscard]] Derived *getPtr() const
        {
            return dynamic_cast<Derived *>(getPtr(std::type_index(typeid(Derived))));
        }

        ///
        /// @brief Check wheter the instance identified by @b type is contained in the set.
        ///
        /// @param[in] type the instance type index.
        ///
        /// @return Whether the given @b type match one of the instance contained in the set.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-17)
        ///
        [[nodiscard]] bool contains(const std::type_index &type) const
        {
            return _instances.contains(type);
        }

        ///
        /// @brief Check whether an instance of @b Derived is contained in the set.
        ///
        /// @tparam Derived The type of instance to test, must inherit from @b ISystem.
        ///
        /// @return  Whether the given instance type is contained in the set.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-17)
        ///
        template <std::derived_from<ISystem> Derived>
        [[nodiscard]] bool contains() const
        {
            return contains(std::type_index(typeid(Derived)));
        }

        ///
        /// @brief Remove all stored instances.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-17)
        ///
        void clear()
        {
            _instances.clear();
        }

        ///
        /// @brief Get the backing container.
        ///
        /// @return const std::unordered_map<std::type_index, std::unique_ptr<ISystem>>& backing container.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-17)
        ///
        [[nodiscard]] constexpr const std::map<key_type, std::unique_ptr<ISystem>, Comparer> &getInner() const noexcept
        {
            return _instances;
        }

      private:
        /// @brief The internal map of instances.
        std::map<key_type, std::unique_ptr<ISystem>, Comparer> _instances;
    };
} // namespace ecstasy

#endif /* !ECSTASY_STORAGE_SYSTEM_INSTANCES_HPP_ */
