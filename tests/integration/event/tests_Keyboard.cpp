#include <gtest/gtest.h>
#include "ecstasy/integrations/event/EventsManager.hpp"
#include "ecstasy/integrations/event/events/Event.hpp"
#include "ecstasy/integrations/event/events/KeyEvent.hpp"
#include "ecstasy/integrations/event/events/KeyPressedEvent.hpp"
#include "ecstasy/integrations/event/events/KeyReleasedEvent.hpp"
#include "ecstasy/integrations/event/inputs/Keyboard.hpp"
#include "ecstasy/registry/Registry.hpp"
#include "ecstasy/storages/MapStorage.hpp"

using namespace ecstasy;
namespace event = ecstasy::integration::event;

TEST(Event, KeyPressed)
{
    Registry registry;
    event::Keyboard &keyboardState = registry.addResource<event::Keyboard>();
    int val1 = 0;
    int val2 = 0;

    GTEST_ASSERT_TRUE(keyboardState.isKeyUp(event::Keyboard::Key::A));
    event::EventsManager::handleEvent(registry, event::KeyPressedEvent(event::Keyboard::Key::A));
    GTEST_ASSERT_TRUE(keyboardState.isKeyDown(event::Keyboard::Key::A));

    registry.entityBuilder()
        .with<event::KeyListener>([&val1](Registry &r, Entity e, const event::KeyEvent &event) {
            (void)r;
            (void)e;
            (void)event;
            if (event.pressed)
                val1++;
        })
        .build();
    registry.entityBuilder()
        .with<event::KeyListener>([&val2](Registry &r, Entity e, const event::KeyEvent &event) {
            (void)r;
            (void)e;
            (void)event;
            if (event.pressed)
                val2--;
        })
        .build();

    event::EventsManager::handleEvent(registry, event::KeyPressedEvent(event::Mouse::Key::B));
    GTEST_ASSERT_EQ(val1, 1);
    GTEST_ASSERT_EQ(val2, -1);
}

TEST(Event, KeyReleased)
{
    Registry registry;
    event::Keyboard &keyboardState = registry.addResource<event::Keyboard>();
    int val1 = 0;
    int val2 = 0;

    GTEST_ASSERT_TRUE(keyboardState.isKeyUp(event::Keyboard::Key::A));
    event::EventsManager::handleEvent(registry, event::KeyPressedEvent(event::Keyboard::Key::A));
    GTEST_ASSERT_TRUE(keyboardState.isKeyDown(event::Keyboard::Key::A));

    registry.entityBuilder()
        .with<event::KeyListener>([&val1](Registry &r, Entity e, const event::KeyEvent &event) {
            (void)r;
            (void)e;
            (void)event;
            if (!event.pressed)
                val1++;
        })
        .build();
    registry.entityBuilder()
        .with<event::KeyListener>([&val2](Registry &r, Entity e, const event::KeyEvent &event) {
            (void)r;
            (void)e;
            (void)event;
            if (!event.pressed)
                val2--;
        })
        .build();

    event::EventsManager::handleEvent(registry, event::KeyReleasedEvent(event::Mouse::Key::A));
    GTEST_ASSERT_TRUE(keyboardState.isKeyUp(event::Keyboard::Key::A));
    GTEST_ASSERT_EQ(val1, 1);
    GTEST_ASSERT_EQ(val2, -1);
}
