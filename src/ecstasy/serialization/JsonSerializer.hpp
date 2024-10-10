///
/// @file JsonSerializer.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2024-10-10
///
/// @copyright Copyright (c) ECSTASY 2024
///
///

#ifndef ECSTASY_SERIALIZATION_JSONSERIALIZER_HPP_
#define ECSTASY_SERIALIZATION_JSONSERIALIZER_HPP_

#include <cstdint>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/pointer.h>
#include <rapidjson/writer.h>
#include <sstream>
#include <stack>
#include <vector>

#include "Serializer.hpp"
#include "util/meta/is_std_vector.hpp"
#include "util/meta/is_type_bounded_array.hpp"

namespace ecstasy::serialization
{

    ///
    /// @brief JsonSerializer class
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-04-30)
    ///
    class JsonSerializer : public Serializer<JsonSerializer> {
      public:
        ///
        /// @brief Nested context operations, used to open and close nested objects and arrays in streams easily.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-10)
        ///
        enum class NestedContextOp { NewObject, NewArray, Close };
        /// @brief Alias for NestedContextOp
        using OP = NestedContextOp;

        ///
        /// @brief Construct a new Raw Serializer instance.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-04-30)
        ///
        JsonSerializer()
        {
            _document.SetArray();
        };

        ///
        /// @brief Construct a new Raw Serializer and import the content from a string.
        ///
        /// @param[in] content String to import.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-09-12)
        ///
        JsonSerializer(const std::string &content)
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
        explicit JsonSerializer(const std::filesystem::path &filename)
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
        JsonSerializer(std::istream &stream)
        {
            importStream(stream);
        }

        ///
        /// @brief Destroy the JsonSerializer
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-06-11)
        ///
        ~JsonSerializer() override = default;

        /// @copydoc ISerializer::clear
        void clear() override final
        {
            _document.Clear();
            _document.SetArray();
        }

        /// @copydoc ISerializer::size
        size_t size() const override final
        {
            return exportBytes().size();
        }

        /// @copydoc ISerializer::importBytes
        void importStream(std::istream &stream) override final
        {
            rapidjson::IStreamWrapper isw(stream);

            clear();
            _document.ParseStream(isw);
        }

        /// @copydoc ISerializer::exportStream
        void exportStream(std::ostream &stream) const override final
        {
            rapidjson::OStreamWrapper osw(stream);
            rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);

            _document.Accept(writer);
        }

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
        JsonSerializer &saveImpl(const T &object)
        {
            if constexpr (std::is_same_v<T, NestedContextOp>) {
                switch (object) {
                    case NestedContextOp::NewObject: newNestedObject(); break;
                    case NestedContextOp::NewArray: newNestedArray(); break;
                    case NestedContextOp::Close: closeNested(); break;
                }
            } else if constexpr (std::is_same_v<T, std::string> || std::is_same_v<T, std::string_view>
                || util::meta::is_type_bounded_array_v<T, char>) {
                rapidjson::Value value;

                if constexpr (std::is_same_v<T, std::string>)
                    value.SetString(
                        object.c_str(), static_cast<rapidjson::SizeType>(object.length()), _document.GetAllocator());
                else if constexpr (std::is_same_v<T, std::string_view>)
                    value.SetString(
                        object.data(), static_cast<rapidjson::SizeType>(object.length()), _document.GetAllocator());
                else
                    value.SetString(object, _document.GetAllocator());

                addValue(std::move(value.Move()));
            } else if constexpr (std::is_bounded_array_v<T>) {
                rapidjson::Value array(rapidjson::kArrayType);

                for (size_t i = 0; i < std::extent_v<T>; i++)
                    array.PushBack(object[i], _document.GetAllocator());
                addValue(std::move(array.Move()));
            } else if constexpr (util::meta::is_std_vector<T>::value) {
                rapidjson::Value array(rapidjson::kArrayType);

                for (const auto &elem : object)
                    array.PushBack(elem, _document.GetAllocator());
                addValue(std::move(array.Move()));
            } else if constexpr (std::is_same_v<T, std::type_info>) {
                save(object.hash_code());
            } else if constexpr (std::is_fundamental_v<T>) {
                addValue(rapidjson::Value(object));
            } else {
                return Parent::save(object);
            }
            return *this;
        }

        /// @copydoc loadComponentHash
        std::size_t loadComponentHash() override final
        {
            return 0; // loadRaw<std::size_t>();
        }

        ///
        /// @brief Open a new nested object or array context in the current object (see @ref getWriteCursor).
        /// The context can be closed with @ref closeNested.
        ///
        /// @warning If the context is an object you must set a key before adding a value to it.
        ///
        /// @param[in] type Type of the nested object or array.
        ///
        /// @return JsonSerializer& Reference to @b this for chain calls.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-10)
        ///
        JsonSerializer &newNested(rapidjson::Type type)
        {
            if (type != rapidjson::Type::kObjectType && type != rapidjson::Type::kArrayType)
                throw std::invalid_argument("Invalid type for nested object.");

            rapidjson::Value &cursor = getWriteCursor();
            std::string key = _nextKey;

            addValue(rapidjson::Value(type));
            if (cursor.IsArray())
                _stack.push(cursor[cursor.Size() - 1]);
            else
                _stack.push(cursor[key.c_str()]);
            return *this;
        }

        ///
        /// @brief Wrapper for @ref newNested(rapidjson::Type::kObjectType).
        ///
        /// @return JsonSerializer& Reference to @b this for chain calls.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-10)
        ///
        JsonSerializer &newNestedObject()
        {
            return newNested(rapidjson::Type::kObjectType);
        }

        ///
        /// @brief Wrapper for @ref newNested(rapidjson::Type::kArrayType).
        ///
        /// @return JsonSerializer& Reference to @b this for chain calls.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-10)
        ///
        JsonSerializer &newNestedArray()
        {
            return newNested(rapidjson::Type::kArrayType);
        }

        ///
        /// @brief Close the current nested object or array opened with @ref newNestedObject or @ref newNestedArray
        /// (or @ref newNested).
        ///
        /// @return JsonSerializer& Reference to @b this for chain calls.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-10)
        ///
        JsonSerializer &closeNested()
        {
            _stack.pop();
            return *this;
        }

        ///
        /// @brief Get a reference to the current cursor (ie nested objects or arrays).
        ///
        /// @return rapidjson::Value& Reference to the current cursor.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-10)
        ///
        rapidjson::Value &getWriteCursor()
        {
            if (_stack.empty())
                return _document;
            return _stack.top();
        }

        ///
        /// @brief Add a value to the current object or array.
        /// If the current object is an array, the value will be appended to it.
        /// If the current object is an object, the first call will set the key (string) for the next value, and the
        /// second call will add the value to the object (using the previous key identifier).
        ///
        /// @param[in] value Value to add.
        ///
        /// @return JsonSerializer& Reference to @b this for chain calls.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-10)
        ///
        JsonSerializer &addValue(rapidjson::Value &&value)
        {
            rapidjson::Value &cursor = getWriteCursor();

            if (cursor.IsObject()) {
                if (_nextKey.empty()) {
                    if (value.IsString())
                        _nextKey = value.GetString();
                    else
                        throw std::logic_error("Json object key is missing.");
                } else {
                    cursor.AddMember(
                        rapidjson::Value(_nextKey.c_str(), _document.GetAllocator()), value, _document.GetAllocator());
                    _nextKey.clear();
                }
            } else {
                cursor.PushBack(value, _document.GetAllocator());
            }
            return *this;
        }

      private:
        rapidjson::Document _document;
        std::stack<std::reference_wrapper<rapidjson::Value>> _stack;
        std::string _nextKey;
    };
} // namespace ecstasy::serialization

#endif /* !ECSTASY_SERIALIZATION_JSONSERIALIZER_HPP_ */
