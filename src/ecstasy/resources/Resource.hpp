///
/// @file Resource.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
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
    class Resource
#ifdef ECSTASY_LOCKABLE_RESOURCES
        : public thread::SharedRecursiveMutex
#endif
    {
      public:
        /// @brief Default destructor.
        virtual ~Resource() = default;
    };
} // namespace ecstasy

#endif /* !ECSTASY_RESOURCE_RESOURCE_HPP_ */
