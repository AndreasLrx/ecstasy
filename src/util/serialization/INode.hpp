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

namespace util::serialization
{
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
            DateTime,     ///< DateTime node.
            Count         ///< Keep last - number of available node types.
        };

        /// @brief Default destructor.
        virtual ~INode() = default;

        ////////////////////
        ///  Types check ///
        ////////////////////

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
    };
} // namespace util::serialization

#endif /* !UTIL_SERIALIZATION_ISERIALIZER_HPP_ */
