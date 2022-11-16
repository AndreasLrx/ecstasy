///
/// @file KeyEvent.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-17
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef ECSTASY_INTEGRATIONS_EVENT_EVENTS_KEYEVENT_HPP_
#define ECSTASY_INTEGRATIONS_EVENT_EVENTS_KEYEVENT_HPP_

#include "ecstasy/integrations/event/inputs/Keyboard.hpp"

namespace ecstasy::integration::event
{
    ///
    /// @brief Event describing a key pressed or released.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-11-17)
    ///
    struct KeyEvent {
        /// @brief Target key.
        Keyboard::Key key;
        /// @brief Whether the key is pressed (down) or not (up).
        bool pressed;

        ///
        /// @brief Construct a new KeyEvent object.
        ///
        /// @param[in] pKey Targeted key.
        /// @param[in] pPressed Whether the key is pressed or not.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-06)
        ///
        constexpr KeyEvent(Keyboard::Key pKey, bool pPressed = true) : key(pKey), pressed(pPressed)
        {
        }
    };
} // namespace ecstasy::integration::event

#endif /* !ECSTASY_INTEGRATIONS_EVENT_EVENTS_KEYEVENT_HPP_ */
