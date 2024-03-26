///
/// @file GamepadConnectedEvent.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-18
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_INTEGRATIONS_EVENT_EVENTS_GAMEPADCONNECTEDEVENT_HPP_
#define ECSTASY_INTEGRATIONS_EVENT_EVENTS_GAMEPADCONNECTEDEVENT_HPP_

#include <cstdint>

namespace ecstasy::integration::event
{
    ///
    /// @brief Event describing a gamepad connection or disconnection.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-11-18)
    ///
    struct GamepadConnectedEvent {
        /// @brief Target gamepad id.
        std::size_t id;
        /// @brief Whether the gamepad has been connected or disconnected.
        bool connected;

        ///
        /// @brief Construct a new GamepadConnectedEvent object.
        ///
        /// @param[in] pId Target gamepad id.
        /// @param[in] pConnected Whether the gamepad has been connected or disconnected.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-18)
        ///
        constexpr GamepadConnectedEvent(std::size_t pId, bool pConnected) : id(pId), connected(pConnected)
        {
        }
    };
} // namespace ecstasy::integration::event

#endif /* !ECSTASY_INTEGRATIONS_EVENT_EVENTS_GAMEPADCONNECTEDEVENT_HPP_ */
