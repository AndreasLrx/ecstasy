///
/// @file TypeRegistry.cpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief TypeRegistry class implementation.
/// @version 1.0.0
/// @date 2024-10-24
///
/// @copyright Copyright (c) ECSTASY 2024
///
///

#include "TypeRegistry.hpp"

namespace ecstasy::rtti
{

    TypeRegistry &TypeRegistry::getInstance() noexcept
    {
        static TypeRegistry instance;

        return instance;
    }

    bool TypeRegistry::has(std::size_t name_hash) const noexcept
    {
        return _types.contains(name_hash);
    }

    TypeRegistry::OptionalATypeReference TypeRegistry::findIf(const Predicate &p) const noexcept
    {
        auto result = std::find_if(_types.begin(), _types.end(), p);

        if (result == _types.end())
            return std::nullopt;
        return std::ref(*result->second);
    }

    AType &TypeRegistry::getIf(const Predicate &p) const
    {
        auto result = findIf(p);

        if (result.has_value())
            return result.value();
        throw std::out_of_range("Type not found.");
    }

    TypeRegistry::OptionalATypeReference TypeRegistry::find(std::size_t name_hash) const noexcept
    {
        if (_types.contains(name_hash))
            return std::ref(*(_types.at(name_hash)));
        return std::nullopt;
    }

    AType &TypeRegistry::get(std::size_t name_hash) const
    {
        return *(_types.at(name_hash));
    }

} // namespace ecstasy::rtti
