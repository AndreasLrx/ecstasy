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
#include "ecstasy/integrations/event/listeners/KeyListener.hpp"
#include "ecstasy/integrations/event/listeners/MouseButtonListener.hpp"
#include "ecstasy/integrations/event/listeners/MouseMoveListener.hpp"
#include "ecstasy/integrations/event/listeners/MouseWheelScrollListener.hpp"
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
            case Event::Type::MouseButtonReleased:
                CALL_LISTENERS(MouseButtonListener, event.mouseButton)

                if (registry.hasResource<Mouse>())
                    registry.getResource<Mouse>().setButtonState(event.mouseButton.button, event.mouseButton.pressed);
                break;
            case Event::Type::MouseWheelScrolled: CALL_LISTENERS(MouseWheelScrollListener, event.mouseWheel) break;
            case Event::Type::MouseMoved:
                CALL_LISTENERS(MouseMoveListener, event.mouseMove)

                if (registry.hasResource<Mouse>()) {
                    Mouse &mouse = registry.getResource<Mouse>();
                    mouse.setPosition(event.mouseMove.x + mouse.getX(), event.mouseMove.y + mouse.getY());
                }
                break;
            case Event::Type::KeyPressed:
            case Event::Type::KeyReleased:
                CALL_LISTENERS(KeyListener, event.key)

                if (registry.hasResource<Keyboard>())
                    registry.getResource<Keyboard>().setKeyState(event.key.key, event.key.pressed);
                break;
            default: break;
        }
    }
} // namespace ecstasy::integration::event