///
/// @file TomlNodeFactory.cpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief Toml node factory implementation.
/// @version 1.0.0
/// @date 2022-12-12
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#include "TomlNodeFactory.hpp"
#include "TomlArrayNode.hpp"
#include "TomlObjectNode.hpp"

namespace util::serialization
{
    TomlNodeFactory &TomlNodeFactory::get() noexcept
    {
        static TomlNodeFactory instance;

        return instance;
    }

    NodePtr TomlNodeFactory::createFromToml(const toml::node &node)
    {
        switch (node.type()) {
            case toml::node_type::table: return std::make_shared<TomlObjectNode>(*node.as_table());
            case toml::node_type::array: return std::make_shared<TomlArrayNode>(*node.as_array());
            case toml::node_type::string:
                return std::make_shared<TomlNode<toml::value<std::string>>>(*node.as_string());
            case toml::node_type::integer: return std::make_shared<TomlNode<toml::value<int64_t>>>(*node.as_integer());
            case toml::node_type::floating_point:
                return std::make_shared<TomlNode<toml::value<double>>>(*node.as_floating_point());
            case toml::node_type::boolean: return std::make_shared<TomlNode<toml::value<bool>>>(*node.as_boolean());
            case toml::node_type::date: return std::make_shared<TomlNode<toml::value<toml::date>>>(*node.as_date());
            case toml::node_type::time: return std::make_shared<TomlNode<toml::value<toml::time>>>(*node.as_time());
            case toml::node_type::date_time:
                return std::make_shared<TomlNode<toml::value<toml::date_time>>>(*node.as_date_time());
            default: return nullptr;
        }
    }

    NodePtr TomlNodeFactory::create(INode::Type type)
    {
        switch (type) {
            case INode::Type::Object: return std::make_shared<TomlObjectNode>();
            case INode::Type::Array: return std::make_shared<TomlArrayNode>();
            case INode::Type::String: return std::make_shared<TomlNode<toml::value<std::string>>>();
            case INode::Type::Integer: return std::make_shared<TomlNode<toml::value<int64_t>>>();
            case INode::Type::Float: return std::make_shared<TomlNode<toml::value<double>>>();
            case INode::Type::Boolean: return std::make_shared<TomlNode<toml::value<bool>>>();
            /// Need to initialize the date/time/datetime values to avoid uninitialized values.
            case INode::Type::Date: return std::make_shared<TomlNode<toml::value<toml::date>>>(0, 0, 0);
            case INode::Type::Time: return std::make_shared<TomlNode<toml::value<toml::time>>>(0, 0, 0, 0);
            case INode::Type::DateTime:
                /// Default @ref std::chrono::time_point is at the epoch (1st january 1970)
                return std::make_shared<TomlNode<toml::value<toml::date_time>>>(
                    toml::date(1970, 1, 1), toml::time(0, 0, 0, 0));
            default: return nullptr;
        }
    }

    NodePtr TomlNodeFactory::create(const INode &node)
    {
        switch (node.getType()) {
            case INode::Type::Object: return createObject(node.asObject());
            case INode::Type::Array: return createArray(node.asArray());
            case INode::Type::String: return create(node.asString());
            case INode::Type::Integer: return create(node.asInteger());
            case INode::Type::Float: return create(node.asFloat());
            case INode::Type::Boolean: return create(node.asBoolean());
            case INode::Type::Date: return create(node.asDate());
            case INode::Type::Time: return create(node.asTime());
            case INode::Type::DateTime: return create(node.asDateTime());
            default: return nullptr;
        }
    }

    NodePtr TomlNodeFactory::create(std::string_view string)
    {
        return std::make_shared<TomlNode<toml::value<std::string>>>(string);
    }

    NodePtr TomlNodeFactory::create(int64_t integer)
    {
        return std::make_shared<TomlNode<toml::value<int64_t>>>(integer);
    }

    NodePtr TomlNodeFactory::create(double floatingPoint)
    {
        return std::make_shared<TomlNode<toml::value<double>>>(floatingPoint);
    }

    NodePtr TomlNodeFactory::create(bool boolean)
    {
        return std::make_shared<TomlNode<toml::value<bool>>>(boolean);
    }

    NodePtr TomlNodeFactory::create(INode::Date date)
    {
        return std::make_shared<TomlNode<toml::value<toml::date>>>(TomlConversion::toToml(date));
    }

    NodePtr TomlNodeFactory::create(INode::Time time)
    {
        return std::make_shared<TomlNode<toml::value<toml::time>>>(TomlConversion::toToml(time));
    }

    NodePtr TomlNodeFactory::create(INode::DateTime dateTime)
    {
        return std::make_shared<TomlNode<toml::value<toml::date_time>>>(TomlConversion::toToml(dateTime));
    }

    NodePtr TomlNodeFactory::createArray(const IArrayNode &array)
    {
        NodePtr res = std::make_shared<TomlArrayNode>();

        for (auto it : array)
            res->asArray().pushBack(*it.lock());
        return res;
    }

    NodePtr TomlNodeFactory::createObject(const IObjectNode &object)
    {
        NodePtr res = std::make_shared<TomlObjectNode>();

        for (auto it : object)
            res->asObject().insert(it.first, *it.second.lock());
        return res;
    }
} // namespace util::serialization
