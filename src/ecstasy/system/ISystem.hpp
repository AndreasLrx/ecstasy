///
/// @file ISystem.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief System interface, base class of all systems.
/// @version 1.0.0
/// @date 2022-10-17
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_SYSTEM_ISYSTEM_HPP_
#define ECSTASY_SYSTEM_ISYSTEM_HPP_

#include "ecstasy/system/Timer.hpp"

namespace ecstasy
{
    /// @brief Forward declaration of Registry class.
    class Registry;

    ///
    /// @brief System interface, base class of all systems.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-17)
    ///
    class ISystem {
      public:
        /// @brief Default destructor.
        virtual ~ISystem() = default;

        ///
        /// @brief Run the system.
        ///
        /// @param[in] registry Reference to the registry running the system.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-17)
        ///
        virtual void run(Registry &registry) = 0;

        ///
        /// @brief Get the system timer.
        ///
        /// @note The timer is used to control the execution of the system. You can also use
        /// @ref ecstasy::Pipeline::Phase "Phase" scale timers.
        ///
        /// @return Timer& Reference to the system timer.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-11-22)
        ///
        [[nodiscard]] constexpr Timer &getTimer() noexcept
        {
            return _timer;
        }

        ///
        /// @brief Get the system timer.
        ///
        /// @note The timer is used to control the execution of the system. You can also use
        /// @ref ecstasy::Pipeline::Phase "Phase" scale timers.
        ///
        /// @return const Timer& Reference to the system timer.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-11-22)
        ///
        [[nodiscard]] constexpr const Timer &getTimer() const noexcept
        {
            return _timer;
        }

        ///
        /// @brief Whether the system is enabled.
        ///
        /// @author Andréas Leroux
        /// @since 1.0.0 (2025-05-02)
        ///
        [[nodiscard]] constexpr bool enabled() const noexcept
        {
            return !_disabled;
        }

        ///
        /// @brief Disable the system.
        ///
        /// @author Andréas Leroux
        /// @since 1.0.0 (2025-05-02)
        ///
        constexpr void disable() noexcept
        {
            _disabled = true;
        }

        ///
        /// @brief Enable the system.
        ///
        /// @author Andréas Leroux
        /// @since 1.0.0 (2025-05-02)
        ///
        constexpr void enable() noexcept
        {
            _disabled = false;
        }

      private:
        /// @brief Timer to control the execution of the system.
        Timer _timer;
        /// @brief Whether the timer is disabled
        bool _disabled = false;
    };
} // namespace ecstasy

#endif /* !ECSTASY_SYSTEM_ISYSTEM_HPP_ */
