///
/// @file ISerializer.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2024-04-29
///
/// @copyright Copyright (c) ECSTASY 2024
///
///

#ifndef ECSTASY_SERIALIZATION_ISERIALIZER_HPP_
#define ECSTASY_SERIALIZATION_ISERIALIZER_HPP_

namespace ecstasy::serialization
{
    ///
    /// @brief Interface for all serializer classes.
    ///
    /// @note As for many ecstasy classes, this class is empty and is used to group all serializer classes under the
    /// same type. This is required to store them in a container or to use them in a virtual function.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-06-11)
    ///
    class ISerializer {
      public:
        ///
        /// @brief Destroy the ISerializer
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-06-11)
        ///
        virtual ~ISerializer() = default;
    };
} // namespace ecstasy::serialization

#endif /* !ECSTASY_SERIALIZATION_ISERIALIZER_HPP_ */
