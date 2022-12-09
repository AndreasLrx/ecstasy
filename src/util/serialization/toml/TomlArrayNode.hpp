///
/// @file TomlArrayNode.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-12-08
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef UTIL_SERIALIZATION_TOML_TOMLARRAYNODE_HPP_
#define UTIL_SERIALIZATION_TOML_TOMLARRAYNODE_HPP_

#include "TomlNode.hpp"
#include "util/serialization/IArrayNode.hpp"

namespace util::serialization
{
    ///
    /// @brief Toml Array node. Contains nodes in an array.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-12-08)
    ///
    class TomlArrayNode : public TomlNode, IArrayNode {
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
        class TomlArrayIterator : public IArrayNode::ArrayIterator<isConst> {
          public:
            using array_type = std::conditional_t<isConst, const TomlArrayNode, TomlArrayNode>;
            explicit TomlArrayIterator()
            {
            }

            TomlArrayIterator(array_type &array, Index pos = 0) : _pos(pos), _array(array)
            {
            }

            /// @copydoc IArrayNode::ArrayIterator::operator==()
            bool operator==(TomlArrayIterator const &other) const
            {
                return _pos == other._pos;
            }

            /// @copydoc IArrayNode::ArrayIterator::operator!=()
            bool operator!=(TomlArrayIterator const &other) const
            {
                return !(*this == other);
            }

            /// @copydoc IArrayNode::ArrayIterator::operator*()
            IArrayNode::ArrayIterator<isConst>::value_type operator*() const override final
            {
                if constexpr (isConst)
                    return std::const_pointer_cast<const TomlNode>(
                        std::make_shared<TomlNode>(*(const_cast<TomlArrayNode &>(_array)).asTomlArray().get(_pos)));
                else
                    return std::make_shared<TomlNode>(*_array.asTomlArray().get(_pos));
            }

            /// @copydoc IArrayNode::ArrayIterator::&operator++()
            TomlArrayIterator &operator++() override final
            {
                ++_pos;
                return *this;
            }

            /// @copydoc IArrayNode::ArrayIterator::operator++()
            TomlArrayIterator operator++(int)
            {
                TomlArrayIterator it = *this;

                return ++it;
            }

          private:
            Index _pos;
            array_type &_array;
        };

      public:
        TomlArrayNode(toml::array &array);

        /// @copydoc IArrayNode::&get()
        std::shared_ptr<const INode> get(Index index) const override final;

        /// @copydoc IArrayNode::&get()
        std::shared_ptr<INode> get(Index index) override final;

        /// @copydoc IArrayNode::tryGet()
        std::shared_ptr<const INode> tryGet(Index index) const override final;

        /// @copydoc IArrayNode::tryGet()
        std::shared_ptr<INode> tryGet(Index index) override final;

        /// @copydoc IArrayNode::pushBack()
        void pushBack(const INode &node) override final;

        /// @copydoc IArrayNode::insert()
        void insert(Index index, const INode &node) override final;

        /// @copydoc IArrayNode::replace()
        void replace(Index index, const INode &node) override final;

        /// @copydoc IArrayNode::popBack()
        void popBack() override final;

        /// @copydoc IArrayNode::erase()
        void erase(Index index) override final;

        /// @copydoc IArrayNode::clear()
        void clear() override final;

        /// @copydoc IArrayNode::empty()
        bool empty() const override final;

        /// @copydoc IArrayNode::size()
        size_t size() const override final;

        /// @copydoc IArrayNode::cbegin()
        const_iterator cbegin() const override final;

        /// @copydoc IArrayNode::begin()
        iterator begin() override final;

        /// @copydoc IArrayNode::cend()
        const_iterator cend() const override final;

        /// @copydoc IArrayNode::end()
        iterator end() override final;

      private:
        toml::array &asTomlArray();
        const toml::array &asTomlArray() const;
    };
} // namespace util::serialization

#endif /* !UTIL_SERIALIZATION_TOML_TOMLARRAYNODE_HPP_ */
