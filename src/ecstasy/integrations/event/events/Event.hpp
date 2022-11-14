///
/// @file Event.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-05
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef ECSTASY_INTEGRATION_EVENT_EVENTS_EVENT_HPP_
#define ECSTASY_INTEGRATION_EVENT_EVENTS_EVENT_HPP_

#include "MouseButtonEvent.hpp"
#include "MouseWheelScrollEvent.hpp"

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
            MouseButtonPressed,  ///< One of the mouse button has been pressed.
            MouseButtonReleased, ///< One of the mouse button has been released.
            MouseWheelScrolled,  ///< The mouse wheel was scrolled.

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
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-06)
        ///
        constexpr Event(MouseWheelScrollEvent &&event) : type(Type::MouseWheelScrolled), mouseWheel(event)
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
                mouseButton; ///< @ref Type::MouseButtonPressed && @ref Type::MouseButtonReleased associated event
            MouseWheelScrollEvent mouseWheel; ///< @ref Type::MouseWheelScrolled associated event.
        };
    };
} // namespace ecstasy::integration::event

#endif /* !ECSTASY_INTEGRATION_EVENT_EVENTS_EVENT_HPP_ */
