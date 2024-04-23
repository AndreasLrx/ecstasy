#include <gtest/gtest.h>
#include "ecstasy/storages/VectorStorage.hpp"

namespace ecstasy
{
    class VectorStorageTest {
      public:
        VectorStorageTest() = default;
        ~VectorStorageTest() = default;

        template <typename C>
        std::vector<C> &getComponents(ecstasy::VectorStorage<C> &storage)
        {
            return storage._components;
        }
    };
} // namespace ecstasy

struct Position {
    using StorageType = ecstasy::VectorStorage<Position>;

    int x;
    int y;

    // Default constructor required for VectorStorage
    Position() : x(0), y(0)
    {
    }

    // Move constructor required for VectorStorage
    constexpr Position(Position &&other) noexcept : x(other.x), y(other.y){};

    Position &operator=(Position &&other) noexcept
    {
        x = other.x;
        y = other.y;
        return *this;
    }

    Position(int px, int py) : x(px), y(py)
    {
    }
    Position(std::pair<int, int> pair) : x(pair.first), y(pair.second)
    {
    }
};

TEST(VectorStorage, emplace)
{
    ecstasy::VectorStorage<Position> storage;
    auto &components = ecstasy::VectorStorageTest().getComponents(storage);

    EXPECT_EQ(storage.getMask().size(), 0);
    EXPECT_EQ(components.size(), 0);

    Position &pos = storage.emplace(0, 5, 3);

    EXPECT_EQ(pos.x, 5);
    EXPECT_EQ(pos.y, 3);

    Position &pos2 = storage.emplace(1, std::make_pair(6, 9));

    EXPECT_EQ(pos2.x, 6);
    EXPECT_EQ(pos2.y, 9);

    /// Little bit of mask test, entity 0 is the LSB (to the right)
    EXPECT_EQ(storage.getMask(), util::BitSet("11"));
    EXPECT_EQ(components.size(), 2);
    storage.erase(0);
    // Entity 0 is now empty but entity 1 is still there the 0 is padding, the mask must be used instead of the vector
    // size
    EXPECT_EQ(components.size(), 2);
    EXPECT_EQ(storage.getMask(), util::BitSet("10"));

    // Add new entity 0
    storage.emplace(0, 5, 3);
    EXPECT_EQ(components.size(), 2);
    EXPECT_EQ(storage.getMask(), util::BitSet("11"));
}

TEST(VectorStorage, contains)
{
    ecstasy::VectorStorage<Position> storage;
    auto &components = ecstasy::VectorStorageTest().getComponents(storage);

    EXPECT_EQ(components.size(), 0);
    EXPECT_FALSE(storage.contains(0));
    EXPECT_FALSE(storage.contains(2));

    storage.emplace(2, 5, 3);
    EXPECT_EQ(components.size(), 3);
    EXPECT_FALSE(storage.contains(0));
    EXPECT_TRUE(storage.contains(2));
}

TEST(VectorStorage, get)
{
    ecstasy::VectorStorage<Position> storage;

    EXPECT_THROW(storage.at(0), std::out_of_range);
    EXPECT_THROW(const_cast<ecstasy::VectorStorage<Position> &>(storage).at(0), std::out_of_range);

    storage.emplace(0, 5, 3);
    EXPECT_EQ(storage.at(0).x, 5);
    storage.at(0).x = 8;
    EXPECT_EQ(const_cast<ecstasy::VectorStorage<Position> &>(storage).at(0).x, 8);
}

TEST(VectorStorage, erase)
{
    ecstasy::VectorStorage<Position> storage;
    const auto &cstorage = storage;
    auto &components = ecstasy::VectorStorageTest().getComponents(storage);

    EXPECT_EQ(storage.getMask(), util::BitSet(""));
    EXPECT_EQ(components.size(), 0);
    EXPECT_THROW(storage.at(0), std::out_of_range);
    EXPECT_THROW(cstorage.at(0), std::out_of_range);

    storage.erase(0);
    EXPECT_THROW(storage.at(0), std::out_of_range);

    storage.emplace(0, 5, 3);
    EXPECT_EQ(storage.getMask(), util::BitSet("1"));
    EXPECT_EQ(components.size(), 1);
    EXPECT_EQ(storage.at(0).x, 5);
    EXPECT_EQ(cstorage.at(0).y, 3);

    storage.emplace(1, 12, 13);
    EXPECT_EQ(storage.getMask(), util::BitSet("11"));
    EXPECT_EQ(components.size(), 2);
    EXPECT_EQ(storage.at(1).x, 12);
    EXPECT_EQ(cstorage.at(1).y, 13);

    storage.emplace(5, 1, 2);
    EXPECT_EQ(storage.getMask(), util::BitSet("100011"));
    EXPECT_EQ(components.size(), 6);
    EXPECT_EQ(storage.at(5).x, 1);
    EXPECT_EQ(cstorage.at(5).y, 2);

    // Erase entity 1 but not 5 so padding is kept between 0-5
    // But the mask is updated and is the source of truth
    storage.erase(1);
    EXPECT_EQ(components.size(), 6);
    EXPECT_THROW(storage.at(1), std::out_of_range);
    EXPECT_EQ(storage.getMask(), util::BitSet("100001"));

    // Erase entity 5, the padding is removed
    storage.erase(5);
    EXPECT_EQ(components.size(), 1);
    EXPECT_THROW(storage.at(5), std::out_of_range);
    EXPECT_EQ(storage.getMask(), util::BitSet("1"));
}
