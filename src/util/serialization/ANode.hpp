///
/// @file ANode.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-12-08
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef UTIL_SERIALIZATION_ANODE_HPP_
#define UTIL_SERIALIZATION_ANODE_HPP_

#include "INode.hpp"

namespace util::serialization
{
    ///
    /// @brief Serialization node. Can be any type of @ref INode::Type.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-12-08)
    ///
    class ANode : public INode {
      public:
        ///
        /// @brief Construct a new Abstract Node with a specified type.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        ANode(Type type = Type::Unknown);

        /// @brief Default destructor.
        virtual ~ANode() = default;

        /// @copydoc INode::getType()
        Type getType() const override final;

        /// @copydoc INode::isType()
        bool isType(Type type) const override final;

        /// @copydoc INode::isNull()
        bool isNull() const override final;

        /// @copydoc INode::isObject()
        bool isObject() const override final;

        /// @copydoc INode::isArray()
        bool isArray() const override final;

        /// @copydoc INode::isString()
        bool isString() const override final;

        /// @copydoc INode::isInteger()
        bool isInteger() const override final;

        /// @copydoc INode::isFloat()
        bool isFloat() const override final;

        /// @copydoc INode::isBoolean()
        bool isBoolean() const override final;

        /// @copydoc INode::isDate()
        bool isDate() const override final;

        /// @copydoc INode::isTime()
        bool isTime() const override final;

        /// @copydoc INode::isDateTime()
        bool isDateTime() const override final;

        /// @copydoc INode::asObject()
        const IObjectNode &asObject() const override final;

        /// @copydoc INode::asObject()
        IObjectNode &asObject() override final;

        /// @copydoc INode::tryAsObject()
        std::optional<std::reference_wrapper<const IObjectNode>> tryAsObject() const override final;

        /// @copydoc INode::tryAsObject()
        std::optional<std::reference_wrapper<IObjectNode>> tryAsObject() override final;

        /// @copydoc INode::asArray()
        const IArrayNode &asArray() const override final;

        /// @copydoc INode::asArray()
        IArrayNode &asArray() override final;

        /// @copydoc INode::tryAsArray()
        std::optional<std::reference_wrapper<const IArrayNode>> tryAsArray() const override final;

        /// @copydoc INode::tryAsArray()
        std::optional<std::reference_wrapper<IArrayNode>> tryAsArray() override final;

      protected:
        Type _type;
    };
} // namespace util::serialization

#endif /* !UTIL_SERIALIZATION_ANODE_HPP_ */
