# Building

To build Ecstasy project from sources, follow these steps:

### Prerequisites

1. Install [CMake](https://cmake.org/resources/) (version 3.19 or higher)
2. Install a C++ compiler with [C++20](https://en.cppreference.com/w/cpp/20) support (Ex: G++ 8, Clang 14 or MSVC 2022)

### Build Steps

1. Clone the Ecstasy repository

   ```sh
   git clone git@github.com:AndreasLrx/ecstasy.git
   ```

2. Configure CMake with the appropriate [options](#cmake-options)

   ```sh
   cmake -B build -DCMAKE_BUILD_TYPE=Release -DECSTASY_THREAD_SAFE=TRUE
   ```

3. Prepare some eggs to cook on your computer and starts the build

   ```sh
   cmake --build build --config Release -j
   ```

#### IDEs

The library may also be built with any IDE that has [CMakePresets.json](/CMakePresets.json) support, such as Visual Sudio, VSCode + CMake Tools, or CLion.

### Using as a CMake package

I recommand using ecstasy as a CMake package. This allows easier static link and if you want to use SFML it is contained in ecstasy with the **ECSTASY_INTEGRATIONS_SFML** option.
An example project may be done one day to have a concrete CMake example. If you want, feel free to do it and I will link it here (or even better, make a PR)
<br>
_I am writing these lines because I suffered too much with CMake and if it can save some of your time and mental health, it's worth it._

1. FindEcstasy.cmake (don't read it loud you would sound like a crackhead)

   Here is a simple skeleton of the `FindEcstasy.cmake` file. You should fix the GIT_TAG with a version because the main branch can change quickly (when I do want to work).
   <br>

   _Don't hesitate to look at [CMake FetchContent official documentation](https://cmake.org/cmake/help/latest/module/FetchContent.html)._

   ```cmake
   cmake_minimum_required(VERSION 3.14)

   include(FetchContent)

   if(NOT ecstasy_FOUND)
      FetchContent_Declare(
         ecstasy        # This is the name CMake will remember
         GIT_REPOSITORY https://github.com/AndreasLrx/ecstasy.git
         GIT_TAG        main) # Maybe set a commit hash

      FetchContent_GetProperties(ecstasy)

      if(NOT ecstasy_POPULATED)
         SET(FETCHCONTENT_QUIET NO)
         # Set the options you need
         SET(ECSTASY_INTEGRATIONS_EVENT ON)
         SET(ECSTASY_INTEGRATIONS_SFML ON)
         SET(ECSTASY_INTEGRATIONS_USER_ACTION ON)
         set(BUILD_TEST_SUITE OFF)
         FetchContent_Populate(ecstasy)
         ADD_SUBDIRECTORY(${ecstasy_SOURCE_DIR} ${ecstasy_BINARY_DIR})
         SET(ecstasy_FOUND TRUE)
      endif()
   endif()
   ```

2. Declare as a dependency

   You can then call it from your main `CMakeLists.txt` like this:

   ```cmake
   find_package(ECSTASY REQUIRED)
   ```

3. Link to your taget
   Because you compiled `ecstasy` as a package, CMake is aware of it and you can link it easily to your target:

   ```cmake
   target_link_libraries(my_super_target PUBLIC ecstasy)
   ```

### CMake options

You can find more details about the options and their dependencies in [Options.cmake](/cmake/Options.cmake). <br>
_In case this documentation is not up to date with the [Options.cmake](/cmake/Options.cmake) file, open an issue._

| Option Name                          | Description                                                                                                                             | Default Value |
| ------------------------------------ | --------------------------------------------------------------------------------------------------------------------------------------- | ------------- |
| BUILD_TEST_SUITE                     | Build the test suite along the ecstasy code                                                                                             | OFF           |
| ENABLE_COVERAGE                      | Enable code coverage tracking. Must be set with **BUILD_TEST_SUITE** to have the tests coverage                                         | OFF           |
| BUILD_SHARED_LIBS                    | Build Ecstasy as shared libaries                                                                                                        | OFF           |
| ECSTASY_THREAD_SAFE                  | Combination of **ECSTASY_AUTO_LOCK**, **ECSTASY_AUTO_LOCK_RESOURCES**, **ECSTASY_LOCKABLE_RESOURCES** and **ECSTASY_LOCKABLE_STORAGES** | OFF           |
| ECSTASY_LOCKABLE_RESOURCES           | Make [Resource](@ref ecstasy::Resource) inherit [SharedRecursiveMutex](@ref ecstasy::thread::SharedRecursiveMutex)                      | OFF           |
| ECSTASY_LOCKABLE_STORAGES            | Make [IStorage](@ref ecstasy::IStorage) inherit [SharedRecursiveMutex](@ref ecstasy::thread::SharedRecursiveMutex)                      | OFF           |
| ECSTASY_AUTO_LOCK                    | Auto lock lockable queryables in registry queries                                                                                       | OFF           |
| ECSTASY_AUTO_LOCK_RESOURCES          | Auto lock resources with registry.getResource. Requires **ECSTASY_LOCKABLE_RESOURCES**                                                  | OFF           |
| ECSTASY_INTEGRATIONS_EVENT           | Enable the Event integration                                                                                                            | OFF           |
| ECSTASY_INTEGRATIONS_SFML            | Enable the Sfml integration. Requires **ECSTASY_INTEGRATIONS_EVENT**                                                                    | OFF           |
| ECSTASY_INTEGRATIONS_SFML_BUILD_DEMO | Enable the Sfml integration demos. Requires **ECSTASY_INTEGRATIONS_SFML**                                                               | OFF           |
| ECSTASY_INTEGRATIONS_USER_ACTION     | Enable the User Actions integration. Requires **ECSTASY_INTEGRATIONS_EVENT**                                                            | OFF           |
| ECSTASY_SERIALIZER_TOML              | Enable the Toml Serializer. Force set if **ECSTASY_INTEGRATIONS_USER_ACTION** is set                                                    | OFF           |
| ECSTASY_ENABLE_ENTITY_SERIALIZERS    | **WIP** - Enable the serialization methods in I/AStorage classes. This allows easy entity (de)serialization.                            | OFF           |
