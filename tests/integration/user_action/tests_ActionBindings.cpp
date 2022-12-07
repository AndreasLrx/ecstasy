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

TEST(ActionBinding, dumpInvalidActionType)
{
    user_action::ActionBindings bindings;
    std::stringstream ss;
    // clang-format off
        std::string_view expected = "\
Action-0 = [ 'MouseButton->Left', 'GamepadButton->FaceLeft' ]";
    // clang-format on

    auto invalid = user_action::ActionBinding(0, event::Keyboard::Key::A);
    invalid.type = user_action::ActionBinding::Type::Count;

    bindings.getBindings().push_back(user_action::ActionBinding(0, event::Mouse::Button::Left));
    bindings.getBindings().push_back(invalid);
    bindings.getBindings().push_back(user_action::ActionBinding(0, event::Gamepad::Button::FaceLeft));
    ss << bindings.dump();
    GTEST_ASSERT_EQ(ss.str(), expected);
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

TEST(ActionBinding, loadInvalidActionName)
{
    user_action::ActionBindings bindings;
    // clang-format off
        toml::table in = toml::parse("\
Action-0 = [ 'MouseButton->Left', 'Key->A', 'GamepadButton->FaceLeft' ]\n\
Action-a = [ 'GamepadAxis->TriggerLeft' ]");
    // clang-format on

    bindings.load(in);
    toml::table out = bindings.dump();

    std::string expected = "Action-0 = [ 'MouseButton->Left', 'Key->A', 'GamepadButton->FaceLeft' ]";
    std::stringstream got;

    got << out;
    GTEST_ASSERT_EQ(got.str(), expected);
}

TEST(ActionBinding, loadInvalidActionArray)
{
    user_action::ActionBindings bindings;
    // clang-format off
        toml::table in = toml::parse("\
Action-0 = [ 'MouseButton->Left', 'Key->A', 'GamepadButton->FaceLeft' ]\n\
Action-1 = 'GamepadAxis->TriggerLeft'");
    // clang-format on

    bindings.load(in);
    toml::table out = bindings.dump();

    std::string expected = "Action-0 = [ 'MouseButton->Left', 'Key->A', 'GamepadButton->FaceLeft' ]";
    std::stringstream got;

    got << out;
    GTEST_ASSERT_EQ(got.str(), expected);
}

TEST(ActionBinding, loadInvalidActionValue)
{
    user_action::ActionBindings bindings;
    // clang-format off
        toml::table in = toml::parse("\
Action-0 = [ 'MouseButton->Left', 'Key->A', 'GamepadButton->FaceLeft' ]\n\
Action-1 = ['GamepadAxis->', '->TriggerLeft', 'GamepadAxis-TriggerLeft', 'Count->None']");
    // clang-format on

    bindings.load(in);
    toml::table out = bindings.dump();

    std::string expected = "Action-0 = [ 'MouseButton->Left', 'Key->A', 'GamepadButton->FaceLeft' ]";
    std::stringstream got;

    got << out;
    GTEST_ASSERT_EQ(got.str(), expected);
}

TEST(ActionBinding, contains)
{
    user_action::ActionBindings bindings;
    user_action::ActionBinding searched(0, event::Mouse::Button::Left);
    user_action::ActionBinding notPresent(0, event::Mouse::Button::Right);

    GTEST_ASSERT_FALSE(bindings.contains(searched));
    /// Not created by copy to test equality constructor
    bindings.getBindings().push_back(user_action::ActionBinding(0, event::Mouse::Button::Left));
    GTEST_ASSERT_TRUE(bindings.contains(searched));
    GTEST_ASSERT_FALSE(bindings.contains(notPresent));
}