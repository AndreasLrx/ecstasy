#include <gtest/gtest.h>
#include "ecstasy/integrations/event/EventsManager.hpp"
#include "ecstasy/integrations/event/events/Event.hpp"
#include "ecstasy/integrations/event/events/GamepadAxisEvent.hpp"
#include "ecstasy/integrations/event/events/GamepadButtonPressedEvent.hpp"
#include "ecstasy/integrations/event/events/GamepadButtonReleasedEvent.hpp"
#include "ecstasy/integrations/event/inputs/Gamepads.hpp"
#include "ecstasy/integrations/event/listeners/GamepadAxisListener.hpp"
#include "ecstasy/integrations/event/listeners/GamepadButtonListener.hpp"
#include "ecstasy/integrations/event/listeners/GamepadConnectedListener.hpp"
#include "ecstasy/registry/Registry.hpp"
#include "ecstasy/storages/MapStorage.hpp"

using namespace ecstasy;
namespace event = ecstasy::integration::event;

TEST(Event, GamepadButtonPressed)
{
    Registry registry;
    int val1 = 0;
    int val2 = 0;

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

    event::Gamepads &gamepadsState = registry.addResource<event::Gamepads>();
    GTEST_ASSERT_TRUE(gamepadsState.get(0).isButtonUp(event::Gamepad::Button::FaceDown));
    event::EventsManager::handleEvent(registry, event::GamepadButtonPressedEvent(0, event::Gamepad::Button::FaceDown));
    GTEST_ASSERT_TRUE(gamepadsState.get(0).isButtonDown(event::Gamepad::Button::FaceDown));

    GTEST_ASSERT_EQ(val1, 2);
    GTEST_ASSERT_EQ(val2, -2);
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

TEST(Event, GamepadConnected)
{
    Registry registry;
    bool connected = false;

    registry.entityBuilder()
        .with<event::GamepadConnectedListener>(
            [&connected](Registry &r, Entity e, const event::GamepadConnectedEvent &event) {
                (void)r;
                (void)e;
                connected = event.connected;
            })
        .build();
    GTEST_ASSERT_FALSE(connected);
    event::EventsManager::handleEvent(registry, event::GamepadConnectedEvent(0, true));
    GTEST_ASSERT_TRUE(connected);

    event::Gamepads &gamepadsState = registry.addResource<event::Gamepads>();
    GTEST_ASSERT_FALSE(gamepadsState.get(0).isConnected());
    event::EventsManager::handleEvent(registry, event::GamepadConnectedEvent(0, true));
    GTEST_ASSERT_TRUE(gamepadsState.get(0).isConnected());
    event::EventsManager::handleEvent(registry, event::GamepadConnectedEvent(0, false));
    GTEST_ASSERT_FALSE(gamepadsState.get(0).isConnected());
}

TEST(Event, GamepadAxis)
{
    Registry registry;
    float value = 0.f;

    registry.entityBuilder()
        .with<event::GamepadAxisListener>([&value](Registry &r, Entity e, const event::GamepadAxisEvent &event) {
            (void)r;
            (void)e;
            value = event.value;
        })
        .build();
    GTEST_ASSERT_EQ(value, 0.f);
    event::EventsManager::handleEvent(registry, event::GamepadAxisEvent(0, event::Gamepad::Axis::LeftX, 0.6f));
    GTEST_ASSERT_EQ(value, 0.6f);

    event::Gamepads &gamepadsState = registry.addResource<event::Gamepads>();
    GTEST_ASSERT_EQ(gamepadsState.get(0).getAxisValue(event::Gamepad::Axis::TriggerLeft), -1.f);
    event::EventsManager::handleEvent(registry, event::GamepadAxisEvent(0, event::Gamepad::Axis::TriggerLeft, 0.3f));
    GTEST_ASSERT_EQ(gamepadsState.get(0).getAxisValue(event::Gamepad::Axis::TriggerLeft), 0.3f);
}

TEST(Gamepad, outputStreamOperators)
{
    for (int i = static_cast<int>(event::Gamepad::Button::Unknown); i < static_cast<int>(event::Gamepad::Button::Count);
         i++) {
        std::stringstream ss;

        ss << static_cast<event::Gamepad::Button>(i);
        GTEST_ASSERT_EQ(ss.str(), event::Gamepad::getButtonName(static_cast<event::Gamepad::Button>(i)));
    }
    GTEST_ASSERT_EQ(event::Gamepad::getButtonName(event::Gamepad::Button::Count), nullptr);

    for (int i = static_cast<int>(event::Gamepad::Axis::Unknown); i < static_cast<int>(event::Gamepad::Axis::Count);
         i++) {
        std::stringstream ss;

        ss << static_cast<event::Gamepad::Axis>(i);
        GTEST_ASSERT_EQ(ss.str(), event::Gamepad::getAxisName(static_cast<event::Gamepad::Axis>(i)));
    }
    GTEST_ASSERT_EQ(event::Gamepad::getAxisName(event::Gamepad::Axis::Count), nullptr);
}