///
/// @file TomlObjectNode.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief Toml object node declaration.
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
        [[nodiscard]] NodeCView get(std::string_view key) const override final;

        /// @copydoc IObjectNode::get()
        [[nodiscard]] NodeView get(std::string_view key) override final;

        /// @copydoc IObjectNode::tryGet()
        [[nodiscard]] NodeCView tryGet(std::string_view key) const noexcept override final;

        /// @copydoc IObjectNode::tryGet()
        [[nodiscard]] NodeView tryGet(std::string_view key) noexcept override final;

        /// @copydoc IObjectNode::insert()
        bool insert(std::string_view key, const INode &value) override final;

        /// @copydoc IObjectNode::insertOrAssign()
        bool insertOrAssign(std::string_view key, const INode &value) override final;

        /// @copydoc IObjectNode::erase()
        void erase(std::string_view key) override final;

        /// @copydoc IObjectNode::clear()
        void clear() noexcept override final;

        /// @copydoc IObjectNode::empty()
        [[nodiscard]] bool empty() const noexcept override final;

        /// @copydoc IObjectNode::size()
        [[nodiscard]] size_t size() const noexcept override final;

        /// @copydoc IObjectNode::contains()
        [[nodiscard]] bool contains(std::string_view key) const noexcept override final;

        /// @copydoc IObjectNode::cbegin()
        [[nodiscard]] const_iterator cbegin() const noexcept override final;

        /// @copydoc IObjectNode::begin()
        [[nodiscard]] const_iterator begin() const noexcept override final;

        /// @copydoc IObjectNode::begin()
        [[nodiscard]] iterator begin() noexcept override final;

        /// @copydoc IObjectNode::cend()
        [[nodiscard]] const_iterator cend() const noexcept override final;

        /// @copydoc IObjectNode::end()
        [[nodiscard]] const_iterator end() const noexcept override final;

        /// @copydoc IObjectNode::end()
        [[nodiscard]] iterator end() noexcept override final;

      private:
        ////
        /// @brief Internal nodes map.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-18)
        ///
        std::map<std::string, NodePtr, std::less<>> _nodes;
    };
} // namespace util::serialization

#endif /* !UTIL_SERIALIZATION_TOML_TOMLOBJECTNODE_HPP_ */
