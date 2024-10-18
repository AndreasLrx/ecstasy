///
/// @file TomlConversion.cpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief Toml conversion implementation.
/// @version 1.0.0
/// @date 2022-12-08
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#include "TomlConversion.hpp"

namespace util::serialization
{
    toml::date TomlConversion::toToml(const INode::Date &date) noexcept
    {
        return toml::date(static_cast<int>(date.year()), static_cast<unsigned int>(date.month()),
            static_cast<unsigned int>(date.day()));
    }

    toml::time TomlConversion::toToml(const INode::Time &timeNs) noexcept
    {
        std::chrono::hh_mm_ss<std::chrono::nanoseconds> time(timeNs);

        return toml::time(
            time.hours().count(), time.minutes().count(), time.seconds().count(), time.subseconds().count());
    }

    toml::date_time TomlConversion::toToml(const INode::DateTime &dateTime) noexcept
    {
#ifdef _MSC_VER
        std::chrono::system_clock::time_point t2(
            std::chrono::duration_cast<std::chrono::microseconds>(dateTime.time_since_epoch()));
        auto dp = floor<std::chrono::days>(t2);

        return toml::date_time(toToml(INode::Date(dp)), toToml(dateTime - floor<std::chrono::days>(dateTime)));

#else
        auto dp = floor<std::chrono::days>(dateTime);
        return toml::date_time(toToml(INode::Date(dp)), toToml(dateTime - dp));
#endif
    }

    INode::Date TomlConversion::fromToml(const toml::date &date) noexcept
    {
        return INode::Date(std::chrono::year(date.year), std::chrono::month(date.month), std::chrono::day(date.day));
    }

    INode::Time TomlConversion::fromToml(const toml::time &time) noexcept
    {
        return INode::Time(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::hours(time.hour))
            + std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::minutes(time.minute))
            + std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::seconds(time.second))
            + std::chrono::nanoseconds(time.nanosecond));
    }

    INode::DateTime TomlConversion::fromToml(const toml::date_time &dateTime) noexcept
    {
        return INode::DateTime(
            static_cast<std::chrono::sys_days>(fromToml(dateTime.date)).time_since_epoch() + fromToml(dateTime.time));
    }
} // namespace util::serialization
