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

namespace util::serialization
{
    TomlArrayNode::TomlArrayNode(toml::array &array) : TomlNode(array)
    {
    }

    std::shared_ptr<INode> TomlArrayNode::get(Index index)
    {
        if (index >= size())
            throw std::out_of_range("Index out of bounds.");

        return std::make_shared<TomlNode>(*_node.as_array()->get(index));
    }

    std::shared_ptr<const INode> TomlArrayNode::get(Index index) const
    {
        return std::const_pointer_cast<const INode>((const_cast<TomlArrayNode &>(*this)).get(index));
    }

    std::shared_ptr<INode> TomlArrayNode::tryGet(Index index)
    {
        if (index >= size())
            return nullptr;

        return std::make_shared<TomlNode>(*_node.as_array()->get(index));
    }

    std::shared_ptr<const INode> TomlArrayNode::tryGet(Index index) const
    {
        return std::const_pointer_cast<const INode>((const_cast<TomlArrayNode &>(*this)).tryGet(index));
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
        auto it = asTomlArray().cbegin() + index;
        if (tomlNode)
            asTomlArray().insert(it, tomlNode->getTomlNode());
        else {
            switch (node.getType()) {
                /// @todo
                case INode::Type::Object: break;
                /// @todo
                case INode::Type::Array: break;
                case INode::Type::String: asTomlArray().insert(it, node.asString()); break;
                case INode::Type::Integer: asTomlArray().insert(it, node.asInteger()); break;
                case INode::Type::Float: asTomlArray().insert(it, node.asFloat()); break;
                case INode::Type::Boolean: asTomlArray().insert(it, node.asBoolean()); break;
                case INode::Type::Date: asTomlArray().insert(it, toToml(node.asDate())); break;
                case INode::Type::Time: asTomlArray().insert(it, toToml(node.asTime())); break;
                case INode::Type::DateTime: asTomlArray().insert(it, toToml(node.asDateTime())); break;
                default: throw std::runtime_error("Type not supported in Toml format."); break;
            }
        }
    }

    void TomlArrayNode::replace(Index index, const INode &node)
    {
        if (index >= size())
            throw std::out_of_range("Index out of bounds.");

        const TomlNode *tomlNode = dynamic_cast<const TomlNode *>(&node);
        auto it = asTomlArray().cbegin() + index;
        if (tomlNode)
            asTomlArray().replace(it, tomlNode->getTomlNode());
        else {
            switch (node.getType()) {
                /// @todo
                case INode::Type::Object: break;
                /// @todo
                case INode::Type::Array: break;
                case INode::Type::String: asTomlArray().replace(it, node.asString()); break;
                case INode::Type::Integer: asTomlArray().replace(it, node.asInteger()); break;
                case INode::Type::Float: asTomlArray().replace(it, node.asFloat()); break;
                case INode::Type::Boolean: asTomlArray().replace(it, node.asBoolean()); break;
                case INode::Type::Date: asTomlArray().replace(it, toToml(node.asDate())); break;
                case INode::Type::Time: asTomlArray().replace(it, toToml(node.asTime())); break;
                case INode::Type::DateTime: asTomlArray().replace(it, toToml(node.asDateTime())); break;
                default: throw std::runtime_error("Type not supported in Toml format."); break;
            }
        }
    }

    void TomlArrayNode::popBack()
    {
        asTomlArray().pop_back();
    }

    void TomlArrayNode::erase(Index index)
    {
        if (index >= size())
            throw std::out_of_range("Index out of bounds.");
        asTomlArray().erase(asTomlArray().cbegin() + index);
    }

    void TomlArrayNode::clear()
    {
        asTomlArray().clear();
    }

    bool TomlArrayNode::empty() const
    {
        return asTomlArray().empty();
    }

    size_t TomlArrayNode::size() const
    {
        return asTomlArray().size();
    }

    toml::array &TomlArrayNode::asTomlArray()
    {
        return *_node.as_array();
    }

    const toml::array &TomlArrayNode::asTomlArray() const
    {
        return *_node.as_array();
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