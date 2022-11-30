#include <gtest/gtest.h>
#include "ecstasy/integrations/user_action/ActionBindings.hpp"

namespace event = ecstasy::integration::event;
namespace user_action = ecstasy::integration::user_action;

TEST(ActionBinding, dump)
{
    {
        user_action::ActionBindings bindings;
        std::stringstream ss;
        // clang-format off
        std::string_view expected = "\
Action-0 = [ 'MouseButton->Left', 'Key->A', 'GamepadButton->FaceLeft' ]\n\
Action-1 = [ 'GamepadAxis->TriggerLeft' ]";
        // clang-format on

        bindings.getBindings().push_back(user_action::ActionBinding(0, event::Mouse::Button::Left));
        bindings.getBindings().push_back(user_action::ActionBinding(0, event::Keyboard::Key::A));
        bindings.getBindings().push_back(user_action::ActionBinding(0, event::Gamepad::Button::FaceLeft));
        bindings.getBindings().push_back(user_action::ActionBinding(1, event::Gamepad::Axis::TriggerLeft));
        ss << bindings.dump();
        GTEST_ASSERT_EQ(ss.str(), expected);
    }
}

TEST(ActionBinding, load)
{
    {
        user_action::ActionBindings bindings;
        // clang-format off
        toml::table in = toml::parse("\
Action-0 = [ 'MouseButton->Left', 'Key->A', 'GamepadButton->FaceLeft' ]\n\
Action-1 = [ 'GamepadAxis->TriggerLeft' ]");
        // clang-format on

        bindings.load(in);
        toml::table out = bindings.dump();

        std::stringstream expected;
        std::stringstream got;

        expected << in;
        got << out;
        GTEST_ASSERT_EQ(got.str(), expected.str());
    }
}