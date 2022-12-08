///
/// @file TomlNode.cpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-12-08
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#include "TomlNode.hpp"

namespace util::serialization
{
    TomlNode::TomlNode(toml::node &node) : _node(node)
    {
    }

    std::optional<std::string_view> TomlNode::tryAsString() const
    {
        return _node.value<std::string_view>();
    }

    std::optional<int> TomlNode::tryAsInteger() const
    {
        return _node.value<int>();
    }

    std::optional<float> TomlNode::tryAsFloat() const
    {
        return _node.value<float>();
    }

    std::optional<bool> TomlNode::tryAsBoolean() const
    {
        return _node.value<bool>();
    }

    std::optional<INode::Date> TomlNode::tryAsDate() const
    {
        auto date = _node.value<toml::date>();

        if (!date)
            return std::optional<Date>();
        return Date(std::chrono::year(date->year), std::chrono::month(date->month), std::chrono::day(date->day));
    }

    std::optional<INode::Time> TomlNode::tryAsTime() const
    {
        auto time = _node.value<toml::time>();

        if (!time)
            return std::optional<Time>();
        return std::chrono::duration_cast<Time>(std::chrono::hours(time->hour) + std::chrono::minutes(time->minute)
            + std::chrono::seconds(time->second) + std::chrono::nanoseconds(time->nanosecond));
    }
} // namespace util::serialization