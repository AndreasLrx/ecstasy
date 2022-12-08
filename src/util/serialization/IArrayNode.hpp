///
/// @file IArrayNode.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-12-08
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef UTIL_SERIALIZATION_IARRAYNODE_HPP_
#define UTIL_SERIALIZATION_IARRAYNODE_HPP_

#include "INode.hpp"

namespace util::serialization
{
    ///
    /// @brief Array node. Contains nodes in an array.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-12-08)
    ///
    class IArrayNode : public INode {
      public:
        /// @brief Array index type.
        using Index = size_t;

        /// @brief Default destructor
        virtual ~IArrayNode() = default;

        ///
        /// @brief Get the node at @p index if existing.
        ///
        /// @param[in] index The node's index.
        ///
        /// @return const INode& A const reference to the requested node.
        ///
        /// @throw std::out_of_range If the index is out of bounds.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual const INode &get(Index index) const = 0;

        ///
        /// @brief Get the node at @p index if existing.
        ///
        /// @param[in] index The node's index.
        ///
        /// @return INode& A reference to the requested node.
        ///
        /// @throw std::out_of_range If the index is out of bounds.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual INode &get(Index index) = 0;

        ///
        /// @brief Get the node at @p index.
        ///
        /// @param[in] index The node's index.
        ///
        /// @return std::optional<const INode&> A const reference to the requested node if it exists, an empty @ref
        /// std::optional otherwise.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual std::optional<const INode &> tryGet(Index index) const = 0;

        ///
        /// @brief Get the node at @p index.
        ///
        /// @param[in] index The node's index.
        ///
        /// @return std::optional<INode&> A reference to the requested node if it exists, an empty @ref
        /// std::optional otherwise.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual std::optional<INode &> tryGet(Index index) = 0;

        ///
        /// @brief Push a new node at the end of the array.
        ///
        /// @param[in] node Node to push.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual void pushBack(INode &node) = 0;

        ///
        /// @brief Insert a node at the given index.
        ///
        /// @note All elements after (greater index) @p index will be moved one index further.
        ///
        /// @param[in] index Index of the new node.
        /// @param[in] node Node to insert.
        ///
        /// @throw std::out_of_range If index is @b greater than the current @ref size().
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual void insert(Index index, INode &node) = 0;

        ///
        /// @brief Replace a node at the given index.
        ///
        /// @note Other indexes are not changed.
        ///
        /// @param[in] index Index of the node.
        /// @param[in] node New node value.
        ///
        /// @throw std::out_of_range If @p index is @b greater than or @b equal the current @ref size().
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual void replace(Index index, INode &node) = 0;

        ///
        /// @brief Delete the last array node.
        ///
        /// @note Does nothing if the array is @ref empty().
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual void popBack() = 0;

        ///
        /// @brief Delete the node at @p index.
        ///
        /// @param[in] index Index of the node to erase.
        ///
        /// @throw std::out_of_range If @p index is @b greater than or @b equal the current @ref size().
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual void erase(Index index) = 0;

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

        /// @todo Iterators
    };
} // namespace util::serialization

#endif /* !UTIL_SERIALIZATION_IARRAYNODE_HPP_ */
