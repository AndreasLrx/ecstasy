///
/// @file UserProfile.cpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-25
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#include "UserProfile.hpp"

namespace ecstasy::integration::user_action
{
    toml::table UserProfile::dump() const
    {
        toml::table table;

        table.emplace("id", static_cast<long long int>(_id));
        table.emplace("bindings", _actionBindings.dump());
        return table;
    }
} // namespace ecstasy::integration::user_action