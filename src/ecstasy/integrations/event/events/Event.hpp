///
/// @file Event.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-05
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_INTEGRATION_EVENT_EVENTS_EVENT_HPP_
#define ECSTASY_INTEGRATION_EVENT_EVENTS_EVENT_HPP_

#include "GamepadAxisEvent.hpp"
#include "GamepadButtonEvent.hpp"
#include "GamepadConnectedEvent.hpp"
#include "KeyEvent.hpp"
#include "MouseButtonEvent.hpp"
#include "MouseMoveEvent.hpp"
#include "MouseWheelScrollEvent.hpp"
#include "TextEnteredEvent.hpp"

namespace ecstasy::integration::event
{
    ///
    /// @brief Event class. Holds the event type (set to Type::Count if empty) and the associated event struct
    /// containing the appropriate data.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-11-06)
    ///
    struct Event {
        /// @brief Event types.
        enum class Type {
            MouseButtonPressed,    ///< One of the mouse button has been pressed.
            MouseButtonReleased,   ///< One of the mouse button has been released.
            MouseWheelScrolled,    ///< The mouse wheel was scrolled.
            MouseMoved,            ///< The mouse cursor moved.
            KeyPressed,            ///< One of the keyboard key has been pressed.
            KeyReleased,           ///< One of the keyboard key has been released.
            TextEntered,           ///< A character has been entered.
            GamepadButtonPressed,  ///< One of the gamepads button has been pressed.
            GamepadButtonReleased, ///< One of the gamepads button has been released.
            GamepadAxis,           ///< One of the gamepads axis value changed.
            GamepadConnected,      ///< One gamepad has been connected.
            GamepadDisconnected,   ///< One gamepad has been disconnected.

            Count ///< Keep last -- the total number of events
        };

        ///
        /// @brief Construct an empty event.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-06)
        ///
        constexpr Event() : type(Type::Count){};

        ///
        /// @brief Construct a mouse button event wrapper.
        ///
        /// @param[in] event Source event.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-06)
        ///
        constexpr Event(MouseButtonEvent &&event)
            : type(event.pressed ? Type::MouseButtonPressed : Type::MouseButtonReleased), mouseButton(event)
        {
        }

        ///
        /// @brief Construct a mouse wheel scroll event wrapper.
        ///
        /// @param[in] event Source event.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-06)
        ///
        constexpr Event(MouseWheelScrollEvent &&event) : type(Type::MouseWheelScrolled), mouseWheel(event)
        {
        }

        ///
        /// @brief Construct a mouse move event wrapper.
        ///
        /// @param[in] event Source event.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-06)
        ///
        constexpr Event(MouseMoveEvent &&event) : type(Type::MouseMoved), mouseMove(event)
        {
        }

        ///
        /// @brief Construct a key event wrapper.
        ///
        /// @param[in] event Source event.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-06)
        ///
        constexpr Event(KeyEvent &&event) : type(event.pressed ? Type::KeyPressed : Type::KeyReleased), key(event)
        {
        }

        ///
        /// @brief Construct a text entered event wrapper.
        ///
        /// @param[in] event Source event.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-06)
        ///
        constexpr Event(TextEnteredEvent &&event) : type(Type::TextEntered), text(event)
        {
        }

        ///
        /// @brief Construct a gamepad button event wrapper.
        ///
        /// @param[in] event Source event.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-06)
        ///
        constexpr Event(GamepadButtonEvent &&event)
            : type(event.pressed ? Type::GamepadButtonPressed : Type::GamepadButtonReleased), gamepadButton(event)
        {
        }

        ///
        /// @brief Construct a gamepad connected event wrapper.
        ///
        /// @param[in] event Source event.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-06)
        ///
        constexpr Event(GamepadConnectedEvent &&event)
            : type(event.connected ? Type::GamepadConnected : Type::GamepadDisconnected), gamepadConnected(event)
        {
        }

        ///
        /// @brief Construct a gamepad axis event wrapper.
        ///
        /// @param[in] event Source event.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-06)
        ///
        constexpr Event(GamepadAxisEvent &&event) : type(Type::GamepadAxis), gamepadAxis(event)
        {
        }

        ///
        /// @brief Default desctructor.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-06)
        ///
        ~Event() = default;

        ///
        /// @brief Type of the event stored.
        ///
        /// @note @ref Type::Count means the event is invalid (empty).
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-06)
        ///
        Type type;

        ///
        /// @brief Anonymous union to ease facilitate the wrapped event access.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-06)
        ///
        union {
            MouseButtonEvent
                mouseButton; ///< @ref Type::MouseButtonPressed && @ref Type::MouseButtonReleased associated events.
            MouseWheelScrollEvent mouseWheel; ///< @ref Type::MouseWheelScrolled associated event.
            MouseMoveEvent mouseMove;         ///< @ref Type::MouseMoved associated event.
            KeyEvent key;                     ///< @ref Type::KeyPressed && @ref Type::KeyReleased associated events.
            TextEnteredEvent text;            ///< @ref Type::TextEntered associated event.
            GamepadButtonEvent gamepadButton; ///< @ref Type::GamepadButtonPressed && @ref Type::GamepadButtonReleased
            ///< associated events.
            GamepadAxisEvent gamepadAxis; ///< @ref Type::GamepadAxis associated event.
            GamepadConnectedEvent
                gamepadConnected; ///< @ref Type::GamepadConnected && @ref Type::GamepadDisconnected associated events.
        };
    };
} // namespace ecstasy::integration::event

#endif /* !ECSTASY_INTEGRATION_EVENT_EVENTS_EVENT_HPP_ */
