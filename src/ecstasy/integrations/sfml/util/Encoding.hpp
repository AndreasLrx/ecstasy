///
/// @file Encoding.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-17
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_INTEGRATIONS_SFML_ENCODING_HPP_
#define ECSTASY_INTEGRATIONS_SFML_ENCODING_HPP_

#include <SFML/System/Utf.hpp>
#include <stdint.h>

namespace ecstasy::integration::sfml
{
    class Encoding {
      public:
        ~Encoding() = default;

        ///
        /// @brief Encode a Utf32 char to a Utf8 string
        ///
        /// @param[in] utf Utf32 character to encode.
        ///
        /// @return std::string Utf8 string.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-17)
        ///
        static std::string utf32CharToUtf8String(uint32_t utf);

      private:
        Encoding() = default;
    };
} // namespace ecstasy::integration::sfml

#endif /* !ECSTASY_INTEGRATIONS_SFML_ENCODING_HPP_ */
