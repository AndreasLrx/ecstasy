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
#include <unordered_map>

/// @internal
#define ECSTASY_BUTTON_NAME_CASE(button) \
    case ecstasy::integration::event::Gamepad::Button::button: return stream << #button
/// @internal
#define ECSTASY_AXIS_NAME_CASE(axis)                       \
    case ecstasy::integration::event::Gamepad::Axis::axis: \
        return stream << #axis

#define MAP_BTN_INPUT(type)                                       \
    {                                                             \
        #type, ecstasy::integration::event::Gamepad::Button::type \
    }

#define MAP_AXIS_INPUT(type)                                    \
    {                                                           \
        #type, ecstasy::integration::event::Gamepad::Axis::type \
    }

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

std::istream &operator>>(std::istream &stream, ecstasy::integration::event::Gamepad::Button &button)
{
    static const std::unordered_map<std::string_view, ecstasy::integration::event::Gamepad::Button> map = {
        MAP_BTN_INPUT(Unknown), MAP_BTN_INPUT(FaceUp), MAP_BTN_INPUT(FaceRight), MAP_BTN_INPUT(FaceDown),
        MAP_BTN_INPUT(FaceLeft), MAP_BTN_INPUT(BumperLeft), MAP_BTN_INPUT(BumperRight), MAP_BTN_INPUT(MiddleLeft),
        MAP_BTN_INPUT(Middle), MAP_BTN_INPUT(MiddleRight), MAP_BTN_INPUT(ThumbLeft), MAP_BTN_INPUT(ThumbRight)};

    std::string buffer;
    stream >> buffer;
    button = map.at(buffer);
    return stream;
}

std::istream &operator>>(std::istream &stream, ecstasy::integration::event::Gamepad::Axis &axis)
{
    static const std::unordered_map<std::string_view, ecstasy::integration::event::Gamepad::Axis> map = {
        MAP_AXIS_INPUT(Unknown), MAP_AXIS_INPUT(LeftX), MAP_AXIS_INPUT(LeftY), MAP_AXIS_INPUT(RightX),
        MAP_AXIS_INPUT(RightY), MAP_AXIS_INPUT(TriggerLeft), MAP_AXIS_INPUT(TriggerRight), MAP_AXIS_INPUT(DPadX),
        MAP_AXIS_INPUT(DPadY)};

    std::string buffer;
    stream >> buffer;
    axis = map.at(buffer);
    return stream;
}
