///
/// @file NodeSerializer.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief NodeSerializer class
/// @version 1.0.0
/// @date 2024-10-10
///
/// @copyright Copyright (c) ECSTASY 2024
///
///

#ifndef ECSTASY_SERIALIZATION_NODESERIALIZER_HPP_
#define ECSTASY_SERIALIZATION_NODESERIALIZER_HPP_

#include <cstdint>
#include <sstream>
#include <stack>
#include <vector>

#include "Serializer.hpp"
#include "util/serialization/IArrayNode.hpp"
#include "util/serialization/INode.hpp"
#include "util/serialization/INodeFactory.hpp"
#include "util/serialization/IObjectNode.hpp"
#include "util/meta/is_std_vector.hpp"
#include "util/meta/is_type_bounded_array.hpp"

namespace ecstasy::serialization
{
    using namespace util::serialization;

    ///
    /// @brief NodeSerializer class
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-04-30)
    ///
    class NodeSerializer : public Serializer<NodeSerializer> {
      public:
        ///
        /// @brief Nested context operations, used to open and close nested objects and arrays in streams easily.
        ///
        /// @note Use the static constexpr instances to use them in stream operations.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-10)
        ///
        enum class NestedContextOp {
            NewObject, ///< Open a new object (Creating it in save mode)
            NewArray,  ///< Open a new array (Creating it in save mode)
            Close      ///< Close the current object or array
        };
        /// @copydoc NestedContextOp::NewObject
        static constexpr NestedContextOp NewObject = NestedContextOp::NewObject;
        /// @copydoc NestedContextOp::NewArray
        static constexpr NestedContextOp NewArray = NestedContextOp::NewArray;
        /// @copydoc NestedContextOp::Close
        static constexpr NestedContextOp Close = NestedContextOp::Close;

        ///
        /// @brief Construct a new Raw Serializer instance.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-04-30)
        ///
        NodeSerializer(INodeFactory &factory) : _factory(factory)
        {
            _root = _factory.create(INode::Type::Array);
        };

        ///
        /// @brief Construct a new Raw Serializer and import the content from a string.
        ///
        /// @param[in] content String to import.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-09-12)
        ///
        NodeSerializer(INodeFactory &factory, const std::string &content) : NodeSerializer(factory)
        {
            importBytes(content);
        }

        ///
        /// @brief Construct a new Raw Serializer and import the content from a file.
        ///
        /// @param[in] filename Path to the file to import.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-09-12)
        ///
        explicit NodeSerializer(INodeFactory &factory, const std::filesystem::path &filename) : NodeSerializer(factory)
        {
            importFile(filename);
        }

        ///
        /// @brief Construct a new Raw Serializer and import the content from a stream.
        ///
        /// @param[in] stream Input stream.
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-09-12)
        ///
        NodeSerializer(INodeFactory &factory, std::istream &stream) : NodeSerializer(factory)
        {
            importStream(stream);
        }

        ///
        /// @brief Destroy the NodeSerializer
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-06-11)
        ///
        ~NodeSerializer() override = default;

        /// @copydoc ISerializer::clear
        void clear() override final
        {
            _root = _factory.create(INode::Type::Array);
            _stack = {};
            _arrayIterators = {};
            _objectIterators = {};
            _nextKey.clear();
        }

        /// @copydoc ISerializer::size
        [[nodiscard]] size_t size() const override final
        {
            return exportBytes().size();
        }

        /// @copydoc ISerializer::importBytes
        void importStream([[maybe_unused]] std::istream &stream) override final
        {
            clear();
            _root = _factory.fromStream(stream);
        }

        /// @copydoc ISerializer::exportStream
        void exportStream([[maybe_unused]] std::ostream &stream) const override final
        {
            _factory.exportStream(*_root, stream);
        }

        ///
        /// @brief Reset the read/write cursor to the begining of the document.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-11)
        ///
        void resetCursor()
        {
            while (!_stack.empty())
                _stack.pop();
            while (!_arrayIterators.empty())
                _arrayIterators.pop();
        }

        /// @copydoc save
        // clang-format off
        template <typename T,
            typename = typename std::enable_if<
                (std::is_same_v<T, std::string> || std::is_same_v<T, std::string_view>) || // String
                std::is_bounded_array_v<T> || // Bounded array
                util::meta::is_std_vector<T>::value || // std::vector
                std::is_fundamental_v<T> || // Fundamental type (int, float, etc.)
                std::is_same_v<T, std::type_info> || // std::type_info
                std::is_same_v<T, NestedContextOp> // Nested context operation
                , int>::type >
        // clang-format on
        NodeSerializer &saveImpl(const T &object)
        {
            if constexpr (std::is_same_v<T, NestedContextOp>) {
                switch (object) {
                    case NestedContextOp::NewObject: newNestedObject(); break;
                    case NestedContextOp::NewArray: newNestedArray(); break;
                    case NestedContextOp::Close: closeNested(); break;
                }
            } else if constexpr (std::is_same_v<T, std::string> || std::is_same_v<T, std::string_view>
                || util::meta::is_type_bounded_array_v<T, char>) {
                if constexpr (util::meta::is_type_bounded_array_v<T, char>)
                    addValue(_factory.create(std::string_view(object)));
                else
                    addValue(_factory.create(object));
            } else if constexpr (std::is_bounded_array_v<T>) {
                newNestedArray();
                for (size_t i = 0; i < std::extent_v<T>; i++)
                    save(object[i]);
                closeNested();
            } else if constexpr (util::meta::is_std_vector<T>::value) {
                newNestedArray();
                for (const auto &elem : object)
                    save(elem);
                closeNested();
            } else if constexpr (std::is_same_v<T, std::type_info>) {
                if (getWriteCursor()->isObject()) {
                    // Use name if the type is registered
                    auto componentSerializer = tryGetEntityComponentSerializer(object.hash_code());

                    if (componentSerializer)
                        save(componentSerializer->get().getTypeName());
                    else
                        save(std::to_string(object.hash_code()));
                } else
                    save(object.hash_code());
            } else if constexpr (std::is_fundamental_v<T>) {
                if constexpr (std::is_integral_v<T>) {
                    addValue(_factory.create(static_cast<int64_t>(object)));
                } else if constexpr (std::is_floating_point_v<T>) {
                    addValue(_factory.create(static_cast<double>(object)));
                } else if constexpr (std::is_same_v<T, bool>) {
                    addValue(_factory.create(object));
                }
            } else {
                return Parent::save(object);
            }
            return *this;
        }

        /// @copydoc update
        template <typename U>
            requires std::is_same_v<U, NestedContextOp>
        NodeSerializer &updateImpl(const U &op)
        {
            switch (op) {
                case NestedContextOp::NewObject: newNestedObject(false); break;
                case NestedContextOp::NewArray: newNestedArray(false); break;
                case NestedContextOp::Close: closeNested(); break;
            }
            return *this;
        }

        /// @copydoc update
        // clang-format off
        template <typename U,
            typename = typename std::enable_if<(
                std::is_same_v<U, std::string> || std::is_same_v<U, std::string_view> || // Load
                std::is_bounded_array_v<U> || // Bounded array
                util::meta::is_std_vector<U>::value) // std::vector
                , int>::type>
        // clang-format on
        NodeSerializer &updateImpl(U &object)
        {
            // Handle string key for object values
            if constexpr (std::is_same_v<U, std::string> || std::is_same_v<U, std::string_view>
                || util::meta::is_type_bounded_array_v<U, char>) {
                if (getWriteCursor()->isObject() && _nextKey.empty())
                    _nextKey = object;
                else {
                    if constexpr (std::is_same_v<U, std::string>)
                        object = load<U>();
                    else
                        throw std::invalid_argument("string_view and char[] can only be used for object keys.");
                }
            }
            // Handle bounded arrays
            else if constexpr (std::is_bounded_array_v<U>) {
                newNestedArray(false);

                // Need to fetch the new cursor after the new nested array
                if (getWriteCursor()->asArray().size() != std::extent_v<U>)
                    throw std::invalid_argument("Array size mismatch.");
                for (size_t i = 0; i < std::extent_v<U>; i++)
                    update(object[i]);

                closeNested();
            }
            // Handle std::vector
            else if constexpr (util::meta::is_std_vector<U>::value) {
                newNestedArray(false);
                size_t size = getWriteCursor()->asArray().size();

                object.clear();
                object.reserve(size);
                for (size_t i = 0; i < size; i++) {
                    if constexpr (is_constructible<typename U::value_type>)
                        object.emplace_back(*this);
                    else
                        object.push_back(std::move(load<typename U::value_type>()));
                }
                closeNested();
            } else
                return Parent::update(object);
            return *this;
        }

        /// @copydoc load
        // clang-format off
        template <typename U>
            requires std::is_fundamental_v<U> || // Fundamental type
            std::is_same_v<U, std::string>
        // clang-format on
        [[nodiscard]] U loadImpl()
        {
            NodePtr value = readCurrentValue(true);

            // Read Boolean
            if constexpr (std::is_same_v<U, bool>) {
                return value->asBoolean();
            }
            // Read Integer
            else if constexpr (std::numeric_limits<U>::is_integer) {
                // What about signed/unsigned conversion? and size?
                return static_cast<U>(value->asInteger());
            }
            // Read Floating point
            else if constexpr (std::is_floating_point_v<U>) {
                // What about float/double conversion?
                return static_cast<U>(value->asFloat());
            }
            // Read String
            else if constexpr (std::is_same_v<U, std::string>) {
                return std::string(value->asString());
            }
        }

        ///
        /// @brief Access the current value in the document, and move the cursor to the next value.
        ///
        /// @param[in] andMoveCursor If true, move the cursor to the next value.
        ///
        /// @return INode &Reference to the current value.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-11)
        ///
        [[nodiscard]] NodePtr readCurrentValue(bool andMoveCursor = true)
        {
            NodePtr cursor = getWriteCursor();

            if (cursor->isObject()) {
                if (_nextKey.empty())
                    throw std::invalid_argument("No key set for object value.");
                NodePtr result = cursor->asObject().get(_nextKey).lock();
                if (andMoveCursor)
                    _nextKey.clear();
                return result;
            } else {
                if (!cursor->isArray()) {
                    throw std::invalid_argument("Invalid cursor type. Expected object or array.");
                }
                IArrayNode &array = cursor->asArray();
                // If the array is empty, push the first element (first document element)
                if (_arrayIterators.empty())
                    _arrayIterators.push(array.begin());

                // If we reached the end of the array, throw an exception
                if (_arrayIterators.top() == array.end())
                    throw std::out_of_range("End of array reached.");

                NodePtr value = (*_arrayIterators.top()).lock();
                // Move the cursor to the next element
                if (andMoveCursor)
                    ++_arrayIterators.top();
                return value;
            }
        }

        ///
        /// @brief Open a new nested object or array context in the current object (see @ref getWriteCursor).
        /// The context can be closed with @ref closeNested.
        ///
        /// @warning If the context is an object you must set a key before adding a value to it.
        ///
        /// @param[in] type Type of the nested object or array.
        /// @param[in] create If true, create the nested object or array (for save mode).
        ///
        /// @return NodeSerializer& Reference to @b this for chain calls.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-10)
        ///
        NodeSerializer &newNested(INode::Type type, bool create = true)
        {
            if (type != INode::Type::Object && type != INode::Type::Array)
                throw std::invalid_argument("Invalid type for nested object.");

            if (create) {
                NodePtr cursor = getWriteCursor();
                std::string key = _nextKey;

                addValue(_factory.create(type));
                if (cursor->isArray())
                    _stack.push(cursor->asArray().get(cursor->asArray().size() - 1));
                else
                    _stack.push(cursor->asObject().get(key).lock());
            } else {
                _stack.push(readCurrentValue());
                if (getWriteCursor()->isArray())
                    _arrayIterators.push(getWriteCursor()->asArray().begin());
            }
            return *this;
        }

        ///
        /// @brief Wrapper for @ref newNested(INode::Type::Object).
        ///
        /// @return NodeSerializer& Reference to @b this for chain calls.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-10)
        ///
        NodeSerializer &newNestedObject(bool create = true)
        {
            return newNested(INode::Type::Object, create);
        }

        ///
        /// @brief Wrapper for @ref newNested(INode::Type::Array).
        ///
        /// @return NodeSerializer& Reference to @b this for chain calls.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-10)
        ///
        NodeSerializer &newNestedArray(bool create = true)
        {
            return newNested(INode::Type::Array, create);
        }

        ///
        /// @brief Close the current nested object or array opened with @ref newNestedObject or @ref newNestedArray
        /// (or @ref newNested).
        ///
        /// @return NodeSerializer& Reference to @b this for chain calls.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-10)
        ///
        NodeSerializer &closeNested()
        {
            if (_stack.top().lock()->isArray())
                _arrayIterators.pop();
            _stack.pop();
            return *this;
        }

        ///
        /// @brief Get a reference to the current cursor (ie nested objects or arrays).
        ///
        /// @return NodePtr Reference to the current cursor.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-10)
        ///
        [[nodiscard]] NodePtr getWriteCursor() noexcept
        {
            if (_stack.empty())
                return _root;
            return _stack.top().lock();
        }

        ///
        /// @brief Add a value to the current object or array.
        /// If the current object is an array, the value will be appended to it.
        /// If the current object is an object, the first call will set the key (string) for the next value, and the
        /// second call will add the value to the object (using the previous key identifier).
        ///
        /// @param[in] value Value to add.
        ///
        /// @return NodeSerializer& Reference to @b this for chain calls.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-10)
        ///
        NodeSerializer &addValue(NodePtr value)
        {
            NodePtr cursor = getWriteCursor();

            if (cursor->isObject()) {
                if (_nextKey.empty()) {
                    if (value->isString())
                        _nextKey = value->asString();
                    else
                        throw std::logic_error("Object key is missing.");
                } else {
                    cursor->asObject().insert(_nextKey, *value);
                    _nextKey.clear();
                }
            } else {
                cursor->asArray().pushBack(*value);
            }
            return *this;
        }

      private:
        /// @brief Reference to the node factory.
        util::serialization::INodeFactory &_factory;
        /// @brief Root node, which can be an object or an array.
        util::serialization::NodePtr _root;
        /// @brief Stack of nested objects or arrays, to keep track of the current context.
        std::stack<util::serialization::NodeView> _stack;
        /// @brief Stack of array iterators, to keep track of the current array element.
        std::stack<util::serialization::IArrayNode::iterator> _arrayIterators;
        /// @brief Stack of object iterators, to keep track of the current object element.
        std::stack<util::serialization::IObjectNode::iterator> _objectIterators;
        /// @brief Next key to use for the next object value.
        std::string _nextKey;

        /// @copydoc loadComponentHash
        [[nodiscard]] std::size_t loadComponentHash() override final
        {
            if (getWriteCursor()->isObject()) {
                if (_objectIterators.empty())
                    throw std::logic_error(
                        "No object iterator. This function should be called in an updateEntity context.");
                if (_objectIterators.top() == getWriteCursor()->asObject().end())
                    return 0;
                _nextKey = (*_objectIterators.top()).first;
                ++_objectIterators.top();
                return getEntityComponentSerializer(_nextKey).getComponentTypeInfo().hash_code();
            } else
                return load<std::size_t>();
        }

        /// @copydoc beforeSaveEntity
        void beforeSaveEntity([[maybe_unused]] RegistryEntity &entity) override final
        {
            newNestedObject();
        }

        /// @copydoc afterSaveEntity
        void afterSaveEntity([[maybe_unused]] RegistryEntity &entity) override final
        {
            closeNested();
        }

        /// @copydoc beforeUpdateEntity
        void beforeUpdateEntity([[maybe_unused]] RegistryEntity &entity) override final
        {
            newNestedObject(false);
            _objectIterators.push(getWriteCursor()->asObject().begin());
        }

        /// @copydoc afterUpdateEntity
        void afterUpdateEntity([[maybe_unused]] RegistryEntity &entity) override final
        {
            _objectIterators.pop();
            closeNested();
        }
    };
} // namespace ecstasy::serialization

#endif /* !ECSTASY_SERIALIZATION_NODESERIALIZER_HPP_ */
