///
/// @file KeyReleasedEvent.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-17
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef ECSTASY_INTEGRATIONS_EVENT_EVENTS_KEYRELEASEDEVENT_HPP_
#define ECSTASY_INTEGRATIONS_EVENT_EVENTS_KEYRELEASEDEVENT_HPP_

#include "KeyEvent.hpp"

namespace ecstasy::integration::event
{
    ///
    /// @brief Event describing a key released.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-11-17)
    ///
    struct KeyReleasedEvent : public KeyEvent {
        ///
        /// @brief Construct a new KeyReleasedEvent object.
        ///
        /// @param[in] pKey Targeted key.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-06)
        ///
        constexpr KeyReleasedEvent(Keyboard::Key pKey) : KeyEvent(pKey, false)
        {
        }
    };
} // namespace ecstasy::integration::event

#endif /* !ECSTASY_INTEGRATIONS_EVENT_EVENTS_KEYRELEASEDEVENT_HPP_ */
