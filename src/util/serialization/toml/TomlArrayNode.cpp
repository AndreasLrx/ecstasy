///
/// @file TomlArrayNode.cpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-12-08
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#include "TomlArrayNode.hpp"
#include "TomlConversion.hpp"
#include "TomlNodeFactory.hpp"

namespace util::serialization
{
    TomlArrayNode::TomlArrayNode(toml::array &array) : TomlNode(array)
    {
    }

    NodeView TomlArrayNode::get(Index index)
    {
        if (index >= size())
            throw std::out_of_range("Index out of bounds.");

        return TomlNodeFactory::createFromToml(*_node.get(index));
    }

    NodeCView TomlArrayNode::get(Index index) const
    {
        if (index >= size())
            throw std::out_of_range("Index out of bounds.");

        return TomlNodeFactory::createFromToml(*_node.get(index));
    }

    NodeView TomlArrayNode::tryGet(Index index)
    {
        if (index >= size())
            return NodeView();

        return TomlNodeFactory::createFromToml(*_node.get(index));
    }

    NodeCView TomlArrayNode::tryGet(Index index) const
    {
        if (index >= size())
            return NodeCView();

        return TomlNodeFactory::createFromToml(*_node.get(index));
    }

    void TomlArrayNode::pushBack(const INode &node)
    {
        insert(size(), node);
    }

    void TomlArrayNode::insert(Index index, const INode &node)
    {
        if (index > size())
            throw std::out_of_range("Index out of bounds.");

        const TomlNode *tomlNode = dynamic_cast<const TomlNode *>(&node);
        auto it = _node.cbegin() + index;
        if (tomlNode)
            _node.insert(it, tomlNode->getTomlNode());
        else {
            switch (node.getType()) {
                /// @todo
                case INode::Type::Object: break;
                /// @todo
                case INode::Type::Array: break;
                case INode::Type::String: _node.insert(it, node.asString()); break;
                case INode::Type::Integer: _node.insert(it, node.asInteger()); break;
                case INode::Type::Float: _node.insert(it, node.asFloat()); break;
                case INode::Type::Boolean: _node.insert(it, node.asBoolean()); break;
                case INode::Type::Date: _node.insert(it, TomlConversion::toToml(node.asDate())); break;
                case INode::Type::Time: _node.insert(it, TomlConversion::toToml(node.asTime())); break;
                case INode::Type::DateTime: _node.insert(it, TomlConversion::toToml(node.asDateTime())); break;
                default: throw std::runtime_error("Type not supported in Toml format."); break;
            }
        }
    }

    void TomlArrayNode::replace(Index index, const INode &node)
    {
        if (index >= size())
            throw std::out_of_range("Index out of bounds.");

        const TomlNode *tomlNode = dynamic_cast<const TomlNode *>(&node);
        auto it = _node.cbegin() + index;
        if (tomlNode)
            _node.replace(it, tomlNode->getTomlNode());
        else {
            switch (node.getType()) {
                /// @todo
                case INode::Type::Object: break;
                /// @todo
                case INode::Type::Array: break;
                case INode::Type::String: _node.replace(it, node.asString()); break;
                case INode::Type::Integer: _node.replace(it, node.asInteger()); break;
                case INode::Type::Float: _node.replace(it, node.asFloat()); break;
                case INode::Type::Boolean: _node.replace(it, node.asBoolean()); break;
                case INode::Type::Date: _node.replace(it, TomlConversion::toToml(node.asDate())); break;
                case INode::Type::Time: _node.replace(it, TomlConversion::toToml(node.asTime())); break;
                case INode::Type::DateTime: _node.replace(it, TomlConversion::toToml(node.asDateTime())); break;
                default: throw std::runtime_error("Type not supported in Toml format."); break;
            }
        }
    }

    void TomlArrayNode::popBack()
    {
        _node.pop_back();
    }

    void TomlArrayNode::erase(Index index)
    {
        if (index >= size())
            throw std::out_of_range("Index out of bounds.");
        _node.erase(_node.cbegin() + index);
    }

    void TomlArrayNode::clear()
    {
        _node.clear();
    }

    bool TomlArrayNode::empty() const
    {
        return _node.empty();
    }

    size_t TomlArrayNode::size() const
    {
        return _node.size();
    }

    TomlArrayNode::const_iterator TomlArrayNode::cbegin() const
    {
        return TomlArrayIterator<true>(*this, 0);
    }

    TomlArrayNode::iterator TomlArrayNode::begin()
    {
        return TomlArrayIterator<false>(*this, 0);
    }

    TomlArrayNode::const_iterator TomlArrayNode::cend() const
    {
        return TomlArrayIterator<true>(*this, size());
    }

    TomlArrayNode::iterator TomlArrayNode::end()
    {
        return TomlArrayIterator<false>(*this, size());
    }

} // namespace util::serialization