///
/// @file Mouse.cpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-29
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#include "Mouse.hpp"

/// @internal
#define ECSTASY_BUTTON_NAME_CASE(btn) \
    case ecstasy::integration::event::Mouse::Button::btn: return stream << #btn

std::ostream &operator<<(std::ostream &stream, const ecstasy::integration::event::Mouse::Button &button)
{
    switch (button) {
        ECSTASY_BUTTON_NAME_CASE(Left);
        ECSTASY_BUTTON_NAME_CASE(Right);
        ECSTASY_BUTTON_NAME_CASE(Middle);
        ECSTASY_BUTTON_NAME_CASE(Extra1);
        ECSTASY_BUTTON_NAME_CASE(Extra2);
        ECSTASY_BUTTON_NAME_CASE(Extra3);
        default: return stream;
    }
}