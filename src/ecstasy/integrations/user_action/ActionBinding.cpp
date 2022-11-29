///
/// @file ActionBinding.cpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-29
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#include "ActionBinding.hpp"

/// @internal
#define ECSTASY_BUTTON_NAME_CASE(type) \
    case ecstasy::integration::user_action::ActionBinding::Type::type: return stream << #type

std::ostream &operator<<(std::ostream &stream, const ecstasy::integration::user_action::ActionBinding::Type &type)
{
    switch (type) {
        ECSTASY_BUTTON_NAME_CASE(MouseButton);
        ECSTASY_BUTTON_NAME_CASE(Key);
        ECSTASY_BUTTON_NAME_CASE(GamepadButton);
        ECSTASY_BUTTON_NAME_CASE(GamepadAxis);
        default: return stream;
    }
}