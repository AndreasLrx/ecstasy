///
/// @file AType.cpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief AType class implementation.
/// @version 1.0.0
/// @date 2024-10-24
///
/// @copyright Copyright (c) ECSTASY 2024
///
///

#include "AType.hpp"

namespace ecstasy::rtti
{
    std::strong_ordering AType::operator<=>(const AType &rhs) const noexcept
    {
        return getHash() <=> rhs.getHash();
    }

    bool AType::operator==(const AType &rhs) const noexcept
    {
        return getHash() == rhs.getHash();
    }

    bool AType::operator==(const std::type_info &rhs) const noexcept
    {
        return getTypeInfo() == rhs;
    }

    bool AType::operator==(const std::string_view &rhs) const noexcept
    {
        return getTypeName() == rhs;
    }

} // namespace ecstasy::rtti
