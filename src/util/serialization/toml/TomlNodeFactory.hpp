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
#include <toml++/toml.h>

namespace util::serialization
{
    class TomlNodeFactory {
      public:
        static NodePtr createFromToml(toml::node &node);

        static NodeCPtr createFromToml(const toml::node &node);

      private:
        TomlNodeFactory();
    };
} // namespace util::serialization

#endif /* !UTIL_SERIALIZATION_TOML_TOMLFACTORY_HPP_ */
