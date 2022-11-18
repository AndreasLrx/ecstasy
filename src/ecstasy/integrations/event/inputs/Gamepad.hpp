///
/// @file Gamepad.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-18
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef ECSTASY_INTEGRATIONS_EVENT_INPUTS_GAMEPAD_HPP_
#define ECSTASY_INTEGRATIONS_EVENT_INPUTS_GAMEPAD_HPP_

#include <array>
#include <cstdint>
#include <ostream>

/// @internal
#define ECSTASY_BUTTON_NAME_CASE(button) \
    case Button::button: return #button
/// @internal
#define ECSTASY_AXIS_NAME_CASE(axis) \
    case Axis::axis: return #axis

namespace ecstasy::integration::event
{
    ///
    /// @brief Current Gamepad state. Used by @ref Gamepads.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-11-18)
    ///
    class Gamepad {
      public:
        /// @brief Gamepad buttons
        enum class Button {
            Unknown = -1, ///< Unhandled button
            /// Face Buttons
            FaceUp = 0, ///< Face button up (i.e. PS: Triangle, Xbox: Y)
            FaceRight,  ///< Face button right (i.e. PS: Square, Xbox: X)
            FaceDown,   ///< Face button down (i.e. PS: Cross, Xbox: A)
            FaceLeft,   ///< Face button left (i.e. PS: Circle, Xbox: B)
            /// Backward buttons
            BumperLeft,  ///< Left bumper (LB / L1)
            BumperRight, ///< Right bumper (RB / R1)
            /// Middle buttons
            MiddleLeft,  ///< Left center button (i.e. PS: Select, Xbox: back)
            Middle,      ///< Center buttons (i.e. PS: PS, Xbox: XBOX)
            MiddleRight, ///< Right center button (i.e. PS: Start, Xbox: Start)
            /// Joystick buttons
            ThumbLeft,  ///< Left joystick button
            ThumbRight, ///< Right joystick button

            Count ///< Keep last -- the total number of gamepad buttons
        };

        /// @brief Gamepad axis, associated value must be in range [-1, 1]
        enum class Axis {
            Unknown = -1, ///< Unhandled axis
            LeftX = 0,    ///< Left joystick X axis (default: 0)
            LeftY,        ///< Left joystick Y axis (default: 0)
            RightX,       ///< Right joystick X axis (default: 0)
            RightY,       ///< Right joystick Y axis (default: 0)
            TriggerLeft,  ///< Left trigger (default: -1)
            TriggerRight, ///< Right trigger (default: -1)
            DPadX,        ///< DPad X axis (default: 0)
            DPadY,        ///< DPad Y Axis (default: 0)

            Count ///< Keep last -- the total number of gamepad axis

        };

        /// @brief Gamepad joysticks (a joystick has 2 combined axis)
        enum class Joystick {
            Unknown = -1, ///< Unhandled joystick
            Left = 0,     ///< Left joystick
            Right,        ///< Right joystick

            Count ///< Keep last -- the total number of gamepad joysticks
        };

        ///
        /// @brief Construct a new Gamepad object.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-18)
        ///
        constexpr Gamepad(std::size_t id = 0) : _id(id), _connected(false), _buttons({false}), _axis({0.f})
        {
            setAxisValue(Axis::TriggerLeft, -1.f);
            setAxisValue(Axis::TriggerRight, -1.f);
        }

        ///
        /// @brief Default destructor.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-18)
        ///
        ~Gamepad() = default;

        ///
        /// @brief Get the gamepad id.
        ///
        /// @return std::size_t id.
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-18)
        ///
        constexpr std::size_t getId() const
        {
            return _id;
        }

        ///
        /// @brief Change the gamepad id.
        ///
        /// @warning It only update the internal state, no event is called.
        ///
        /// @param[in] id new gamepad id.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-18)
        ///
        constexpr void setId(std::size_t id)
        {
            _id = id;
        }

        ///
        /// @brief Check whether the gamepad is connected or not.
        ///
        /// @return bool Whether the gamepad is connected or not.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-18)
        ///
        constexpr bool isConnected() const
        {
            return _connected;
        }

        ///
        /// @brief Update the connected state of the gamepad.
        ///
        /// @warning It only update the internal state, no event is called.
        ///
        /// @param[in] connected New connected value (true if connected).
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-18)
        ///
        constexpr void setConnected(bool connected)
        {
            _connected = connected;
        }

        ///
        /// @brief Check whether a button is down.
        ///
        /// @param[in] button evaluated button.
        ///
        /// @return bool Whether the button is down.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-16)
        ///
        constexpr bool isButtonDown(Button button) const
        {
            return _buttons[static_cast<std::size_t>(button)];
        }

        ///
        /// @brief Check whether a button is up.
        ///
        /// @param[in] button evaluated button.
        ///
        /// @return bool Whether the button is up.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-16)
        ///
        constexpr bool isButtonUp(Button button) const
        {
            return !isButtonDown(button);
        }

        ///
        /// @brief Update a given button state.
        ///
        /// @warning It only update the internal state, no event is called.
        ///
        /// @param[in] button button to update.
        /// @param[in] down Whether the button must be set down or not.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-16)
        ///
        constexpr void setButtonState(Button button, bool down)
        {
            _buttons[static_cast<std::size_t>(button)] = down;
        }

        ///
        /// @brief Get the given axis value.
        ///
        /// @param[in] axis evaluated axis.
        ///
        /// @return constexpr float axis value.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-18)
        ///
        constexpr float getAxisValue(Axis axis) const
        {
            return _axis[static_cast<std::size_t>(axis)];
        }

        ///
        /// @brief Update an axis value.
        ///
        /// @warning It only update the internal state, no event is called.
        ///
        /// @param[in] axis modified axis.
        /// @param[in] value new value.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-18)
        ///
        constexpr void setAxisValue(Axis axis, float value)
        {
            _axis[static_cast<std::size_t>(axis)] = value;
        }

        ///
        /// @brief Get the name of a button.
        ///
        /// @warning If the button is Button::Count or isn't a valid button, @ref nullptr is returned.
        ///
        /// @param[in] button Evaluated button.
        ///
        /// @return const char* Button name if valid, nullptr otherwise.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-17)
        ///
        constexpr static const char *getButtonName(Button button)
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
                default: return nullptr;
            }
        }

        ///
        /// @brief Get the name of an axis.
        ///
        /// @warning If the axis is Axis::Count or isn't a valid axis, @ref nullptr is returned.
        ///
        /// @param[in] axis Evaluated axis.
        ///
        /// @return const char* Axis name if valid, nullptr otherwise.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-17)
        ///
        constexpr static const char *getAxisName(Axis axis)
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
                default: return nullptr;
            }
        }

      private:
        std::size_t _id;
        bool _connected;
        std::array<bool, static_cast<std::size_t>(Button::Count)> _buttons;
        std::array<float, static_cast<std::size_t>(Axis::Count)> _axis;
    };
} // namespace ecstasy::integration::event

#undef ECSTASY_BUTTON_NAME_CASE
#undef ECSTASY_AXIS_NAME_CASE

std::ostream &operator<<(std::ostream &stream, const ecstasy::integration::event::Gamepad::Button &button);
std::ostream &operator<<(std::ostream &stream, const ecstasy::integration::event::Gamepad::Axis &axis);

#endif /* !ECSTASY_INTEGRATIONS_EVENT_INPUTS_GAMEPAD_HPP_ */
