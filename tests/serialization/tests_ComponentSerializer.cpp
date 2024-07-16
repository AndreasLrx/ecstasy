#include <gtest/gtest.h>
#include "ecstasy/registry/Registry.hpp"
#include "ecstasy/resources/entity/RegistryEntity.hpp"
#include "ecstasy/serialization/ComponentSerializer.hpp"
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

TEST(ComponentSerializer, SaveComponent)
{
    RawSerializer rawSerializer;
    Position position(1.f, 2.f);

    rawSerializer << typeid(Position) << position;
    std::string expected = rawSerializer.getStream().str();

    rawSerializer.getStream().str("");
    ComponentSerializer<Serializers>::save(rawSerializer, typeid(RawSerializer), position);
    GTEST_ASSERT_EQ(rawSerializer.str(), expected);
}

TEST(ComponentSerializer, UpdateComponent)
{
    RawSerializer rawSerializer;
    Position position(1.f, 2.f);

    // Don't include typeid here
    rawSerializer << position;
    std::string expected = rawSerializer.getStream().str();
    position.x = 420.f;
    position.y = 69.f;

    rawSerializer.getStream().seekg(0);
    ComponentSerializer<Serializers>::update(rawSerializer, typeid(RawSerializer), position);
    GTEST_ASSERT_EQ(position.x, 1.f);
    GTEST_ASSERT_EQ(position.y, 2.f);
}
