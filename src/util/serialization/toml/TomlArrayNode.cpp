///
/// @file TomlArrayNode.cpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-12-08
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#include "TomlArrayNode.hpp"
#include "TomlConversion.hpp"
#include "TomlNodeFactory.hpp"

namespace util::serialization
{
    TomlArrayNode::TomlArrayNode(const toml::array &array)
    {
        for (auto &node : array)
            _nodes.push_back(TomlNodeFactory::get().createFromToml(node));
    }

    NodeView TomlArrayNode::get(Index index)
    {
        return _nodes.at(index);
    }

    NodeCView TomlArrayNode::get(Index index) const
    {
        return _nodes.at(index);
    }

    NodeView TomlArrayNode::tryGet(Index index)
    {
        if (index >= size())
            return NodeView();

        return _nodes.at(index);
    }

    NodeCView TomlArrayNode::tryGet(Index index) const
    {
        if (index >= size())
            return NodeCView();

        return _nodes.at(index);
    }

    void TomlArrayNode::pushBack(const INode &node)
    {
        insert(size(), node);
    }

    void TomlArrayNode::insert(Index index, const INode &node)
    {
        if (index > size())
            throw std::out_of_range("Index out of bounds.");

        _nodes.insert(_nodes.cbegin() + static_cast<long>(index), TomlNodeFactory::get().create(node));
    }

    void TomlArrayNode::replace(Index index, const INode &node)
    {
        if (index >= size())
            throw std::out_of_range("Index out of bounds.");

        _nodes.at(index) = TomlNodeFactory::get().create(node);
    }

    void TomlArrayNode::popBack()
    {
        _nodes.pop_back();
    }

    void TomlArrayNode::erase(Index index)
    {
        if (index <= size())
            _nodes.erase(_nodes.cbegin() + static_cast<long>(index));
    }

    void TomlArrayNode::clear()
    {
        _nodes.clear();
    }

    bool TomlArrayNode::empty() const
    {
        return _nodes.empty();
    }

    size_t TomlArrayNode::size() const
    {
        return _nodes.size();
    }

    TomlArrayNode::const_iterator TomlArrayNode::cbegin() const
    {
        return _nodes.cbegin();
    }

    TomlArrayNode::const_iterator TomlArrayNode::begin() const
    {
        return cbegin();
    }

    TomlArrayNode::iterator TomlArrayNode::begin()
    {
        return _nodes.begin();
    }

    TomlArrayNode::const_iterator TomlArrayNode::cend() const
    {
        return _nodes.cend();
    }

    TomlArrayNode::const_iterator TomlArrayNode::end() const
    {
        return cend();
    }

    TomlArrayNode::iterator TomlArrayNode::end()
    {
        return _nodes.end();
    }

} // namespace util::serialization
