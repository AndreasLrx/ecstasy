///
/// @file Users.cpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-12-02
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#include "Users.hpp"
#include "ActionListener.hpp"
#include "PendingActions.hpp"
#include "ecstasy/registry/modifiers/Maybe.hpp"
#include "ecstasy/registry/modifiers/Or.hpp"

namespace ecstasy::integration::user_action
{
    Users::Users(size_t count) noexcept : _users(count)
    {
        for (size_t i = 1; i < count; i++)
            _users[i].setId(i);
    }

    void Users::updateBindings(Registry &registry) noexcept
    {
        if (!registry.hasResource<Users>())
            return;
        registry.getResource<Users>()->updateBindings();
    }

    void Users::updateBindings() noexcept
    {
        removeOutdatedBindings(_mouseButtonToAction);
        removeOutdatedBindings(_keyToAction);
        removeOutdatedBindings(_gamepadButtonToAction);
        removeOutdatedBindings(_gamepadAxisToAction);

        for (UserProfile &user : _users) {
            auto &bindings = user.getActionBindings().getBindings();

            for (ActionBinding binding : bindings) {
                switch (binding.type) {
                    case ActionBinding::Type::MouseButton:
                        addBindingIfMissing(user, binding, binding.mouseButton, _mouseButtonToAction);
                        break;
                    case ActionBinding::Type::Key: addBindingIfMissing(user, binding, binding.key, _keyToAction); break;
                    case ActionBinding::Type::GamepadButton:
                        addBindingIfMissing(user, binding, binding.gamepadButton, _gamepadButtonToAction);
                        break;
                    case ActionBinding::Type::GamepadAxis:
                        addBindingIfMissing(user, binding, binding.gamepadAxis, _gamepadAxisToAction);
                        break;
                    default: break;
                }
            }
        }
    }

    void Users::callActionListeners(Registry &registry, Action action)
    {
        if (registry.hasResource<PendingActions>())
            registry.getResource<PendingActions>()->get().push(action);

        for (auto [entity, maybeListener, maybeListeners] :
            registry.select<Entities, Maybe<ActionListener>, Maybe<ActionListeners>>()
                .where<Entities, Or<ActionListener, ActionListeners>>()) {
            if (maybeListener) {
                if (maybeListener->get().actionId == Action::All || maybeListener->get().actionId == action.id)
                    maybeListener->get().listener(registry, entity, action);
            } else if (maybeListeners->get().contains(action.id))
                maybeListeners->get()[action.id](registry, entity, action);
        }
    }

    void Users::handleEvent(Registry &registry, const event::Event &e) const
    {
        switch (e.type) {
            case event::Event::Type::MouseButtonPressed:
            case event::Event::Type::MouseButtonReleased:
                callListenersFromMap(
                    registry, _mouseButtonToAction, e.mouseButton.button, static_cast<float>(e.mouseButton.pressed));
                break;

            case event::Event::Type::KeyPressed:
            case event::Event::Type::KeyReleased:
                callListenersFromMap(registry, _keyToAction, e.key.key, static_cast<float>(e.key.pressed));
                break;

            /// @todo Associate @ref UserProfile::Id to gamepad id.
            case event::Event::Type::GamepadButtonPressed:
            case event::Event::Type::GamepadButtonReleased:
                callListenersFromMap(registry, _gamepadButtonToAction, e.gamepadButton.button,
                    static_cast<float>(e.gamepadButton.pressed));
                break;

            /// @todo Associate @ref UserProfile::Id to gamepad id.
            case event::Event::Type::GamepadAxis:
                callListenersFromMap(
                    registry, _gamepadAxisToAction, e.gamepadAxis.axis, static_cast<float>(e.gamepadAxis.value));
                break;
            default: break;
        }
    }
} // namespace ecstasy::integration::user_action
