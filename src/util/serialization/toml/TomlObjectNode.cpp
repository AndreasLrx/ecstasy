///
/// @file TomlObjectNode.cpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief Toml object node implementation.
/// @version 1.0.0
/// @date 2022-12-08
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#include "TomlObjectNode.hpp"
#include "TomlConversion.hpp"
#include "TomlNodeFactory.hpp"

namespace util::serialization
{
    TomlObjectNode::TomlObjectNode(const toml::table &table)
    {
        for (auto &node : table)
            _nodes.insert({std::string(node.first.str()), TomlNodeFactory::get().createFromToml(node.second)});
    }

    NodeView TomlObjectNode::get(std::string_view key)
    {
        auto it = _nodes.find(key);
        if (it == _nodes.end()) [[unlikely]]
            throw std::out_of_range(std::string("Key '") + std::string(key) + "' no found in Toml Object.");

        return it->second;
    }

    NodeCView TomlObjectNode::get(std::string_view key) const
    {
        auto it = _nodes.find(key);
        if (it == _nodes.end()) [[unlikely]]
            throw std::out_of_range(std::string("Key '") + std::string(key) + "' no found in Toml Object.");

        return it->second;
    }

    NodeView TomlObjectNode::tryGet(std::string_view key) noexcept
    {
        auto it = _nodes.find(key);
        if (it == _nodes.end())
            return NodeView();

        return it->second;
    }

    NodeCView TomlObjectNode::tryGet(std::string_view key) const noexcept
    {
        auto it = _nodes.find(key);
        if (it == _nodes.end())
            return NodeCView();

        return it->second;
    }

    bool TomlObjectNode::insert(std::string_view key, const INode &node)
    {
        auto it = _nodes.find(key);

        if (it != _nodes.end())
            return false;

        _nodes.insert({std::string(key), TomlNodeFactory::get().create(node)});
        return true;
    }

    bool TomlObjectNode::insertOrAssign(std::string_view key, const INode &node)
    {
        auto it = _nodes.find(key);
        NodePtr nodePtr = TomlNodeFactory::get().create(node);

        if (it != _nodes.end()) {
            it->second = nodePtr;
            return false;
        } else
            _nodes.insert({std::string(key), TomlNodeFactory::get().create(node)});
        return true;
    }

    void TomlObjectNode::erase(std::string_view key)
    {
        auto it = _nodes.find(key);

        if (it != _nodes.end())
            _nodes.erase(it);
    }

    void TomlObjectNode::clear() noexcept
    {
        _nodes.clear();
    }

    bool TomlObjectNode::empty() const noexcept
    {
        return _nodes.empty();
    }

    size_t TomlObjectNode::size() const noexcept
    {
        return _nodes.size();
    }

    bool TomlObjectNode::contains(std::string_view key) const noexcept
    {
        return _nodes.find(key) != _nodes.end();
    }

    TomlObjectNode::const_iterator TomlObjectNode::cbegin() const noexcept
    {
        return _nodes.cbegin();
    }

    TomlObjectNode::const_iterator TomlObjectNode::begin() const noexcept
    {
        return cbegin();
    }

    TomlObjectNode::iterator TomlObjectNode::begin() noexcept
    {
        return _nodes.begin();
    }

    TomlObjectNode::const_iterator TomlObjectNode::cend() const noexcept
    {
        return _nodes.cend();
    }

    TomlObjectNode::const_iterator TomlObjectNode::end() const noexcept
    {
        return cend();
    }

    TomlObjectNode::iterator TomlObjectNode::end() noexcept
    {
        return _nodes.end();
    }

} // namespace util::serialization
