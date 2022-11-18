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
    template <typename E>
    using EventListener = std::function<void(Registry &, Entity, const E &)>;

    template <typename E>
    class EventListeners {
      public:
        constexpr EventListeners() = default;
        constexpr EventListeners(std::vector<EventListener<E>> &&listeners) : _listeners(listeners)
        {
        }

        ~EventListeners() = default;

        void add(EventListener<E> listener)
        {
            _listeners.push_back(listener);
        }

        constexpr void clear()
        {
            _listeners.clear();
        }

        constexpr void operator()(Registry &registry, Entity entity, const E &event)
        {
            for (EventListener<E> &e : _listeners)
                e(registry, entity, event);
        }

        constexpr std::vector<EventListener<E>> &getInner()
        {
            return _listeners;
        }

        constexpr const std::vector<EventListener<E>> &getInner() const
        {
            return _listeners;
        }

      private:
        std::vector<EventListener<E>> _listeners;
    };
} // namespace ecstasy::integration::event

#endif /* !ECSTASY_INTEGRATIONS_EVENT_LISTENERS_EVENTLISTENERS_HPP_ */
