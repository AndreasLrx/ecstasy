///
/// @file Allocator.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-10-25
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef UTIL_ALLOCATOR_HPP_
#define UTIL_ALLOCATOR_HPP_

#include <memory>
#include <vector>

namespace util
{
    /// @internal
    /// @brief Wrapper of a vector to manage lifetime of multiple instances inheriting the same base class.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-24)
    ///
    template <typename Base>
    class Allocator {
      public:
        ///
        /// @brief Instanciate a new instance of type T with lifetime attached to @b this lifetime.
        ///
        /// @tparam T Type of the new object.
        /// @tparam Args Arguments Types of the object constructor.
        ///
        /// @param[in] args Arguments to forward to the object constructor.
        ///
        /// @return T& newly created object.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-24)
        ///
        template <std::derived_from<Base> T, typename... Args>
        T &instanciate(Args &&...args)
        {
            return dynamic_cast<T &>(*_instances.emplace_back(std::make_unique<T>(std::forward<Args>(args)...)).get());
        }

      private:
        std::vector<std::unique_ptr<Base>> _instances;
    };
} // namespace util

#endif /* !UTIL_ALLOCATOR_HPP_ */
