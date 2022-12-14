#include <gtest/gtest.h>
#include "util/serialization/IArrayNode.hpp"
#include "util/serialization/toml/TomlArrayNode.hpp"
#include "util/serialization/toml/TomlConversion.hpp"
#include "util/serialization/toml/TomlNode.hpp"
#include "util/serialization/toml/TomlNodeFactory.hpp"
#include "util/serialization/toml/TomlObjectNode.hpp"

using namespace util::serialization;

struct TestTomlValue : public toml::value<std::string> {
    TestTomlValue() = default;
};

TEST(TomlNodeFactory, ObjectFromToml)
{
    using namespace std::string_view_literals;

    toml::table table = toml::parse(R"(
        name = "toml++"
        authors = ["Mark Gillard <mark.gillard@outlook.com.au>"]
        cpp = 17
    )"sv);
    NodePtr node = TomlNodeFactory::get().createFromToml(table);

    GTEST_ASSERT_EQ(node->asObject().size(), 3);
    GTEST_ASSERT_EQ(node->asObject().get("name").lock()->getType(), INode::Type::String);
    GTEST_ASSERT_EQ(node->asObject().get("authors").lock()->getType(), INode::Type::Array);
    GTEST_ASSERT_EQ(node->asObject().get("cpp").lock()->getType(), INode::Type::Integer);
}

TEST(TomlNodeFactory, ArrayFromToml)
{
    using namespace std::string_view_literals;
    toml::table table = toml::parse(R"(array = [1, 2, 3, 5, 9])"sv);

    NodePtr node = TomlNodeFactory::get().createFromToml(*table.get_as<toml::array>("array"));

    GTEST_ASSERT_EQ(node->asArray().size(), 5);
    GTEST_ASSERT_EQ(node->asArray().get(0).lock()->asInteger(), 1);
    GTEST_ASSERT_EQ(node->asArray().get(1).lock()->asInteger(), 2);
    GTEST_ASSERT_EQ(node->asArray().get(2).lock()->asInteger(), 3);
    GTEST_ASSERT_EQ(node->asArray().get(3).lock()->asInteger(), 5);
    GTEST_ASSERT_EQ(node->asArray().get(4).lock()->asInteger(), 9);
}

TEST(TomlNodeFactory, StringFromToml)
{
    toml::value<std::string> toml("Hello world!");
    NodePtr node = TomlNodeFactory::get().createFromToml(toml);
    NodeCPtr cnode = TomlNodeFactory::get().createFromToml(const_cast<const toml::value<std::string> &>(toml));

    GTEST_ASSERT_EQ(node->asString(), "Hello world!");
    GTEST_ASSERT_EQ(node->asString(), cnode->asString());
}

TEST(TomlNodeFactory, IntegerFromToml)
{
    toml::value<int64_t> toml(42);
    NodePtr node = TomlNodeFactory::get().createFromToml(toml);
    NodeCPtr cnode = TomlNodeFactory::get().createFromToml(const_cast<const toml::value<int64_t> &>(toml));

    GTEST_ASSERT_EQ(node->asInteger(), 42);
    GTEST_ASSERT_EQ(node->asInteger(), cnode->asInteger());
}

TEST(TomlNodeFactory, FloatFromToml)
{
    toml::value<double> toml(42.0);
    NodePtr node = TomlNodeFactory::get().createFromToml(toml);
    NodeCPtr cnode = TomlNodeFactory::get().createFromToml(const_cast<const toml::value<double> &>(toml));

    GTEST_ASSERT_EQ(node->asFloat(), 42.0);
    GTEST_ASSERT_EQ(node->asFloat(), cnode->asFloat());
}

TEST(TomlNodeFactory, BooleanFromToml)
{
    toml::value<bool> toml(true);
    NodePtr node = TomlNodeFactory::get().createFromToml(toml);
    NodeCPtr cnode = TomlNodeFactory::get().createFromToml(const_cast<const toml::value<bool> &>(toml));

    GTEST_ASSERT_TRUE(node->asBoolean());
    GTEST_ASSERT_EQ(node->asBoolean(), cnode->asBoolean());
}

TEST(TomlNodeFactory, DateFromToml)
{
    INode::Date date(std::chrono::year(1), std::chrono::month(2), std::chrono::day(3));
    toml::value<toml::date> toml(TomlConversion::toToml(date));
    NodePtr node = TomlNodeFactory::get().createFromToml(toml);
    NodeCPtr cnode = TomlNodeFactory::get().createFromToml(const_cast<const toml::value<toml::date> &>(toml));

    GTEST_ASSERT_EQ(node->asDate(), date);
    GTEST_ASSERT_EQ(node->asDate(), cnode->asDate());
}

TEST(TomlNodeFactory, TimeFromToml)
{
    INode::Time time(420);
    toml::value<toml::time> toml(TomlConversion::toToml(time));
    NodePtr node = TomlNodeFactory::get().createFromToml(toml);
    NodeCPtr cnode = TomlNodeFactory::get().createFromToml(const_cast<const toml::value<toml::time> &>(toml));

    GTEST_ASSERT_EQ(node->asTime(), time);
    GTEST_ASSERT_EQ(node->asTime(), cnode->asTime());
}

TEST(TomlNodeFactory, DateTimeFromToml)
{
    INode::DateTime dateTime(std::chrono::high_resolution_clock::now());
    toml::value<toml::date_time> toml(TomlConversion::toToml(dateTime));
    NodePtr node = TomlNodeFactory::get().createFromToml(toml);
    NodeCPtr cnode = TomlNodeFactory::get().createFromToml(const_cast<const toml::value<toml::date_time> &>(toml));

    GTEST_ASSERT_EQ(node->asDateTime(), dateTime);
    GTEST_ASSERT_EQ(node->asDateTime(), cnode->asDateTime());
}

TEST(TomlNodeFactory, NullFromNode)
{
    TomlNode<TestTomlValue> in;
    NodePtr node = TomlNodeFactory::get().create(in);

    GTEST_ASSERT_FALSE(node);
}

TEST(TomlNodeFactory, ObjectFromNode)
{
    TomlObjectNode object;

    for (int i = 0; i < 10; i++)
        object.insert(std::to_string(i), TomlNode<toml::value<int64_t>>(i * 2));
    NodePtr node = TomlNodeFactory::get().create(object);

    for (int i = 0; i < 10; i++)
        GTEST_ASSERT_EQ(node->asObject().get(std::to_string(i)).lock()->asInteger(), i * 2);
}

TEST(TomlNodeFactory, ArrayFromNode)
{
    TomlArrayNode array;

    for (int i = 0; i < 10; i++)
        array.pushBack(TomlNode<toml::value<int64_t>>(i));
    NodePtr node = TomlNodeFactory::get().create(array);

    for (int i = 0; i < 10; i++)
        GTEST_ASSERT_EQ(node->asArray().get(i).lock()->asInteger(), i);
}

TEST(TomlNodeFactory, StringFromNode)
{
    TomlNode<toml::value<std::string>> in("hello");
    NodePtr node = TomlNodeFactory::get().create(in);

    GTEST_ASSERT_EQ(node->asString(), "hello");
    GTEST_ASSERT_EQ(node->asString(), in.asString());
}

TEST(TomlNodeFactory, IntegerFromNode)
{
    TomlNode<toml::value<int64_t>> in(42);
    NodePtr node = TomlNodeFactory::get().create(in);

    GTEST_ASSERT_EQ(node->asInteger(), 42);
    GTEST_ASSERT_EQ(node->asInteger(), in.asInteger());
}

TEST(TomlNodeFactory, FloatFromNode)
{
    TomlNode<toml::value<double>> in(42.0);
    NodePtr node = TomlNodeFactory::get().create(in);

    GTEST_ASSERT_EQ(node->asFloat(), 42);
    GTEST_ASSERT_EQ(node->asFloat(), in.asFloat());
}

TEST(TomlNodeFactory, BooleanFromNode)
{
    TomlNode<toml::value<bool>> in(true);
    NodePtr node = TomlNodeFactory::get().create(in);

    GTEST_ASSERT_EQ(node->asBoolean(), true);
    GTEST_ASSERT_EQ(node->asBoolean(), in.asBoolean());
}

TEST(TomlNodeFactory, DateFromNode)
{
    TomlNode<toml::value<toml::date>> in(1, 2, 3);
    NodePtr node = TomlNodeFactory::get().create(in);

    GTEST_ASSERT_EQ(node->asDate(), INode::Date(std::chrono::year(1), std::chrono::month(2), std::chrono::day(3)));
    GTEST_ASSERT_EQ(node->asDate(), in.asDate());
}

TEST(TomlNodeFactory, TimeFromNode)
{
    TomlNode<toml::value<toml::time>> in(0, 0, 0, 420);
    NodePtr node = TomlNodeFactory::get().create(in);

    GTEST_ASSERT_EQ(node->asTime(), INode::Time(420));
    GTEST_ASSERT_EQ(node->asTime(), in.asTime());
}

TEST(TomlNodeFactory, DateTimeFromNode)
{
    TomlNode<toml::value<toml::date_time>> in(toml::date(1, 2, 3), toml::time(0, 0, 0, 420));
    NodePtr node = TomlNodeFactory::get().create(in);

    GTEST_ASSERT_EQ(node->asDateTime(),
        INode::DateTime(static_cast<std::chrono::sys_days>(
                            INode::Date(std::chrono::year(1), std::chrono::month(2), std::chrono::day(3)))
                            .time_since_epoch()
            + INode::Time(420)));

    GTEST_ASSERT_EQ(node->asDateTime(), in.asDateTime());
}

TEST(TomlNodeFactory, DefaultNull)
{
    NodePtr node = TomlNodeFactory::get().create(INode::Type::Null);

    GTEST_ASSERT_FALSE(node);
}

TEST(TomlNodeFactory, DefaultObject)
{
    NodePtr node = TomlNodeFactory::get().create(INode::Type::Object);

    GTEST_ASSERT_EQ(node->getType(), INode::Type::Object);
    GTEST_ASSERT_EQ(node->asObject().size(), 0);
}

TEST(TomlNodeFactory, DefaultArray)
{
    NodePtr node = TomlNodeFactory::get().create(INode::Type::Array);

    GTEST_ASSERT_EQ(node->getType(), INode::Type::Array);
    GTEST_ASSERT_EQ(node->asArray().size(), 0);
}

TEST(TomlNodeFactory, DefaultString)
{
    NodePtr node = TomlNodeFactory::get().create(INode::Type::String);

    GTEST_ASSERT_EQ(node->getType(), INode::Type::String);
    GTEST_ASSERT_EQ(node->asString(), "");
}

TEST(TomlNodeFactory, DefaultInteger)
{
    NodePtr node = TomlNodeFactory::get().create(INode::Type::Integer);

    GTEST_ASSERT_EQ(node->getType(), INode::Type::Integer);
    GTEST_ASSERT_EQ(node->asInteger(), 0);
}

TEST(TomlNodeFactory, DefaultFloat)
{
    NodePtr node = TomlNodeFactory::get().create(INode::Type::Float);

    GTEST_ASSERT_EQ(node->getType(), INode::Type::Float);
    GTEST_ASSERT_EQ(node->asFloat(), 0.0);
}

TEST(TomlNodeFactory, DefaultBoolean)
{
    NodePtr node = TomlNodeFactory::get().create(INode::Type::Boolean);

    GTEST_ASSERT_EQ(node->getType(), INode::Type::Boolean);
    GTEST_ASSERT_EQ(node->asBoolean(), false);
}

TEST(TomlNodeFactory, DefaultDate)
{
    NodePtr node = TomlNodeFactory::get().create(INode::Type::Date);

    GTEST_ASSERT_EQ(node->getType(), INode::Type::Date);
    GTEST_ASSERT_EQ(node->asDate(), INode::Date());
}

TEST(TomlNodeFactory, DefaultTime)
{
    NodePtr node = TomlNodeFactory::get().create(INode::Type::Time);

    GTEST_ASSERT_EQ(node->getType(), INode::Type::Time);
    GTEST_ASSERT_EQ(node->asTime(), INode::Time());
}

TEST(TomlNodeFactory, DefaultDateTime)
{
    NodePtr node = TomlNodeFactory::get().create(INode::Type::DateTime);

    GTEST_ASSERT_EQ(node->getType(), INode::Type::DateTime);
    GTEST_ASSERT_EQ(node->asDateTime(), INode::DateTime());
}

TEST(TomlNodeFactory, CreateString)
{
    NodePtr node = TomlNodeFactory::get().create(std::string_view("Hello world!"));

    GTEST_ASSERT_EQ(node->getType(), INode::Type::String);
    GTEST_ASSERT_EQ(node->asString(), "Hello world!");
}

TEST(TomlNodeFactory, CreateInteger)
{
    NodePtr node = TomlNodeFactory::get().create(static_cast<int64_t>(42));

    GTEST_ASSERT_EQ(node->getType(), INode::Type::Integer);
    GTEST_ASSERT_EQ(node->asInteger(), 42);
}

TEST(TomlNodeFactory, CreateFloat)
{
    NodePtr node = TomlNodeFactory::get().create(42.0);

    GTEST_ASSERT_EQ(node->getType(), INode::Type::Float);
    GTEST_ASSERT_EQ(node->asFloat(), 42.0);
}

TEST(TomlNodeFactory, CreateBoolean)
{
    NodePtr node = TomlNodeFactory::get().create(true);

    GTEST_ASSERT_EQ(node->getType(), INode::Type::Boolean);
    GTEST_ASSERT_EQ(node->asBoolean(), true);
}

TEST(TomlNodeFactory, CreateDate)
{
    NodePtr node =
        TomlNodeFactory::get().create(INode::Date(std::chrono::year(1), std::chrono::month(2), std::chrono::day(3)));

    GTEST_ASSERT_EQ(node->getType(), INode::Type::Date);
    GTEST_ASSERT_EQ(node->asDate(), INode::Date(std::chrono::year(1), std::chrono::month(2), std::chrono::day(3)));
}

TEST(TomlNodeFactory, CreateTime)
{
    NodePtr node = TomlNodeFactory::get().create(INode::Time(42));

    GTEST_ASSERT_EQ(node->getType(), INode::Type::Time);
    GTEST_ASSERT_EQ(node->asTime(), INode::Time(42));
}

TEST(TomlNodeFactory, CreateDateTime)
{
    auto tp = std::chrono::high_resolution_clock::now();
    NodePtr node = TomlNodeFactory::get().create(tp);

    GTEST_ASSERT_EQ(node->getType(), INode::Type::DateTime);
    GTEST_ASSERT_EQ(node->asDateTime(), tp);
}