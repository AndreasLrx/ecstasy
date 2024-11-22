///
/// @file Timer.cpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2024-11-21
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#include "Timer.hpp"
#include <stdexcept>

namespace ecstasy
{
    Timer::Timer()
    {
        _lastTrigger = TimePoint::min();
        setRate(0);
    }

    Timer::Timer(Timer::Interval interval) : Timer()
    {
        setInterval(interval);
    }

    Timer::Timer(std::uint32_t rate) : Timer()
    {
        setRate(rate);
    }

    void Timer::setRate(std::uint32_t rate) noexcept
    {
        _type = Type::Rate;
        if (rate == 0) {
            rate = 1;
        }
        _rate.rate = rate;
        // Reset the countdown to trigger to run the first time
        _rate.triggerCountdown = 0;
    }

    std::uint32_t Timer::getRate() const
    {
        if (_type != Type::Rate)
            throw std::runtime_error("Timer is not of type Rate");
        return _rate.rate;
    }

    void Timer::setInterval(Timer::Interval interval) noexcept
    {
        _type = Type::TimeInterval;
        _timer.interval = interval;
    }

    Timer::Interval Timer::getInterval() const
    {
        if (_type != Type::TimeInterval)
            throw std::runtime_error("Timer is not of type Rate");
        return _timer.interval;
    }

    bool Timer::trigger() noexcept
    {
        switch (_type) {
            case Type::TimeInterval: {
                auto tp = std::chrono::system_clock::now();

                if (tp - _timer.interval >= _lastTrigger) {
                    _lastTrigger = tp;
                    return true;
                }
                return false;
            }
            case Type::Rate: {
                if (_rate.triggerCountdown == 0) {
                    _rate.triggerCountdown = _rate.rate - 1;
                    _lastTrigger = std::chrono::system_clock::now();
                    return true;
                }
                --_rate.triggerCountdown;
                return false;
            }
        }
        return false;
    }
} // namespace ecstasy
