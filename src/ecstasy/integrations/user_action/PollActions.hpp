///
/// @file PollActions.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-12-07
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_INTEGRATIONS_USER_ACTION_POLLACTIONS_HPP_
#define ECSTASY_INTEGRATIONS_USER_ACTION_POLLACTIONS_HPP_

#include <utility>
#include "Action.hpp"
#include "ActionListener.hpp"
#include "PendingActions.hpp"
#include "ecstasy/registry/Registry.hpp"
#include "ecstasy/system/ISystem.hpp"

namespace ecstasy::integration::user_action
{
    ///
    /// @brief Primary template of PollActions to unpack the action parameter pack.
    ///
    /// @tparam Actions Integer sequence of @ref Action::Id.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-12-07)
    ///
    template <typename Actions>
    class PollActions;

    ///
    /// @brief Templated system to poll pending actions using the @ref PendingActions resource.
    ///
    /// @tparam Actions @ref Action::Id ids to listen to.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-12-07)
    ///
    template <Action::Id... Actions>
    class PollActions<std::integer_sequence<Action::Id, Actions...>> : public ecstasy::ISystem {
      public:
        /// @copydoc ecstasy::ISystem::run()
        void run(Registry &registry) override final
        {
            if (!registry.hasResource<PendingActions>())
                return;
            RR<PendingActions> pendingActions = registry.getResource<PendingActions>();
            while (!pendingActions->get().empty()) {
                std::ignore = std::make_tuple((callListeners<Actions>(registry, pendingActions->get().front()), 0)...);
                pendingActions->get().pop();
            }
        }

      private:
        /// @internal
        template <Action::Id A>
        void callListeners(Registry &registry, Action action)
        {
            if (action.id != A && A != Action::All)
                return;

            for (auto [entity, listener] : registry.query<ecstasy::Entities, ActionIdListener<A>>())
                listener.listener(registry, entity, action);
        }
    };
} // namespace ecstasy::integration::user_action

#endif /* !ECSTASY_INTEGRATIONS_USER_ACTION_POLLACTIONS_HPP_ */
