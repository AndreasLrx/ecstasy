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

#include "MouseButtonPressedEvent.hpp"
#include "MouseButtonReleasedEvent.hpp"

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
        /// @brief Construct a mouse button pressed event wrapper.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-06)
        ///
        constexpr Event(MouseButtonPressedEvent &&event) : type(Type::MouseButtonPressed), mouseButtonPressed(event)
        {
        }

        ///
        /// @brief Construct a mouse button released event wrapper.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-06)
        ///
        constexpr Event(MouseButtonReleasedEvent &&event) : type(Type::MouseButtonReleased), mouseButtonReleased(event)
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
            MouseButtonPressedEvent mouseButtonPressed;   ///< @ref Type::MouseButtonPressed associated event
            MouseButtonReleasedEvent mouseButtonReleased; ///< @ref Type::MouseButtonReleasedEvent associated event
        };
    };
} // namespace ecstasy::integration::event

#endif /* !ECSTASY_INTEGRATION_EVENT_EVENTS_EVENT_HPP_ */
