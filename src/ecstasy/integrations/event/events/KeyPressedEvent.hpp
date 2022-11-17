///
/// @file KeyPressedEvent.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-17
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef ECSTASY_INTEGRATIONS_EVENT_EVENTS_KEYPRESSEDEVENT_HPP_
#define ECSTASY_INTEGRATIONS_EVENT_EVENTS_KEYPRESSEDEVENT_HPP_

#include "KeyEvent.hpp"

namespace ecstasy::integration::event
{
    ///
    /// @brief Event describing a key pressed.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-11-17)
    ///
    struct KeyPressedEvent : public KeyEvent {
        ///
        /// @brief Construct a new KeyPressedEvent object.
        ///
        /// @param[in] pKey Targeted key.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-06)
        ///
        constexpr KeyPressedEvent(Keyboard::Key pKey) : KeyEvent(pKey, true)
        {
        }
    };
} // namespace ecstasy::integration::event

#endif /* !ECSTASY_INTEGRATIONS_EVENT_EVENTS_KEYPRESSEDEVENT_HPP_ */
