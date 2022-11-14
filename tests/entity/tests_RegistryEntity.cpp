#include <gtest/gtest.h>
#include "ecstasy/resources/entity/Entities.hpp"
#include "ecstasy/resources/entity/RegistryEntity.hpp"
#include "ecstasy/storages/MapStorage.hpp"

struct Position {
    int x;
    int y;

    Position(int px = 0, int py = 0) : x(px), y(py)
    {
    }
};

TEST(RegistryEntity, add_has)
{
    ecstasy::Registry registry;
    ecstasy::Entities entities;

    /// Create an entity alive
    ecstasy::RegistryEntity e1(entities.create(), registry);

    /// Has nothing by default then add component
    GTEST_ASSERT_FALSE(e1.has<Position>());
    e1.add<Position>(5, 3);
    GTEST_ASSERT_TRUE(e1.has<Position>());

    GTEST_ASSERT_EQ(&registry, &e1.getRegistry());
}

TEST(RegistryEntity, get)
{
    ecstasy::Registry registry;
    ecstasy::Entities entities;

    /// Create an entity alive
    ecstasy::RegistryEntity e1(entities.create(), registry);
    ecstasy::RegistryEntity e2(entities.create(), registry);
    const ecstasy::RegistryEntity &ce1 = e1;
    const ecstasy::RegistryEntity &ce2 = e2;

    /// Throw on missing component (const independent) -> storage missing
    GTEST_ASSERT_FALSE(e1.has<Position>());
    EXPECT_THROW(ce1.get<Position>(), std::logic_error);
    EXPECT_THROW(e1.get<Position>(), std::logic_error);

    e1.add<Position>(5, 9);
    GTEST_ASSERT_TRUE(e1.has<Position>());
    GTEST_ASSERT_EQ(e1.get<Position>().x, 5);
    GTEST_ASSERT_EQ(ce1.get<Position>().x, 5);

    e1.get<Position>().x = 42;
    GTEST_ASSERT_EQ(e1.get<Position>().x, 42);
    GTEST_ASSERT_EQ(ce1.get<Position>().x, 42);

    /// Throw on missing component (const independent) -> component missing
    GTEST_ASSERT_FALSE(e2.has<Position>());
    EXPECT_THROW(ce2.get<Position>(), std::out_of_range);
    EXPECT_THROW(e2.get<Position>(), std::out_of_range);
}
