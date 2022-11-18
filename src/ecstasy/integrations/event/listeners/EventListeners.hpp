///
/// @file EventListeners.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-17
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef ECSTASY_INTEGRATIONS_EVENT_LISTENERS_EVENTLISTENERS_HPP_
#define ECSTASY_INTEGRATIONS_EVENT_LISTENERS_EVENTLISTENERS_HPP_

#include <functional>
#include <vector>

#include "ecstasy/resources/entity/Entity.hpp"

namespace ecstasy
{
    class Registry;
}

namespace ecstasy::integration::event
{
    ///
    /// @brief Event listener component type.
    ///
    /// @tparam E Event type to listen to.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-11-18)
    ///
    template <typename E>
    using EventListener = std::function<void(Registry &, Entity, const E &)>;

    ///
    /// @brief Event listeners component type. Allows to have several listeners for the same event in a single entity.
    ///
    /// @note This is just a vector of @ref EventListener<E>.
    ///
    /// @tparam E Event type.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-11-18)
    ///
    template <typename E>
    class EventListeners {
      public:
        ///
        /// @brief Construct a new empty EventListeners.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-18)
        ///
        constexpr EventListeners() = default;

        ///
        /// @brief Construct a new EventListeners with the given listeners.
        ///
        /// @param[in] listeners Initial listeners.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-18)
        ///
        constexpr EventListeners(std::vector<EventListener<E>> &&listeners) : _listeners(listeners)
        {
        }

        ///
        /// @brief Destroy the Event Listeners.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-18)
        ///
        ~EventListeners() = default;

        ///
        /// @brief Add a listener.
        ///
        /// @param[in] listener new listener.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-18)
        ///
        void add(EventListener<E> listener)
        {
            _listeners.push_back(listener);
        }

        ///
        /// @brief Delete all the listeners.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-18)
        ///
        constexpr void clear()
        {
            _listeners.clear();
        }

        ///
        /// @brief Call the internal listeners.
        ///
        /// @param[in] registry Registry to forward to the listeners.
        /// @param[in] entity Entity owning this component.
        /// @param[in] event Source event.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-18)
        ///
        constexpr void operator()(Registry &registry, Entity entity, const E &event)
        {
            for (EventListener<E> &e : _listeners)
                e(registry, entity, event);
        }

        ///
        /// @brief Get the Inner listeners container.
        ///
        /// @return std::vector<EventListener<E>>& A reference to the inner container.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-18)
        ///
        constexpr std::vector<EventListener<E>> &getInner()
        {
            return _listeners;
        }

        ///
        /// @brief Get the Inner listeners container.
        ///
        /// @return const std::vector<EventListener<E>>& A const reference to the inner container.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-18)
        ///
        constexpr const std::vector<EventListener<E>> &getInner() const
        {
            return _listeners;
        }

      private:
        std::vector<EventListener<E>> _listeners;
    };
} // namespace ecstasy::integration::event

#endif /* !ECSTASY_INTEGRATIONS_EVENT_LISTENERS_EVENTLISTENERS_HPP_ */
