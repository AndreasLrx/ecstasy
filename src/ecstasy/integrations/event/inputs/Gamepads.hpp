///
/// @file Gamepads.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-18
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_INTEGRATIONS_EVENT_INPUTS_GAMEPADS_HPP_
#define ECSTASY_INTEGRATIONS_EVENT_INPUTS_GAMEPADS_HPP_

#include "Gamepad.hpp"
#include "ecstasy/resources/IResource.hpp"

namespace ecstasy::integration::event
{
    ///
    /// @brief Current Gamepads states. Hold informations about all the gamepads.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-11-18)
    ///
    class Gamepads : public ecstasy::IResource {
      public:
        /// @brief Number of supported gamepads.
        static const size_t GamepadCount = 4;

        ///
        /// @brief Construct a new Gamepads Resource.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-18)
        ///
        Gamepads() noexcept
        {
            for (size_t i = 0; i < GamepadCount; i++)
                _gamepads[i] = Gamepad(i);
        }

        ///
        /// @brief Default destructor.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-18)
        ///
        ~Gamepads() noexcept = default;

        ///
        /// @brief Fetch a gamepad.
        ///
        /// @warning This function doesn't perform bounds check.
        ///
        /// @param[in] i Id of the gamepad to fetch, must be lower than N.
        ///
        /// @return Gamepad& A reference to the gamepad at the id @p i.
        ///
        /// @throw std::out_of_range If the id is out of bounds (ie > GamepadCount).
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-18)
        ///
        [[nodiscard]] Gamepad &get(std::size_t i)
        {
            return _gamepads[i];
        }

        ///
        /// @brief Fetch a gamepad.
        ///
        /// @warning This function doesn't perform bounds check.
        ///
        /// @param[in] i Id of the gamepad to fetch, must be lower than N.
        ///
        /// @return Gamepad& A const reference to the gamepad at the id @p i.
        ///
        /// @throw std::out_of_range If the id is out of bounds (ie > GamepadCount).
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-18)
        ///
        [[nodiscard]] const Gamepad &get(std::size_t i) const
        {
            return _gamepads[i];
        }

      private:
        /// Array of gamepads.
        std::array<Gamepad, GamepadCount> _gamepads;
    };
} // namespace ecstasy::integration::event

#endif /* !ECSTASY_INTEGRATIONS_EVENT_INPUTS_GAMEPADS_HPP_ */
