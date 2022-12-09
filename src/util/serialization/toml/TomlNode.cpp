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

    toml::node &TomlNode::getTomlNode()
    {
        return _node;
    }

    const toml::node &TomlNode::getTomlNode() const
    {
        return _node;
    }

    toml::date TomlNode::toToml(INode::Date date)
    {
        return toml::date(static_cast<int>(date.year()), static_cast<unsigned int>(date.month()),
            static_cast<unsigned int>(date.day()));
    }

    toml::time TomlNode::toToml(INode::Time timeNs)
    {
        std::chrono::hh_mm_ss<std::chrono::nanoseconds> time(timeNs);

        return toml::time(
            time.hours().count(), time.minutes().count(), time.seconds().count(), time.subseconds().count());
    }

    toml::date_time TomlNode::toToml(INode::DateTime dateTime)
    {
        auto dp = floor<std::chrono::days>(dateTime);

        return toml::date_time(toToml(Date(dp)), toToml(dateTime - dp));
    }

    INode::Date TomlNode::fromToml(toml::date date)
    {
        return INode::Date(std::chrono::year(date.year), std::chrono::month(date.month), std::chrono::day(date.day));
    }

    INode::Time TomlNode::fromToml(toml::time time)
    {
        return INode::Time(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::hours(time.hour))
            + std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::minutes(time.minute))
            + std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::seconds(time.second))
            + std::chrono::nanoseconds(time.nanosecond));
    }

    INode::DateTime TomlNode::fromToml(toml::date_time dateTime)
    {
        return INode::DateTime(static_cast<std::chrono::sys_days>(fromToml(dateTime.date)) + fromToml(dateTime.time));
    }
} // namespace util::serialization