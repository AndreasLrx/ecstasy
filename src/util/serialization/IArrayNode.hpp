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

#include <memory>
#include "INode.hpp"

namespace util::serialization
{
    ///
    /// @brief Array node. Contains nodes in an array.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-12-08)
    ///
    class IArrayNode {
      protected:
        ///
        /// @brief Abstract Array Iterator.
        ///
        /// @tparam isConst Whether the iterator is a const iterator or not.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        template <bool isConst>
        class ArrayIterator {
          public:
            using value_type = std::conditional_t<isConst, NodeCView, NodeView>;
            using reference = value_type &;
            using pointer = value_type *;
            using difference_type = std::ptrdiff_t;
            using iterator_category = std::random_access_iterator_tag;

            ///////////////////////////////////////////////////////////////////////////////////////////////////////////
            // All iterators must be constructible, copy-constructible, copy-assignable, destructible and swappable.///
            ///////////////////////////////////////////////////////////////////////////////////////////////////////////

            ///
            /// @brief Construct a new uninitialized ArrayIterator.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-12-08)
            ///
            explicit ArrayIterator()
            {
            }

            ///
            /// @brief Default copy operator.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-12-08)
            ///
            ArrayIterator(ArrayIterator const &) = default;

            ///
            /// @brief Default assignment operator.
            ///
            /// @return ArrayIterator& @b this.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-12-08)
            ///
            ArrayIterator &operator=(ArrayIterator const &) = default;

            ///
            /// @brief Default move constructor.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-12-08)
            ///
            ArrayIterator(ArrayIterator &&) = default;

            ///
            /// @brief Default move assignment operator.
            ///
            /// @return ArrayIterator& @b this.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-12-08)
            ///
            ArrayIterator &operator=(ArrayIterator &&) = default;

            ///
            /// @brief Compare two iterators from the same @ref IArrayNode.
            ///
            /// @warning It is undefined behavior to compare two iterators that do not belong to the same array.
            ///
            /// @param[in] other iterator to compare.
            ///
            /// @return bool Whether the two iterators are equals.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-12-08)
            ///
            virtual bool operator==(ArrayIterator const &other) const
            {
                (void)other;
                return false;
            }

            ///
            /// @brief Compare two iterators from the same @ref IArrayNode.
            ///
            /// @warning It is undefined behavior to compare two iterators that do not belong to the same array.
            ///
            /// @param[in] other iterator to compare.
            ///
            /// @return bool Whether the two iterators are different.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-12-08)
            ///
            virtual bool operator!=(ArrayIterator const &other) const
            {
                (void)other;
                return true;
            }

            ///
            /// @brief Fetch the node at the current position.
            ///
            /// @return @ref value_type Node.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-12-08)
            ///
            virtual value_type operator*() const
            {
                return value_type();
            }

            ///
            /// @brief Increments the iterator in place.
            ///
            /// @warning It is undefined behavior to increment the iterator past the end sentinel ( @ref
            /// IArrayNode::end() ).
            ///
            /// @return ArrayIterator& @b this.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-12-08)
            ///
            virtual ArrayIterator &operator++()
            {
                return *this;
            }

            ///
            /// @brief Copies the iterator and increments the copy, please use pre-incrementation instead.
            ///
            /// @warning It is undefined behavior to increment the iterator past the end sentinel ( @ref
            /// IArrayNode::end() ).
            /// @warning This creates a copy of the iterator!
            ///
            /// @return ArrayIterator The incremented copy.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-12-08)
            ///
            // virtual ArrayIterator operator++(int)
            // {
            //     return *this;
            // }

            ///
            /// @brief Fetch the node at the current position.
            ///
            /// @return @ref value_type Node.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-12-08)
            ///
            value_type operator->() const
            {
                return *this;
            }
        };

      public:
        /// @brief Array index type.
        using Index = size_t;
        /// @brief Array iterator type.
        using iterator = ArrayIterator<false>;
        /// @brief Array const iterator type.
        using const_iterator = ArrayIterator<true>;

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
        virtual NodeCView get(Index index) const = 0;

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
        virtual NodeView get(Index index) = 0;

        ///
        /// @brief Get the node at @p index.
        ///
        /// @param[in] index The node's index.
        ///
        /// @return std::shared_ptr<const INode> A const reference to the requested node if it exists, an empty @ref
        /// std::optional otherwise.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual NodeCView tryGet(Index index) const = 0;

        ///
        /// @brief Get the node at @p index.
        ///
        /// @param[in] index The node's index.
        ///
        /// @return std::shared_ptr<INode> A reference to the requested node if it exists, an empty @ref
        /// std::optional otherwise.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual NodeView tryGet(Index index) = 0;

        ///
        /// @brief Push a new node at the end of the array.
        ///
        /// @param[in] node Node to push.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual void pushBack(const INode &node) = 0;

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
        virtual void insert(Index index, const INode &node) = 0;

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
        virtual void replace(Index index, const INode &node) = 0;

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

        ///
        /// @brief Get the start iterator of the internal nodes.
        ///
        /// @return const_iterator Iterator to the first node.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual const_iterator cbegin() const = 0;

        ///
        /// @brief Get the start iterator of the internal nodes.
        ///
        /// @return iterator Iterator to the first node.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual iterator begin() = 0;

        ///
        /// @brief Get the end iterator of the internal nodes.
        ///
        /// @warning This iterator is @b never valid.
        ///
        /// @return const_iterator Iterator to the last node + 1.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual const_iterator cend() const = 0;

        ///
        /// @brief Get the end iterator of the internal nodes.
        ///
        /// @warning This iterator is @b never valid.
        ///
        /// @return iterator Iterator to the last node + 1.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-08)
        ///
        virtual iterator end() = 0;
    };
} // namespace util::serialization

#endif /* !UTIL_SERIALIZATION_IARRAYNODE_HPP_ */
