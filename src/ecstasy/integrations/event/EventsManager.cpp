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
#include "ecstasy/config.hpp"
#include "ecstasy/integrations/event/listeners/EventListeners.hpp"
#include "ecstasy/query/Query.hpp"
#include "ecstasy/registry/Registry.hpp"
#include "ecstasy/registry/modifiers/Maybe.hpp"
#include "ecstasy/registry/modifiers/Or.hpp"
#include "ecstasy/resources/entity/Entities.hpp"
#include "ecstasy/storages/MapStorage.hpp"
#include "events/Event.hpp"
#include "inputs/Gamepads.hpp"
#include "listeners/KeyCombinationListener.hpp"
#include "listeners/KeySequenceListener.hpp"

#ifdef ECSTASY_INTEGRATIONS_USER_ACTION
    #include "ecstasy/integrations/user_action/Users.hpp"
#endif

namespace ecstasy::integration::event
{
    template <typename E>
    static void callListeners(Registry &registry, const E &event)
    {
        for (auto [entity, listener, listeners] :
            registry.select<Entities, Maybe<EventListener<E>>, Maybe<EventListeners<E>>>()
                .template where<Or<EventListener<E>, EventListeners<E>>>()) {
            if (listener)
                listener.value()(registry, entity, event);
            if (listeners)
                listeners.value()(registry, entity, event);
        }
    }

    static void callKeyListeners(Registry &registry, const KeyEvent &event)
    {
        for (auto [entity, listener, listeners, sequence, combination] :
            registry
                .select<Entities, Maybe<EventListener<KeyEvent>>, Maybe<EventListeners<KeyEvent>>,
                    Maybe<KeySequenceListener>, Maybe<KeyCombinationListener>>()
                .where<Or<EventListener<KeyEvent>, EventListeners<KeyEvent>, KeySequenceListener,
                    KeyCombinationListener>>()) {
            if (listener)
                listener.value()(registry, entity, event);
            if (listeners)
                listeners.value()(registry, entity, event);
            if (sequence && sequence.value().get().update(event))
                sequence.value()(registry, entity);
            if (combination && combination.value().get().update(event))
                combination.value()(registry, entity);
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
                callKeyListeners(registry, event.key);

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
            case Event::Type::GamepadAxis:
                callListeners(registry, event.gamepadAxis);

                if (registry.hasResource<Gamepads>())
                    registry.getResource<Gamepads>()
                        .get(event.gamepadAxis.id)
                        .setAxisValue(event.gamepadAxis.axis, event.gamepadAxis.value);
                break;
            default: break;
        }

#ifdef ECSTASY_INTEGRATIONS_USER_ACTION
        if (registry.hasResource<user_action::Users>())
            registry.getResource<user_action::Users>().handleEvent(registry, event);
#endif
    }
} // namespace ecstasy::integration::event
