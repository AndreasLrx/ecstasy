///
/// @file PollEvents.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-16
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef ECSTASY_INTEGRATIONS_SFML_SYSTEMS_POLLEVENTS_HPP_
#define ECSTASY_INTEGRATIONS_SFML_SYSTEMS_POLLEVENTS_HPP_

#include "ecstasy/system/ISystem.hpp"

namespace ecstasy
{
    class Registry;
} // namespace ecstasy

namespace ecstasy::integration::sfml
{
    ///
    /// @brief Poll events system, polling the events from the @ref RenderWindow resource if present.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-11-16)
    ///
    class PollEvents : public ecstasy::ISystem {
      public:
        /// @brief Run the system.
        void run(ecstasy::Registry &registry) override final;
    };
} // namespace ecstasy::integration::sfml

#endif /* !ECSTASY_INTEGRATIONS_SFML_SYSTEMS_POLLEVENTS_HPP_ */
