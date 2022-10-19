#include <gtest/gtest.h>
#include "ecstasy/resource/entity/Entities.hpp"

TEST(Entities, all)
{
    ecstasy::Entities entities;

    /// Create an entity alive
    ecstasy::Entity e = entities.create();
    EXPECT_EQ(e.getIndex(), 0);
    EXPECT_EQ(e.getGeneration(), 1);
    EXPECT_TRUE(entities.isAlive(e));

    /// Create a second entity dead
    ecstasy::Entity e2 = entities.create(false);
    EXPECT_EQ(e2.getIndex(), 1);
    EXPECT_EQ(e2.getGeneration(), 1);
    EXPECT_FALSE(entities.isAlive(e2));

    /// Kill our first entity
    EXPECT_TRUE(entities.kill(e));
    EXPECT_FALSE(entities.kill(e));

    /// Create a new entity, it should use the empty slot from our previous entity kill
    ecstasy::Entity e3 = entities.create();
    e2 = entities.get(0);
    EXPECT_EQ(e3 <=> e2, std::strong_ordering::equal);
    EXPECT_EQ(e3.getIndex(), 0);
    EXPECT_EQ(e3.getGeneration(), 2);

    /// Unexisting entity
    EXPECT_FALSE(entities.isAlive(entities.get(42)));
    EXPECT_FALSE(entities.kill(entities.get(42)));
}
