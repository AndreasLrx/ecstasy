///
/// @file Encoding.cpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-17
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#include "Encoding.hpp"

namespace ecstasy::integration::sfml
{
    std::string Encoding::utf32CharToUtf8String(uint32_t utf)
    {
        std::string output;
        sf::Utf32::toUtf8(&utf, &utf + 1, std::back_inserter(output));

        return output;
    }
} // namespace ecstasy::integration::sfml
