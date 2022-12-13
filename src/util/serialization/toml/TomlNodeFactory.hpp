///
/// @file TomlNodeFactory.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-12-12
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef UTIL_SERIALIZATION_TOML_TOMLFACTORY_HPP_
#define UTIL_SERIALIZATION_TOML_TOMLFACTORY_HPP_

#include "util/serialization/INode.hpp"
#include "util/serialization/INodeFactory.hpp"
#include <toml++/toml.h>

namespace util::serialization
{
    ///
    /// @brief Toml Node factory singleton.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-12-12)
    ///
    class TomlNodeFactory : public INodeFactory {
      public:
        ///
        /// @brief Retrieve the @ref TomlNodeFactory singleton instance.
        ///
        /// @return TomlNodeFactory& singleton instance.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-12)
        ///
        static TomlNodeFactory &get();

        ///
        /// @brief Create a node from a @ref toml::node object.
        ///
        /// @param[in] node Toml node.
        ///
        /// @return NodePtr @ref TomlNode pointer.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-12)
        ///
        NodePtr createFromToml(toml::node &node);

        ///
        /// @brief Create a node from a @ref toml::node object.
        ///
        /// @param[in] node Toml node.
        ///
        /// @return NodePtr @ref TomlNode const  pointer.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-12)
        ///
        NodeCPtr createFromToml(const toml::node &node);

        /// @copydoc INodeFactory::create().
        NodePtr create(INode::Type type) override final;

        /// @copydoc INodeFactory::create().
        NodePtr create(const INode &node) override final;

        /// @copydoc INodeFactory::create().
        NodePtr create(std::string_view string) override final;

        /// @copydoc INodeFactory::create().
        NodePtr create(int64_t integer) override final;

        /// @copydoc INodeFactory::create().
        NodePtr create(double floatingPoint) override final;

        /// @copydoc INodeFactory::create().
        NodePtr create(bool boolean) override final;

        /// @copydoc INodeFactory::create().
        NodePtr create(INode::Date date) override final;

        /// @copydoc INodeFactory::create().
        NodePtr create(INode::Time time) override final;

        /// @copydoc INodeFactory::create().
        NodePtr create(INode::DateTime dateTime) override final;

      private:
        TomlNodeFactory() = default;
    };
} // namespace util::serialization

#endif /* !UTIL_SERIALIZATION_TOML_TOMLFACTORY_HPP_ */
