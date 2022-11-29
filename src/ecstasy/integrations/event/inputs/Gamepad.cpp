///
/// @file Gamepad.cpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-18
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#include "Gamepad.hpp"

/// @internal
#define ECSTASY_BUTTON_NAME_CASE(button) \
    case ecstasy::integration::event::Gamepad::Button::button: return stream << #button
/// @internal
#define ECSTASY_AXIS_NAME_CASE(axis) \
    case ecstasy::integration::event::Gamepad::Axis::axis: return stream << #axis

std::ostream &operator<<(std::ostream &stream, const ecstasy::integration::event::Gamepad::Button &button)
{
    switch (button) {
        ECSTASY_BUTTON_NAME_CASE(Unknown);
        ECSTASY_BUTTON_NAME_CASE(FaceUp);
        ECSTASY_BUTTON_NAME_CASE(FaceRight);
        ECSTASY_BUTTON_NAME_CASE(FaceDown);
        ECSTASY_BUTTON_NAME_CASE(FaceLeft);
        ECSTASY_BUTTON_NAME_CASE(BumperLeft);
        ECSTASY_BUTTON_NAME_CASE(BumperRight);
        ECSTASY_BUTTON_NAME_CASE(MiddleLeft);
        ECSTASY_BUTTON_NAME_CASE(Middle);
        ECSTASY_BUTTON_NAME_CASE(MiddleRight);
        ECSTASY_BUTTON_NAME_CASE(ThumbLeft);
        ECSTASY_BUTTON_NAME_CASE(ThumbRight);
        default: return stream;
    }
}

std::ostream &operator<<(std::ostream &stream, const ecstasy::integration::event::Gamepad::Axis &axis)
{
    switch (axis) {
        ECSTASY_AXIS_NAME_CASE(Unknown);
        ECSTASY_AXIS_NAME_CASE(LeftX);
        ECSTASY_AXIS_NAME_CASE(LeftY);
        ECSTASY_AXIS_NAME_CASE(RightX);
        ECSTASY_AXIS_NAME_CASE(RightY);
        ECSTASY_AXIS_NAME_CASE(TriggerLeft);
        ECSTASY_AXIS_NAME_CASE(TriggerRight);
        ECSTASY_AXIS_NAME_CASE(DPadX);
        ECSTASY_AXIS_NAME_CASE(DPadY);
        default: return stream;
    }
}