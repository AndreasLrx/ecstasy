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

    bool UserProfile::load(const toml::table &infos)
    {
        /// User Id
        auto id = infos.get("id");
        if (id && id->is_integer())
            _id = static_cast<UserProfile::Id>(id->as_integer()->get());

        /// User bindings
        auto bindings = infos.get("bindings");
        if (bindings && bindings->is_table())
            _actionBindings.load(*bindings->as_table());
        return true;
    }

} // namespace ecstasy::integration::user_action