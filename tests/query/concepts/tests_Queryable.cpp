#include <gtest/gtest.h>
#include "ecstasy/config.hpp"
#include "ecstasy/query/concepts/Queryable.hpp"
#include "util/BitSet.hpp"

using namespace ecstasy::query;

template <typename T>
class WrapperImpl {
  public:
    using WrappedType = T;

    WrapperImpl(T &data) : _data(data)
    {
    }

    T *operator->() const
    {
        return &_data;
    }

  private:
    T &_data;
};

class QueryableObjectImpl {
  public:
    using QueryData = std::string &;

    std::string &getQueryData(size_t index)
    {
        (void)index;
        return _data;
    }

    const util::BitSet &getMask() const
    {
        return _mask;
    }

  protected:
    std::string _data;
    util::BitSet _mask;
};

class PartialConstQueryableObjectImpl : public QueryableObjectImpl {
  public:
    std::string &getQueryData(size_t index)
    {
        (void)index;
        return _data;
    }

    const std::string &getQueryData(size_t index) const
    {
        (void)index;
        return _data;
    }
};

class ConstQueryableObjectImpl : public PartialConstQueryableObjectImpl {
  public:
    using ConstQueryData = const std::string &;
};

/// Debugging helpers, this print types T1 and T2 in the compilation error message
template <typename T1, typename T2>
void assert_equals()
{
    static_assert(std::is_same_v<T1, T2>, "Types T1 and T2 differs.");
}

TEST(QueryableObject, QueryableObjectImpl)
{
    // This is a valid queryable object
    static_assert(QueryableObject<QueryableObjectImpl>);
    static_assert(Queryable<QueryableObjectImpl>);

    // getQueryData() is not const and cannot be called by const QueryableObjectImpl
    static_assert(!QueryableObject<const QueryableObjectImpl>);
    static_assert(!ConstQueryableObject<const QueryableObjectImpl>);
    // getQueryData() is const but it doesn't return QueryData
    static_assert(!QueryableObject<const PartialConstQueryableObjectImpl>);
    // getQueryData() is const but there is no ConstQueryData
    static_assert(!ConstQueryableObject<const PartialConstQueryableObjectImpl>);

    // getQueryData() is const and returns ConstQueryData
    static_assert(QueryableObject<ConstQueryableObjectImpl>);
    static_assert(ConstQueryableObject<ConstQueryableObjectImpl>);
    static_assert(Queryable<ConstQueryableObjectImpl>);
    static_assert(Queryable<const ConstQueryableObjectImpl>);
}

TEST(QueryableWrapper, WrapperImpl)
{
    // This is a valid queryable object wrapper
    static_assert(QueryableWrapper<WrapperImpl<QueryableObjectImpl>>);
    static_assert(Queryable<WrapperImpl<QueryableObjectImpl>>);
    static_assert(QueryableWrapper<WrapperImpl<const ConstQueryableObjectImpl>>);
    static_assert(Queryable<WrapperImpl<const ConstQueryableObjectImpl>>);

    // Cannot wrap a const type which is not a ConstQueryableObject
    static_assert(!QueryableWrapper<WrapperImpl<const QueryableObjectImpl>>);
}

TEST(queryable_data, QueryableObjectImpl)
{
    assert_equals<queryable_data_t<QueryableObjectImpl>, std::string &>();
    assert_equals<queryable_data_t<ConstQueryableObjectImpl>, std::string &>();
    assert_equals<queryable_data_t<const ConstQueryableObjectImpl>, const std::string &>();

    assert_equals<queryable_data_t<WrapperImpl<QueryableObjectImpl>>, std::string &>();
    assert_equals<queryable_data_t<WrapperImpl<ConstQueryableObjectImpl>>, std::string &>();
    assert_equals<queryable_data_t<WrapperImpl<const ConstQueryableObjectImpl>>, const std::string &>();
}
