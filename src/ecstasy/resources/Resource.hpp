///
/// @file Resource.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief Base class of all registry resources.
/// @version 1.0.0
/// @date 2022-10-17
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_RESOURCE_RESOURCE_HPP_
#define ECSTASY_RESOURCE_RESOURCE_HPP_

#include "ecstasy/config.hpp"

#ifdef ECSTASY_LOCKABLE_RESOURCES
    #include "ecstasy/thread/SharedRecursiveMutex.hpp"
#endif

namespace ecstasy
{
    ///
    /// @brief Base class of all registry resources.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-17)
    ///
    class ResourceBase
#ifdef ECSTASY_LOCKABLE_RESOURCES
        : public thread::SharedRecursiveMutex
#endif
    {
      public:
        /// @brief Default destructor.
        virtual ~ResourceBase() = default;
    }; // namespace ecstasy

    ///
    /// @brief Resource class.
    /// This class is templated to allow compatibility with LockableView.
    ///
    /// @tparam R Type of the resource.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-04-17)
    ///
    template <typename R>
    class Resource : public ResourceBase {
      public:
        ///
        /// @brief Compatibility for LockableView. Using -> on a Resource or a LockableView will return the pointer to
        /// the resource.
        ///
        /// @return R* Pointer to the resource.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-04-17)
        ///
        [[nodiscard]] R *operator->() noexcept
        {
            return dynamic_cast<R *>(this);
        }

        /// @copydoc Resource::operator->()
        [[nodiscard]] const R *operator->() const noexcept
        {
            return dynamic_cast<const R *>(this);
        }
    };
} // namespace ecstasy

#endif /* !ECSTASY_RESOURCE_RESOURCE_HPP_ */
