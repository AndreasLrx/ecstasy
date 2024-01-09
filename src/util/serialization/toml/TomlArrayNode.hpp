///
/// @file TomlArrayNode.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-12-08
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef UTIL_SERIALIZATION_TOML_TOMLARRAYNODE_HPP_
#define UTIL_SERIALIZATION_TOML_TOMLARRAYNODE_HPP_

#include "TomlNode.hpp"
#include "util/serialization/IArrayNode.hpp"

namespace util::serialization
{
    ///
    /// @brief Toml Array node. Contains nodes in an array.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-12-08)
    ///
    class TomlArrayNode : public TomlNode<toml::array>, public IArrayNode {
      public:
        /// @brief Default constructor.
        TomlArrayNode() = default;

        ///
        /// @brief Construct a new Toml Array Node.
        ///
        /// @param[in] array Toml array.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-13)
        ///
        TomlArrayNode(const toml::array &array);

        /// @copydoc IArrayNode::get()
        NodeCView get(Index index) const override final;

        /// @copydoc IArrayNode::get()
        NodeView get(Index index) override final;

        /// @copydoc IArrayNode::tryGet()
        NodeCView tryGet(Index index) const override final;

        /// @copydoc IArrayNode::tryGet()
        NodeView tryGet(Index index) override final;

        /// @copydoc IArrayNode::pushBack()
        void pushBack(const INode &node) override final;

        /// @copydoc IArrayNode::insert()
        void insert(Index index, const INode &node) override final;

        /// @copydoc IArrayNode::replace()
        void replace(Index index, const INode &node) override final;

        /// @copydoc IArrayNode::popBack()
        void popBack() override final;

        /// @copydoc IArrayNode::erase()
        void erase(Index index) override final;

        /// @copydoc IArrayNode::clear()
        void clear() override final;

        /// @copydoc IArrayNode::empty()
        bool empty() const override final;

        /// @copydoc IArrayNode::size()
        size_t size() const override final;

        /// @copydoc IArrayNode::cbegin()
        const_iterator cbegin() const override final;

        /// @copydoc IArrayNode::begin()
        const_iterator begin() const override final;

        /// @copydoc IArrayNode::begin()
        iterator begin() override final;

        /// @copydoc IArrayNode::cend()
        const_iterator cend() const override final;

        /// @copydoc IArrayNode::end()
        const_iterator end() const override final;

        /// @copydoc IArrayNode::end()
        iterator end() override final;

      private:
        std::vector<NodePtr> _nodes;
    };
} // namespace util::serialization

#endif /* !UTIL_SERIALIZATION_TOML_TOMLARRAYNODE_HPP_ */
