///
/// @file EventsManager.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-06
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef ECSTASY_INTEGRATION_EVENT_EVENTSMANAGER_HPP_
#define ECSTASY_INTEGRATION_EVENT_EVENTSMANAGER_HPP_

#include "events/Event.hpp"

namespace ecstasy
{
    class Registry;
}

namespace ecstasy::integration::event
{
    ///
    /// @brief Static helper class to handle ecstasy events.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-11-06)
    ///
    class EventsManager {
      public:
        ///
        /// @brief Handle an event in a given registry. When an event is send, it update the associated input registry
        /// resources (if any) and then calls the associated event listeners in the registry (if any).
        ///
        /// @param[in] registry Reference to the registry owning the input resources/event listeners components.
        /// @param[in] event Event to handle.
        ///
        /// @return bool Whether the event has been consumed or not.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-06)
        ///
        static bool handleEvent(Registry &registry, const Event &event);

      private:
        EventsManager();
    };
} // namespace ecstasy::integration::event

#endif /* !ECSTASY_INTEGRATION_EVENT_EVENTSMANAGER_HPP_ */
