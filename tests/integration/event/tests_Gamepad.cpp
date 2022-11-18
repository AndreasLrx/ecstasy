#include <gtest/gtest.h>
#include "ecstasy/integrations/event/EventsManager.hpp"
#include "ecstasy/integrations/event/events/Event.hpp"
#include "ecstasy/integrations/event/events/GamepadButtonPressedEvent.hpp"
#include "ecstasy/integrations/event/events/GamepadButtonReleasedEvent.hpp"
#include "ecstasy/integrations/event/inputs/Gamepads.hpp"
#include "ecstasy/integrations/event/listeners/GamepadButtonListener.hpp"
#include "ecstasy/registry/Registry.hpp"
#include "ecstasy/storages/MapStorage.hpp"

using namespace ecstasy;
namespace event = ecstasy::integration::event;

TEST(Event, GamepadButtonPressed)
{
    Registry registry;
    event::Gamepads &gamepadsState = registry.addResource<event::Gamepads>();
    int val1 = 0;
    int val2 = 0;

    GTEST_ASSERT_TRUE(gamepadsState.get(0).isButtonUp(event::Gamepad::Button::FaceDown));
    event::EventsManager::handleEvent(registry, event::GamepadButtonPressedEvent(0, event::Gamepad::Button::FaceDown));
    GTEST_ASSERT_TRUE(gamepadsState.get(0).isButtonDown(event::Gamepad::Button::FaceDown));

    registry.entityBuilder()
        .with<event::GamepadButtonListener>([&val1](Registry &r, Entity e, const event::GamepadButtonEvent &event) {
            (void)r;
            (void)e;
            (void)event;
            if (event.pressed)
                val1++;
        })
        .build();
    registry.entityBuilder()
        .with<event::GamepadButtonListener>([&val2](Registry &r, Entity e, const event::GamepadButtonEvent &event) {
            (void)r;
            (void)e;
            (void)event;
            if (event.pressed)
                val2--;
        })
        .build();

    event::EventsManager::handleEvent(registry, event::GamepadButtonPressedEvent(0, event::Gamepad::Button::FaceDown));
    GTEST_ASSERT_EQ(val1, 1);
    GTEST_ASSERT_EQ(val2, -1);
}

TEST(Event, GamepadButtonReleased)
{
    Registry registry;
    event::Gamepads &gamepadsState = registry.addResource<event::Gamepads>();
    int val1 = 0;
    int val2 = 0;

    GTEST_ASSERT_TRUE(gamepadsState.get(0).isButtonUp(event::Gamepad::Button::FaceDown));
    event::EventsManager::handleEvent(registry, event::GamepadButtonPressedEvent(0, event::Gamepad::Button::FaceDown));
    GTEST_ASSERT_TRUE(gamepadsState.get(0).isButtonDown(event::Gamepad::Button::FaceDown));

    registry.entityBuilder()
        .with<event::GamepadButtonListener>([&val1](Registry &r, Entity e, const event::GamepadButtonEvent &event) {
            (void)r;
            (void)e;
            (void)event;
            if (!event.pressed)
                val1++;
        })
        .build();
    registry.entityBuilder()
        .with<event::GamepadButtonListener>([&val2](Registry &r, Entity e, const event::GamepadButtonEvent &event) {
            (void)r;
            (void)e;
            (void)event;
            if (!event.pressed)
                val2--;
        })
        .build();

    event::EventsManager::handleEvent(registry, event::GamepadButtonReleasedEvent(0, event::Gamepad::Button::FaceDown));
    GTEST_ASSERT_TRUE(gamepadsState.get(0).isButtonUp(event::Gamepad::Button::FaceDown));
    GTEST_ASSERT_EQ(val1, 1);
    GTEST_ASSERT_EQ(val2, -1);
}
