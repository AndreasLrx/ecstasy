///
/// @file Serializer.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2024-04-29
///
/// @copyright Copyright (c) ECSTASY 2024
///
///

#ifndef ECSTASY_SERIALIZATION_RAWSERIALIZER_HPP_
#define ECSTASY_SERIALIZATION_RAWSERIALIZER_HPP_

#include <cstdint>
#include <sstream>
#include <vector>

#include "Serializer.hpp"
#include "util/meta/is_std_vector.hpp"

namespace ecstasy::serialization
{

    ///
    /// @brief RawSerializer class. This class is used to serialize and deserialize raw data in a string stream used as
    /// a bytes buffer. Fundamental types are serialized as is, and other types are serialized by calling the custom
    /// save/update methods.
    ///
    /// @note Some fundamental types are handled by the serializer:
    /// - fundamental types are saved as is.
    /// - std::string and std::string_view are saved as a uint32_t size followed by the string bytes.
    /// - Bounded arrays are saved by iterating over each element and saving them.
    /// - std::vector are saved as a uint32_t size followed by each element.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-04-30)
    ///
    class RawSerializer : public Serializer<RawSerializer> {
      public:
        ///
        /// @brief Construct a new Raw Serializer instance.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-04-30)
        ///
        RawSerializer() = default;

        ///
        /// @brief Construct a new Raw Serializer and import the content from a string.
        ///
        /// @param[in] content String to import.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-09-12)
        ///
        RawSerializer(const std::string &content)
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
        explicit RawSerializer(const std::filesystem::path &filename)
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
        RawSerializer(std::istream &stream)
        {
            importStream(stream);
        }

        ///
        /// @brief Destroy the RawSerializer
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-06-11)
        ///
        ~RawSerializer() override = default;

        /// @copydoc ISerializer::clear
        void clear() override final
        {
            _stream.str("");
        }

        ///
        /// @brief Reset the read and write cursor to the begining of the stream.
        ///
        /// @param[in] read Whether to reset the read cursor.
        /// @param[in] write Whether to reset the write cursor.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-09-12)
        ///
        void resetCursors(bool read = true, bool write = true)
        {
            if (read)
                resetReadCursor();
            if (write)
                resetWriteCursor();
        }

        ///
        /// @brief Reset the read cursor to the begining of the stream.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-09-12)
        ///
        void resetReadCursor()
        {
            _stream.seekg(0);
        }

        ///
        /// @brief Reset the write cursor to the begining of the stream.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-09-12)
        ///
        void resetWriteCursor()
        {
            _stream.seekp(0);
        }

        /// @copydoc ISerializer::size
        size_t size() const override final
        {
            return _stream.str().size();
        }

        /// @copydoc ISerializer::importBytes
        void importStream(std::istream &stream) override final
        {
            clear();
            _stream << stream.rdbuf();
            _stream.seekp(0);
        }

        /// @copydoc ISerializer::exportStream
        void exportStream(std::ostream &stream) override final
        {
            std::streampos pos = _stream.tellg();

            // Read from the begining of the stream
            _stream.seekg(0);
            stream << _stream.rdbuf();
            // Replace the read cursor at the original position
            _stream.seekg(pos);
        }

        /// @copydoc save
        // clang-format off
        template <typename T,
            typename = typename std::enable_if<
                (std::is_same_v<T, std::string> || std::is_same_v<T, std::string_view>) || // String
                std::is_bounded_array_v<T> || // Bounded array
                util::meta::is_std_vector<T>::value || // std::vector
                std::is_same_v<T, std::type_info> // std::type_info
                , int>::type >
                requires(!std::is_fundamental_v<T>)
        // clang-format on
        RawSerializer &saveImpl(const T &object)
        {
            if constexpr (std::is_same_v<T, std::string> || std::is_same_v<T, std::string_view>) {
                save(static_cast<uint32_t>(object.size()));
                _stream << object;
            } else if constexpr (std::is_bounded_array_v<T>) {
                for (size_t i = 0; i < std::extent_v<T>; i++)
                    save(object[i]);
            } else if constexpr (util::meta::is_std_vector<T>::value) {
                *this << object.size();
                for (const auto &elem : object)
                    *this << elem;
            } else if constexpr (std::is_same_v<T, std::type_info>) {
                save(object.hash_code());
            } else {
                return Parent::save(object);
            }
            return *this;
        }

        /// @copydoc save
        template <typename U>
            requires std::is_fundamental_v<U>
        RawSerializer &saveImpl(U object)
        {
            write(reinterpret_cast<const char *>(&object), sizeof(object));
            return *this;
        }

        // clang-format off
        /// @copydoc update
        template <typename U,
            typename = typename std::enable_if<(
                std::is_same_v<U, std::string> || // Load
                 std::is_bounded_array_v<U> || // Bounded array
                 util::meta::is_std_vector<U>::value) // std::vector
                ,
                int>::type>
        // clang-format on
        RawSerializer &updateImpl(U &object)
        {
            if constexpr (std::is_same_v<U, std::string>)
                object = load<U>();
            else if constexpr (std::is_bounded_array_v<U>)
                for (size_t i = 0; i < std::extent_v<U>; i++)
                    update(object[i]);
            else if constexpr (util::meta::is_std_vector<U>::value) {
                auto size = load<decltype(object.size())>();
                object.clear();
                object.reserve(size);
                for (size_t i = 0; i < size; i++) {
                    if constexpr (is_constructible<typename U::value_type>)
                        object.emplace_back(*this);
                    else
                        object.push_back(std::move(load<typename U::value_type>()));
                }
            } else
                return Parent::update(object);
            return *this;
        }

        /// @copydoc load
        template <typename U>
            requires std::is_fundamental_v<U> || std::is_same_v<U, std::string>
        U loadImpl()
        {
            if constexpr (std::is_fundamental_v<U>)
                return loadRaw<U>();
            else if constexpr (std::is_same<U, std::string>::value) {
                uint32_t size = loadRaw<uint32_t>();
                std::streampos pos = _stream.tellg();
                _stream.seekg(pos + std::streamoff(size));

                return std::string(_stream.view().data() + pos, size);
            }
        }

        ///
        /// @brief Append a type to the serializer as it is in the memory.
        ///
        /// @warning You must not use this for pointers or references.
        ///
        /// @tparam U Type of the object to append.
        ///
        /// @param[in] object Object to append.
        ///
        /// @return RawSerializer& Reference to @b this for chain calls.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-04-30)
        ///
        template <typename U>
        RawSerializer &appendRaw(const U &object)
        {
            write(reinterpret_cast<const char *>(&object), sizeof(object));
            return *this;
        }

        ///
        /// @brief Append a string to the serializer.
        ///
        /// @param[in] bytes Bytes to append.
        /// @param[in] size Size of the bytes buffer to append.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-04-30)
        ///
        void write(const char *bytes, size_t size)
        {
            _stream.write(bytes, static_cast<std::streamsize>(size));
        }

        ///
        /// @brief Load a raw type from the serializer.
        ///
        /// @note This is the opposite of @ref appendRaw.
        ///
        /// @tparam U Type of the object to load.
        ///
        /// @return U Loaded object.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-04-30)
        ///
        template <typename U>
        U loadRaw()
        {
            std::streampos pos = _stream.tellg();
            const U *result = reinterpret_cast<const U *>(_stream.view().data() + pos);
            _stream.seekg(pos + std::streamoff(sizeof(U)));
            return *result;
        }

        ///
        /// @brief Get the string stream of the serializer.
        ///
        /// @return std::stringstream& Reference to the string stream of the serializer.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-06-11)
        ///
        std::stringstream &getStream()
        {
            return _stream;
        }

      private:
        std::stringstream _stream;
    };
} // namespace ecstasy::serialization

#endif /* !ECSTASY_SERIALIZATION_RAWSERIALIZER_HPP_ */
