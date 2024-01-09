# Glossary

This page will help you define the various concepts used in ecstasy (most are common to other ECS).
Do not hesitate to create an issue if a definition is still complicated or if a complex term is missing.

## Entity

An entity represents a distinct object or item in the registry. It is an abstract, unique identifier that groups together components to define its characteristics and behavior.
Entities in an ECS can be seen as a link between multiple components.

References: [Entity](@ref ecstasy::Entity), [Entities](@ref ecstasy::Entities)

## Component

A component is a modular and reusable piece of data that encapsulates a specific aspect of an entity's state or behavior. Components are combined to compose the overall properties and functionalities of an entity.

It is usually a simple `struct` for example:

```cpp
struct Position {
    float x;
    float y;
};
```

## Storage

A storage is a component container. It is used to store and manage components. It organizes components for efficient access and iteration during system execution, facilitating quick retrieval and manipulation of entity data.

@note
Ecstasy currently implements a single storage type: [MapStorage](@ref ecstasy::MapStorage).
However you can create [custom storages](#CustomStorageTutorial).

References: [IStorage](@ref ecstasy::IStorage), [getStorageType](@ref ecstasy::getStorageType), [IsStorage](@ref ecstasy::IsStorage), [MapStorage](@ref ecstasy::MapStorage), [Registry.addStorage()](@ref ecstasy::Registry::addStorage)

## System

A system is a logic unit responsible for performing actions on the registry. It usually operate on entities with specific sets of components. Systems operate independently and collectively define the overall behavior of the ECS. They encapsulate the logic that acts on entities.

In fact it is a function called with an instance of the [registry](@ref ecstasy::Registry):

```cpp
struct Movement : public ecstasy::ISystem {
    void run(ecstasy::Registry &registry) override final
    {
        for (auto [position, velocity] : registry.query<Position, Velocity>()) {
            position.x += velocity.x;
            position.y += velocity.y;
        }
    }
};
```

References: [ISystem](@ref ecstasy::ISystem), [Registry.addSystem()](@ref ecstasy::Registry::addSystem), [Registry.runSystem()](@ref ecstasy::Registry::runSystem), [Registry.runSystems()](@ref ecstasy::Registry::runSystems)

## Resource

A resource is a shared and globally accessible piece of data that can be utilized by systems or entities in the ECS. Resources typically represent data that is not tied to a specific entity but is needed by various parts of the [registry](@ref ecstasy::Registry), such as configuration settings or global parameters.

@note
In ecstasy, the [Entities](@ref ecstasy::Entities) class is a resource present by default in the [registry](@ref ecstasy::Registry).

A simple use case would be a Timer resource to improve our previous Movement system:

```cpp
class Timer : public ecstasy::Resource {
public:
    /// Creates a new timer.
    explicit Timer() noexcept : _lastReset(std::chrono::steady_clock::now()) {}

    /// Resets the timer.
    void reset() noexcept { this->_lastReset = std::chrono::steady_clock::now(); }

    /// @returns The seconds since the last call to reset() (or construction of the Timer).
    double elapsed() const noexcept
    {
        std::chrono::steady_clock::time_point now(std::chrono::steady_clock::now());
        std::chrono::duration<float, std::ratio<1>> elapsed = now - this->_lastReset;

        return elapsed.count();
    }

private:
    std::chrono::steady_clock::time_point _lastReset;
};

struct Movement : public ecstasy::ISystem {
    void run(ecstasy::Registry &registry) override final
    {
        Timer &timer = registry.getResource<Timer>()
        float seconds = timer.elapsed();
        timer.reset();

        for (auto [position, velocity] : registry.query<Position, Velocity>()) {
            position.x += velocity.x * seconds;
            position.y += velocity.y * seconds;
        }
    }
};
```

References: [Resource](@ref ecstasy::Resource), [registry.addResource()](@ref ecstasy::Registry::addResource), [registry.getResource()](@ref ecstasy::Registry.getResource), [Entities](@ref ecstasy::Entities)

## Registry

The registry is a centralized database or manager responsible for creating, managing, and tracking entities within the ECS. It maintains the relationships between entities and their components and provides essential functionality for entity lifecycle management.

The registry contains:

- Systems
- Resources (therefore entities)
- Component storages

It is the link between the different data, the orchestrator of the application.

References: [Registry](@ref ecstasy::Registry)

## Query

A query in the context of this ECS refers to the process of selecting entities based on specific criteria or conditions. It allows developers to identify a subset of entities that possess certain components or meet certain requirements.

Ecstasy provides a SQL like way of querying your entities which means more concepts to define.
You can follow [this tutorial](#QueryTutorial) to use them for more informations but you should first understand the associated concepts explained below.

### Queryable

[Queryable](@ref ecstasy::query::Queryable) is a ecstasy concept (in the meaning of c++20 template concepts) on which the ecstasy [query](@ref ecstasy::query::Query) can operate.

They must:

- Define the type of the queried value
- Allow to check whether the value is existing at a given index (entity)
- Allow to fetch the value at a given index (entity)

The easiest example are the component storages:

- The queried value type is the component
- They provide a way to test if an entity have the component
- They allows to access it.

But it is also true for the [Entities](@ref ecstasy::Entities) resource.
The goal of the queryable is to allow performing queries on various data types, not limiting them to only Storages but any type matching the concept.

Concept definition:

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

References: [Queryable](@ref ecstasy::query::Queryable)

### Batch Query

A batch query is a multi threaded query. Instead of iterating over the matching entities yourself in the current thread you can use the splitThreads method. It split the iteration over multiple threads of a custom batch size.

In the following example, if you have 167 entities with a position and velocity components, it will starts 4 threads (4 batches) and execute the function given in parameter:

- Thread 1: 1-50
- Thread 2: 51-100
- Thread 3: 101-150
- Thread 4: 151-167

```cpp
// Will make one thread for every 50 matching entities
registry.query<Position, Velocity>().splitThreads(50, [](auto components) {
    auto [position, velocity] = components;
    position.x += velocity.x;
    position.y += velocity.y;
});
```

### Modifier

[Modifiers](@ref ecstasy::query::Modifier) is another ecstasy concept allowing to perform more complex queries in the registry.
They must also match the [queryable](@ref ecstasy::query::Queryable) concept but they are in fact queryable wrappers.

If you are familiar with the decorators concepts in Python, Java, or any other language you can see the modifiers as a Queryable decorator:

- You can set a modifier on any queryable, like you can apply a decorator on any function
- A modifier is also a queryable, like decorators are the original function with some modifications
- Therefore you can chain the modifiers, like you can apply decorators over decorators and so on.

@warning
There is in fact one big difference when using modifiers: They change the queried value type and you must look at their documentation to knows what is the modified type.

Here is the list of the implemented modifiers with their return data type. Note that nothing prevent you to implement other modifiers on your personal projects or by submitting an issue to ecstasy.

- [Maybe<T>](@ref ecstasy::query::modifier::Maybe) -> [std::optional<T>](@ref std::optional)
- [Not<T>](@ref ecstasy::query::modifier::Not) /!\ Do not fetch /!\
- [Or\<T1, T2...\>](@ref ecstasy::query::modifier::Or) -> [std::tuple](@ref std::tuple)<[std::optional](@ref std::optional)\<T1\>, [std::optional](@ref std::optional)\<T2\>...>
- [Xor\<T1, T2...\>](@ref ecstasy::query::modifier::Xor) -> [std::tuple](@ref std::tuple)<[std::optional](@ref std::optional)\<T1\>, [std::optional](@ref std::optional)\<T2\>...>
- [And\<T1, T2...\>](@ref ecstasy::query::modifier::And) -> [std::tuple](@ref std::tuple)\<T1, T2...\>

As always, I think examples are more readable:

```cpp
// Match all entities with: Position and Velocity
// Returns: position and velocity components. If existing, the density component too (using std::optional).
registry.query<Position, Velocity, Maybe<Density>>()


// Match all entities with: Position or Positions
// Returns: a tuple of 2 std::optional, one for each component. At least one of them will be defined.
registry.query<Or<Position, Positions>>()

// Warning: this doesn't make sense, just to illustrate nested modifiers
// Match all entities with: Position or maybe Positions
// Returns: a tuple of 2 std::optional, one for each component. At least one of them will be defined.
registry.query<Or<Position, Maybe<Positions>>>()
```

Follow [this tutorial](#ModifiersTutorial) to learn how to use them in details.

Concept definition:

```cpp
template <typename M>
concept Modifier = requires(M &modifier)
{
    /// Data stored in the queryable.
    requires Queryable<M>;

    /// Modifiers must inherit from the ecstasy::query::modifier::Modifier .
    requires std::derived_from<M, ecstasy::query::modifier::Modifier>;

    /// Modifier operands
    typename M::Operands;
};
```

References: [Modifier](@ref ecstasy::query::Modifier), [Maybe](@ref ecstasy::query::modifier::Maybe) , [Not](@ref ecstasy::query::modifier::Not), [Or](@ref ecstasy::query::modifier::Or), [Xor](@ref ecstasy::query::modifier::Xor), [And](@ref ecstasy::query::modifier::And)

### Condition

[Condition](@ref ecstasy::query::QCondition) is (yet) another ecstasy concept allowing to perform conditions on the fetched data instead of on the presence of data. They are not queryables and are evaluated when iterating on the query.

When you are using [queryables](@ref ecstasy::query::Queryable) or [modifiers](@ref ecstasy::query::Modifier) the query performs verification on the presence of the component and not on the component state. The conditions however operates on the component state.
This is just syntactic sugar to remove one small line in your code.

@warning
The condition cannot works if the evaluated type is not in the query queryables.

Ecstasy provides the following conditions:

- [EqualTo =](@ref ecstasy::EqualTo)
- [NotEqualTo !=](@ref ecstasy::NotEqualTo)
- [Greater >](@ref ecstasy::Greater)
- [GreaterEqual >=](@ref ecstasy::GreaterEqual)
- [Less <](@ref ecstasy::Less)
- [LessEqual <=](@ref ecstasy::LessEqual)

And here is a little example where the two loops does the same thing:

```cpp
for (auto [life] : registry.query<Life>()) {
    if (life.value != 0)
        continue;
    // Entity deletion code
}

for (auto [life] : registry.select<Life>().where<ecstasy::EqualTo<&Life::value, 0>>()) {
    // Entity deletion code
}
```

Follow [this tutorial](#ConditionsTutorial) to learn how to use them in details.

References: [QCondition](@ref ecstasy::query::QCondition), [EqualTo](@ref ecstasy::EqualTo), [NotEqualTo](@ref ecstasy::NotEqualTo), [Greater](@ref ecstasy::Greater), [GreaterEqual](@ref ecstasy::GreaterEqual), [Less](@ref ecstasy::Less), [LessEqual](@ref ecstasy::LessEqual)
