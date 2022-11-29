#include <gtest/gtest.h>
#include "ecstasy/integrations/user_action/UserProfile.hpp"

namespace event = ecstasy::integration::event;
namespace user_action = ecstasy::integration::user_action;

TEST(UserProfile, dump)
{
    {
        user_action::UserProfile user;
        user_action::ActionBindings &bindings = user.getActionBindings();
        std::stringstream ss;
        // clang-format off
        std::string_view expected = "\
id = 0\n\
\n\
[bindings]\n\
Action-0 = [ 'MouseButton->Left', 'Key->A', 'GamepadButton->FaceLeft' ]\n\
Action-1 = [ 'GamepadAxis->TriggerLeft' ]";
        // clang-format on

        bindings.getBindings().push_back(user_action::ActionBinding(0, event::Mouse::Button::Left));
        bindings.getBindings().push_back(user_action::ActionBinding(0, event::Keyboard::Key::A));
        bindings.getBindings().push_back(user_action::ActionBinding(0, event::Gamepad::Button::FaceLeft));
        bindings.getBindings().push_back(user_action::ActionBinding(1, event::Gamepad::Axis::TriggerLeft));
        ss << user.dump();
        GTEST_ASSERT_EQ(ss.str(), expected);
    }
}