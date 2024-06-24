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

struct NonSerializable {
  public:
    float x;
    float y;

    NonSerializable(float px = 0.f, float py = 0.f) : x(px), y(py)
    {
    }
};

TEST(is_serializer, all)
{
    static_assert(concepts::is_serializer<RawSerializer>, "False negative on RawSerializer");
    static_assert(!concepts::is_serializer<ISerializer>, "False positive on interface ISerializer");
    static_assert(!concepts::is_serializer<Serializer<Position>>, "False positive on interface ISerializer");
    static_assert(!concepts::is_serializer<Position>, "False positive on Position component");
}

TEST(has_extraction_operator, all)
{
    static_assert(concepts::has_extraction_operator<Position, RawSerializer>, "False negative on RawSerializer");
    static_assert(!concepts::has_extraction_operator<NonSerializable, RawSerializer>,
        "False positive on NonSerializable component");
    static_assert(!concepts::has_extraction_operator<int, RawSerializer>, "False positive on fundamental type (int)");
}
