///
/// @file GamepadCombinationListener.cpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2024-03-27
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#include "GamepadCombinationListener.hpp"
#include "ecstasy/integrations/event/events/GamepadButtonEvent.hpp"

namespace ecstasy::integration::event
{
    GamepadCombinationListener::GamepadCombinationListener(
        const std::vector<Gamepad::Button> &combination, Callback callback, size_t gamepadId)
        : _combination(combination), _callback(callback), _gamepadId(gamepadId)
    {
        reset();
    }

    bool GamepadCombinationListener::update(const GamepadButtonEvent &event)
    {
        if (event.id != _gamepadId)
            return false;

        auto it = _buttonStates.find(event.button);

        if (it != _buttonStates.end() && it->second != event.pressed) {
            if (event.pressed)
                _validatedButtons++;
            else
                _validatedButtons--;
            it->second = event.pressed;
            return isComplete();
        }
        return false;
    }

    bool GamepadCombinationListener::isComplete() const
    {
        return _validatedButtons == _combination.size();
    }

    void GamepadCombinationListener::reset()
    {
        _validatedButtons = 0;
        _buttonStates.clear();
        for (Gamepad::Button button : _combination)
            _buttonStates[button] = false;
    }

    void GamepadCombinationListener::operator()(Registry &registry, Entity e, bool force)
    {
        if (force || isComplete()) {
            _callback(registry, e, *this);
            reset();
        }
    }

    void GamepadCombinationListener::setCombination(const std::vector<Gamepad::Button> &newCombination)
    {
        _combination = newCombination;
        reset();
    }
} // namespace ecstasy::integration::event
