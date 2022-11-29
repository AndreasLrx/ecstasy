///
/// @file ActionBindings.cpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-25
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#include "ActionBindings.hpp"

namespace ecstasy::integration::user_action
{
    toml::table ActionBindings::dump() const
    {
        toml::table table;

        /// There is no check for node type because we control the total flow of the toml nodes
        for (ActionBinding action : _bindings) {
            /// The actions are named 'Action-<action_id>'
            std::string key = std::string("Action-") + std::to_string(action.actionId);
            toml::node *nodePtr = table.get(key);
            std::stringstream value;

            /// If there isn't any node for the action we create one
            if (!nodePtr)
                nodePtr = &table.emplace<toml::array>(key).first->second;

            toml::node &node = *nodePtr;
            value << action.type << "->";
            switch (action.type) {
                case ActionBinding::Type::MouseButton: value << action.mouseButton; break;
                case ActionBinding::Type::Key: value << action.key; break;
                case ActionBinding::Type::GamepadButton: value << action.gamepadButton; break;
                case ActionBinding::Type::GamepadAxis: value << action.gamepadAxis; break;
                default: break;
            }
            node.as_array()->emplace_back(value.str());
        }

        return table;
    }
} // namespace ecstasy::integration::user_action