///
/// @file TomlNode.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief Toml node implementation.
/// @version 1.0.0
/// @date 2022-12-08
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
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
        TomlNode() noexcept(std::is_nothrow_default_constructible_v<N>) = default;

        ///
        /// @brief Construct a new Toml Node from a @ref toml::node derived class object.
        ///
        /// @param[in] node Inner node.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-12)
        ///
        TomlNode(const N &node) noexcept(std::is_nothrow_copy_constructible_v<N>) : _node(node)
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
        TomlNode(Args &&...args) noexcept(std::is_nothrow_constructible_v<N, Args...>)
            : _node(std::forward<Args &&>(args)...)
        {
        }

        /// @copydoc INode::getType()
        [[nodiscard]] constexpr INode::Type getType() const noexcept override final
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
        [[nodiscard]] std::optional<std::string_view> tryAsString() const noexcept override final
        {
            if constexpr (std::same_as<toml::value<std::string>, N>)
                return _node.get();
            else
                return std::optional<std::string_view>();
        }

        /// @copydoc INode::tryAsInteger()
        [[nodiscard]] std::optional<int64_t> tryAsInteger() const noexcept override final
        {
            if constexpr (std::same_as<toml::value<int64_t>, N>)
                return _node.get();
            else
                return std::optional<int64_t>();
        }

        /// @copydoc INode::tryAsFloat()
        [[nodiscard]] std::optional<double> tryAsFloat() const noexcept override final
        {
            if constexpr (std::same_as<toml::value<double>, N>)
                return _node.get();
            else
                return std::optional<double>();
        }

        /// @copydoc INode::tryAsBoolean()
        [[nodiscard]] std::optional<bool> tryAsBoolean() const noexcept override final
        {
            if constexpr (std::same_as<toml::value<bool>, N>)
                return _node.get();
            else
                return std::optional<bool>();
        }

        /// @copydoc INode::tryAsDate()
        [[nodiscard]] std::optional<Date> tryAsDate() const noexcept override final
        {
            if constexpr (std::same_as<toml::value<toml::date>, N>)
                return TomlConversion::fromToml(_node.get());
            else
                return std::optional<Date>();
        }

        /// @copydoc INode::tryAsTime()
        [[nodiscard]] std::optional<Time> tryAsTime() const noexcept override final
        {
            if constexpr (std::same_as<toml::value<toml::time>, N>)
                return TomlConversion::fromToml(_node.get());
            else
                return std::optional<Time>();
        }

        /// @copydoc INode::tryAsDateTime()
        [[nodiscard]] std::optional<DateTime> tryAsDateTime() const noexcept override final
        {
            if constexpr (std::same_as<toml::value<toml::date_time>, N>)
                return TomlConversion::fromToml(_node.get());
            else
                return std::optional<DateTime>();
        }

      protected:
        /// @brief Inner node.
        N _node;
    };

} // namespace util::serialization

#endif /* !UTIL_SERIALIZATION_TOML_TOMLNODE_HPP_ */
