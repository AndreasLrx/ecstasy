#include <gtest/gtest.h>
#include "ecstasy/config.hpp"
#include "ecstasy/query/concepts/Queryable.hpp"
#include "util/BitSet.hpp"

using namespace ecstasy::query;

#ifdef ECSTASY_MULTI_THREAD
    #include "ecstasy/thread/LockableView.hpp"
    #include "ecstasy/thread/SharedRecursiveMutex.hpp"

using namespace ecstasy::thread;
#endif

class QueryableObjectImpl
#ifdef ECSTASY_MULTI_THREAD
    : public SharedRecursiveMutex
#endif
{
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

#ifdef ECSTASY_MULTI_THREAD

TEST(QueryableWrapper, LockableView)
{
    // This is a valid queryable object wrapper
    static_assert(QueryableWrapper<LockableView<QueryableObjectImpl>>);
    static_assert(Queryable<LockableView<QueryableObjectImpl>>);
    static_assert(QueryableWrapper<LockableView<const ConstQueryableObjectImpl>>);
    static_assert(Queryable<LockableView<const ConstQueryableObjectImpl>>);

    // Cannot wrap a const type which is not a ConstQueryableObject
    static_assert(!QueryableWrapper<LockableView<const QueryableObjectImpl>>);
}

#endif
