///
/// @file TomlObjectNode.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-12-08
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef UTIL_SERIALIZATION_TOML_TOMLOBJECTNODE_HPP_
#define UTIL_SERIALIZATION_TOML_TOMLOBJECTNODE_HPP_

#include "TomlNode.hpp"
#include "util/serialization/IObjectNode.hpp"

namespace util::serialization
{
    ///
    /// @brief Toml Object node. Contains nodes in an map.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-12-08)
    ///
    class TomlObjectNode : public TomlNode<toml::table>, public IObjectNode {
      public:
        /// @brief Default constructor.
        TomlObjectNode() = default;

        ///
        /// @brief Construct a new Toml Object Node.
        ///
        /// @param[in] table Toml table.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-13)
        ///
        TomlObjectNode(const toml::table &table);

        /// @copydoc IObjectNode::get()
        NodeCView get(std::string_view key) const override final;

        /// @copydoc IObjectNode::get()
        NodeView get(std::string_view key) override final;

        /// @copydoc IObjectNode::tryGet()
        NodeCView tryGet(std::string_view key) const override final;

        /// @copydoc IObjectNode::tryGet()
        NodeView tryGet(std::string_view key) override final;

        /// @copydoc IObjectNode::insert()
        bool insert(std::string_view key, const INode &value) override final;

        /// @copydoc IObjectNode::insertOrAssign()
        bool insertOrAssign(std::string_view key, const INode &value) override final;

        /// @copydoc IObjectNode::erase()
        void erase(std::string_view key) override final;

        /// @copydoc IObjectNode::clear()
        void clear() override final;

        /// @copydoc IObjectNode::empty()
        bool empty() const override final;

        /// @copydoc IObjectNode::size()
        size_t size() const override final;

        /// @copydoc IObjectNode::contains()
        bool contains(std::string_view key) const override final;

        /// @copydoc IObjectNode::cbegin()
        const_iterator cbegin() const override final;

        /// @copydoc IObjectNode::begin()
        const_iterator begin() const override final;

        /// @copydoc IObjectNode::begin()
        iterator begin() override final;

        /// @copydoc IObjectNode::cend()
        const_iterator cend() const override final;

        /// @copydoc IObjectNode::end()
        const_iterator end() const override final;

        /// @copydoc IObjectNode::end()
        iterator end() override final;

      private:
        std::map<std::string, NodePtr, std::less<>> _nodes;
    };
} // namespace util::serialization

#endif /* !UTIL_SERIALIZATION_TOML_TOMLOBJECTNODE_HPP_ */
