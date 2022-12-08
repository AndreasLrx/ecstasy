///
/// @file ISerializer.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-12-08
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef UTIL_SERIALIZATION_ISERIALIZER_HPP_
#define UTIL_SERIALIZATION_ISERIALIZER_HPP_

#include <chrono>
#include <optional>
#include <string>
#include <string_view>

namespace util::serialization
{
    class IObjectNode;

    ///
    /// @brief Serialization node. Can be any type of @ref INode::Type.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-12-08)
    ///
    class INode {
      public:
        ///
        /// @brief Available node types.
        ///
        /// @warning Some types may not be available depending on the underlying format.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        enum class Type {
            Unknown = -1, ///< Unknown type.
            Null,         ///< Null node (none/null).
            Object,       ///< Node containing multiple named nodes. (similar to a std::map<string, INode>)
            Array,        ///< Node containing multiple unnamed nodes. (similar to a std::vector<string>)
            String,       ///< String node.
            Integer,      ///< Integer number node.
            Float,        ///< Floating point number node.
            Boolean,      ///< Boolean value node.
            Date,         ///< Date node.
            Time,         ///< Time node.
            DateTime,     ///< DateTime node. (Timestamp)
            Count         ///< Keep last - number of available node types.
        };

        /// @brief @ref Type::Date type.
        using Date = std::chrono::year_month_day;

        /// @brief @ref Type::Time type.
        using Time = std::chrono::duration<std::chrono::seconds>;

        /// @brief @ref Type::DateTime type.
        using DateTime = std::time_t;

        /// @brief Default destructor.
        virtual ~INode() = default;

        /////////////////////
        ///  Types check  ///
        /////////////////////

        ///
        /// @brief Get the node type.
        ///
        /// @return Type Node type.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual Type getType() const = 0;

        ///
        /// @brief Check if the node type is @p type.
        ///
        /// @return bool Whether the node type is @p type.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual bool isType(Type type) const = 0;

        ///
        /// @brief Check if the node type is @ref Type::Null.
        ///
        /// @return bool Whether the node type is @ref Type::Null.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual bool isNull() const = 0;

        ///
        /// @brief Check if the node type is @ref Type::Object.
        ///
        /// @return bool Whether the node type is @ref Type::Object.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual bool isObject() const = 0;

        ///
        /// @brief Check if the node type is @ref Type::Array.
        ///
        /// @return bool Whether the node type is @ref Type::Array.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual bool isArray() const = 0;

        ///
        /// @brief Check if the node type is @ref Type::String.
        ///
        /// @return bool Whether the node type is @ref Type::String.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual bool isString() const = 0;

        ///
        /// @brief Check if the node type is @ref Type::Integer.
        ///
        /// @return bool Whether the node type is @ref Type::Integer.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual bool isInteger() const = 0;

        ///
        /// @brief Check if the node type is @ref Type::Float.
        ///
        /// @return bool Whether the node type is @ref Type::Float.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual bool isFloat() const = 0;

        ///
        /// @brief Check if the node type is @ref Type::Boolean.
        ///
        /// @return bool Whether the node type is @ref Type::Boolean.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual bool isBoolean() const = 0;

        ///
        /// @brief Check if the node type is @ref Type::Date.
        ///
        /// @return bool Whether the node type is @ref Type::Date.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual bool isDate() const = 0;

        ///
        /// @brief Check if the node type is @ref Type::Time.
        ///
        /// @return bool Whether the node type is @ref Type::Time.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual bool isTime() const = 0;

        ///
        /// @brief Check if the node type is @ref Type::DateTime.
        ///
        /// @return bool Whether the node type is @ref Type::DateTime.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual bool isDateTime() const = 0;

        //////////////////////
        ///  Value getter  ///
        //////////////////////

        ///
        /// @brief Get the object node value.
        ///
        /// @return const IObjectNode& node value.
        ///
        /// @throw std::runtime_error If the node is the node type is not @ref Type::Object.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual const IObjectNode &asObject() const = 0;

        ///
        /// @brief Get the object node value.
        ///
        /// @return IObjectNode& node value.
        ///
        /// @throw std::runtime_error If the node is the node type is not @ref Type::Object.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual IObjectNode &asObject() = 0;

        ///
        /// @brief Try to get the node object value.
        ///
        /// @return std::optional<const IObjectNode &> Filled with the node string if the type is @ref Type::Object,
        /// empty otherwise.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual std::optional<const IObjectNode &> tryAsObject() const = 0;

        ///
        /// @brief Try to get the node object value.
        ///
        /// @return std::optional<IObjectNode &> Filled with the node string if the type is @ref Type::Object,
        /// empty otherwise.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual std::optional<IObjectNode &> tryAsObject() = 0;

        /// @todo Create IArrayNode
        // virtual const INode &asArray() const = 0;
        // virtual INode &asArray() = 0;

        ///
        /// @brief Get the string node value.
        ///
        /// @return std::string_view node value.
        ///
        /// @throw std::runtime_error If the node is the node type is not @ref Type::String.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual std::string_view asString() const = 0;

        ///
        /// @brief Try to get the node string value.
        ///
        /// @return std::optional<std::string_view> Filled with the node string if the type is @ref Type::String, empty
        /// otherwise.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual std::optional<std::string_view> tryAsString() const = 0;

        ///
        /// @brief Get the integer node value.
        ///
        /// @return int node value.
        ///
        /// @throw std::runtime_error If the node is the node type is not @ref Type::Integer.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual int asInteger() const = 0;

        ///
        /// @brief Try to get the node integer value.
        ///
        /// @return std::optional<int> Filled with the node integer if the type is @ref Type::Integer, empty
        /// otherwise.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual std::optional<int> tryAsInteger() const = 0;

        ///
        /// @brief Get the float node value.
        ///
        /// @return float node value.
        ///
        /// @throw std::runtime_error If the node is the node type is not @ref Type::Float.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual float asFloat() const = 0;

        ///
        /// @brief Try to get the node float value.
        ///
        /// @return std::optional<float> Filled with the node float if the type is @ref Type::Float, empty
        /// otherwise.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual std::optional<float> tryAsFloat() const = 0;

        ///
        /// @brief Get the boolean node value.
        ///
        /// @return bool node value.
        ///
        /// @throw std::runtime_error If the node is the node type is not @ref Type::Boolean.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual bool asBoolean() const = 0;

        ///
        /// @brief Try to get the node boolean value.
        ///
        /// @return std::optional<bool> Filled with the node boolean if the type is @ref Type::Boolean, empty
        /// otherwise.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual std::optional<bool> tryAsBoolean() const = 0;

        ///
        /// @brief Get the date node value.
        ///
        /// @return Date node value.
        ///
        /// @throw std::runtime_error If the node is the node type is not @ref Type::Date.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual Date asDate() const = 0;

        ///
        /// @brief Try to get the node date value.
        ///
        /// @return std::optional<Date> Filled with the node date if the type is @ref Type::Date, empty
        /// otherwise.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual std::optional<Date> tryAsDate() const = 0;

        ///
        /// @brief Get the time node value.
        ///
        /// @return Time node value.
        ///
        /// @throw std::runtime_error If the node is the node type is not @ref Type::Time.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual Time asTime() const = 0;

        ///
        /// @brief Try to get the node time value.
        ///
        /// @return std::optional<Time> Filled with the node time if the type is @ref Type::Time, empty
        /// otherwise.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual std::optional<Time> tryAsTime() const = 0;

        ///
        /// @brief Get the datetime node value.
        ///
        /// @return DateTime node value.
        ///
        /// @throw std::runtime_error If the node is the node type is not @ref Type::DateTime.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual DateTime asDateTime() const = 0;

        ///
        /// @brief Try to get the node datetime value.
        ///
        /// @return std::optional<DateTime> Filled with the node datetime if the type is @ref Type::DateTime, empty
        /// otherwise.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual std::optional<DateTime> tryAsDateTime() const = 0;
    };
} // namespace util::serialization

#endif /* !UTIL_SERIALIZATION_ISERIALIZER_HPP_ */
