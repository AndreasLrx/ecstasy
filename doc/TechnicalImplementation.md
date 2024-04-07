# Technical Implementations

Explore the inner workings and technical details of Ecstasy. Gain insights into the core concepts, queryable structures, modifiers, conditions, and delve into the intricacies of implementing and utilizing these features. This section provides a deeper understanding of the ECS architecture and serves as a valuable resource for developers looking to integrate and optimize their systems with Ecstasy.

## Entities Management

The @ref ecstasy::Entities resource is the container of the registry entities informations. It is designed to be used within a registry and doesn't make sense alone. This resource only manage the _lifetime_ of the entities. Meaning it only knows if an entity exist, if it is alive or killed etc... It has no knowledge about the entity components.

An entity is in fact only an identifier, an index, it doesn't really contains components.
To keep track of the entities and their status, two @ref util::BitSet and a @ref std::vector are used.

```cpp
    // Keep track of the entity generation at each index
    // _generations[index] = generation of the current entity at the index
    std::vector<Entity::Generation> _generations;
    // Keep tracks of all existing entities.
    // _alive[index] = True (1) if the entity is alive, False (0) if not
    util::BitSet _alive;
    // Keep tracks of the deletion markers.
    // _killed[index] = True (1) if the entity needs to be killed, False(0) if not
    util::BitSet _killed;
```

1. Initialisation

   The @ref ecstasy::Entities resource is initialized with a sentinel bit is set at the end of the bitsets, therefore they are never empty.

2. Entity creation (@ref ecstasy::Entities::create)

   A new bit is appended to the \_alive bitset, representing the new entity. If there is a killed entity in the bitset, takes this place instead of resizing the bitset.

3. Entity deletion (@ref ecstasy::Entities::erase)

   Erasing an entity just means setting the bit at its index to false in \_alive bitset. The deletion is instant.

4. Entity kill (@ref ecstasy::Entities::kill)

   Killing an entity does not erase the entity, it only marks it for deletion. It set the bit at the entity index to true in \_killed bitset. If the bitset is too small, it is resize appropriately before.
   To effectively delete the killed entities, you need to call @ref ecstasy::Entities::maintain.

## Components Management (MapStorage)

Components management is made through the @ref ecstasy::IStorage. You can implement your own storage but the default storage is the @ref ecstasy::MapStorage and this part will only talk about it.

The MapStorage keep tracks of the components using two members:

```cpp
    // Component container, keeps data associated with the entity at the given index (bitset index)
    std::unordered_map<Entity::Index, Component> _components;
    // BitSet for a fast access on entity having or not the component (bit set to 1 means the entity at the bit index has the component)
    util::BitSet _mask;
```

## Query

The query implementation is mostly in the **ecstasy::query** namespace. It rely on **Queryable**, **Conditions** and mostly on **BitSet**.
The registry have some query code too, mostly to fetch the queryables from the registry storages or resources.
But the query class can be used without the registry parts if you provide the queryables yourself.
Meaning the query system could be extracted to be used in another context with some work.

So how does the query work behind the scene of the template black magic ?

To simplify things I will start to explains with the internal **ecstasy::query** behavior and then explain the autolinks on the registry.

If you read the Entity/Component parts I talked about the @ref util::BitSet class. The query operates on bitsets.
When you query all entities with the component position and velocity, 3 bitsets are sent to the query.

- Entities Bitset (\_alive)
- Position storage bitset (\_mask)
- Velocity storage bitset (\_mask)

The query can then perform a simple bitwise `and` on all the bitset and has its final bitset.
When you query the registry, the bitset is computed only upon creation and you iterate on the computed bitset.
Then for each bit set, the query fetch the associated component in the position/velocity storage.

#### Queryable and BitSet

A storage is in fact a @ref ecstasy::query::Queryable. It is defined by three properties:

- A bitset, accessible through a `getMask()` method
- A type for the value to return on iteration, defined as a nested `QueryData` type
- A method `getQueryData()` to fetch the value on iteration

```cpp
template <typename Q>
concept Queryable = requires(Q &queryable, Q const &cqueryable, std::size_t index)
{
    /// Data stored in the queryable.
    typename Q::QueryData;

    /// Const method returning the mask where all bits set to 1 means a query data at the bit index is available.
    { cqueryable.getMask() } -> std::same_as<util::BitSet const &>;

    /// Fetch the query data at the given index.
    { queryable.getQueryData(index) } -> std::same_as<typename Q::QueryData>;
};
```

Knowing this I can re explain the query behavior.
The query takes only @ref ecstasy::query::Queryable in inputs. It then does a bitwise `and` on the associated bitsets and save it.
The class also defines an iterator to... iterate on the matching entities and on each match it returns a tuple of the values, fetched through the `getQueryData` method.

You can also create a Query with a precomputed bitset as you can see below with the first constructor.

_util::meta::\* are helpers meta programming "functions"/types_

```cpp
template <Queryable First, Queryable... Others>
class Query : public QueryImplementation<util::meta::Traits<First, Others...>, util::meta::Traits<>> {
    public:
    // Construct a query with a precomputed bitset. Storages are still required to access the values.
    Query(util::BitSet &mask, const std::tuple<First &, Others &...> &storages)
        : QueryImplementation<util::meta::Traits<First, Others...>, util::meta::Traits<>>(mask, storages)
    {
    }

    // Construct a query from multiple queryables
    Query(First &first, Others &...others)
        : QueryImplementation<util::meta::Traits<First, Others...>, util::meta::Traits<>>(
            first, std::forward<Others &>(others)...)
    {
    }
};
```

#### Registry basic query

As we seen before, the @ref ecstasy::query::Query needs queryables. But we don't send any in the registry query:

```cpp
registry.query<Position, Velocity>()
```

Because magic happens in the registry, all the storages or resources are fetched with the @ref ecstasy::Registry::getQueryable template method:

```cpp
// Don't take in account the allocator, it is for next steps
template <typename C, typename... Cs, typename A = ModifiersAllocator>
query::Query<queryable_type_t<C>, queryable_type_t<Cs>...> query(OptionalModifiersAllocator<A> allocator)
{
    return query::Query(getQueryable<C>(allocator), getQueryable<Cs>(allocator)...);
}
```

The following functionnalities (modifiers, conditions etc) respect the the zero-overhead C++ principle meaning if you don't use them it won't slow down your application.

#### Select where

We already have almost everything to implement the select where syntax. These are the steps of resolution of the query:

- Detect the missing queryables in the where clause (everything in select must be in where clause too)
- Fetch all the queryables from the registry
- Compute the query mask with the where queryables
- Construct our query with the precomputed mask and our selected queryables

The hardest part is the first step, we already have the others. This basically means doing a left outer join to find the queryables present in the select clause (left) but not in the where clause (right).
This was a nightmare to do and I can't explain better than the documented code so go see @ref ecstasy::query::Select and @ref ecstasy::Registry::Select classes as long as the @ref util::meta namespace (@ref util::meta::left_outer_join).

#### Modifiers

Modifiers are not complicated themselves. In fact they are only queryable wrappers.
The easier example is the Maybe. What does it means to query `registry.query<Maybe<Position>>` ?

It means querying `Position`. But having all bit sets. And not returning a `Position` but a `std::optional<Position>`.
Finally a Modifier is really a Queryable, returning a bitset and a query data. See @ref ecstasy::query::modifier::Maybe implementation for more details.

The hard part about the modifiers is not their implementation but their allocation. Usual queryables are allocated by the registry and stored in the registry.
Modifiers should be allocated only for the query lifetime. And we need to allocate data to store the wrapped queryable (example **Position** storage) and eventually the computed mask if there is one.

And because allocations means allocator, every registry query method can take an Allocator.
First a heap allocator was used (@ref ecstasy::ModifiersAllocator) and was required if there was at least one modifier to allocate in the query and it raised an exception otherwise.
But now you don't need to take care of it. I already did it for you. The modifiers are now allocated on the stack because the required size is known at compile time.

This magic hidden stack allocator use multiple inheritance black magic tricks and you can find more about it in @ref ecstasy::Registry::RegistryStackQuery.

#### Conditions

In opposite to modifiers, conditions are not queryable and doesn't use bitset either.
The conditions are checked at runtime (because it check runtime values) on the query iteration.

Meaning if you iterate three times the same query the conditions will be evaluated three times for each matching entities.
But the good part about it is that it check the condition only when the values are fetched and doesn't make an extra fetch just for the condition.

#### AutoLock

The query parts have an `AutoLock` boolean template parameter. If it is set to true, it will wrap any queryables validating the [Lockable](@ref ecstasy::thread::Lockable) concept in a [LockableView](@ref ecstasy::thread::LockableView). This use the [RAII](https://en.cppreference.com/w/cpp/language/raii) concept: wrapping the lockable in the views will lock them upon view construction and unlock them upon view destruction.

1. Make everything lockable

   If you want thread safety, you need to be able to lock your storages and resources (especially @ref Entities) otherwise they will never be locked.

   For this you can use the compilation options @b ECSTASY_LOCKABLE_RESOURCES and/or @b ECSTASY_LOCKABLE_STORAGES. It will make the @ref Resources and/or @ref IStorage inherit from [SharedRecursiveMutex](@ref ecstasy::thread::SharedRecursiveMutex), thus validating the Lockable concept.

   @note
   This is disabled by default to avoid memory overhead of the mutex fields (ie: Zero-overhead principle).

   If you want to set the `AutoLock` to true by default for every **registry** queries, you can compile ecstasy with @b ECSTASY_AUTO_LOCK option.

2. Skeptical Lock Mechanism (ie: [SharedRecursiveMutex](@ref ecstasy::thread::SharedRecursiveMutex))

   The [SharedRecursiveMutex](@ref ecstasy::thread::SharedRecursiveMutex) (SRM, too loong to write) is made in a way to be @ref Lockable either with or without the const qualifier but the behavior will change:

   - Calling `lock()`/`unlock()` on a non-const SRM will perform an **exclusive lock/unlock on the mutex**
   - Calling `lock()`/`unlock()` on a const SRM will perform a **shared lock/unlock on the mutex**

   @note
   The mutex is recursive mostly because it is hard for ecstasy (meaning for my little brain) to detect whether a queryable is already locked by the current query or not, especially when dealing with the same queryable multiple times in the same query because of modifiers. Therefore it will be locked multiple times by the same query.

   @warning
   As said above the same lockable can be locked multiple times by the same query. You can easily have a deadlock if in the same query you request a const queryable and the non const version. Because it will try to perform an exclusive lock and a shared lock, which are not compatible. See [SharedRecursiveMutex](@ref ecstasy::thread::SharedRecursiveMutex) documentation.

3. [LockableView](@ref ecstasy::thread::LockableView) instanciation

   @ref [LockableView](@ref ecstasy::thread::LockableView) (LV, I'm not too lazy for this parenthesis but too lazy to write [LockableView](@ref ecstasy::thread::LockableView)) can be instanciated at multiple times depending of the query.

   1. In the Query itself

      If you set @b AutoLock to true in the [QueryImplementation](@ref ecstasy::query::QueryImplementation) the query will take the queryables references as inputs as usual. However it will store them in an LV using implicit constructor, because LV takes a reference to the lockable in constructor.
      Therefore the lifetime of the LV will be bound to the query.

   2. In the registry stack query

      However as you can guess it does not work well for precomputed mask like in the select where syntax because in these cases the query only keep the mask and the selected queryables, the where queryables are not kept in the query.

      Therefore when doing query from the registry, the registry use the magical [RegistryStackQueryMemory](@ref ecstasy::Registry::RegistryStackQueryMemory) class. This class compute the required size for the LVs and for the modifiers to allocate them on the stack (because why allocate on the heap when you can know at compile time the required memory size ?).
      The LV lifetime is the same as the RegistryStackQueryMemory, which is the same as the [RegistryStackQuery](@ref ecstasy::Registry::RegistryStackQuery) (being the query).

   3. And the modifiers ?

      The modifiers also have an @b AutoLock template parameters. If it it set it works exactly as the QueryImplementation: it locks the queryables upon construction using implicit LV constructor.

      @note
      By default the RegistryModifiers AutoLock value will be set from @b ECSTASY_AUTO_LOCK but you can use the extended version if you want to set/unset only one (@ref AndEx, @ref OrEx, @ref XorEx , the @ref Maybe and @ref Not don't have extended version because the classic version already supports a second AutoLock parameter).

#### Resolution Order

We have seen everything, not in the lowest details but enough to understand most of the query behaviors. Here is the resolution order of all those functionnality.

**Compile Time**

- Detect missing Queryables in where clause from select clause
- If no allocator provided prepare place in the stack with the appropriate size

**Query construction**

- Fetch required queryables from registry (where and select clause)
- Allocate required modifiers and views with the given allocator (which is by default the stack allocator)
- Compute query bitset from the where clause
- Creates the query object from the computed bitset and the selected queryables

**Query iteration**

- Fetch the values
- Check the conditions if any
- Return the values
