///
/// @file TomlNodeFactory.cpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-12-12
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#include "TomlNodeFactory.hpp"
#include "TomlArrayNode.hpp"

namespace util::serialization
{
    NodePtr TomlNodeFactory::createFromToml(toml::node &node)
    {
        switch (node.type()) {
            // case toml::node_type::table: return std::make_shared<TomlObjectNode>(node);
            case toml::node_type::array: return std::make_shared<TomlArrayNode>(*node.as_array());
            case toml::node_type::string:
                return std::make_shared<TomlNode<toml::value<std::string>>>(*node.as_string());
            case toml::node_type::integer: return std::make_shared<TomlNode<toml::value<int64_t>>>(*node.as_integer());
            case toml::node_type::floating_point:
                return std::make_shared<TomlNode<toml::value<double>>>(*node.as_floating_point());
            case toml::node_type::boolean: return std::make_shared<TomlNode<toml::value<bool>>>(*node.as_boolean());
            case toml::node_type::date: return std::make_shared<TomlNode<toml::value<toml::date>>>(*node.as_date());
            case toml::node_type::time: return std::make_shared<TomlNode<toml::value<toml::time>>>(*node.as_time());
            case toml::node_type::date_time:
                return std::make_shared<TomlNode<toml::value<toml::date_time>>>(*node.as_date_time());
            default: return nullptr;
        }
    }

    NodeCPtr TomlNodeFactory::createFromToml(const toml::node &node)
    {
        switch (node.type()) {
            // case toml::node_type::table: return std::make_shared<TomlObjectNode>(node);
            case toml::node_type::array:
                return std::make_shared<TomlArrayNode>(const_cast<toml::array &>(*node.as_array()));
            case toml::node_type::string:
                return std::make_shared<TomlNode<toml::value<std::string>>>(*node.as_string());
            case toml::node_type::integer: return std::make_shared<TomlNode<toml::value<int64_t>>>(*node.as_integer());
            case toml::node_type::floating_point:
                return std::make_shared<TomlNode<toml::value<double>>>(*node.as_floating_point());
            case toml::node_type::boolean: return std::make_shared<TomlNode<toml::value<bool>>>(*node.as_boolean());
            case toml::node_type::date: return std::make_shared<TomlNode<toml::value<toml::date>>>(*node.as_date());
            case toml::node_type::time: return std::make_shared<TomlNode<toml::value<toml::time>>>(*node.as_time());
            case toml::node_type::date_time:
                return std::make_shared<TomlNode<toml::value<toml::date_time>>>(*node.as_date_time());
            default: return nullptr;
        }
    }
} // namespace util::serialization