///
/// @file KeyCombinationListener.cpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-20
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#include "KeyCombinationListener.hpp"
#include "ecstasy/integrations/event/events/KeyEvent.hpp"

namespace ecstasy::integration::event
{
    KeyCombinationListener::KeyCombinationListener(const std::vector<Keyboard::Key> &combination, Callback callback)
        : _combination(combination), _callback(callback)
    {
        reset();
    }

    bool KeyCombinationListener::update(const KeyEvent &event)
    {
        auto it = _keyStates.find(event.key);

        if (it != _keyStates.end() && it->second != event.pressed) {
            if (event.pressed)
                _validatedKeys++;
            else
                _validatedKeys--;
            it->second = event.pressed;
            return isComplete();
        }
        return false;
    }

    bool KeyCombinationListener::isComplete() const
    {
        return _validatedKeys == _combination.size();
    }

    void KeyCombinationListener::reset()
    {
        _validatedKeys = 0;
        _keyStates.clear();
        for (Keyboard::Key key : _combination)
            _keyStates[key] = false;
    }

    void KeyCombinationListener::operator()(Registry &registry, Entity e, bool force)
    {
        if (force || isComplete()) {
            _callback(registry, e, *this);
            reset();
        }
    }

    void KeyCombinationListener::setCombination(const std::vector<Keyboard::Key> &newCombination)
    {
        _combination = newCombination;
        reset();
    }
} // namespace ecstasy::integration::event
