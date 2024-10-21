#include <gtest/gtest.h>
#include "ecstasy/registry/Registry.hpp"
#include "ecstasy/resources/entity/RegistryEntity.hpp"
#include "ecstasy/serialization/RawSerializer.hpp"
#include "ecstasy/serialization/Serializer.hpp"
#include "ecstasy/storages/MapStorage.hpp"

#ifdef ECSTASY_SERIALIZER_JSON
    #include "ecstasy/serialization/JsonSerializer.hpp"
#endif
#include "ecstasy/serialization/NodeSerializer.hpp"
#include "util/serialization/toml/TomlNodeFactory.hpp"

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

    JsonSerializer &operator>>(JsonSerializer &serializer) const
    {
        return serializer << JsonSerializer::NewObject << "x" << x << "y" << y << JsonSerializer::Close;
    }

    Position &operator<<(JsonSerializer &serializer)
    {
        serializer >> JsonSerializer::NewObject >> "x" >> x >> "y" >> y >> JsonSerializer::Close;
        return *this;
    }

    Position &operator<<(RawSerializer &serializer)
    {
        serializer >> x >> y;
        return *this;
    }
};
REGISTER_SERIALIZABLES(Position, RawSerializer, JsonSerializer)

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

    JsonSerializer &operator>>(JsonSerializer &serializer) const
    {
        return serializer << JsonSerializer::NewObject << "pos" << pos << "name" << std::string_view(name)
                          << JsonSerializer::Close;
    }

    NPC &operator<<(JsonSerializer &serializer)
    {
        serializer >> JsonSerializer::NewObject >> "pos" >> pos >> "name" >> name >> JsonSerializer::Close;
        return *this;
    }

    NPC &operator<<(RawSerializer &serializer)
    {
        serializer >> pos >> name;
        return *this;
    }
};
REGISTER_SERIALIZABLES(NPC, JsonSerializer)

TEST(RawSerializer, fundamental_types)
{
    RawSerializer rawSerializer;

    // INT
    rawSerializer << static_cast<uint8_t>(21) << static_cast<uint16_t>(42) << static_cast<uint32_t>(84)
                  << static_cast<uint64_t>(168) << -45612.f;
    std::string bytes = rawSerializer.exportBytes();
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
    // Double read
    rawSerializer.resetReadCursor();
    rawSerializer >> u8 >> u16 >> u32 >> u64 >> f;
    EXPECT_EQ(u8, 21);
    EXPECT_EQ(u16, 42);
    EXPECT_EQ(u32, 84);
    EXPECT_EQ(u64, 168);
    EXPECT_EQ(f, -45612.f);
}

TEST(Serializer, common_methods)
{
    RawSerializer serializer;
    std::stringstream stream;

    // Save a uint8_t
    uint8_t u8 = 42;
    serializer << u8;
    GTEST_ASSERT_EQ(serializer.size(), sizeof(uint8_t));

    // Test stream export
    serializer.exportStream(stream);
    GTEST_ASSERT_EQ(stream.str().size(), sizeof(uint8_t));
    GTEST_ASSERT_EQ(*reinterpret_cast<const uint8_t *>(stream.str().data()), 42);

    // Test bytes export
    std::string bytes = serializer.exportBytes();
    GTEST_ASSERT_EQ(bytes.size(), sizeof(uint8_t));
    GTEST_ASSERT_EQ(*reinterpret_cast<const uint8_t *>(bytes.data()), 42);

    // Test stream and bytes have same behavior
    GTEST_ASSERT_EQ(bytes, stream.str());

    // Test clear
    serializer.clear();
    GTEST_ASSERT_EQ(serializer.size(), 0);
    serializer.clear();
    GTEST_ASSERT_EQ(serializer.size(), 0);

    // Test import
    RawSerializer serializer2(bytes);
    GTEST_ASSERT_EQ(serializer2.size(), sizeof(uint8_t));
    uint8_t u8Loaded = serializer2.load<uint8_t>();
    GTEST_ASSERT_EQ(u8Loaded, 42);
}

TEST(Serializer, strings)
{
    RawSerializer serializer;
    std::string empty(nullptr, 0);
    std::string loaded;

    serializer << empty << std::string_view("this is a test");
    serializer >> loaded;

    GTEST_ASSERT_EQ(serializer.size(), sizeof(uint32_t) * 2 + 14);
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
        GTEST_ASSERT_EQ(serializer.size(), sizeof(int) * 10);

        // We can't load them, returning an array is not possible
        int updated[10];
        serializer >> updated;
        for (int i = 0; i < 10; i++)
            GTEST_ASSERT_EQ(updated[i], i + 1);
    }

    // Reset the stream
    serializer.clear();

    // Vector of fundamental types
    {
        std::vector<int> vec(10);
        for (int i = 0; i < 10; i++)
            vec[i] = i + 1;

        serializer << vec;
        GTEST_ASSERT_EQ(serializer.size(), sizeof(decltype(vec.size())) + sizeof(int) * 10);

        std::vector<int> updated;
        serializer >> updated;
        for (int i = 0; i < 10; i++)
            GTEST_ASSERT_EQ(updated[i], i + 1);

        // Load (Double read)
        serializer.resetReadCursor();
        std::vector<int> loaded = serializer.load<std::vector<int>>();
        for (int i = 0; i < 10; i++)
            GTEST_ASSERT_EQ(loaded[i], i + 1);
    }

    // Reset the stream
    serializer.clear();

    // Vector of compound type
    {
        static_assert(std::is_constructible_v<Position, RawSerializer &>);
        static_assert(std::is_constructible_v<Position, std::add_lvalue_reference_t<RawSerializer>>);
        static_assert(RawSerializer::is_constructible<Position>);
        std::vector<Position> vec(10);
        for (int i = 0; i < 10; i++)
            vec[i].x = static_cast<float>(i) + 1;

        serializer << vec;
        GTEST_ASSERT_EQ(serializer.size(), sizeof(decltype(vec.size())) + sizeof(Position) * 10);

        std::vector<Position> updated;
        serializer >> updated;
        for (int i = 0; i < 10; i++)
            GTEST_ASSERT_EQ(updated[i].x, i + 1);

        // Load (Double read)
        serializer.resetReadCursor();
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
    std::string posSerialized = rawSerializer.exportBytes();
    EXPECT_EQ(*reinterpret_cast<const float *>(&posSerialized.c_str()[0]), 1.0f);
    EXPECT_EQ(*reinterpret_cast<const float *>(&posSerialized.c_str()[4]), -8456.0f);

    // Update
    Position posDeserialized;
    rawSerializer >> posDeserialized;
    GTEST_ASSERT_EQ(posDeserialized.x, 1.0f);
    GTEST_ASSERT_EQ(posDeserialized.y, -8456.0f);

    // Load (Double read)
    rawSerializer.resetReadCursor();
    Position posLoaded = Position(rawSerializer);
    GTEST_ASSERT_EQ(posLoaded.x, 1.0f);
    GTEST_ASSERT_EQ(posLoaded.y, -8456.0f);

    /// NPC
    rawSerializer.clear();
    NPC npc{position, "Steve"};

    // Save
    rawSerializer << npc;
    std::string npcSerialized = rawSerializer.exportBytes();

    // Update
    NPC npcDeserialized;
    rawSerializer >> npcDeserialized;
    GTEST_ASSERT_EQ(npcDeserialized.name, "Steve");
    GTEST_ASSERT_EQ(npcDeserialized.pos.x, 1.0f);
    GTEST_ASSERT_EQ(npcDeserialized.pos.y, -8456.0f);

    // Load (Double read)
    rawSerializer.resetReadCursor();
    NPC npcLoaded = NPC(rawSerializer);
    GTEST_ASSERT_EQ(npcLoaded.name, "Steve");
    GTEST_ASSERT_EQ(npcLoaded.pos.x, 1.0f);
    GTEST_ASSERT_EQ(npcLoaded.pos.y, -8456.0f);
}

TEST(Serializer, ComponentsRTTI)
{
    RawSerializer rawSerializer;
    ecstasy::Registry registry;

    // Ensure components are registered (or not)
    GTEST_ASSERT_TRUE(RawSerializer::hasEntityComponentSerializer(typeid(Position).hash_code()));
    GTEST_ASSERT_FALSE(RawSerializer::hasEntityComponentSerializer(typeid(NPC).hash_code()));

    ecstasy::RegistryEntity entity(
        registry.entityBuilder().with<Position>(1.0f, -8456.0f).with<NPC>(Position(42.f, 0.f), "Steve").build(),
        registry);

    // Only save the Position component (NPC is not registered)
    rawSerializer.saveEntity(entity);
    // Check the serialized data
    rawSerializer.resetReadCursor();
    std::size_t component_hash = rawSerializer.load<std::size_t>();
    GTEST_ASSERT_EQ(component_hash, typeid(Position).hash_code());
    Position posLoaded = Position(rawSerializer);
    GTEST_ASSERT_EQ(posLoaded.x, 1.0f);
    GTEST_ASSERT_EQ(posLoaded.y, -8456.0f);

    // Update the Position component
    entity.get<Position>().x = 2.0f;
    entity.get<Position>().y = -8457.0f;
    rawSerializer.resetReadCursor();
    rawSerializer.updateEntity(entity);

    // Check the updated data
    GTEST_ASSERT_EQ(entity.get<Position>().x, 1.0f);
    GTEST_ASSERT_EQ(entity.get<Position>().y, -8456.0f);

    // Load the Position component
    rawSerializer.resetReadCursor();
    ecstasy::RegistryEntity e2 = rawSerializer.loadEntity(registry);
    GTEST_ASSERT_EQ(e2.get<Position>().x, 1.0f);
    GTEST_ASSERT_EQ(e2.get<Position>().y, -8456.0f);
}

TEST(Serializer, entityComponents)
{
    RawSerializer rawSerializer;
    ecstasy::Registry registry;

    ecstasy::RegistryEntity entity(
        registry.entityBuilder().with<Position>(1.0f, -8456.0f).with<NPC>(Position(42.f, 0.f), "Steve").build(),
        registry);

    //// Test saveEntity
    rawSerializer.saveEntity<NPC, Position>(entity);
    std::string entitySerializedExplicit = rawSerializer.exportBytes();

    rawSerializer.clear();
    rawSerializer << typeid(NPC) << entity.get<NPC>() << typeid(Position) << entity.get<Position>();
    std::string expected = rawSerializer.exportBytes();
    GTEST_ASSERT_EQ(entitySerializedExplicit, expected);
}

TEST(JsonSerializer, all)
{
    JsonSerializer jsonSerializer;

    // Fundamental types
    std::string empty = "";
    jsonSerializer << static_cast<uint8_t>(21) << static_cast<uint16_t>(42) << static_cast<uint32_t>(84)
                   << static_cast<uint64_t>(168) << -45612.f << empty << std::string_view("this is a test")
                   << "raw string";

    std::string json = jsonSerializer.exportBytes();
    GTEST_ASSERT_EQ(json, "[21,42,84,168,-45612.0,\"\",\"this is a test\",\"raw string\"]");

    jsonSerializer.resetCursor();
    uint8_t u8 = 0;
    uint16_t u16 = 0;
    uint32_t u32 = 0;
    uint64_t u64 = 0;
    float f = 0;
    std::string emptyLoaded, loaded, rawLoaded;
    jsonSerializer >> u8 >> u16 >> u32 >> u64 >> f >> emptyLoaded >> loaded >> rawLoaded;
    GTEST_ASSERT_EQ(u8, 21);
    GTEST_ASSERT_EQ(u16, 42);
    GTEST_ASSERT_EQ(u32, 84);
    GTEST_ASSERT_EQ(u64, 168);
    GTEST_ASSERT_EQ(f, -45612.f);
    GTEST_ASSERT_EQ(emptyLoaded, "");
    GTEST_ASSERT_EQ(loaded, "this is a test");
    GTEST_ASSERT_EQ(rawLoaded, "raw string");

    jsonSerializer.clear();
    // Array types
    std::vector<int> vec(5);
    int someInts[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    for (int i = 0; i < 5; i++)
        vec[i] = i + 1;

    jsonSerializer << someInts << vec;
    json = jsonSerializer.exportBytes();
    GTEST_ASSERT_EQ(json, "[[1,2,3,4,5,6,7,8,9,10],[1,2,3,4,5]]");

    jsonSerializer.resetCursor();
    int updated[std::extent_v<decltype(someInts)>];
    std::vector<int> updatedVec;
    jsonSerializer >> updated >> updatedVec;
    for (int i = 0; i < 10; i++) {
        GTEST_ASSERT_EQ(updated[i], i + 1);
        if (i < 5) {
            GTEST_ASSERT_EQ(updatedVec[i], i + 1);
        }
    }

    jsonSerializer.clear();
    // Compound types
    Position pos{1.0f, -8456.0f};
    NPC npc{Position(42.f, 0.f), "Steve"};

    jsonSerializer << pos << npc;
    json = jsonSerializer.exportBytes();
    GTEST_ASSERT_EQ(json, "[{\"x\":1.0,\"y\":-8456.0},{\"pos\":{\"x\":42.0,\"y\":0.0},\"name\":\"Steve\"}]");

    jsonSerializer.resetCursor();
    Position posUpdated;
    NPC npcUpdated;
    jsonSerializer >> posUpdated >> npcUpdated;
    GTEST_ASSERT_EQ(posUpdated.x, 1.0f);
    GTEST_ASSERT_EQ(posUpdated.y, -8456.0f);
    GTEST_ASSERT_EQ(npcUpdated.name, "Steve");
    GTEST_ASSERT_EQ(npcUpdated.pos.x, 42.f);
    GTEST_ASSERT_EQ(npcUpdated.pos.y, 0.f);

    jsonSerializer.resetCursor();
    Position posLoaded = jsonSerializer.load<Position>();
    NPC npcLoaded = jsonSerializer.load<NPC>();
    GTEST_ASSERT_EQ(posLoaded.x, 1.0f);
    GTEST_ASSERT_EQ(posLoaded.y, -8456.0f);
    GTEST_ASSERT_EQ(npcLoaded.name, "Steve");
    GTEST_ASSERT_EQ(npcLoaded.pos.x, 42.f);
    GTEST_ASSERT_EQ(npcLoaded.pos.y, 0.f);

    jsonSerializer.clear();
    // Test with entire entities
    ecstasy::Registry registry;
    ecstasy::RegistryEntity entity(
        registry.entityBuilder().with<Position>(1.0f, -8456.0f).with<NPC>(Position(42.f, 0.f), "Steve").build(),
        registry);

    // Save both components as they are registered (Position and NPC)
    jsonSerializer.saveEntity(entity);
    json = jsonSerializer.exportBytes();
#ifndef _WIN32
    GTEST_ASSERT_EQ(
        json, "[{\"NPC\":{\"pos\":{\"x\":42.0,\"y\":0.0},\"name\":\"Steve\"},\"Position\":{\"x\":1.0,\"y\":-8456.0}}]");
#else
    GTEST_ASSERT_EQ(
        json, "[{\"Position\":{\"x\":1.0,\"y\":-8456.0},\"NPC\":{\"pos\":{\"x\":42.0,\"y\":0.0},\"name\":\"Steve\"}}]");
#endif

    jsonSerializer.resetCursor();
    ecstasy::RegistryEntity e2 = jsonSerializer.loadEntity(registry);
    GTEST_ASSERT_EQ(e2.get<NPC>().name, "Steve");
    GTEST_ASSERT_EQ(e2.get<NPC>().pos.x, 42.f);
    GTEST_ASSERT_EQ(e2.get<NPC>().pos.y, 0.f);
    GTEST_ASSERT_EQ(e2.get<Position>().x, 1.0f);
    GTEST_ASSERT_EQ(e2.get<Position>().y, -8456.0f);
}

TEST(NodeSerializer, all)
{
    NodeSerializer nodeSerializer(util::serialization::TomlNodeFactory::get());

    // Fundamental types save and load
    std::string empty = "";
    nodeSerializer << static_cast<uint8_t>(21) << static_cast<uint16_t>(42) << static_cast<uint32_t>(84)
                   << static_cast<uint64_t>(168) << -45612.f << empty << std::string_view("this is a test")
                   << "raw string" << NodeSerializer::NewObject << "x" << 1.0f << "y" << -8456.0f
                   << NodeSerializer::Close;

    std::string toml = nodeSerializer.exportBytes();
    GTEST_ASSERT_EQ(
        toml, "[ 21, 42, 84, 168, -45612.0, '', 'this is a test', 'raw string', { x = 1.0, y = -8456.0 } ]");

    nodeSerializer.resetCursor();
    uint8_t u8 = 0;
    uint16_t u16 = 0;
    uint32_t u32 = 0;
    uint64_t u64 = 0;
    float f = 0;
    std::string emptyLoaded, loaded, rawLoaded;
    nodeSerializer >> u8 >> u16 >> u32 >> u64 >> f >> emptyLoaded >> loaded >> rawLoaded;
    GTEST_ASSERT_EQ(u8, 21);
    GTEST_ASSERT_EQ(u16, 42);
    GTEST_ASSERT_EQ(u32, 84);
    GTEST_ASSERT_EQ(u64, 168);
    GTEST_ASSERT_EQ(f, -45612.f);
    GTEST_ASSERT_EQ(emptyLoaded, "");
    GTEST_ASSERT_EQ(loaded, "this is a test");
    GTEST_ASSERT_EQ(rawLoaded, "raw string");
    float x, y;
    nodeSerializer >> NodeSerializer::NewObject >> "x" >> x >> "y" >> y >> NodeSerializer::Close;
}
