#include <gtest/gtest.h>
#include "ecstasy/registry/Registry.hpp"
#include "ecstasy/resources/entity/RegistryEntity.hpp"
#include "ecstasy/serialization/RawSerializer.hpp"
#include "ecstasy/serialization/Serializer.hpp"
#include "ecstasy/storages/MapStorage.hpp"

using namespace ecstasy::serialization;

struct Position {
  public:
    float x;
    float y;

    Position(float px = 0.f, float py = 0.f) : x(px), y(py)
    {
    }

    Position(RawSerializer &serializer) : x(serializer.load<float>()), y(serializer.load<float>())
    {
    }

    RawSerializer &operator>>(RawSerializer &serializer) const
    {
        return serializer.appendRaw(*this);
    }

    Position &operator<<(RawSerializer &serializer)
    {
        serializer >> x >> y;
        return *this;
    }
};

struct NPC {
    Position pos;
    std::string name;

    using RawDTO = char *;

    NPC(Position p = Position(), std::string n = "") : pos(p), name(std::move(n))
    {
    }

    NPC(RawSerializer &serializer) : pos(serializer), name(serializer.load<std::string>())
    {
    }

    RawSerializer &operator>>(RawSerializer &serializer) const
    {
        return serializer << pos << std::string_view(name);
    }

    NPC &operator<<(RawSerializer &serializer)
    {
        serializer >> pos >> name;
        return *this;
    }
};

TEST(RawSerializer, fundamental_types)
{
    RawSerializer rawSerializer;

    // INT
    rawSerializer << static_cast<uint8_t>(21) << static_cast<uint16_t>(42) << static_cast<uint32_t>(84)
                  << static_cast<uint64_t>(168) << -45612.f;
    std::string bytes = rawSerializer.str();
    EXPECT_EQ(bytes.length(), 1 + 2 + 4 + 8 + 4);
    EXPECT_EQ(*reinterpret_cast<const uint8_t *>(&bytes.c_str()[0]), 21);
    EXPECT_EQ(*reinterpret_cast<const uint16_t *>(&bytes.c_str()[1]), 42);
    EXPECT_EQ(*reinterpret_cast<const uint32_t *>(&bytes.c_str()[3]), 84);
    EXPECT_EQ(*reinterpret_cast<const uint64_t *>(&bytes.c_str()[7]), 168);
    EXPECT_EQ(*reinterpret_cast<const float *>(&bytes.c_str()[15]), -45612.f);

    uint8_t u8 = rawSerializer.load<uint8_t>();
    uint16_t u16 = rawSerializer.load<uint16_t>();
    uint32_t u32 = rawSerializer.load<uint32_t>();
    uint64_t u64 = rawSerializer.load<uint64_t>();
    float f = rawSerializer.load<float>();

    EXPECT_EQ(u8, 21);
    EXPECT_EQ(u16, 42);
    EXPECT_EQ(u32, 84);
    EXPECT_EQ(u64, 168);
    EXPECT_EQ(f, -45612.f);
    u8 = 0;
    u16 = 0;
    u32 = 0;
    u64 = 0;
    f = 0;
    rawSerializer.getStream().seekg(0);
    rawSerializer >> u8 >> u16 >> u32 >> u64 >> f;
    EXPECT_EQ(u8, 21);
    EXPECT_EQ(u16, 42);
    EXPECT_EQ(u32, 84);
    EXPECT_EQ(u64, 168);
    EXPECT_EQ(f, -45612.f);
}

TEST(Serializer, strings)
{
    RawSerializer serializer;
    std::string empty(nullptr, 0);
    std::string loaded;

    serializer << empty << std::string_view("this is a test");
    serializer >> loaded;

    GTEST_ASSERT_EQ(serializer.str().length(), sizeof(uint32_t) * 2 + 14);
    GTEST_ASSERT_EQ(reinterpret_cast<const uint32_t *>(serializer.getStream().view().data())[0], 0);
    GTEST_ASSERT_EQ(reinterpret_cast<const uint32_t *>(serializer.getStream().view().data())[1], 14);

    loaded = "";
    std::string view = serializer.load<std::string>();
    GTEST_ASSERT_EQ(empty, loaded);
    GTEST_ASSERT_EQ(view, "this is a test");
}

TEST(Serializer, arrays)
{
    RawSerializer serializer;

    // Array
    {
        int someInts[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

        serializer << someInts;
        GTEST_ASSERT_EQ(serializer.str().length(), sizeof(int) * 10);

        // We can't load them, returning an array is not possible
        int updated[10];
        serializer >> updated;
        for (int i = 0; i < 10; i++)
            GTEST_ASSERT_EQ(updated[i], i + 1);
    }

    // Reset the stream
    serializer.getStream().str("");

    // Vector of fundamental types
    {
        std::vector<int> vec(10);
        for (int i = 0; i < 10; i++)
            vec[i] = i + 1;

        serializer << vec;
        GTEST_ASSERT_EQ(serializer.str().length(), sizeof(decltype(vec.size())) + sizeof(int) * 10);

        std::vector<int> updated;
        serializer >> updated;
        for (int i = 0; i < 10; i++)
            GTEST_ASSERT_EQ(updated[i], i + 1);

        // Load
        serializer.getStream().seekg(0);
        std::vector<int> loaded = serializer.load<std::vector<int>>();
        for (int i = 0; i < 10; i++)
            GTEST_ASSERT_EQ(loaded[i], i + 1);
    }

    // Reset the stream
    serializer.getStream().str("");

    // Vector of compound type
    {
        static_assert(std::is_constructible_v<Position, RawSerializer &>);
        static_assert(std::is_constructible_v<Position, std::add_lvalue_reference_t<RawSerializer>>);
        static_assert(RawSerializer::is_constructible<Position>);
        std::vector<Position> vec(10);
        for (int i = 0; i < 10; i++)
            vec[i].x = static_cast<float>(i) + 1;

        serializer << vec;
        GTEST_ASSERT_EQ(serializer.str().length(), sizeof(decltype(vec.size())) + sizeof(Position) * 10);

        std::vector<Position> updated;
        serializer >> updated;
        for (int i = 0; i < 10; i++)
            GTEST_ASSERT_EQ(updated[i].x, i + 1);

        // Load
        serializer.getStream().seekg(0);
        std::vector<Position> loaded = serializer.load<std::vector<Position>>();
        for (int i = 0; i < 10; i++)
            GTEST_ASSERT_EQ(loaded[i].x, i + 1);
    }
}

TEST(Serializer, compound_struct)
{
    RawSerializer rawSerializer;

    // Position
    Position position{1.0f, -8456.0f};

    // Save
    rawSerializer << position;
    std::string posSerialized = rawSerializer.getStream().str();
    EXPECT_EQ(*reinterpret_cast<const float *>(&posSerialized.c_str()[0]), 1.0f);
    EXPECT_EQ(*reinterpret_cast<const float *>(&posSerialized.c_str()[4]), -8456.0f);

    // Update
    rawSerializer.getStream().seekg(0);
    Position posDeserialized;
    rawSerializer >> posDeserialized;
    GTEST_ASSERT_EQ(posDeserialized.x, 1.0f);
    GTEST_ASSERT_EQ(posDeserialized.y, -8456.0f);

    // Load
    rawSerializer.getStream().seekg(0);
    Position posLoaded = Position(rawSerializer);
    GTEST_ASSERT_EQ(posLoaded.x, 1.0f);
    GTEST_ASSERT_EQ(posLoaded.y, -8456.0f);

    /// NPC
    rawSerializer.getStream().str("");
    NPC npc{position, "Steve"};

    // Save
    rawSerializer << npc;
    std::string npcSerialized = rawSerializer.getStream().str();

    // Update
    rawSerializer.getStream().seekg(0);
    NPC npcDeserialized;
    rawSerializer >> npcDeserialized;
    GTEST_ASSERT_EQ(npcDeserialized.name, "Steve");
    GTEST_ASSERT_EQ(npcDeserialized.pos.x, 1.0f);
    GTEST_ASSERT_EQ(npcDeserialized.pos.y, -8456.0f);

    // Load
    rawSerializer.getStream().seekg(0);
    NPC npcLoaded = NPC(rawSerializer);
    GTEST_ASSERT_EQ(npcLoaded.name, "Steve");
    GTEST_ASSERT_EQ(npcLoaded.pos.x, 1.0f);
    GTEST_ASSERT_EQ(npcLoaded.pos.y, -8456.0f);
}
