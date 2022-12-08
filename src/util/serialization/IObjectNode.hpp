///
/// @file IObjectNode.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-12-08
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef UTIL_SERIALIZATION_IOBJECTNODE_HPP_
#define UTIL_SERIALIZATION_IOBJECTNODE_HPP_

#include "INode.hpp"

namespace util::serialization
{
    ///
    /// @brief Object node. Contains nodes identified by strings.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-12-08)
    ///
    class IObjectNode : public INode {
      public:
        /// @brief Default destructor.
        virtual ~IObjectNode() = default;

        ///
        /// @brief Get the node matching @p key if existing.
        ///
        /// @param[in] key The node's key.
        ///
        /// @return const INode& A const reference to the requested node.
        ///
        /// @throw std::out_of_range If the key doesn't exists.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual const INode &get(std::string_view key) const = 0;

        ///
        /// @brief Get the node matching @p key if existing.
        ///
        /// @param[in] key The node's key.
        ///
        /// @return INode& A reference to the requested node.
        ///
        /// @throw std::out_of_range If the key doesn't exists.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual INode &get(std::string_view key) = 0;

        ///
        /// @brief Get the node matching @p key.
        ///
        /// @param[in] key The node's key.
        ///
        /// @return std::optional<const INode&> A const reference to the requested node if it exists, an empty @ref
        /// std::optional otherwise.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual std::optional<const INode &> tryGet(std::string_view key) const = 0;

        ///
        /// @brief Get the node matching @p key.
        ///
        /// @param[in] key The node's key.
        ///
        /// @return std::optional<INode&> A reference to the requested node if it exists, an empty @ref
        /// std::optional otherwise.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual std::optional<INode &> tryGet(std::string_view key) = 0;

        ///
        /// @brief Try to insert a new node at @p key.
        ///
        /// @warning If the key is already used, this method will fail. Use @ref insertOrAssign() instead.
        ///
        /// @param[in] key New node key.
        /// @param[in] value New node.
        ///
        /// @return bool Whether the node was inserted or not.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual bool insert(std::string_view key, const INode &value) = 0;

        ///
        /// @brief Try to insert a new node at @p key or replace the existing one.
        ///
        /// @param[in] key New node key.
        /// @param[in] value New node.
        ///
        /// @return bool Whether the node was inserted. It is false if it was assigned.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual bool insertOrAssign(std::string_view key, const INode &value) = 0;

        ///
        /// @brief Erase the node identified by @p key.
        ///
        /// @param[in] key Node's key.
        ///
        /// @return bool Whether a node was erased or not.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual bool erase(std::string_view key) = 0;

        ///
        /// @brief Remove all the internal nodes.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual void clear() = 0;

        ///
        /// @brief Check if the object is empty.
        ///
        /// @return bool True if @p this doesn't contains any node.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual bool empty() const = 0;

        ///
        /// @brief Get the number of node in @p this.
        ///
        /// @return size_t Number of node contained in @p this.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual size_t size() const = 0;

        ///
        /// @brief Check if @p key match a node.
        ///
        /// @param[in] key Evaluated key.
        ///
        /// @return bool Whether there is a node associated to @p key.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual bool contains(std::string_view key) const = 0;

        /// @todo iterator
    };
} // namespace util::serialization
#endif /* !UTIL_SERIALIZATION_IOBJECTNODE_HPP_ */
