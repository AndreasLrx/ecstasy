///
/// @file Instances.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-10-17
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef ECSTASY_STORAGE_INSTANCES_HPP_
#define ECSTASY_STORAGE_INSTANCES_HPP_

#include <concepts>
#include <memory>
#include <typeindex>
#include <unordered_map>

namespace ecstasy
{
    ///
    /// @brief Set of instances inheriting from the @b Base type.
    ///
    /// @tparam Base base type for all instances.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-17)
    ///
    template <typename Base>
    class Instances {
      public:
        ///
        /// @brief Construct a new Instances storage.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-17)
        ///
        Instances() = default;

        ///
        /// @brief Destroy the Instances storage. All elements are freed.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-17)
        ///
        ~Instances() = default;

        ///
        /// @brief Instances aren't copy constructible.
        ///
        /// @param[in] other instance to copy.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-17)
        ///
        Instances(const Instances &other) = delete;

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
        template <std::derived_from<Base> Derived, typename... Args>
        Derived &emplace(Args &&...args)
        {
            std::type_index id(typeid(Derived));

            if (_instances.contains(id))
                throw std::logic_error("Duplicate instance");

            return *dynamic_cast<Derived *>(
                _instances.emplace(std::make_pair(id, std::make_unique<Derived, Args...>(std::forward<Args>(args)...)))
                    .first->second.get());
        }

        ///
        /// @brief Get a const reference to the contained instance identified by @b type.
        ///
        /// @param[in] type the instance type index.
        ///
        /// @return const Base& A reference to the instance of @b Base contained within.
        ///
        /// @throw std::logic_error If no instance matching @b type is in the set.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-17)
        ///
        const Base &get(const std::type_index &type) const
        {
            return *getPtr(type);
        }

        ///
        /// @brief Get a const reference to the contained instance of @b Derived.
        ///
        /// @tparam Derived Type of the instance to get, must inherit @b Base.
        ///
        /// @return const Derived& A reference to the instance of @b Derived contained within.
        ///
        /// @throw std::logic_error If no instance of @b Derived is in the set.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-17)
        ///
        template <std::derived_from<Base> Derived>
        const Derived &get() const
        {
            return *getPtr<Derived>();
        }

        ///
        /// @brief Get a reference to the contained instance identified by @b type.
        ///
        /// @param[in] type the instance type index.
        ///
        /// @return Base& A reference to the instance of @b Base contained within.
        ///
        /// @throw std::logic_error If no instance matching @b type is in the set.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-17)
        ///
        Base &get(const std::type_index &type)
        {
            return *getPtr(type);
        }

        ///
        /// @brief Get a reference to the contained instance of @b Derived.
        ///
        /// @tparam Derived Type of the instance to get, must inherit @b Base.
        ///
        /// @return Derived& A reference to the instance of @b Derived contained within.
        ///
        /// @throw std::logic_error If no instance of @b Derived is in the set.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-17)
        ///
        template <std::derived_from<Base> Derived>
        Derived &get()
        {
            return *getPtr<Derived>();
        }

        ///
        /// @brief Get a pointer to the contained instance identified by @b type.
        ///
        /// @param[in] type the instance type index.
        ///
        /// @return Base* A pointer to the instance of @b Base contained within.
        ///
        /// @throw std::logic_error If no instance matching @b type is in the set.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-17)
        ///
        Base *getPtr(const std::type_index &type) const
        {
            const auto &valueIt = _instances.find(type);

            if (valueIt == _instances.end())
                throw std::logic_error("Instance not found");
            return valueIt->second.get();
        }

        ///
        /// @brief Get a pointer to the contained instance of @b Derived.
        ///
        /// @tparam Derived Type of the instance to get, must inherit @b Base.
        ///
        /// @return Derived* A pointer to the instance of @b Derived contained within.
        ///
        /// @throw std::logic_error If no instance of @b Derived is in the set.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-17)
        ///
        template <std::derived_from<Base> Derived>
        Derived *getPtr() const
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
        bool contains(const std::type_index &type) const
        {
            return _instances.contains(type);
        }

        ///
        /// @brief Check whether an instance of @b Derived is contained in the set.
        ///
        /// @tparam Derived The type of instance to test, must inherit from @b Base.
        ///
        /// @return  Whether the given instance type is contained in the set.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-17)
        ///
        template <std::derived_from<Base> Derived>
        bool contains() const
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
        /// @return constexpr const std::unordered_map<std::type_index, std::unique_ptr<Base>>& backing container.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-17)
        ///
        constexpr const std::unordered_map<std::type_index, std::unique_ptr<Base>> &getInner() const
        {
            return _instances;
        }

      private:
        std::unordered_map<std::type_index, std::unique_ptr<Base>> _instances;
    };
} // namespace ecstasy

#endif /* !ECSTASY_STORAGE_INSTANCES_HPP_ */
