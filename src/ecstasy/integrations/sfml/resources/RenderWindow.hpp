///
/// @file RenderWindow.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-16
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_INTEGRATIONS_SFML_RESOURCES_RENDERWINDOW_HPP_
#define ECSTASY_INTEGRATIONS_SFML_RESOURCES_RENDERWINDOW_HPP_

#include <SFML/Graphics/RenderWindow.hpp>
#include <functional>

#include "ecstasy/resources/ObjectWrapper.hpp"

namespace ecstasy::integration::sfml
{

    ///
    /// @brief Event listener type.
    ///
    /// @param[in] event Event to handle.
    /// @return true If the event was handled (ie must not be processed anymore).
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-09-26)
    ///
    using EventListener = std::function<bool(const sf::Event &)>;

    ///
    /// @brief SFML RenderWindow wrapper.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-11-16)
    ///
    class RenderWindow : public ecstasy::ObjectWrapper<sf::RenderWindow> {
      public:
        ///
        /// @brief Construct a new Render Window.
        ///
        /// @tparam Args Argument types of the object constructor.
        ///
        /// @param[in] args Arguments of the object constructor.
        ///
        template <typename... Args>
        RenderWindow(Args &&...args)
            : ecstasy::ObjectWrapper<sf::RenderWindow>(std::forward<Args>(args)...), _eventListener(nullptr)
        {
        }

        ///
        /// @brief Destroy the Render Window.
        ///
        ~RenderWindow() = default;

        ///
        /// @brief Set the event listener.
        ///
        /// @param[in] listener Event listener.
        ///
        void setEventListener(EventListener listener)
        {
            _eventListener = listener;
        }

        ///
        /// @brief Get the Event Listener
        ///
        /// @return constexpr EventListener& Event listener.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-09-26)
        ///
        constexpr EventListener &getEventListener()
        {
            return _eventListener;
        }

        ///
        /// @brief Poll the next event.
        ///
        /// @param[out] event Event to fill.
        ///
        /// @return true If an event was polled.
        ///
        bool pollEvent(sf::Event &event)
        {
            while (_object.pollEvent(event)) {
                // Return true if a usable (ie not stoled by the listener) event was polled
                if (!_eventListener || (!_eventListener(event)))
                    return true;
            }
            return false;
        }

      private:
        EventListener _eventListener;
    };
} // namespace ecstasy::integration::sfml

#endif /* !ECSTASY_INTEGRATIONS_SFML_RESOURCES_RENDERWINDOW_HPP_ */
