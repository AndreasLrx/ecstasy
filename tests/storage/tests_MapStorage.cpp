#include <gtest/gtest.h>
#include "ecstasy/storage/MapStorage.hpp"

struct Position {
    int x;
    int y;

    Position(int px, int py) : x(px), y(py)
    {
    }
    Position(std::pair<int, int> pair) : x(pair.first), y(pair.second)
    {
    }
};

TEST(MapStorage, emplace)
{
    ecstasy::MapStorage<Position> storage;

    Position &pos = storage.emplace(0, 5, 3);
    Position &pos2 = storage.emplace(1, std::make_pair(6, 9));

    EXPECT_EQ(pos.x, 5);
    EXPECT_EQ(pos.y, 3);

    EXPECT_EQ(pos2.x, 6);
    EXPECT_EQ(pos2.y, 9);
}

TEST(MapStorage, contains)
{
    ecstasy::MapStorage<Position> storage;

    EXPECT_FALSE(storage.contains(0));
    storage.emplace(0, 5, 3);
    EXPECT_TRUE(storage.contains(0));
}

TEST(MapStorage, get)
{
    ecstasy::MapStorage<Position> storage;

    EXPECT_THROW(storage[0], std::out_of_range);
    EXPECT_THROW(const_cast<ecstasy::MapStorage<Position> &>(storage)[0], std::out_of_range);

    storage.emplace(0, 5, 3);
    EXPECT_EQ(storage[0].x, 5);
    storage[0].x = 8;
    EXPECT_EQ(const_cast<ecstasy::MapStorage<Position> &>(storage)[0].x, 8);
}

TEST(MapStorage, erase)
{
    ecstasy::MapStorage<Position> storage;

    EXPECT_THROW(storage[0], std::out_of_range);
    storage.erase(0);
    EXPECT_THROW(storage[0], std::out_of_range);

    storage.emplace(0, 5, 3);
    EXPECT_EQ(storage[0].x, 5);
    storage.erase(0);
    EXPECT_THROW(storage[0], std::out_of_range);
}