///
/// @file main.cpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-15
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#include <iostream>
#include "ecstasy/registry/Registry.hpp"

int main(int argc, char **argv)
{
    ecstasy::Registry registry;

    (void)argv;
    std::cout << argc << " arguments" << std::endl;
    return argc;
}