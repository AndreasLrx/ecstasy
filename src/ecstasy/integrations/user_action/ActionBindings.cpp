///
/// @file ActionBindings.cpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-25
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#include "ActionBindings.hpp"
#include <iostream>
#include <regex>
#include "util/serialization/Serializer.hpp"

namespace event = ecstasy::integration::event;

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
                default: continue;
            }
            node.as_array()->emplace_back(value.str());
        }

        return table;
    }

    bool ActionBindings::load(const toml::table &bindings)
    {
        _bindings.clear();
        const std::regex reActionName("^Action-([0-9]+)$");
        const std::regex reActionValue("^(\\w+)->(\\w+)$");
        std::cmatch match;

        for (auto &&[key, value] : bindings) {
            if (!std::regex_match(key.data(), match, reActionName)) {
                std::cerr << "Invalid action name." << std::endl;
                break;
            }

            Action::Id actionId = std::stoul(match[1].str());
            if (!value.is_array()) {
                std::cerr << "Action '" << key.data() << "' is not a valid toml array." << std::endl;
                continue;
            }

            value.as_array()->for_each([this, &match, &reActionValue, &actionId](auto &&input) {
                if constexpr (toml::is_string<decltype(input)>) {
                    if (!std::regex_match(input.as_string()->get().data(), match, reActionValue))
                        return;
                    ActionBinding::Type type;

                    std::istringstream ss(match[1].str());
                    ss >> type;
                    ss.clear();
                    ss.str(match[2].str());

                    switch (type) {
                        case ActionBinding::Type::MouseButton:
                            _bindings.emplace_back(
                                actionId, util::serialization::Serializer::deserialize<event::Mouse::Button>(ss));
                            break;
                        case ActionBinding::Type::Key:
                            _bindings.emplace_back(
                                actionId, util::serialization::Serializer::deserialize<event::Keyboard::Key>(ss));
                            break;
                        case ActionBinding::Type::GamepadButton:
                            _bindings.emplace_back(
                                actionId, util::serialization::Serializer::deserialize<event::Gamepad::Button>(ss));
                            break;
                        case ActionBinding::Type::GamepadAxis:
                            _bindings.emplace_back(
                                actionId, util::serialization::Serializer::deserialize<event::Gamepad::Axis>(ss));
                            break;
                        default: break;
                    }
                    // clang-format off
                }});
            // clang-format on
        }
        return true;
    }

    bool ActionBindings::contains(ActionBinding binding) const
    {
        return std::find(_bindings.cbegin(), _bindings.cend(), binding) != _bindings.cend();
    }
} // namespace ecstasy::integration::user_action
