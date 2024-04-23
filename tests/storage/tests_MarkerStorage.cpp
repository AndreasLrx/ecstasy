#include <gtest/gtest.h>
#include "ecstasy/storages/MarkerStorage.hpp"

struct Static {
    using StorageType = ecstasy::MarkerStorage<Static>;
    size_t some_id;

    Static() : some_id(0)
    {
    }

    Static(size_t id) : some_id(id)
    {
    }
};

TEST(MarkerStorage, constructor)
{
    ecstasy::MarkerStorage<Static> storage_default;
    ecstasy::MarkerStorage<Static> storage_explicit(42);

    EXPECT_EQ(storage_default.GetInternalComponent().some_id, 0);
    EXPECT_EQ(storage_explicit.GetInternalComponent().some_id, 42);
}

TEST(MarkerStorage, emplace)
{
    ecstasy::MarkerStorage<Static> storage;
    Static &internal = storage.GetInternalComponent();

    EXPECT_EQ(storage.getMask().size(), 0);
    auto &component = storage.emplace(0);
    EXPECT_EQ(storage.getMask().size(), 1);
    EXPECT_EQ(&component, &internal);

    // Emplace with force resize to fit
    component = storage.emplace(4);
    EXPECT_EQ(storage.getMask().size(), 5);
    EXPECT_EQ(&component, &internal);

    /// Little bit of mask test, entity 0 is the LSB (to the right)
    EXPECT_EQ(storage.getMask(), util::BitSet("10001"));
    storage.erase(0);
    EXPECT_EQ(storage.getMask(), util::BitSet("10000"));
    storage.emplace(0);
    EXPECT_EQ(storage.getMask(), util::BitSet("10001"));
}

TEST(MarkerStorage, contains)
{
    ecstasy::MarkerStorage<Static> storage;

    EXPECT_FALSE(storage.contains(0));
    storage.emplace(0);
    EXPECT_TRUE(storage.contains(0));
}

TEST(MarkerStorage, get)
{
    ecstasy::MarkerStorage<Static> storage;
    const auto &cstorage = storage;

    EXPECT_THROW(storage.at(0), std::out_of_range);
    EXPECT_THROW(cstorage.at(0), std::out_of_range);

    storage.emplace(0);
    EXPECT_EQ(&storage.at(0), &storage.GetInternalComponent());
    EXPECT_EQ(&cstorage.at(0), &storage.GetInternalComponent());
}

TEST(MarkerStorage, erase)
{
    ecstasy::MarkerStorage<Static> storage;
    const auto &cstorage = storage;

    EXPECT_EQ(storage.getMask(), util::BitSet(""));
    EXPECT_THROW(storage.at(0), std::out_of_range);
    EXPECT_THROW(cstorage.at(0), std::out_of_range);

    storage.erase(0);
    EXPECT_THROW(storage.at(0), std::out_of_range);

    storage.emplace(0);
    EXPECT_EQ(storage.getMask(), util::BitSet("1"));
    storage.erase(0);
    EXPECT_THROW(storage.at(0), std::out_of_range);
    EXPECT_EQ(storage.getMask(), util::BitSet("0"));
}
