///
/// @file Keyboard.cpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-17
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#include "Keyboard.hpp"

std::ostream &operator<<(std::ostream &stream, const ecstasy::integration::event::Keyboard::Key &key)
{
    return stream << ecstasy::integration::event::Keyboard::getKeyName(key);
}