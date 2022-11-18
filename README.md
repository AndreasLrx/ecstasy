# ECSTASY (Entity Component System Toward Architecture Saving Years)

[![Build](https://github.com/AndreasLrx/ecstasy/actions/workflows/build-tests.yml/badge.svg)](https://github.com/AndreasLrx/ecstasy/actions/workflows/build-tests.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![codecov](https://codecov.io/gh/AndreasLrx/ecstasy/branch/main/graph/badge.svg?token=9TIAMB7WTF)](https://codecov.io/gh/AndreasLrx/ecstasy)

ECSTASY is an ECS made in modern C++.

###### If you're wondering if the name of the project has any real significance, it doesn't.

## Authors

- Andréas Leroux (andreas.leroux@epitech.eu)

## Download

You can get the current development version from the [Git repository](https://github.com/AndreasLrx/ecstasy)

# Building

Required tools:

- CMake 3.16 (minimum)

## Command Line

For Linux:

```sh
# Configure the project
cmake -B build/release_unix .. -G 'Unix Makefiles' -DCMAKE_BUILD_TYPE=Release

# Build the executable and libraries
cmake --build release_unix
```

## IDEs

The library may also be built with any IDE that has `CMakePresets.json` support, such as Visual Sudio, VSCode + CMake Tools, or CLion.

## Documentation

You can see the documentation [online](https://andreaslrx.github.io/ecstasy/).

You can also build it locally:

Required tools:

- Doxygen

on Linux:

```sh
# Run at the root of the project
doxygen

# Open the generated pages
xdg-open doc/build/html/index.html
```

## Contributing

ECSTASY is an open source project. If you want to get involved and suggest some additional features, file a bug report or submit a patch, create an issue or submit a pull request.
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

## License

The ECSTASY library is distributed under the [MIT licence](https://opensource.org/licenses/MIT).
In short, ECSTASY is free for any use (commercial or personal, proprietary or open-source). You can use ECSTASY in your project without any restriction. You can even omit to mention that you use ECSTASY -- although it would be appreciated.
