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

        ///
        /// @brief Get the internal toml node.
        ///
        /// @return toml::node& internal node.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-09)
        ///
        toml::node &getTomlNode();

        ///
        /// @brief Get the internal toml node.
        ///
        /// @return toml::node& internal node.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-09)
        ///
        const toml::node &getTomlNode() const;

        ///
        /// @brief Convert a @ref INode::Date to a @ref toml::date.
        ///
        /// @param[in] date common serializer date.
        ///
        /// @return toml::date Equivalent toml date.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-09)
        ///
        static toml::date toToml(INode::Date date);

        ///
        /// @brief Convert a @ref INode::Time to a @ref toml::time.
        ///
        /// @param[in] time common serializer time.
        ///
        /// @return toml::time Equivalent toml time.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-09)
        ///
        static toml::time toToml(INode::Time time);

        ///
        /// @brief Convert a @ref INode::DateTime to a @ref toml::date_time.
        ///
        /// @param[in] dateTime common serializer date time.
        ///
        /// @return toml::date_time Equivalent toml date time.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-09)
        ///
        static toml::date_time toToml(INode::DateTime dateTime);

        ///
        /// @brief Convert a @ref toml::date to a @ref INode::Date.
        ///
        /// @param[in] date toml date.
        ///
        /// @return INode::Date Equivalent serialize date.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-09)
        ///
        static INode::Date fromToml(toml::date date);

        ///
        /// @brief Convert a @ref toml::time to a @ref INode::Time.
        ///
        /// @param[in] time toml time.
        ///
        /// @return INode::Time Equivalent serializer time.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-09)
        ///
        static INode::Time fromToml(toml::time time);

        ///
        /// @brief Convert a @ref toml::date_time to a @ref INode::DateTime.
        ///
        /// @param[in] dateTime toml date time.
        ///
        /// @return INode::DateTime Equivalent serializer date time.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-09)
        ///
        static INode::DateTime fromToml(toml::date_time dateTime);

      protected:
        toml::node &_node;
    };
} // namespace util::serialization

#endif /* !UTIL_SERIALIZATION_TOML_TOMLNODE_HPP_ */
