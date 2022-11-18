///
/// @file Gamepads.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-18
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef ECSTASY_INTEGRATIONS_EVENT_INPUTS_GAMEPADS_HPP_
#define ECSTASY_INTEGRATIONS_EVENT_INPUTS_GAMEPADS_HPP_

#include "Gamepad.hpp"
#include "ecstasy/resources/Resource.hpp"

namespace ecstasy::integration::event
{
    ///
    /// @brief Current Gamepads states. Hold informations about all the gamepads.
    ///
    /// @tparam N Number of gamepads supported.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-11-18)
    ///
    template <std::size_t N>
    class Gamepads : public ecstasy::Resource {
      public:
        ///
        /// @brief Construct a new Gamepads Resource.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-18)
        ///
        constexpr Gamepads()
        {
            for (size_t i = 0; i < N; i++)
                _gamepads[i] = Gamepad(i);
        }

        ///
        /// @brief Default destructor.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-18)
        ///
        ~Gamepads() = default;

        ///
        /// @brief Fetch a gamepad.
        ///
        /// @warning This function doesn't perform bounds check.
        ///
        /// @param[in] i Id of the gamepad to fetch, must be lower than N.
        ///
        /// @return Gamepad& A reference to the gamepad at the id @p i.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-18)
        ///
        Gamepad &get(std::size_t i)
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
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-18)
        ///
        const Gamepad &get(std::size_t i) const
        {
            return _gamepads[i];
        }

      private:
        std::array<Gamepad, N> _gamepads;
    };
} // namespace ecstasy::integration::event

#endif /* !ECSTASY_INTEGRATIONS_EVENT_INPUTS_GAMEPADS_HPP_ */
