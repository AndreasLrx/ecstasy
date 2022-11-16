///
/// @file RenderWindow.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-16
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef ECSTASY_INTEGRATIONS_SFML_RESOURCES_RENDERWINDOW_HPP_
#define ECSTASY_INTEGRATIONS_SFML_RESOURCES_RENDERWINDOW_HPP_

#include <SFML/Graphics/RenderWindow.hpp>

#include "ecstasy/resources/ObjectWrapper.hpp"

namespace ecstasy::integration::sfml
{
    ///
    /// @brief SFML RenderWindow wrapper.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-11-16)
    ///
    using RenderWindow = ecstasy::ObjectWrapper<sf::RenderWindow>;
} // namespace ecstasy::integration::sfml

#endif /* !ECSTASY_INTEGRATIONS_SFML_RESOURCES_RENDERWINDOW_HPP_ */
