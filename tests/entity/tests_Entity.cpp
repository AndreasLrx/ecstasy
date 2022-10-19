#include <gtest/gtest.h>
#include "ecstasy/resource/entity/Entities.hpp"
#include "ecstasy/storage/MapStorage.hpp"

struct Position {
    int x;
    int y;

    Position(int px = 0, int py = 0) : x(px), y(py)
    {
    }
};

TEST(Entity, add_has)
{
    ecstasy::MapStorage<Position> positionStorage;
    ecstasy::Entities entities;

    /// Create an entity alive
    ecstasy::Entity e1 = entities.create();

    /// Has nothing by default then add component
    GTEST_ASSERT_FALSE(e1.has(positionStorage));
    e1.add(positionStorage, 5, 3);
    GTEST_ASSERT_TRUE(e1.has(positionStorage));
}

TEST(Entity, subscript)
{
    ecstasy::MapStorage<Position> positionStorage;
    ecstasy::Entities entities;

    /// Create an entity alive
    ecstasy::Entity e1 = entities.create();
    ecstasy::Entity e2 = entities.create();
    const ecstasy::Entity &ce1 = e1;

    /// Const subscript operator (read only)
    EXPECT_THROW(ce1[positionStorage], std::out_of_range);
    GTEST_ASSERT_FALSE(ce1.has(positionStorage));
    e1.add(positionStorage, 5, 9);
    GTEST_ASSERT_TRUE(ce1.has(positionStorage));
    GTEST_ASSERT_EQ(ce1[positionStorage].x, 5);
    /// Test with non const element (same effect)
    GTEST_ASSERT_EQ(e1[positionStorage].x, 5);

    /// Non Const subscript operator (create if not existing)
    GTEST_ASSERT_FALSE(e2.has(positionStorage));
    GTEST_ASSERT_EQ(e2[positionStorage].x, 0);
    GTEST_ASSERT_TRUE(e2.has(positionStorage));
}

TEST(Entity, get)
{
    ecstasy::MapStorage<Position> positionStorage;
    ecstasy::Entities entities;

    /// Create an entity alive
    ecstasy::Entity e1 = entities.create();
    const ecstasy::Entity &ce1 = e1;

    /// Throw on missing component (const independent)
    GTEST_ASSERT_FALSE(e1.has(positionStorage));
    EXPECT_THROW(ce1.get(positionStorage), std::out_of_range);
    EXPECT_THROW(e1.get(positionStorage), std::out_of_range);

    e1.add(positionStorage, 5, 9);
    GTEST_ASSERT_TRUE(e1.has(positionStorage));
    GTEST_ASSERT_EQ(e1.get(positionStorage).x, 5);
    GTEST_ASSERT_EQ(ce1.get(positionStorage).x, 5);

    e1.get(positionStorage).x = 42;
    GTEST_ASSERT_EQ(e1.get(positionStorage).x, 42);
    GTEST_ASSERT_EQ(ce1.get(positionStorage).x, 42);
}
