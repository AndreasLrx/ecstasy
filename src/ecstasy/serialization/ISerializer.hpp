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

#include <filesystem>
#include <iostream>

namespace ecstasy::serialization
{
    ///
    /// @brief Interface for all serializer classes.
    ///
    /// @note As for many ecstasy classes, this class is empty and is used to group all serializer classes under the
    /// same type. This is required to store them in a container or to use them in a virtual function.
    ///
    /// @note The serializer classes can be seen as a stream of bytes that can be read and written to.
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

        ///
        /// @brief Clear the serializer content.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-09-12)
        ///
        virtual void clear() = 0;

        ///
        /// @brief Get the size of the serializer content (in bytes).
        ///
        /// @return size_t Size of the serializer content in bytes.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-09-12)
        ///
        virtual size_t size() const = 0;

        ///
        /// @brief Import data from a file into the serializer.
        ///
        /// @note Calls the importStream method with a file stream.
        ///
        /// @param[in] filename Path to the file to import.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-09-12)
        ///
        virtual void importFile(const std::filesystem::path &filename) = 0;

        ///
        /// @brief Import data from a string into the serializer.
        ///
        /// @note Calls the importStream method with a stringstream.
        ///
        /// @param[in] content String to import.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-09-12)
        ///
        virtual void importBytes(const std::string &content) = 0;

        ///
        /// @brief Import data from a stream into the serializer.
        ///
        /// @param[in] stream Stream to import.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-09-12)
        ///
        virtual void importStream(std::istream &stream) = 0;

        ///
        /// @brief Export the serializer content to a file.
        ///
        /// @note Calls the exportStream method with a file stream.
        ///
        /// @param[in] filename Path to the file to export the serializer content to.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-09-12)
        ///
        virtual void exportFile(const std::filesystem::path &filename) = 0;

        ///
        /// @brief Export the serializer content to a string.
        ///
        /// @note Calls the exportStream method with a stringstream.
        ///
        /// @return std::string String representation of the serializer content.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-09-12)
        ///
        virtual std::string exportBytes() = 0;

        ///
        /// @brief Export the serializer content to a stream.
        ///
        /// @param[in] stream Stream to export the serializer content to.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-09-12)
        ///
        virtual void exportStream(std::ostream &stream) = 0;
    };
} // namespace ecstasy::serialization

#endif /* !ECSTASY_SERIALIZATION_ISERIALIZER_HPP_ */
