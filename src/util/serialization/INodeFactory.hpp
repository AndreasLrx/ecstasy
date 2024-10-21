///
/// @file INodeFactory.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief Node factory to hide underlying format.
/// @version 1.0.0
/// @date 2022-12-12
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef UTIL_SERIALIZATION_INODEFACTORY_HPP_
#define UTIL_SERIALIZATION_INODEFACTORY_HPP_

#include "INode.hpp"

namespace util::serialization
{
    ///
    /// @brief Node factory to hide underlying format.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-12-12)
    ///
    class INodeFactory {
      public:
        /// @brief Default destructor.
        virtual ~INodeFactory() = default;

        ///
        /// @brief Export a node to a stream.
        ///
        /// @param[in] node Node to export.
        /// @param[in] stream Stream to export to.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-21)
        ///
        virtual void exportStream(const INode &node, std::ostream &stream) = 0;

        ///
        /// @brief Import a node from a stream.
        ///
        /// @param[in] stream Stream to import from.
        ///
        /// @return NodePtr Imported node, or nullptr if the stream is invalid.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-21)
        ///
        virtual NodePtr fromStream(std::istream &stream) = 0;

        ///
        /// @brief Construct an empty node from its type.
        ///
        /// @note @ref INode::Object and @ref INode::Array will return @ref IObjectNode and @ref IArrayNode pointers
        /// respectively.
        ///
        /// @param[in] type Node type.
        ///
        /// @return NodePtr New node instance, or nullptr if the type is not supported.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-12)
        ///
        [[nodiscard]] virtual NodePtr create(INode::Type type) = 0;

        ///
        /// @brief Construct a copy of an existing node.
        ///
        /// @param[in] node Node to copy.
        ///
        /// @return NodePtr New node instance, or nullptr if the type/copy is not supported.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-12)
        ///
        [[nodiscard]] virtual NodePtr create(const INode &node) = 0;

        ///
        /// @brief Construct a @ref INode::Type::Object node.
        ///
        /// @param[in] object node data.
        ///
        /// @return NodePtr New node instance, or nullptr if the type/copy is not supported.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-12)
        ///
        [[nodiscard]] virtual NodePtr createObject(const IObjectNode &object) = 0;

        ///
        /// @brief Construct a @ref INode::Type::Array node.
        ///
        /// @param[in] array node data.
        ///
        /// @return NodePtr New node instance, or nullptr if the type/copy is not supported.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-12)
        ///
        [[nodiscard]] virtual NodePtr createArray(const IArrayNode &array) = 0;

        ///
        /// @brief Construct a @ref INode::Type::String node.
        ///
        /// @param[in] string node data.
        ///
        /// @return NodePtr New node instance, or nullptr if the type/copy is not supported.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-12)
        ///
        [[nodiscard]] virtual NodePtr create(std::string_view string) = 0;

        ///
        /// @brief Construct a @ref INode::Type::Integer node.
        ///
        /// @param[in] integer node data.
        ///
        /// @return NodePtr New node instance, or nullptr if the type/copy is not supported.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-12)
        ///
        [[nodiscard]] virtual NodePtr create(int64_t integer) = 0;

        ///
        /// @brief Construct a @ref INode::Type::Float node.
        ///
        /// @param[in] floatingPoint node data.
        ///
        /// @return NodePtr New node instance, or nullptr if the type/copy is not supported.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-12)
        ///
        [[nodiscard]] virtual NodePtr create(double floatingPoint) = 0;

        ///
        /// @brief Construct a @ref INode::Type::Boolean node.
        ///
        /// @param[in] boolean node data.
        ///
        /// @return NodePtr New node instance, or nullptr if the type/copy is not supported.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-12)
        ///
        [[nodiscard]] virtual NodePtr create(bool boolean) = 0;

        ///
        /// @brief Construct a @ref INode::Type::Date node.
        ///
        /// @param[in] date node data.
        ///
        /// @return NodePtr New node instance, or nullptr if the type/copy is not supported.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-12)
        ///
        [[nodiscard]] virtual NodePtr create(INode::Date date) = 0;

        ///
        /// @brief Construct a @ref INode::Type::Time node.
        ///
        /// @param[in] time node data.
        ///
        /// @return NodePtr New node instance, or nullptr if the type/copy is not supported.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-12)
        ///
        [[nodiscard]] virtual NodePtr create(INode::Time time) = 0;

        ///
        /// @brief Construct a @ref INode::Type::DateTime node.
        ///
        /// @param[in] dateTime node data.
        ///
        /// @return NodePtr New node instance, or nullptr if the type/copy is not supported.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-12)
        ///
        [[nodiscard]] virtual NodePtr create(INode::DateTime dateTime) = 0;
    };
} // namespace util::serialization

#endif /* !UTIL_SERIALIZATION_INODEFACTORY_HPP_ */
