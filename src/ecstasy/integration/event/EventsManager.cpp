///
/// @file EventsManager.cpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-06
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#include "EventsManager.hpp"
#include "ecstasy/integration/event/listeners/MouseButtonPressedListener.hpp"
#include "ecstasy/registry/Registry.hpp"
#include "ecstasy/resource/entity/Entities.hpp"
#include "ecstasy/storage/MapStorage.hpp"

// clang-format off
#define CALL_LISTENERS(listenerType, e)                                                        \
    for (auto [entity, listener] : registry.query<Entities, listenerType>())                   \
    {                                                                                          \
        if (listener(registry, entity, e))                                                     \
            return true;                                                                       \
    }
// clang-format on

namespace ecstasy::integration::event
{
    bool EventsManager::handleEvent(Registry &registry, const Event &event)
    {
        switch (event.type) {
            case Event::Type::MouseButtonPressed:
                try {
                    registry.getResource<Mouse>().setButtonState(event.mouseButtonPressed.button, true);
                } catch (std::logic_error &) {
                }

                CALL_LISTENERS(MouseButtonPressedListener, event.mouseButtonPressed)
                break;
            default: break;
        }
        return false;
    }
} // namespace ecstasy::integration::event