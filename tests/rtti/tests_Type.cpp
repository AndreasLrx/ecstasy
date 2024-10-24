#include <gtest/gtest.h>
#include <unordered_map>

#include "ecstasy/rtti/Type.hpp"
#include "ecstasy/storages/MapStorage.hpp"
#include "ecstasy/storages/StorageConcepts.hpp"

#include "ecstasy/rtti/TypeRegistry.hpp"

using namespace ecstasy::rtti;

struct Position {
    float x;
    float y;
};

TEST(Type, getters)
{
    using PositionStorage = ecstasy::getStorageType<Position>;
    Type<Position> type("Position");
    size_t hash = std::hash<std::string_view>{}("Position");

    ASSERT_EQ(type.getTypeName(), "Position");
    ASSERT_EQ(type.getTypeInfo(), typeid(Position));
    ASSERT_EQ(type.getHash(), hash);
    ASSERT_EQ(type.getStorageTypeInfo(), typeid(PositionStorage));
}

TEST(Type, comparisons)
{
    // Test equality operators
    Type<Position> type("Position");
    Type<size_t> type2("size_t");

    ASSERT_EQ(type, type);
    ASSERT_EQ(type, Type<Position>("Position"));
    ASSERT_EQ(type, type.getTypeInfo());
    ASSERT_EQ(type, type.getTypeName());
    ASSERT_NE(type, type2);
    ASSERT_NE(type, Type<size_t>("size_t"));
    ASSERT_NE(type, type2.getTypeInfo());
    ASSERT_NE(type, type2.getTypeName());
}
