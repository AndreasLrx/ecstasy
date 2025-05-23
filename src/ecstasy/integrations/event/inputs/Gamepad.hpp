///
/// @file Gamepad.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-18
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_INTEGRATIONS_EVENT_INPUTS_GAMEPAD_HPP_
#define ECSTASY_INTEGRATIONS_EVENT_INPUTS_GAMEPAD_HPP_

#include <array>
#include <cstdint>
#include <iostream>
#include <unordered_map>

#include "util/serialization/SerializableEnum.hpp"

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
#ifndef _DOXYGEN_

        // LCOV_EXCL_START

        SERIALIZABLE_ENUM(Button, -1, Unknown, FaceUp, FaceRight, FaceDown, FaceLeft, BumperLeft, BumperRight,
            MiddleLeft, Middle, MiddleRight, ThumbLeft, ThumbRight, Count)
        SERIALIZABLE_ENUM(
            Axis, -1, Unknown, LeftX, LeftY, RightX, RightY, TriggerLeft, TriggerRight, DPadX, DPadY, Count)

            // LCOV_EXCL_STOP
#else
        /// @brief Gamepad buttons
        enum class Button {
            Unknown = -1, ///< Unhandled button
            /// Face Buttons
            FaceUp,    ///< Face button up (i.e. PS: Triangle, Xbox: Y)
            FaceRight, ///< Face button right (i.e. PS: Square, Xbox: X)
            FaceDown,  ///< Face button down (i.e. PS: Cross, Xbox: A)
            FaceLeft,  ///< Face button left (i.e. PS: Circle, Xbox: B)
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
            LeftX,        ///< Left joystick X axis (default: 0)
            LeftY,        ///< Left joystick Y axis (default: 0)
            RightX,       ///< Right joystick X axis (default: 0)
            RightY,       ///< Right joystick Y axis (default: 0)
            TriggerLeft,  ///< Left trigger (default: -1)
            TriggerRight, ///< Right trigger (default: -1)
            DPadX,        ///< DPad X axis (default: 0)
            DPadY,        ///< DPad Y Axis (default: 0)

            Count ///< Keep last -- the total number of gamepad axis

        };
#endif

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
        constexpr Gamepad(std::size_t id = 0) noexcept : _id(id), _connected(false), _buttons({false}), _axis({0.f})
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
        ~Gamepad() noexcept = default;

        ///
        /// @brief Get the gamepad id.
        ///
        /// @return std::size_t id.
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-18)
        ///
        [[nodiscard]] constexpr std::size_t getId() const noexcept
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
        constexpr void setId(std::size_t id) noexcept
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
        [[nodiscard]] constexpr bool isConnected() const noexcept
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
        constexpr void setConnected(bool connected) noexcept
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
        /// @throw std::invalid_argument If the button is invalid
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-16)
        ///
        [[nodiscard]] constexpr bool isButtonDown(Button button) const
        {
            assertButtonValid(button);
            return _buttons[static_cast<std::size_t>(button)];
        }

        ///
        /// @brief Check whether a button is up.
        ///
        /// @param[in] button evaluated button.
        ///
        /// @return bool Whether the button is up.
        ///
        /// @throw std::invalid_argument If the button is invalid
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-16)
        ///
        [[nodiscard]] constexpr bool isButtonUp(Button button) const
        {
            assertButtonValid(button);
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
        /// @throw std::invalid_argument If the button is invalid.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-16)
        ///
        constexpr void setButtonState(Button button, bool down)
        {
            assertButtonValid(button);
            _buttons[static_cast<std::size_t>(button)] = down;
        }

        ///
        /// @brief Get the given axis value.
        ///
        /// @param[in] axis evaluated axis.
        ///
        /// @return float axis value.
        ///
        /// @throw std::invalid_argument If the axis is invalid.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-18)
        ///
        [[nodiscard]] constexpr float getAxisValue(Axis axis) const
        {
            assertAxisValid(axis);
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
        /// @throw std::invalid_argument If the axis is invalid.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-18)
        ///
        constexpr void setAxisValue(Axis axis, float value)
        {
            assertAxisValid(axis);
            _axis[static_cast<std::size_t>(axis)] = value;
        }

        ///
        /// @brief Check whether a button is valid.
        ///
        /// @param[in] button evaluated button.
        /// @param[in] allowUnknown Whether the Unknown button is valid or not.
        ///
        /// @throw std::invalid_argument If the button is invalid
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-13)
        ///
        constexpr static void assertButtonValid(Button button, bool allowUnknown = false)
        {
            if (static_cast<std::size_t>(button) >= static_cast<std::size_t>(Button::Count)
                || (!allowUnknown && button == Button::Unknown)) [[unlikely]]
                throw std::invalid_argument("Invalid button");
        }

        ///
        /// @brief Check whether an axis is valid.
        ///
        /// @param[in] axis evaluated axis.
        /// @param[in] allowUnknown Whether the Unknown axis is valid or not.
        ///
        /// @throw std::invalid_argument If the axis is invalid.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-13)
        ///
        constexpr static void assertAxisValid(Axis axis, bool allowUnknown = false)
        {
            if (static_cast<std::size_t>(axis) >= static_cast<std::size_t>(Axis::Count)
                || (!allowUnknown && axis == Axis::Unknown)) [[unlikely]]
                throw std::invalid_argument("Invalid axis");
        }

      private:
        /// Gamepad id.
        std::size_t _id;
        /// Gamepad connection state.
        bool _connected;
        /// Gamepad buttons state.
        std::array<bool, static_cast<std::size_t>(Button::Count)> _buttons;
        /// Gamepad axis values.
        std::array<float, static_cast<std::size_t>(Axis::Count)> _axis;
    };
} // namespace ecstasy::integration::event

#endif /* !ECSTASY_INTEGRATIONS_EVENT_INPUTS_GAMEPAD_HPP_ */
