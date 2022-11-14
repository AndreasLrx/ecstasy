#include <gtest/gtest.h>
#include "ecstasy/integrations/event/EventsManager.hpp"
#include "ecstasy/integrations/event/events/Event.hpp"
#include "ecstasy/integrations/event/inputs/Mouse.hpp"
#include "ecstasy/integrations/event/listeners/MouseButtonPressedListener.hpp"
#include "ecstasy/registry/Registry.hpp"
#include "ecstasy/storages/MapStorage.hpp"

using namespace ecstasy;
namespace event = ecstasy::integration::event;

TEST(Event, MouseButtonPressed)
{
    Registry registry;
    event::Mouse &mouseState = registry.addResource<event::Mouse>();
    int val1 = 0;
    int val2 = 0;

    GTEST_ASSERT_TRUE(mouseState.isButtonUp(event::Mouse::Button::Left));
    event::EventsManager::handleEvent(registry, event::MouseButtonPressedEvent(event::Mouse::Button::Left));
    GTEST_ASSERT_TRUE(mouseState.isButtonDown(event::Mouse::Button::Left));

    registry.entityBuilder()
        .with<event::MouseButtonPressedListener>(
            [&val1](Registry &r, Entity e, const event::MouseButtonPressedEvent &event) {
                (void)r;
                (void)e;
                (void)event;
                val1++;
            })
        .build();
    registry.entityBuilder()
        .with<event::MouseButtonPressedListener>(
            [&val2](Registry &r, Entity e, const event::MouseButtonPressedEvent &event) {
                (void)r;
                (void)e;
                (void)event;
                val2--;
            })
        .build();

    event::EventsManager::handleEvent(registry, event::MouseButtonPressedEvent(event::Mouse::Button::Left));
    GTEST_ASSERT_EQ(val1, 1);
    GTEST_ASSERT_EQ(val2, -1);
}
