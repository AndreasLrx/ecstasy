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

#include "TomlConversion.hpp"
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
    template <typename N>
    requires std::derived_from<N, toml::node> && std::copy_constructible<N>
    class TomlNode : public ANode {
      public:
        TomlNode(const N &node) : _node(node)
        {
            switch (_node.type()) {
                case toml::node_type::table: _type = INode::Type::Object; break;
                case toml::node_type::array: _type = INode::Type::Array; break;
                case toml::node_type::string: _type = INode::Type::String; break;
                case toml::node_type::integer: _type = INode::Type::Integer; break;
                case toml::node_type::floating_point: _type = INode::Type::Float; break;
                case toml::node_type::boolean: _type = INode::Type::Boolean; break;
                case toml::node_type::date: _type = INode::Type::Date; break;
                case toml::node_type::time: _type = INode::Type::Time; break;
                case toml::node_type::date_time: _type = INode::Type::DateTime; break;
                default: _type = INode::Type::Unknown; break;
            }
        }

        /// @copydoc INode::getType()
        constexpr INode::Type getType() const override final
        {
            return _type;
        }

        /// @copydoc INode::tryAsString()
        std::optional<std::string_view> tryAsString() const override final
        {
            return _node.template value<std::string_view>();
        }

        /// @copydoc INode::tryAsInteger()
        std::optional<int> tryAsInteger() const override final
        {
            return _node.template value<int>();
        }

        /// @copydoc INode::tryAsFloat()
        std::optional<float> tryAsFloat() const override final
        {
            return _node.template value<float>();
        }

        /// @copydoc INode::tryAsBoolean()
        std::optional<bool> tryAsBoolean() const override final
        {
            return _node.template value<bool>();
        }

        /// @copydoc INode::tryAsDate()
        std::optional<Date> tryAsDate() const override final
        {
            auto date = _node.template value<toml::date>();

            if (!date)
                return std::optional<Date>();

            return TomlConversion::fromToml(*date);
        }

        /// @copydoc INode::tryAsTime()
        std::optional<Time> tryAsTime() const override final
        {
            auto time = _node.template value<toml::time>();

            if (!time)
                return std::optional<Time>();
            return TomlConversion::fromToml(*time);
        }

        /// @copydoc INode::tryAsDateTime()
        std::optional<DateTime> tryAsDateTime() const override final
        {
            auto dateTime = _node.template value<toml::date_time>();

            if (!dateTime)
                return std::optional<DateTime>();
            return TomlConversion::fromToml(*dateTime);
        }

        ///
        /// @brief Get the internal toml node.
        ///
        /// @return toml::node& internal node.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-09)
        ///
        constexpr N &getTomlNode()
        {
            return _node;
        }

        ///
        /// @brief Get the internal toml node.
        ///
        /// @return toml::node& internal node.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-09)
        ///
        constexpr const N &getTomlNode() const
        {
            return _node;
        }

      protected:
        INode::Type _type;
        N _node;
    };
} // namespace util::serialization

#endif /* !UTIL_SERIALIZATION_TOML_TOMLNODE_HPP_ */
