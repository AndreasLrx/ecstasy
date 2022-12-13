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
        ///
        /// @brief Default constructor.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-12)
        ///
        TomlNode() = default;

        ///
        /// @brief Construct a new Toml Node from a @ref toml::node derived class object.
        ///
        /// @param[in] node Inner node.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-12)
        ///
        TomlNode(const N &node) : _node(node)
        {
        }

        ///
        /// @brief Construct a new Toml Node from a node data.
        ///
        /// @tparam Args Types of the internal node constructor.
        ///
        /// @param[in] args Arguments to forward to the internal node constructor.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-12)
        ///
        template <typename... Args>
        TomlNode(Args &&...args) : _node(std::forward<Args &&>(args)...)
        {
        }

        /// @copydoc INode::getType()
        constexpr INode::Type getType() const override final
        {
            if constexpr (std::same_as<toml::table, N>)
                return INode::Type::Object;
            else if constexpr (std::same_as<toml::array, N>)
                return INode::Type::Array;
            else if constexpr (std::same_as<toml::value<std::string>, N>)
                return INode::Type::String;
            else if constexpr (std::same_as<toml::value<int64_t>, N>)
                return INode::Type::Integer;
            else if constexpr (std::same_as<toml::value<double>, N>)
                return INode::Type::Float;
            else if constexpr (std::same_as<toml::value<bool>, N>)
                return INode::Type::Boolean;
            else if constexpr (std::same_as<toml::value<toml::date>, N>)
                return INode::Type::Date;
            else if constexpr (std::same_as<toml::value<toml::time>, N>)
                return INode::Type::Time;
            else if constexpr (std::same_as<toml::value<toml::date_time>, N>)
                return INode::Type::DateTime;
            else
                return INode::Type::Unknown;
        }

        /// @copydoc INode::tryAsString()
        std::optional<std::string_view> tryAsString() const override final
        {
            if constexpr (std::same_as<toml::value<std::string>, N>)
                return _node.get();
            else
                return std::optional<std::string_view>();
        }

        /// @copydoc INode::tryAsInteger()
        std::optional<int64_t> tryAsInteger() const override final
        {
            if constexpr (std::same_as<toml::value<int64_t>, N>)
                return _node.get();
            else
                return std::optional<int64_t>();
        }

        /// @copydoc INode::tryAsFloat()
        std::optional<double> tryAsFloat() const override final
        {
            if constexpr (std::same_as<toml::value<double>, N>)
                return _node.get();
            else
                return std::optional<double>();
        }

        /// @copydoc INode::tryAsBoolean()
        std::optional<bool> tryAsBoolean() const override final
        {
            if constexpr (std::same_as<toml::value<bool>, N>)
                return _node.get();
            else
                return std::optional<bool>();
        }

        /// @copydoc INode::tryAsDate()
        std::optional<Date> tryAsDate() const override final
        {
            if constexpr (std::same_as<toml::value<toml::date>, N>)
                return TomlConversion::fromToml(_node.get());
            else
                return std::optional<Date>();
        }

        /// @copydoc INode::tryAsTime()
        std::optional<Time> tryAsTime() const override final
        {
            if constexpr (std::same_as<toml::value<toml::time>, N>)
                return TomlConversion::fromToml(_node.get());
            else
                return std::optional<Time>();
        }

        /// @copydoc INode::tryAsDateTime()
        std::optional<DateTime> tryAsDateTime() const override final
        {
            if constexpr (std::same_as<toml::value<toml::date_time>, N>)
                return TomlConversion::fromToml(_node.get());
            else
                return std::optional<DateTime>();
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
        N _node;
    };

} // namespace util::serialization

#endif /* !UTIL_SERIALIZATION_TOML_TOMLNODE_HPP_ */
