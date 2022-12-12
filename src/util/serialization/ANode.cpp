///
/// @file ANode.cpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-12-08
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#include "ANode.hpp"
#include "IArrayNode.hpp"
#include "IObjectNode.hpp"

namespace util::serialization
{

    bool ANode::isType(Type type) const
    {
        return getType() == type;
    }

    bool ANode::isNull() const
    {
        return isType(Type::Null);
    }

    bool ANode::isObject() const
    {
        return isType(Type::Object);
    }

    bool ANode::isArray() const
    {
        return isType(Type::Array);
    }

    bool ANode::isString() const
    {
        return isType(Type::String);
    }

    bool ANode::isInteger() const
    {
        return isType(Type::Integer);
    }

    bool ANode::isFloat() const
    {
        return isType(Type::Float);
    }

    bool ANode::isBoolean() const
    {
        return isType(Type::Boolean);
    }

    bool ANode::isDate() const
    {
        return isType(Type::Date);
    }

    bool ANode::isTime() const
    {
        return isType(Type::Time);
    }

    bool ANode::isDateTime() const
    {
        return isType(Type::DateTime);
    }

    const IObjectNode &ANode::asObject() const
    {
        return const_cast<ANode &>(*this).asObject();
    }

    IObjectNode &ANode::asObject()
    {
        if (!isObject())
            throw std::runtime_error("Node is not an Object Node.");
        return dynamic_cast<IObjectNode &>(*this);
    }

    std::optional<std::reference_wrapper<const IObjectNode>> ANode::tryAsObject() const
    {
        if (isObject())
            return dynamic_cast<const IObjectNode &>(*this);
        return std::optional<std::reference_wrapper<const IObjectNode>>();
    }

    std::optional<std::reference_wrapper<IObjectNode>> ANode::tryAsObject()
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
        if (!isArray())
            throw std::runtime_error("Node is not an Array Node.");
        return dynamic_cast<IArrayNode &>(*this);
    }

    std::optional<std::reference_wrapper<const IArrayNode>> ANode::tryAsArray() const
    {
        if (isArray())
            return dynamic_cast<const IArrayNode &>(*this);
        return std::optional<std::reference_wrapper<const IArrayNode>>();
    }

    std::optional<std::reference_wrapper<IArrayNode>> ANode::tryAsArray()
    {
        if (isArray())
            return dynamic_cast<IArrayNode &>(*this);
        return std::optional<std::reference_wrapper<IArrayNode>>();
    }

    std::string_view ANode::asString() const
    {
        return tryAsString().value();
    }

    int ANode::asInteger() const
    {
        return tryAsInteger().value();
    }

    float ANode::asFloat() const
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