///
/// @file ANode.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief Abstract node partial implementation.
/// @version 1.0.0
/// @date 2022-12-08
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
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
        /// @brief Default destructor.
        virtual ~ANode() = default;

        /// @copydoc INode::isType()
        [[nodiscard]] bool isType(Type type) const noexcept override final;

        /// @copydoc INode::isNull()
        [[nodiscard]] bool isNull() const noexcept override final;

        /// @copydoc INode::isObject()
        [[nodiscard]] bool isObject() const noexcept override final;

        /// @copydoc INode::isArray()
        [[nodiscard]] bool isArray() const noexcept override final;

        /// @copydoc INode::isString()
        [[nodiscard]] bool isString() const noexcept override final;

        /// @copydoc INode::isInteger()
        [[nodiscard]] bool isInteger() const noexcept override final;

        /// @copydoc INode::isFloat()
        [[nodiscard]] bool isFloat() const noexcept override final;

        /// @copydoc INode::isBoolean()
        [[nodiscard]] bool isBoolean() const noexcept override final;

        /// @copydoc INode::isDate()
        [[nodiscard]] bool isDate() const noexcept override final;

        /// @copydoc INode::isTime()
        [[nodiscard]] bool isTime() const noexcept override final;

        /// @copydoc INode::isDateTime()
        [[nodiscard]] bool isDateTime() const noexcept override final;

        /// @copydoc INode::asObject()
        [[nodiscard]] const IObjectNode &asObject() const override final;

        /// @copydoc INode::asObject()
        [[nodiscard]] IObjectNode &asObject() override final;

        /// @copydoc INode::tryAsObject()
        [[nodiscard]] std::optional<std::reference_wrapper<const IObjectNode>>
        tryAsObject() const noexcept override final;

        /// @copydoc INode::tryAsObject()
        [[nodiscard]] std::optional<std::reference_wrapper<IObjectNode>> tryAsObject() noexcept override final;

        /// @copydoc INode::asArray()
        [[nodiscard]] const IArrayNode &asArray() const override final;

        /// @copydoc INode::asArray()
        [[nodiscard]] IArrayNode &asArray() override final;

        /// @copydoc INode::tryAsArray()
        [[nodiscard]] std::optional<std::reference_wrapper<const IArrayNode>>
        tryAsArray() const noexcept override final;

        /// @copydoc INode::tryAsArray()
        [[nodiscard]] std::optional<std::reference_wrapper<IArrayNode>> tryAsArray() noexcept override final;

        /// @copydoc INode::asString()
        [[nodiscard]] std::string_view asString() const override final;

        /// @copydoc INode::asInteger()
        [[nodiscard]] int64_t asInteger() const override final;

        /// @copydoc INode::asFloat()
        [[nodiscard]] double asFloat() const override final;

        /// @copydoc INode::asBoolean()
        [[nodiscard]] bool asBoolean() const override final;

        /// @copydoc INode::asDate()
        [[nodiscard]] Date asDate() const override final;

        /// @copydoc INode::asTime()
        [[nodiscard]] Time asTime() const override final;

        /// @copydoc INode::asDateTime()
        [[nodiscard]] DateTime asDateTime() const override final;
    };
} // namespace util::serialization

#endif /* !UTIL_SERIALIZATION_ANODE_HPP_ */
