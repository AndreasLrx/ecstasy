///
/// @file ObjectWrapper.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief Basic object wrapper as a resource.
/// @version 1.0.0
/// @date 2022-11-16
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_RESOURCES_OBJECTWRAPPER_HPP_
#define ECSTASY_RESOURCES_OBJECTWRAPPER_HPP_

#include "IResource.hpp"

namespace ecstasy
{
    ///
    /// @brief Basic object wrapper as a resource.
    ///
    /// @tparam T Type of the wrapped object.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-11-16)
    ///
    template <typename T>
    class ObjectWrapper : public IResource {
      public:
        ///
        /// @brief Construct a new Object Wrapper.
        ///
        /// @tparam Args Argument types of the object constructor.
        ///
        /// @param[in] args Arguments of the object constructor.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-16)
        ///
        template <typename... Args>
        ObjectWrapper(Args &&...args) : _object(std::forward<Args>(args)...)
        {
        }

        ///
        /// @brief Destroy the Object Wrapper.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-16)
        ///
        ~ObjectWrapper() = default;

        ///
        /// @brief Access the wrapped object.
        ///
        /// @return T& Reference to the wrapped object.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-18)
        ///
        [[nodiscard]] constexpr T &operator*() noexcept
        {
            return _object;
        }

        ///
        /// @brief Access the wrapped object.
        ///
        /// @return const T& Const reference to the wrapped object.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-18)
        ///
        [[nodiscard]] constexpr const T &operator*() const noexcept
        {
            return _object;
        }

        ///
        /// @brief Access the wrapped object pointer.
        ///
        /// @return T* Pointer to the wrapped object.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-18)
        ///
        [[nodiscard]] constexpr T *operator->() noexcept
        {
            return &_object;
        }

        ///
        /// @brief Access the wrapped object pointer.
        ///
        /// @return T const* Const pointer to the wrapped object.
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-18)
        ///
        [[nodiscard]] constexpr T const *operator->() const noexcept
        {
            return &_object;
        }

        ///
        /// @brief Get a reference to the object.
        ///
        /// @return T& Reference to the object.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-16)
        ///
        [[nodiscard]] constexpr T &get() noexcept
        {
            return _object;
        }

        ///
        /// @brief Get a const reference to the object.
        ///
        /// @return const T& Const reference to the object.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-16)
        ///
        [[nodiscard]] constexpr const T &get() const noexcept
        {
            return _object;
        }

      protected:
        /// @brief Wrapped object.
        T _object;
    };
} // namespace ecstasy

#endif /* !ECSTASY_RESOURCES_OBJECTWRAPPER_HPP_ */
