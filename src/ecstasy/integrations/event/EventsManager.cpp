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
#include "ecstasy/integrations/event/listeners/EventListeners.hpp"
#include "ecstasy/query/Query.hpp"
#include "ecstasy/registry/Registry.hpp"
#include "ecstasy/registry/modifiers/Maybe.hpp"
#include "ecstasy/resources/entity/Entities.hpp"
#include "ecstasy/storages/MapStorage.hpp"
#include "events/Event.hpp"
#include "inputs/Gamepads.hpp"

namespace ecstasy::integration::event
{
    template <typename E>
    constexpr void callListeners(Registry &registry, const E &event)
    {
        /// Stack allocation instead of dynamic using allocator
        auto &entities = registry.getEntities();
        auto maybeListener =
            typename ecstasy::Maybe<EventListener<E>>::Modifier(registry.getStorageSafe<EventListener<E>>());
        auto maybeListeners =
            typename ecstasy::Maybe<EventListeners<E>>::Modifier(registry.getStorageSafe<EventListeners<E>>());

        for (auto [entity, listener, listeners] : ecstasy::query::Query(entities, maybeListener, maybeListeners)) {
            if (listener)
                listener.value()(registry, entity, event);
            if (listeners)
                listeners.value()(registry, entity, event);
        }
    }

    void EventsManager::handleEvent(Registry &registry, const Event &event)
    {
        switch (event.type) {
            case Event::Type::MouseButtonPressed:
            case Event::Type::MouseButtonReleased:
                callListeners(registry, event.mouseButton);

                if (registry.hasResource<Mouse>())
                    registry.getResource<Mouse>().setButtonState(event.mouseButton.button, event.mouseButton.pressed);
                break;
            case Event::Type::MouseWheelScrolled: callListeners(registry, event.mouseWheel); break;
            case Event::Type::MouseMoved:
                callListeners(registry, event.mouseMove);

                if (registry.hasResource<Mouse>()) {
                    Mouse &mouse = registry.getResource<Mouse>();
                    mouse.setPosition(event.mouseMove.x + mouse.getX(), event.mouseMove.y + mouse.getY());
                }
                break;
            case Event::Type::KeyPressed:
            case Event::Type::KeyReleased:
                callListeners(registry, event.key);

                if (registry.hasResource<Keyboard>())
                    registry.getResource<Keyboard>().setKeyState(event.key.key, event.key.pressed);
                break;
            case Event::Type::TextEntered: callListeners(registry, event.text); break;
            case Event::Type::GamepadButtonPressed:
            case Event::Type::GamepadButtonReleased:
                callListeners(registry, event.gamepadButton);

                if (registry.hasResource<Gamepads>())
                    registry.getResource<Gamepads>()
                        .get(event.gamepadButton.id)
                        .setButtonState(event.gamepadButton.button, event.gamepadButton.pressed);
                break;
            case Event::Type::GamepadConnected:
            case Event::Type::GamepadDisconnected:
                callListeners(registry, event.gamepadConnected);

                if (registry.hasResource<Gamepads>())
                    registry.getResource<Gamepads>()
                        .get(event.gamepadConnected.id)
                        .setConnected(event.gamepadConnected.connected);
                break;
            default: break;
        }
    }
} // namespace ecstasy::integration::event