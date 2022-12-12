#include <gtest/gtest.h>
#include "util/serialization/toml/TomlConversion.hpp"
#include "util/serialization/toml/TomlNodeFactory.hpp"

using namespace util::serialization;

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
    INode::DateTime dateTime(std::chrono::system_clock::now());
    toml::value<toml::date_time> toml(TomlConversion::toToml(dateTime));
    NodePtr node = TomlNodeFactory::get().createFromToml(toml);
    NodeCPtr cnode = TomlNodeFactory::get().createFromToml(const_cast<const toml::value<toml::date_time> &>(toml));

    GTEST_ASSERT_EQ(node->asDateTime(), dateTime);
    GTEST_ASSERT_EQ(node->asDateTime(), cnode->asDateTime());
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

// TEST(TomlNodeFactory, DefaultDateTime)
// {
//     NodePtr node = TomlNodeFactory::get().create(INode::Type::DateTime);

//     GTEST_ASSERT_EQ(node->getType(), INode::Type::DateTime);
//     GTEST_ASSERT_EQ(node->asDateTime(), INode::DateTime());
// }