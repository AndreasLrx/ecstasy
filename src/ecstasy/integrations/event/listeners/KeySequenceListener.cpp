///
/// @file KeySequenceListener.cpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-20
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#include "KeySequenceListener.hpp"
#include "ecstasy/integrations/event/events/KeyEvent.hpp"

namespace ecstasy::integration::event
{
    KeySequenceListener::KeySequenceListener(const std::vector<Keyboard::Key> &sequence, Callback callback)
        : _sequence(sequence), _callback(callback)
    {
        reset();
    }

    bool KeySequenceListener::update(const KeyEvent &event)
    {
        if (event.pressed) {
            if (_heldKey == Keyboard::Key::Unknown && event.key == _sequence.at(_validatedKeys.size()))
                _heldKey = event.key;
            else if (_heldKey != Keyboard::Key::Unknown || !_validatedKeys.empty()) {
                reset();
                /// In case the failing key is the first key of the sequence we must start again the sequence.
                return update(event);
            }
        } else {
            if (_heldKey != Keyboard::Key::Unknown && _heldKey == event.key) {
                _validatedKeys.push_back(_heldKey);
                _heldKey = Keyboard::Key::Unknown;
                return isComplete();
            }
        }
        return false;
    }

    bool KeySequenceListener::isComplete() const
    {
        return _sequence.size() == _validatedKeys.size();
    }

    void KeySequenceListener::operator()(Registry &registry, Entity e, bool force)
    {
        if (force || isComplete()) {
            _callback(registry, e, *this);
            reset();
        }
    }

    void KeySequenceListener::setSequence(const std::vector<Keyboard::Key> &newSequence)
    {
        _sequence = newSequence;
        reset();
    }

    void KeySequenceListener::reset()
    {
        _validatedKeys.clear();
        _heldKey = Keyboard::Key::Unknown;
    }
} // namespace ecstasy::integration::event