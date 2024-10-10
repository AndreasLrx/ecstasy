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
                std::is_same_v<T, std::type_info> // std::type_info
                , int>::type >
        // clang-format on
        JsonSerializer &saveImpl(const T &object)
        {
            if constexpr (std::is_same_v<T, std::string> || std::is_same_v<T, std::string_view>
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

                _document.PushBack(value.Move(), _document.GetAllocator());
            } else if constexpr (std::is_bounded_array_v<T>) {
                rapidjson::Value array(rapidjson::kArrayType);

                for (size_t i = 0; i < std::extent_v<T>; i++)
                    array.PushBack(object[i], _document.GetAllocator());
                _document.PushBack(array, _document.GetAllocator());
            } else if constexpr (util::meta::is_std_vector<T>::value) {
                rapidjson::Value array(rapidjson::kArrayType);

                for (const auto &elem : object)
                    array.PushBack(elem, _document.GetAllocator());
                _document.PushBack(array, _document.GetAllocator());
            } else if constexpr (std::is_same_v<T, std::type_info>) {
                save(object.hash_code());
            } else if constexpr (std::is_fundamental_v<T>) {
                _document.PushBack(object, _document.GetAllocator());
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

      private:
        rapidjson::Document _document;
    };
} // namespace ecstasy::serialization

#endif /* !ECSTASY_SERIALIZATION_JSONSERIALIZER_HPP_ */
