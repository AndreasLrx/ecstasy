///
/// @file Timer.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2024-11-22
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_SYSTEM_TIMER_HPP_
#define ECSTASY_SYSTEM_TIMER_HPP_

#include <chrono>
#include <cstdint>
#include <ctime>

namespace ecstasy
{
    /// @brief Forward declaration of Registry class.
    class Registry;

    ///
    /// @brief Timer class to control the execution of systems.
    ///
    /// Timers can be assigned to systems or phases to control their execution.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-11-22)
    ///
    class Timer {
      public:
        /// @brief Type of time points.
        using TimePoint = std::chrono::time_point<std::chrono::system_clock>;
        /// @brief Type of time intervals.
        using Interval = std::chrono::milliseconds;

      private:
        struct TimeInterval {
            Interval interval;
        };

        struct Rate {
            std::uint32_t rate;
            std::uint32_t triggerCountdown;
        };

      public:
        ///
        /// @brief Possible types of timers.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-11-22)
        ///
        enum class Type {
            TimeInterval, ///< Timer that triggers every time interval. Ex: every 5 seconds.
            Rate,         ///< Timer that triggers at a fixed rate. Ex: every 5 frames.
        };

        ///
        /// @brief Construct a new Timer.
        ///
        /// @note The default timer is a Rate with a 0 interval, ie run every frame.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-11-22)
        ///
        Timer();

        ///
        /// @brief Construct a new Timer with the given interval.
        ///
        /// @param[in] interval Interval of the timer.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-11-22)
        ///
        Timer(Interval interval);

        ///
        /// @brief Construct a new Timer with the given rate.
        ///
        /// @param[in] rate Rate of the timer.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-11-22)
        ///
        Timer(std::uint32_t rate);

        ///
        /// @brief Copy constructor.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-11-22)
        ///
        Timer(const Timer &) = default;

        ///
        /// @brief Move constructor.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-11-22)
        ///
        Timer(Timer &&) = default;

        ///
        /// @brief Destroy the Timer
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-11-22)
        ///
        ~Timer() = default;

        ///
        /// @brief Copy assignment operator.
        ///
        /// @return Timer&
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-11-22)
        ///
        Timer &operator=(const Timer &) = default;

        ///
        /// @brief Move assignment operator.
        ///
        /// @return Timer&
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-11-22)
        ///
        Timer &operator=(Timer &&) = default;

        ///
        /// @brief Get the Type of the timer.
        ///
        /// @return Type Type of the timer.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-11-22)
        ///
        [[nodiscard]] constexpr Type getType() const noexcept
        {
            return _type;
        }

        ///
        /// @brief Get the last time the timer was triggered.
        ///
        /// @return TimePoint Last time the timer was triggered.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-11-22)
        ///
        [[nodiscard]] constexpr TimePoint getLastTrigger() const noexcept
        {
            return _lastTrigger;
        }

        ///
        /// @brief Set the Rate of the timer. This will convert the timer to a Rate timer.
        ///
        /// @param[in] rate Rate of the timer.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-11-22)
        ///
        void setRate(std::uint32_t rate) noexcept;

        ///
        /// @brief Get the Rate of the timer.
        ///
        /// @return std::uint32_t Rate of the timer.
        ///
        /// @throw std::runtime_error If the timer is not a Rate timer.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-11-22)
        ///
        [[nodiscard]] std::uint32_t getRate() const;

        ///
        /// @brief Set the Interval of the timer. This will convert the timer to an Interval timer.
        ///
        /// @param[in] interval Interval of the timer.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-11-22)
        ///
        void setInterval(Interval interval) noexcept;

        ///
        /// @brief Get the Interval of the timer.
        ///
        /// @return Interval Interval of the timer.
        ///
        /// @throw std::runtime_error If the timer is not an Interval timer.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-11-22)
        ///
        [[nodiscard]] Interval getInterval() const;

        ///
        /// @brief Trigger the timer if it is time to do so.
        ///
        /// @warning This does not call the system or phase, it only checks if it is allowed to run now.
        ///
        /// @return bool @c true if the timer was triggered, @c false otherwise.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-11-22)
        ///
        [[nodiscard]] bool trigger() noexcept;

      private:
        /// @brief Type of the timer.
        Type _type;
        /// @brief Last time the timer was triggered.
        TimePoint _lastTrigger;
        /// @brief Interval or rate of the timer.
        union {
            TimeInterval _timer;
            Rate _rate;
        };
    };
} // namespace ecstasy

#endif /* !ECSTASY_SYSTEM_TIMER_HPP_ */
