///
/// @file EventsManager.cpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-06
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#include "EventsManager.hpp"
#include "ecstasy/integrations/event/listeners/MouseButtonPressedListener.hpp"
#include "ecstasy/integrations/event/listeners/MouseButtonReleasedListener.hpp"
#include "ecstasy/registry/Registry.hpp"
#include "ecstasy/resources/entity/Entities.hpp"
#include "ecstasy/storages/MapStorage.hpp"
#include "events/Event.hpp"

// clang-format off
#define CALL_LISTENERS(listenerType, e)                                                        \
    for (auto [entity, listener] : registry.query<Entities, listenerType>())                   \
        listener(registry, entity, e);

// clang-format on

namespace ecstasy::integration::event
{
    void EventsManager::handleEvent(Registry &registry, const Event &event)
    {
        switch (event.type) {
            case Event::Type::MouseButtonPressed:
                if (registry.hasResource<Mouse>())
                    registry.getResource<Mouse>().setButtonState(event.mouseButtonPressed.button, true);

                CALL_LISTENERS(MouseButtonPressedListener, event.mouseButtonPressed)
                break;
            case Event::Type::MouseButtonReleased:
                if (registry.hasResource<Mouse>())
                    registry.getResource<Mouse>().setButtonState(event.mouseButtonReleased.button, false);

                CALL_LISTENERS(MouseButtonReleasedListener, event.mouseButtonReleased)
                break;
            default: break;
        }
    }
} // namespace ecstasy::integration::event