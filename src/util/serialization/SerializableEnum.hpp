///
/// @file SerializableEnum.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-30
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef UTIL_SERIALIZATION_SERIALIZABLE_ENUM_HPP_
#define UTIL_SERIALIZATION_SERIALIZABLE_ENUM_HPP_

#include "foreach.hpp"

#define _ENUM_TO_STRING_ENTRY(name)  {Enum::name, #name},
#define _STRING_TO_ENUM_ENTRY(name)  {#name, Enum::name},
#define _GET_FIRST_ARG(arg1, ...)    arg1
#define _REMOVE_FIRST_ARG(arg1, ...) __VA_ARGS__

#define SERIALIZABLE_ENUM(NAME, STARTING_AT, ...)                                                                    \
    struct __##NAME {                                                                                                \
      public:                                                                                                        \
        enum class Enum { _GET_FIRST_ARG(__VA_ARGS__) = STARTING_AT, _REMOVE_FIRST_ARG(__VA_ARGS__) };               \
                                                                                                                     \
        friend inline std::ostream &operator<<(std::ostream &stream, const Enum &e)                                  \
        {                                                                                                            \
            static const std::unordered_map<Enum, std::string> map = {FOR_EACH(_ENUM_TO_STRING_ENTRY, __VA_ARGS__)}; \
            return stream << map.at(e);                                                                              \
        }                                                                                                            \
        friend inline std::istream &operator>>(std::istream &stream, Enum &e)                                        \
        {                                                                                                            \
            static const std::unordered_map<std::string, Enum> map = {FOR_EACH(_STRING_TO_ENUM_ENTRY, __VA_ARGS__)}; \
            std::string buffer;                                                                                      \
                                                                                                                     \
            stream >> buffer;                                                                                        \
            e = map.at(buffer);                                                                                      \
            return stream;                                                                                           \
        }                                                                                                            \
    };                                                                                                               \
    using NAME = __##NAME::Enum;

#endif /* !UTIL_SERIALIZATION_SERIALIZABLE_ENUM_HPP_ */
