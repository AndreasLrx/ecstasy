///
/// @file TomlNode.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-12-08
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef UTIL_SERIALIZATION_TOML_TOMLNODE_HPP_
#define UTIL_SERIALIZATION_TOML_TOMLNODE_HPP_

#include "util/serialization/ANode.hpp"
#include <toml++/toml.h>

namespace util::serialization
{
    ///
    /// @brief Serialization node. Can be any type of @ref INode::Type.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-12-08)
    ///
    class TomlNode : public ANode {
      public:
        ///
        /// @brief Construct a new Toml Node.
        ///
        /// @param[in] node Toml underlying node.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        TomlNode(toml::node &node);

        /// @copydoc INode::tryAsString()
        std::optional<std::string_view> tryAsString() const override final;

        /// @copydoc INode::tryAsInteger()
        std::optional<int> tryAsInteger() const override final;

        /// @copydoc INode::tryAsFloat()
        std::optional<float> tryAsFloat() const override final;

        /// @copydoc INode::tryAsBoolean()
        std::optional<bool> tryAsBoolean() const override final;

        /// @copydoc INode::tryAsDate()
        std::optional<Date> tryAsDate() const override final;

        /// @copydoc INode::tryAsTime()
        std::optional<Time> tryAsTime() const override final;

        /// @copydoc INode::tryAsDateTime()
        std::optional<DateTime> tryAsDateTime() const override final;

      private:
        toml::node &_node;
    };
} // namespace util::serialization

#endif /* !UTIL_SERIALIZATION_TOML_TOMLNODE_HPP_ */
