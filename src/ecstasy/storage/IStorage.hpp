///
/// @file IStorage.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-10-19
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef ECSTASY_STORAGE_ISTORAGE_HPP_
#define ECSTASY_STORAGE_ISTORAGE_HPP_

namespace ecstasy
{
    ///
    /// @brief Base class of all components storage.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-19)
    ///
    class IStorage {
      public:
        virtual ~IStorage() = default;
    };

} // namespace ecstasy

#endif /* !ECSTASY_STORAGE_ISTORAGE_HPP_ */
