///
/// @file Serializer.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-12-01
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef UTIL_SERIALIZATION_SERIALIZER_HPP_
#define UTIL_SERIALIZATION_SERIALIZER_HPP_

#include <iostream>

namespace util::serialization
{
    ///
    /// @brief Static class
    ///
    /// @tparam T
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-12-01)
    ///
    class Serializer {
      public:
        ///
        /// @brief Serialize an object in a stream.
        ///
        /// @tparam Object Type of the object to serialize.
        ///
        /// @param[in] stream Output stream in which the object must be serialized.
        /// @param[in] object Object to serialize.
        ///
        /// @return std::ostream& @p stream.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-01)
        ///
        template <typename Object>
        static inline std::ostream &serialize(std::ostream &stream, const Object &object)
        {
            return stream << object;
        }

        ///
        /// @brief Serialize an object to a @ref std::string.
        ///
        /// @tparam Object Type of the object to serialize.
        ///
        /// @param[in] object Object to serialize.
        ///
        /// @return std::string seri
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-01)
        ///
        template <typename Object>
        static inline std::string serialize(const Object &object)
        {
            std::ostringstream ss;

            serialize(ss, object);
            return ss.str();
        }

        ///
        /// @brief Deserialize an object from an input stream.
        ///
        /// @tparam Object Object type.
        ///
        /// @param[in] stream Input stream containing the serialized object.
        ///
        /// @return Object Deserialized object.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-01)
        ///
        template <typename Object>
        static inline Object deserialize(std::istream &stream)
        {
            Object oblect;

            stream >> oblect;
            return oblect;
        }

        ///
        /// @brief Deserialize an object from its bytes representation.
        ///
        /// @tparam Object Object type.
        ///
        /// @param[in] bytes serialized object.
        ///
        /// @return Object Deserialized object.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-01)
        ///
        template <typename Object>
        static inline Object deserialize(std::string_view bytes)
        {
            std::istream ss(bytes);

            return deserialize<Object>(ss);
        }

      private:
        Serializer() = default;
    };
} // namespace util::serialization

#endif /* !UTIL_SERIALIZATION_SERIALIZER_HPP_ */
