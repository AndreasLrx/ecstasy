///
/// @file INode.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief Serialization node interface.
/// @version 1.0.0
/// @date 2022-12-08
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef UTIL_SERIALIZATION_INODE_HPP_
#define UTIL_SERIALIZATION_INODE_HPP_

#include <chrono>
#include <memory>
#include <optional>
#include <string>
#include <string_view>

namespace util::serialization
{
    class IObjectNode;
    class IArrayNode;

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

        /// @brief @ref Type::Date underlying type.
        using Date = std::chrono::year_month_day;

        /// @brief @ref Type::Time underlying type.
        using Time = std::chrono::nanoseconds;

        /// @brief @ref Type::DateTime underlying type.
        using DateTime = std::chrono::high_resolution_clock::time_point;

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
        [[nodiscard]] virtual Type getType() const noexcept = 0;

        ///
        /// @brief Check if the node type is @p type.
        ///
        /// @return bool Whether the node type is @p type.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        [[nodiscard]] virtual bool isType(Type type) const noexcept = 0;

        ///
        /// @brief Check if the node type is @ref Type::Null.
        ///
        /// @return bool Whether the node type is @ref Type::Null.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        [[nodiscard]] virtual bool isNull() const noexcept = 0;

        ///
        /// @brief Check if the node type is @ref Type::Object.
        ///
        /// @return bool Whether the node type is @ref Type::Object.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        [[nodiscard]] virtual bool isObject() const noexcept = 0;

        ///
        /// @brief Check if the node type is @ref Type::Array.
        ///
        /// @return bool Whether the node type is @ref Type::Array.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        [[nodiscard]] virtual bool isArray() const noexcept = 0;

        ///
        /// @brief Check if the node type is @ref Type::String.
        ///
        /// @return bool Whether the node type is @ref Type::String.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        [[nodiscard]] virtual bool isString() const noexcept = 0;

        ///
        /// @brief Check if the node type is @ref Type::Integer.
        ///
        /// @return bool Whether the node type is @ref Type::Integer.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        [[nodiscard]] virtual bool isInteger() const noexcept = 0;

        ///
        /// @brief Check if the node type is @ref Type::Float.
        ///
        /// @return bool Whether the node type is @ref Type::Float.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        [[nodiscard]] virtual bool isFloat() const noexcept = 0;

        ///
        /// @brief Check if the node type is @ref Type::Boolean.
        ///
        /// @return bool Whether the node type is @ref Type::Boolean.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        [[nodiscard]] virtual bool isBoolean() const noexcept = 0;

        ///
        /// @brief Check if the node type is @ref Type::Date.
        ///
        /// @return bool Whether the node type is @ref Type::Date.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        [[nodiscard]] virtual bool isDate() const noexcept = 0;

        ///
        /// @brief Check if the node type is @ref Type::Time.
        ///
        /// @return bool Whether the node type is @ref Type::Time.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        [[nodiscard]] virtual bool isTime() const noexcept = 0;

        ///
        /// @brief Check if the node type is @ref Type::DateTime.
        ///
        /// @return bool Whether the node type is @ref Type::DateTime.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        [[nodiscard]] virtual bool isDateTime() const noexcept = 0;

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
        [[nodiscard]] virtual const IObjectNode &asObject() const = 0;

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
        [[nodiscard]] virtual IObjectNode &asObject() = 0;

        ///
        /// @brief Try to get the node object value.
        ///
        /// @return std::optional<const IObjectNode &> Filled with the node string if the type is @ref Type::Object,
        /// empty otherwise.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        [[nodiscard]] virtual std::optional<std::reference_wrapper<const IObjectNode>> tryAsObject() const noexcept = 0;

        ///
        /// @brief Try to get the node object value.
        ///
        /// @return std::optional<IObjectNode &> Filled with the node string if the type is @ref Type::Object,
        /// empty otherwise.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        [[nodiscard]] virtual std::optional<std::reference_wrapper<IObjectNode>> tryAsObject() noexcept = 0;

        ///
        /// @brief Get the array node value.
        ///
        /// @return const IArrayNode& node value.
        ///
        /// @throw std::runtime_error If the node is the node type is not @ref Type::Array.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        [[nodiscard]] virtual const IArrayNode &asArray() const = 0;

        ///
        /// @brief Get the array node value.
        ///
        /// @return IArrayNode& node value.
        ///
        /// @throw std::runtime_error If the node is the node type is not @ref Type::Array.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        [[nodiscard]] virtual IArrayNode &asArray() = 0;

        ///
        /// @brief Try to get the node array value.
        ///
        /// @return std::optional<const IArrayNode &> Filled with the node string if the type is @ref Type::Array,
        /// empty otherwise.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        [[nodiscard]] virtual std::optional<std::reference_wrapper<const IArrayNode>> tryAsArray() const noexcept = 0;

        ///
        /// @brief Try to get the node array value.
        ///
        /// @return std::optional<IArrayNode &> Filled with the node string if the type is @ref Type::Array,
        /// empty otherwise.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        [[nodiscard]] virtual std::optional<std::reference_wrapper<IArrayNode>> tryAsArray() noexcept = 0;

        ///
        /// @brief Set the String value of the node.
        ///
        /// @param[in] value New node value.
        ///
        /// @throw std::runtime_error If the node is the node type is not @ref Type::String.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-18)
        ///
        virtual void setString(std::string_view value) = 0;

        ///
        /// @brief Get the string node value.
        ///
        /// @return std::string_view node value.
        ///
        /// @throw std::bad_optional_cast If the node is the node type is not @ref Type::String.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        [[nodiscard]] virtual std::string_view asString() const = 0;

        ///
        /// @brief Try to get the node string value.
        ///
        /// @return std::optional<std::string_view> Filled with the node string if the type is @ref Type::String, empty
        /// otherwise.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        [[nodiscard]] virtual std::optional<std::string_view> tryAsString() const noexcept = 0;

        ///
        /// @brief Set the Integer value of the node.
        ///
        /// @param[in] value New node value.
        ///
        /// @throw std::runtime_error If the node is the node type is not @ref Type::Integer.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-18)
        ///
        virtual void setInteger(int64_t value) = 0;

        ///
        /// @brief Get the integer node value.
        ///
        /// @return int node value.
        ///
        /// @throw std::bad_optional_cast If the node is the node type is not @ref Type::Integer.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        [[nodiscard]] virtual int64_t asInteger() const = 0;

        ///
        /// @brief Try to get the node integer value.
        ///
        /// @return std::optional<int64_t> Filled with the node integer if the type is @ref Type::Integer, empty
        /// otherwise.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        [[nodiscard]] virtual std::optional<int64_t> tryAsInteger() const noexcept = 0;

        ///
        /// @brief Set the Float value of the node.
        ///
        /// @param[in] value New node value.
        ///
        /// @throw std::runtime_error If the node is the node type is not @ref Type::Float.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-18)
        ///
        virtual void setFloat(double value) = 0;

        ///
        /// @brief Get the float node value.
        ///
        /// @return double node value.
        ///
        /// @throw std::bad_optional_cast If the node is the node type is not @ref Type::Float.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        [[nodiscard]] virtual double asFloat() const = 0;

        ///
        /// @brief Try to get the node float value.
        ///
        /// @return std::optional<double> Filled with the node float if the type is @ref Type::Float, empty
        /// otherwise.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        [[nodiscard]] virtual std::optional<double> tryAsFloat() const noexcept = 0;

        ///
        /// @brief Set the Boolean value of the node.
        ///
        /// @param[in] value New node value.
        ///
        /// @throw std::runtime_error If the node is the node type is not @ref Type::Boolean.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-18)
        ///
        virtual void setBoolean(bool value) = 0;

        ///
        /// @brief Get the boolean node value.
        ///
        /// @return bool node value.
        ///
        /// @throw std::bad_optional_cast If the node is the node type is not @ref Type::Boolean.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        [[nodiscard]] virtual bool asBoolean() const = 0;

        ///
        /// @brief Try to get the node boolean value.
        ///
        /// @return std::optional<bool> Filled with the node boolean if the type is @ref Type::Boolean, empty
        /// otherwise.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        [[nodiscard]] virtual std::optional<bool> tryAsBoolean() const noexcept = 0;

        ///
        /// @brief Set the Date value of the node.
        ///
        /// @param[in] value New node value.
        ///
        /// @throw std::runtime_error If the node is the node type is not @ref Type::Date.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-18)
        ///
        virtual void setDate(const Date &value) = 0;

        ///
        /// @brief Get the date node value.
        ///
        /// @return Date node value.
        ///
        /// @throw std::bad_optional_cast If the node is the node type is not @ref Type::Date.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        [[nodiscard]] virtual Date asDate() const = 0;

        ///
        /// @brief Try to get the node date value.
        ///
        /// @return std::optional<Date> Filled with the node date if the type is @ref Type::Date, empty
        /// otherwise.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        [[nodiscard]] virtual std::optional<Date> tryAsDate() const noexcept = 0;

        ///
        /// @brief Set the Time value of the node.
        ///
        /// @param[in] value New node value.
        ///
        /// @throw std::runtime_error If the node is the node type is not @ref Type::Time.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-18)
        ///
        virtual void setTime(const Time &value) = 0;

        ///
        /// @brief Get the time node value.
        ///
        /// @return Time node value.
        ///
        /// @throw std::bad_optional_cast If the node is the node type is not @ref Type::Time.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        [[nodiscard]] virtual Time asTime() const = 0;

        ///
        /// @brief Try to get the node time value.
        ///
        /// @return std::optional<Time> Filled with the node time if the type is @ref Type::Time, empty
        /// otherwise.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        [[nodiscard]] virtual std::optional<Time> tryAsTime() const noexcept = 0;

        ///
        /// @brief Set the Date Time value of the node.
        ///
        /// @param[in] value New node value.
        ///
        /// @throw std::runtime_error If the node is the node type is not @ref Type::DateTime.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-18)
        ///
        virtual void setDateTime(const DateTime &value) = 0;

        ///
        /// @brief Get the datetime node value.
        ///
        /// @return DateTime node value.
        ///
        /// @throw std::bad_optional_cast If the node is the node type is not @ref Type::DateTime.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        [[nodiscard]] virtual DateTime asDateTime() const = 0;

        ///
        /// @brief Try to get the node datetime value.
        ///
        /// @return std::optional<DateTime> Filled with the node datetime if the type is @ref Type::DateTime, empty
        /// otherwise.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        [[nodiscard]] virtual std::optional<DateTime> tryAsDateTime() const noexcept = 0;
    };

    /// @brief Pointer to node.
    using NodePtr = std::shared_ptr<INode>;
    /// @brief Pointer to const node.
    using NodeCPtr = std::shared_ptr<const INode>;

    /// @brief Non owning pointer to node.
    using NodeView = std::weak_ptr<INode>;
    /// @brief Non owning pointer to const node.
    using NodeCView = std::weak_ptr<const INode>;
} // namespace util::serialization

#endif /* !UTIL_SERIALIZATION_INODE_HPP_ */
