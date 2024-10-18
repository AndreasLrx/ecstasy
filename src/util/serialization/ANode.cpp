///
/// @file ANode.cpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief Abstract node partial implementation.
/// @version 1.0.0
/// @date 2022-12-08
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#include "ANode.hpp"
#include "IArrayNode.hpp"
#include "IObjectNode.hpp"

namespace util::serialization
{
    bool ANode::isType(Type type) const noexcept
    {
        return getType() == type;
    }

    bool ANode::isNull() const noexcept
    {
        return isType(Type::Null);
    }

    bool ANode::isObject() const noexcept
    {
        return isType(Type::Object);
    }

    bool ANode::isArray() const noexcept
    {
        return isType(Type::Array);
    }

    bool ANode::isString() const noexcept
    {
        return isType(Type::String);
    }

    bool ANode::isInteger() const noexcept
    {
        return isType(Type::Integer);
    }

    bool ANode::isFloat() const noexcept
    {
        return isType(Type::Float);
    }

    bool ANode::isBoolean() const noexcept
    {
        return isType(Type::Boolean);
    }

    bool ANode::isDate() const noexcept
    {
        return isType(Type::Date);
    }

    bool ANode::isTime() const noexcept
    {
        return isType(Type::Time);
    }

    bool ANode::isDateTime() const noexcept
    {
        return isType(Type::DateTime);
    }

    const IObjectNode &ANode::asObject() const
    {
        return const_cast<ANode &>(*this).asObject();
    }

    IObjectNode &ANode::asObject()
    {
        if (!isObject()) [[unlikely]]
            throw std::runtime_error("Node is not an Object Node.");
        return dynamic_cast<IObjectNode &>(*this);
    }

    std::optional<std::reference_wrapper<const IObjectNode>> ANode::tryAsObject() const noexcept
    {
        if (isObject())
            return dynamic_cast<const IObjectNode &>(*this);
        return std::optional<std::reference_wrapper<const IObjectNode>>();
    }

    std::optional<std::reference_wrapper<IObjectNode>> ANode::tryAsObject() noexcept
    {
        if (isObject())
            return dynamic_cast<IObjectNode &>(*this);
        return std::optional<std::reference_wrapper<IObjectNode>>();
    }

    const IArrayNode &ANode::asArray() const
    {
        return const_cast<ANode &>(*this).asArray();
    }

    IArrayNode &ANode::asArray()
    {
        if (!isArray()) [[unlikely]]
            throw std::runtime_error("Node is not an Array Node.");
        return dynamic_cast<IArrayNode &>(*this);
    }

    std::optional<std::reference_wrapper<const IArrayNode>> ANode::tryAsArray() const noexcept
    {
        if (isArray())
            return dynamic_cast<const IArrayNode &>(*this);
        return std::optional<std::reference_wrapper<const IArrayNode>>();
    }

    std::optional<std::reference_wrapper<IArrayNode>> ANode::tryAsArray() noexcept
    {
        if (isArray())
            return dynamic_cast<IArrayNode &>(*this);
        return std::optional<std::reference_wrapper<IArrayNode>>();
    }

    std::string_view ANode::asString() const
    {
        return tryAsString().value();
    }

    int64_t ANode::asInteger() const
    {
        return tryAsInteger().value();
    }

    double ANode::asFloat() const
    {
        return tryAsFloat().value();
    }

    bool ANode::asBoolean() const
    {
        return tryAsBoolean().value();
    }

    INode::Date ANode::asDate() const
    {
        return tryAsDate().value();
    }

    INode::Time ANode::asTime() const
    {
        return tryAsTime().value();
    }

    INode::DateTime ANode::asDateTime() const
    {
        return tryAsDateTime().value();
    }

} // namespace util::serialization
