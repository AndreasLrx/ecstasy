///
/// @file ObjectWrapper.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-16
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_RESOURCES_OBJECTWRAPPER_HPP_
#define ECSTASY_RESOURCES_OBJECTWRAPPER_HPP_

#include "Resource.hpp"

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
    class ObjectWrapper : public Resource<ObjectWrapper<T>> {
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
        /// @brief Get a reference to the object.
        ///
        /// @return T& Reference to the object.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-16)
        ///
        constexpr T &get()
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
        constexpr const T &get() const
        {
            return _object;
        }

      protected:
        T _object;
    };
} // namespace ecstasy

#endif /* !ECSTASY_RESOURCES_OBJECTWRAPPER_HPP_ */
