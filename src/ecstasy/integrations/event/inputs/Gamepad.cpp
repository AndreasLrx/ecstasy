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

std::ostream &operator<<(std::ostream &stream, const ecstasy::integration::event::Gamepad::Button &button)
{
    return stream << ecstasy::integration::event::Gamepad::getButtonName(button);
}

std::ostream &operator<<(std::ostream &stream, const ecstasy::integration::event::Gamepad::Axis &axis)
{
    return stream << ecstasy::integration::event::Gamepad::getAxisName(axis);
}