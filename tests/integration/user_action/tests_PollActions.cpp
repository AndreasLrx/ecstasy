#include <gtest/gtest.h>
#include "ecstasy/integrations/event/EventsManager.hpp"
#include "ecstasy/integrations/user_action/ActionListener.hpp"
#include "ecstasy/integrations/user_action/PollActions.hpp"
#include "ecstasy/integrations/user_action/Users.hpp"

namespace event = ecstasy::integration::event;
namespace user_action = ecstasy::integration::user_action;

TEST(PollActions, NoPendingActionsResource)
{
    ecstasy::Registry registry;
    registry.addResource<user_action::Users>();

    registry.addSystem<user_action::PollActions<std::integer_sequence<user_action::Action::Id, 0, 1, 2>>>();
    event::EventsManager::handleEvent(registry, event::MouseButtonEvent(event::Mouse::Button::Right, false));
    registry.runSystems();
}

TEST(PollActions, MultipleActions)
{
    ecstasy::Registry registry;
    user_action::Users &users = registry.addResource<user_action::Users>();
    registry.addResource<user_action::PendingActions>();
    registry.addSystem<
        user_action::PollActions<std::integer_sequence<user_action::Action::Id, user_action::Action::All, 0, 1, 2>>>();

    auto &binds = users.getUserProfile().getActionBindings().getBindings();
    binds.emplace_back(0, event::Mouse::Button::Left);
    binds.emplace_back(1, event::Keyboard::Key::G);
    binds.emplace_back(2, event::Gamepad::Button::BumperLeft);
    binds.emplace_back(3, event::Gamepad::Axis::TriggerLeft);
    users.updateBindings();

    float values[5] = {0.f};

    /// Match any action
    registry.entityBuilder()
        .with<user_action::ActionIdListener<user_action::Action::All>>(
            [&values](ecstasy::Registry &r, ecstasy::Entity e, user_action::Action a) {
                (void)r;
                (void)e;
                values[0] += a.value + 1.f;
            })
        .build();
    /// Match Action 0 (mouse)
    registry.entityBuilder()
        .with<user_action::ActionIdListener<0>>(
            [&values](ecstasy::Registry &r, ecstasy::Entity e, user_action::Action a) {
                (void)r;
                (void)e;
                values[1] += a.value + 1.f;
            })
        .build();
    /// Match action 1 (keyboard)
    registry.entityBuilder()
        .with<user_action::ActionIdListener<1>>(
            [&values](ecstasy::Registry &r, ecstasy::Entity e, user_action::Action a) {
                (void)r;
                (void)e;
                values[2] += a.value + 1.f;
            })
        .build();
    /// Match action 2 (gamepad button)
    registry.entityBuilder()
        .with<user_action::ActionIdListener<2>>(
            [&values](ecstasy::Registry &r, ecstasy::Entity e, user_action::Action a) {
                (void)r;
                (void)e;
                values[3] += a.value + 1.f;
            })
        .build();
    /// Match action 3 (gamepad axis)
    registry.entityBuilder()
        .with<user_action::ActionIdListener<3>>(
            [&values](ecstasy::Registry &r, ecstasy::Entity e, user_action::Action a) {
                (void)r;
                (void)e;
                values[4] += a.value + 1.f;
            })
        .build();

    /// Event not handled by the Users::handleEvent
    event::EventsManager::handleEvent(registry, event::MouseMoveEvent(50, 40));
    GTEST_ASSERT_EQ(values[0], 0.f);
    /// Event handled but not matching any action
    event::EventsManager::handleEvent(registry, event::MouseButtonEvent(event::Mouse::Button::Right, false));
    GTEST_ASSERT_EQ(values[0], 0.f);

    /// Mouse button release and pressed event (value += 0 + 1, value += 1 + 1)
    event::EventsManager::handleEvent(registry, event::MouseButtonEvent(event::Mouse::Button::Left, false));
    registry.runSystems();
    GTEST_ASSERT_EQ(values[0], 1.f);
    GTEST_ASSERT_EQ(values[1], 1.f);
    event::EventsManager::handleEvent(registry, event::MouseButtonEvent(event::Mouse::Button::Left, true));
    registry.runSystems();
    GTEST_ASSERT_EQ(values[0], 3.f);
    GTEST_ASSERT_EQ(values[1], 3.f);

    /// Keyboard release and pressed event (value += 0 + 1, value += 1 + 1)
    event::EventsManager::handleEvent(registry, event::KeyEvent(event::Keyboard::Key::G, false));
    registry.runSystems();
    GTEST_ASSERT_EQ(values[0], 4.f);
    GTEST_ASSERT_EQ(values[2], 1.f);
    event::EventsManager::handleEvent(registry, event::KeyEvent(event::Keyboard::Key::G, true));
    registry.runSystems();
    GTEST_ASSERT_EQ(values[0], 6.f);
    GTEST_ASSERT_EQ(values[2], 3.f);

    /// Gamepad button release and pressed event (value += 0 + 1, value += 1 + 1)
    event::EventsManager::handleEvent(
        registry, event::GamepadButtonEvent(0, event::Gamepad::Button::BumperLeft, false));
    registry.runSystems();
    GTEST_ASSERT_EQ(values[0], 7.f);
    GTEST_ASSERT_EQ(values[3], 1.f);
    event::EventsManager::handleEvent(registry, event::GamepadButtonEvent(0, event::Gamepad::Button::BumperLeft, true));
    registry.runSystems();
    GTEST_ASSERT_EQ(values[0], 9.f);
    GTEST_ASSERT_EQ(values[3], 3.f);

    /// Gamepad axis release and pressed event (value += 0.5 + 1)
    event::EventsManager::handleEvent(registry, event::GamepadAxisEvent(0, event::Gamepad::Axis::TriggerLeft, 0.5f));
    registry.runSystems();
    GTEST_ASSERT_EQ(values[0], 10.5f);
    /// No ActionIdListener<3>
    GTEST_ASSERT_EQ(values[4], 0.f);
}