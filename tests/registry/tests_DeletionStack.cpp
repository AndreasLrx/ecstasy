#include <gtest/gtest.h>
#include "ecstasy/registry/DeletionStack.hpp"
#include "ecstasy/registry/Registry.hpp"

struct Vector2i {
    int x;
    int y;

    Vector2i(int px, int py) : x(px), y(py)
    {
    }

    bool operator==(const Vector2i &other) const
    {
        return x == other.x && y == other.y;
    }
};

TEST(DeletionStack, basic_cases)
{
    ecstasy::Registry registry;

    for (int i = 0; i < 10; i++)
        registry.entityBuilder().build();

    /// No deletion
    {
        GTEST_ASSERT_EQ(registry.getEntities()->getMask(), util::BitSet("1111111111"));
        ecstasy::DeletionStack delStack(registry);
    }
    GTEST_ASSERT_EQ(registry.getEntities()->getMask(), util::BitSet("1111111111"));

    {
        ecstasy::DeletionStack delStack(registry);
        int i = 0;

        for (auto [entity] : registry.query<ecstasy::Entities>()) {
            if (++i % 2 == 0)
                delStack.push(entity);
            /// Even if entity was marked for deletion it is still alive while the delStack isn't destroyed.
            GTEST_ASSERT_TRUE(registry.getEntities()->isAlive(entity));
        }
        GTEST_ASSERT_EQ(registry.getEntities()->getMask(), util::BitSet("1111111111"));
        GTEST_ASSERT_EQ(delStack.size(), 5);
    }
    /// delStack deleted and so are the entities
    GTEST_ASSERT_EQ(registry.getEntities()->getMask(), util::BitSet("0101010101"));
}
