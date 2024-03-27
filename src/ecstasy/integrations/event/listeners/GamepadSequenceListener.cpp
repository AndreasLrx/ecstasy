///
/// @file GamepadSequenceListener.cpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2024-03-27
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#include "GamepadSequenceListener.hpp"
#include "ecstasy/integrations/event/events/GamepadButtonEvent.hpp"

namespace ecstasy::integration::event
{
    GamepadSequenceListener::GamepadSequenceListener(
        const std::vector<Gamepad::Button> &sequence, Callback callback, size_t gamepadId)
        : _sequence(sequence), _callback(callback), _gamepadId(gamepadId)
    {
        reset();
    }

    bool GamepadSequenceListener::update(const GamepadButtonEvent &event)
    {
        if (event.id != _gamepadId)
            return false;

        if (event.pressed) {
            if (_heldButton == Gamepad::Button::Unknown && event.button == _sequence.at(_validatedButtons.size()))
                _heldButton = event.button;
            else if (_heldButton != Gamepad::Button::Unknown || !_validatedButtons.empty()) {
                reset();
                /// In case the failing key is the first key of the sequence we must start again the sequence.
                return update(event);
            }
        } else {
            if (_heldButton != Gamepad::Button::Unknown && _heldButton == event.button) {
                _validatedButtons.push_back(_heldButton);
                _heldButton = Gamepad::Button::Unknown;
                return isComplete();
            }
        }
        return false;
    }

    bool GamepadSequenceListener::isComplete() const
    {
        return _sequence.size() == _validatedButtons.size();
    }

    void GamepadSequenceListener::operator()(Registry &registry, Entity e, bool force)
    {
        if (force || isComplete()) {
            _callback(registry, e, *this);
            reset();
        }
    }

    void GamepadSequenceListener::setSequence(const std::vector<Gamepad::Button> &newSequence)
    {
        _sequence = newSequence;
        reset();
    }

} // namespace ecstasy::integration::event
