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

#include "TomlConversion.hpp"

namespace util::serialization
{

    toml::date TomlConversion::toToml(INode::Date date)
    {
        return toml::date(static_cast<int>(date.year()), static_cast<unsigned int>(date.month()),
            static_cast<unsigned int>(date.day()));
    }

    toml::time TomlConversion::toToml(INode::Time timeNs)
    {
        std::chrono::hh_mm_ss<std::chrono::nanoseconds> time(timeNs);

        return toml::time(
            time.hours().count(), time.minutes().count(), time.seconds().count(), time.subseconds().count());
    }

    toml::date_time TomlConversion::toToml(INode::DateTime dateTime)
    {
        auto dp = floor<std::chrono::days>(dateTime);

        return toml::date_time(toToml(INode::Date(dp)), toToml(dateTime - dp));
    }

    INode::Date TomlConversion::fromToml(toml::date date)
    {
        return INode::Date(std::chrono::year(date.year), std::chrono::month(date.month), std::chrono::day(date.day));
    }

    INode::Time TomlConversion::fromToml(toml::time time)
    {
        return INode::Time(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::hours(time.hour))
            + std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::minutes(time.minute))
            + std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::seconds(time.second))
            + std::chrono::nanoseconds(time.nanosecond));
    }

    INode::DateTime TomlConversion::fromToml(toml::date_time dateTime)
    {
        return INode::DateTime(static_cast<std::chrono::sys_days>(fromToml(dateTime.date)) + fromToml(dateTime.time));
    }
} // namespace util::serialization