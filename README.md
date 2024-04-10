# ECSTASY (Entity Component System Toward Architecture Saving Years)

<!--
@cond TURN_OFF_DOXYGEN
-->

[![Build](https://github.com/AndreasLrx/ecstasy/actions/workflows/build-tests.yml/badge.svg)](https://github.com/AndreasLrx/ecstasy/actions/workflows/build-tests.yml)
[![Documentation](https://img.shields.io/badge/Documentation-Doxygen-blue)](https://andreaslrx.github.io/ecstasy/)
[![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)](https://opensource.org/licenses/MIT)
[![codecov](https://codecov.io/gh/AndreasLrx/ecstasy/branch/main/graph/badge.svg?token=9TIAMB7WTF)](https://codecov.io/gh/AndreasLrx/ecstasy)

<!--
@endcond TURN_OFF_DOXYGEN
-->

###### If you're wondering if the name of the project has any real significance, it doesn't.

<!--
@cond TURN_OFF_DOXYGEN
-->

# Table of Contents

- [Introduction](#introduction)
- [Code Example](#code-example)
- [Building](#building)
- [Documentation](#documentation)
- [Contributing](#contributing)
- [License](#license)
- [Authors](#authors)

<!--
@endcond TURN_OFF_DOXYGEN
-->

### Introduction

Ecstasy is a modern C++ library that implements an Entity Component System (ECS) architecture with a strong focus on performance, flexibility, and ease of use.

Some key features include:

- Advanced usage of templates to shift computations from runtime to compile time, resulting in faster and more efficient applications (in theory, because I don't know how to make benchmarks).
- Support for complex queries using boolean operations (And/Or/Xor/Not) to filter and retrieve entities based on component criteria.
- Built-in integrations for common game mechanics, such as events, user actions, and graphic libraries to simplify game development.
- Designed with the zero-overhead principle in mind to ensure minimal runtime impact and maximum performance.
- Provides an excellent opportunity for cooking eggs during compilation 😉

Get started with Ecstasy today and experience a new level of performance and productivity in your ECS-based projects!

### Code Example

_The following is a basic example extracted from the [Tutorial](https://andreaslrx.github.io/ecstasy/md_doc__tutorial.html)._

```cpp
#include <ecstasy/registry/Registry.hpp>
#include <ecstasy/system/ISystem.hpp>

struct Position {
    float x;
    float y;
};

struct Velocity {
    float x;
    float y;
};

struct Movement : public ecstasy::ISystem {
    void run(ecstasy::Registry &registry) override final
    {
        for (auto [position, velocity] : registry.query<Position, const Velocity>()) {
            position.x += velocity.x;
            position.y += velocity.y;
        }
    }
};


int main() {
    // Declare your registry
    ecstasy::Registry registry;

    // Register your systems
    registry.addSystem<Movement>();

    // Populate the registry with some entities
    for (int i = 0; i < 10; i++) {
        auto builder = registry.entityBuilder().with<Position>(i * 2, i * 10);
        if (i % 2 == 0)
            builder.with<Velocity>(i * 10, i * 2);
        ecstasy::Entity entity = builder.build();
        // If needed, use the entity
    }

    while (true) {
        registry.runSystems();
    }
}
```

### Building

Follow the [building documentation](https://andreaslrx.github.io/ecstasy/md_doc__building.html)

### Documentation

You can see the documentation [online](https://andreaslrx.github.io/ecstasy/).

You can also build it locally using [Doxygen](https://www.doxygen.nl/):

```sh
# Run at the root of the project
doxygen

# Open the generated pages
xdg-open doc/build/html/index.html
```

### Contributing

ECSTASY is an open source project. If you want to get involved and suggest some additional features, file a bug report or submit a patch, create an issue or submit a pull request.
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.
If you want to contribute with pull requests, look at the [Contributing.md](/CONTRIBUTING.md)

### License

The ECSTASY library is distributed under the [MIT license](https://opensource.org/licenses/MIT).
In short, ECSTASY is free for any use (commercial or personal, proprietary or open-source). You can use ECSTASY in your project without any restriction. You can even omit to mention that you use ECSTASY -- although it would be appreciated.

### Authors

- Andréas Leroux (andreas.leroux@epitech.eu)
