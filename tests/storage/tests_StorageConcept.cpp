#include <gtest/gtest.h>
#include "ecstasy/config.hpp"
#include "ecstasy/storages/MapStorage.hpp"
#include "ecstasy/storages/MarkerStorage.hpp"
#include "ecstasy/storages/StorageConcepts.hpp"

using namespace ecstasy;

struct Position {
    float x;
    float y;
};

struct Static {
    using StorageType = MarkerStorage<Static>;
};

struct Dynamic {};
SET_COMPONENT_STORAGE(Dynamic, MarkerStorage);

/// Debugging helpers, this print types T1 and T2 in the compilation error message
template <typename T1, typename T2>
void assert_equals()
{
    static_assert(std::is_same_v<T1, T2>, "Types T1 and T2 differs.");
}

TEST(getStorageType, defaults)
{
    assert_equals<getStorageType<Position>, MapStorage<Position>>();
    assert_equals<getStorageType<const Position>, const MapStorage<Position>>();
    assert_equals<getStorageType<Static>, MarkerStorage<Static>>();
    assert_equals<getStorageType<const Static>, const MarkerStorage<Static>>();
    assert_equals<getStorageType<Dynamic>, MarkerStorage<Dynamic>>();
    assert_equals<getStorageType<const Dynamic>, const MarkerStorage<Dynamic>>();
}
