# Tutorial

@note
The tutorials on this page will help you to use ecstasy, not to understands the concepts and terms. I strongly advise you to look at the [glossary](@ref Glossary.md) before following any tutorials (except the getting started).

## Getting Started {#GettingStarted}

Here is a basic example of how to use the ECS.
The usual order when using ecstasy is the following:

- Define the components and systems (here Position, Velocity and Movement)
- Create the [Registry](@ref ecstasy::Registry)
- Register the [Systems](@ref ecstasy::ISystem)
- Populate the register with [Entities](@ref ecstasy::Entity)
- Run the registry systems with [runSystems](@ref ecstasy::Registry::runSystems)

In this example we create 10 entities. Every entities have a position component and only even entities have a velocity component.
The Movement system will iterate on every entities having a position **and** a velocity component and add the velocity vector to the position. (Know this is not a perfect approach because the velocity should depend on the elapsed time)
Finally we run the systems over and over.

```cpp
#include <ecstasy/registry/Registry.hpp>
#include <ecstasy/system/ISystem.hpp>
#include <ecstasy/storages/MapStorage.hpp>

struct Position {
    float x;
    float y;
};

struct Velocity {
    float x;
    float y;
};

struct Movement : public ecstasy::ISystem {
    void run(ecstasy::Registry &registry) override final
    {
        for (auto [position, velocity] : registry.query<Position, const Velocity>()) {
            position.x += velocity.x;
            position.y += velocity.y;
        }
    }
};


int main() {
    // Declare your registry
    ecstasy::Registry registry;

    // Register your systems
    registry.addSystem<Movement>();

    // Populate the registry with some entities
    for (int i = 0; i < 10; i++) {
        auto builder = registry.entityBuilder();
        builder.with<Position>(i * 2, i * 10);
        if (i % 2 == 0)
            builder.with<Velocity>(i * 10, i * 2);
        ecstasy::Entity entity = builder.build();
        // If needed, use the entity
    }

    while (true) {
        registry.runSystems();
    }
}
```

## Using entities

### Creating entities

There is 3 ways to create new entities in the registry:

- Using the [registry](@ref ecstasy::Registry) [builder](@ref ecstasy::Registry::EntityBuilder)
- Using the [entities](@ref ecstasy::Entities) [builder](@ref ecstasy::Entities::Builder)
- Using the [entities](@ref ecstasy::Entities) [create()](@ref ecstasy::Entities::create) method

The builders allows to create entities with components easily whereas the create() method is for internal purposes.
They have the same syntax except one difference: the registry builder takes components as template parameters, and fetch the associated storages whereas the entities builder takes storages directly.

The builders have two methods:

- with(): Adds a new component to the entities (supports constructor parameter or explicit [std::initializer_list](@ref std::initializer_list))
- build(): Finalize the entity creation

@note
The with() method returns the builder, allowing to chain the calls.

@note
Since the [entities builder](@ref ecstasy::Entities::Builder) [with()](@ref ecstasy::Entities::Builder::with) method takes the storage in parameters, it can deduce the storage/component type. You don't have to use the template parameters.

@warning
You should never try to use persistent pointers/reference to components. A component memory address is unstable and can change due to its storage. For example adding a new element to the storage may make the storage resize and **move** its components.
Therefore you should be very carefull when using **this** inside a component constructor for example.
In case you really need an identifier, use the entity ID.

Here is an example using the two builders:

```cpp
ecstasy::Registry registry;
ecstasy::Entities &entities = registry.getEntities();

// Registry builder
ecstasy::Entity e1 = registry.entityBuilder().with<Position>(2, 10)
                        .with<Velocity>(10, 2)
                        .build();

// Entities builder
ecstasy::Entity e2 = entities.builder().with(registry.getStorage<Position>(), 2, 10)
                        .with(registry.getStorage<Velocity>(), 10, 2)
                        .build();
```

#### Using type resolution

You can add Component storages, Resources, Systems, or even the Registry instance to the `with` template parameters.
This will fetch them from the registry and forward them to the component constructor.

```cpp
struct NeedStorage {
    const ecstasy::MapStorage<Position> &storage;

    NeedStorage(const ecstasy::MapStorage<Position> &aStorage) : storage(aStorage)
    {
    }
};

ecstasy::Registry registry;
ecstasy::Entity e1 = registry.entityBuilder()
                        .with<NeedStorage, Position>() // No need to send registry.getStorage<Position>() as parameters
                        .build();
```

### Manage entity component

When you have an instance of an [Entity](@ref ecstasy::Entity) you can check the presence of a component, get it or add one. Always by sending the component storage. However, you can also use the [RegistryEntity](@ref ecstasy::RegistryEntity) class which, as its name says, is linked to a registry. It is only syntactic sugar to avoid fetching yourself storages from the registry.

Examples:

```cpp
ecstasy::Registry registry;
// Populate the registry...

// Entity class
ecstasy::Entity entity = registry.getEntity(0); // Get your entity
auto &positionStorage = registry.getStorage<Position>();
entity.has(storage); // check if the entity has the component Position
entity.add(storage, 5, 12) // add a new component Position(5, 12)
auto &pos = entity[storage]; // Get the position component
pos = entity.get(storage); // Same as above

// RegistryEntity class
ecstasy::RegistryEntity entity = ecstasy::RegistryEntity(entity, registry); // Link your entity to the registry
entity.has<Position>(); // check if the entity has the component Position
entity.add<Position>(5, 12) // add a new component Position(5, 12)
pos = entity.get<Position>(); // Same as above
```

### Delete entities

First I need to define two words used in the entities context

- kill: mark an entity for deletion without deleting it, until a call to [Entities.maintain()](@ref ecstasy::Entities::maintain)
- erase: delete the entity instantly, and its components if any (and accessible)

@note
You should never erase entities when iterating on them. Kill them instead.

If you don't want to wory about killing inside the loop and calling the maintain after you can use the simple [DeletionStack](@ref ecstasy::DeletionStack) helper. You can push entities to delete in it and they will be erased when the deletion stack is destroyed.

Examples:

```cpp
ecstasy::Registry registry;
auto &entities = registry.getEntities();

// Using kill/maintain
for (auto [entity] : registry.query<ecstasy::Entities>()) {
    entities.kill(entity);
}
entities.maintain();

// Deletion stack
{
    ecstasy::DeletionStack delStack(registry);

    for (auto [entity] : registry.query<ecstasy::Entities>()) {
        delStack.push(entity);
    }
} // delStack is out of scope -> destroyed, and entities are erased
```

## Implement a custom storage {#CustomStorageTutorial}

You can specify which storage you want a component to use with the define @ref SET_COMPONENT_STORAGE. By default the @ref ecstasy::MapStorage is used for all components but you may implement a more suitable storage based on your component.

To create a custom storage, you need to validate the @ref ecstasy::IsStorage concept. In addition you also need to implement the contains and emplace methods (see MapStorage).

## Making registry queries {#QueryTutorial}

When you populated your registry with entities and resources you can query them with the... @ref ecstasy::Registry::query method.
It will iterate on every entities having the requested components and return a reference of them.

The easiest syntax is to use tuple unpacking in a for loop:

@note
You should get used to querying your types as const if you don't modify them because of [thread safety](#EnsuringThreadSafety)

```cpp
for (auto [position, velocity] : registry.query<Position, const Velocity>()) {
    position.x += velocity.x;
    position.y += velocity.y;
}
```

You can also parallelized your query in multiple threads using the splitThreads query method:

```cpp
// Will make one thread for every 50 matching entities
registry.query<Position, const Velocity>().splitThreads(50, [](auto components) {
    auto [position, velocity] = components;
    position.x += velocity.x;
    position.y += velocity.y;
});
```

@note
Batch queriess (multi threaded) are not always better than single threaded queries. The more complex the task is for each entity, the more you may improve performances with batch queries.

You can request as many component as you want in the template parameters of the query method.

If you need more complex queries, check the following parts.

### Select ... Where ... {#SelectWhereTutorial}

Sometimes you need to check whether an entity has a given component but you don't need to access it.
Let's say you have a `Dynamic` component required to move your entities but it is only a marker, it stores no data.

You can use the Select ... Where ... syntax:

```cpp
for (auto [position, velocity] : registry.select<Position, const Velocity>().where<const Dynamic>()) {
    position.x += velocity.x;
    position.y += velocity.y;
}
```

This syntax retrieve only the components in the `Select` clause but it ensures all components in the `Select` **and** `Where` clauses are present.

@note
You can have the same component in the select and the where clauses. By default all components in Select not present in Where clause are implicitly appended to the where clause.

### Using modifiers {#ModifiersTutorial}

Even more complex queries, not sure everything here will be usefull but anyway it is implemented !
By default, all components in the query are joined together with a **and** operator: component A **and** component B **and** component C must be present.
To modify this behavior you can use `modifiers`, at the moment the modifiers implemented are mostly boolean operators.

@note
Modifiers can be nested and used in simple queries as well as in Select ... Where ...

1. @ref ecstasy::Maybe

   The first and important modifier is the @ref ecstasy::Maybe operator. It match on any entity and instead of returning a reference to the component it will returns a @ref std::optional containing (or not) the component.
   Here comes an example:

   ```cpp
   for (auto [position, velocity, mDensity] : registry.query<Position, const Velocity, Maybe<const Density>>()) {
       float multiplier = 1;
       // Density is a std::optional<std::reference_wrappper<const Density&>>
       if (mDensity)
           multiplier = mDensity->value;
       position.x += velocity.x * multiplier;
       position.y += velocity.y * multiplier;
   }
   ```

2. @ref ecstasy::Not

   This one is kinda explicit. We will use a reverse example of the previous `Dynamic` marker component:

   @warning
   Don't use in `Select` clause because it doesn't make sense to select non existing data.

   ```cpp
   for (auto [position, velocity] : registry.select<Position, const Velocity>().where<Not<const Static>>()) {
       position.x += velocity.x;
       position.y += velocity.y;
   }
   ```

3. @ref ecstasy::Or / @ref ecstasy::Xor

   Performs a `Or`/`XOr` between the given components. Returned values are of type std::tuple<std::optional<Q1>, std::optional<Q2>...>.
   It works for at least two components but can take more than 2.
   I really don't know real use case of this but well that was fun to code.
   For an easier access on the tuple you could leave the Or in the where clause and add Maybe selects in the Select:

   ```cpp
   // Assuming positions is a std::vector<Position>
   for (auto [position, positions, velocity] : registry.select<Maybe<Position>, Maybe<Positions>, const Velocity>().where<Or<Position, Positions>>()) {
       if (position) {
           position.x += velocity.x;
           position.y += velocity.y;
       } else {
           for (auto pos in positions.value()) {
               pos.x += velocity.x;
               pos.y += velocity.y;
           }
       }
   }
   ```

4. @ref ecstasy::And

   Same as Or but performs an And. Meaning the returned values are not optional. It looks useless alone because it is the default behavior.
   But If you are creative you can do something fun with nested modifier... or just ignore that functionnality until you need it (if this times comes)

   ```cpp
   // Assuming working with 2D or 3D (yes this is dumb you should do 2 systems)
   for (auto [position2, position3, velocity2, velocity3] : registry
       .select<Maybe<Position2D>, Maybe<Position3D>, Maybe<const Velocity2D>, Maybe<const Velocity3>>()
       .where<Or<And<Position2D, const Velocity2D>, And<Position3D, const Velocity3D>>>()) {
       if (position2) {
           position2.x += velocity2.x;
           position2.y += velocity2.y;
       } else {
           position3.x += velocity3.x;
           position3.y += velocity3.y;
           position3.z += velocity3.z;
       }
   }
   ```

### Using conditions {#ConditionsTutorial}

In addition to the different modifiers, you can add runtime conditions to your query.
Basic comparisons are implemented but you can add custom ones.

Conditions supports comparisons with:

- Constant values: `0`
- Class member: `&Life::value`
- Class getter: `&Life::getValue`

1. @ref ecstasy::EqualTo

   Ensure the two operands are equal for each matching entities.

   ```cpp
   for (auto [life] : registry.select<const Life>().where<ecstasy::EqualTo<&Life::value, 0>>()) {
       // Condition equals: if (!(life.value == 0)) continue;
   }
   ```

2. @ref ecstasy::NotEqualTo

   Ensure the two operands are not equal for each matching entities.

   ```cpp
   for (auto [life] : registry.select<const Life>().where<ecstasy::NotEqualTo<&Life::value, 0>>()) {
       // Condition equals: if (!(life.value != 0)) continue;
   }
   ```

3. @ref ecstasy::Less

   Ensure the first operand is strictly lower than the second operand.

   ```cpp
   for (auto [life] : registry.select<const Life>().where<Shield, ecstasy::Less<&Life::value, &Shield::getValue>>()) {
       // Condition equals: if (!(life.value < shield.getValue())) continue;
   }
   ```

4. @ref ecstasy::LessEqual

   Ensure the first operand is lower than or equal to the second operand.

   ```cpp
   for (auto [life] : registry.select<const Life>().where<Shield, ecstasy::LessEqual<&Life::value, &Shield::getValue>>()) {
       // Condition equals: if (!(life.value <= shield.getValue())) continue;
   }
   ```

5. @ref ecstasy::Greater

   Ensure the first operand is strictly greater than the second operand.

   ```cpp
   for (auto [life] : registry.select<const Life>().where<Shield, ecstasy::Greater<&Life::value, &Shield::getValue>>()) {
       // Condition equals: if (!(life.value > shield.getValue())) continue;
   }
   ```

6. @ref ecstasy::GreaterEqual

   Ensure the first operand is greater than or equal to the second operand.

   ```cpp
   for (auto [life] : registry.select<const Life>().where<Shield, ecstasy::GreaterEqual<&Life::value, &Shield::getValue>>()) {
       // Condition equals: if (!(life.value >= shield.getValue())) continue;
   }
   ```

## Using systems

To create systems, you only have to create a new class inheriting the @ref ecstasy::ISystem class and implement the @ref ecstasy::ISystem::run method.

For example here is how you would create a Movement system:

```cpp
struct Movement : public ecstasy::ISystem {
    void run(ecstasy::Registry &registry) override final
    {
        for (auto [position, velocity] : registry.query<Position, const Velocity>()) {
            position.x += velocity.x;
            position.y += velocity.y;
        }
    }
};

// Add it to the registry
ecstasy::Registry registry;

registry.addSystem<Movement>();

registry.runSystem<Movement>();
// or
registry.runSystems();
```

By default systems added with `Registry.addSystem()` have the same priority and the order of execution is undefined when calling `Registry.runSystems()`.
But you can use the system priorities/groups to order them and be able to specify their priority and run a given system group.

1. Priority

   When adding a system to the registry you can add a priority to each system as a second template parameter.
   Calling `runSystems()` will run the systems in ascending priority order.

   @warning
   You can set the same priority for multiple systems but it doesn't ensure they will always run in the same order.

   ```cpp
       ecstasy::Registry registry;

       registry.addSystem<A, 1>();
       registry.addSystem<B, 2>();
       registry.addSystem<C, 3>();
       registry.addSystem<D, 4>();
       registry.addSystem<E, 5>();
       registry.addSystem<F, 6>();
       registry.runSystems(); // ABCDEF
       // If you want you can still call systems one by one
       registry.runSystem<A>(); // A
   ```

2. Group and masks

   In fact their is nothing more than the priority value. But using bitmask you can create groups.

   The following example use 8 bits for the group id and 8 for the system priority.

   ```cpp
       ecstasy::Registry registry;
       // Mask of the group part on the priority (bits 8->15)
       size_t mask = 0xff00;
       // Groups 'identifiers'
       const size_t abc = 0x0100;
       const size_t def = 0x0200;

       // Group 'abc'
       registry.addSystem<A, abc + 1>();
       registry.addSystem<B, abc + 2>();
       registry.addSystem<C, abc + 3>();
       // Group 'def'
       registry.addSystem<D, def + 1>();
       registry.addSystem<E, def + 2>();
       registry.addSystem<F, def + 3>();

       // The following still calls the systems in the right order because their priorities are still ascending
       registry.runSystems(); // ABCDEF

       // You can run a system group by sending the group id and the mask
       registry.runSystems(abc, mask); // ABC
       registry.runSystems(def, mask); // DEF
   ```

## Ensuring Thread Safety {#EnsuringThreadSafety}

By default ecstasy is not thread safe. By not thread safe I mean there is absolutely nothing enabled to prevent your threads to do whatever they want at the same time.

But no worries it is just some compilation options to set:

- @b ECSTASY_LOCKABLE_RESOURCES will make @ref Resource class validate the [Lockable](@ref ecstasy::thread::Lockable) concept.
- @b ECSTASY_LOCKABLE_STORAGES will make @ref IStorage class validate the [Lockable](@ref ecstasy::thread::Lockable) concept.
- @b ECSTASY_AUTO_LOCK will lock any [Lockable](@ref ecstasy::thread::Lockable) queryables in any registry queries or registry modifiers.
- @b ECSTASY_THREAD_SAFE will enable the three options above.

This being done every lockables will be locked (if @b ECSTASY_AUTO_LOCK is set) when performing queries from the registry:

- If they are queried const qualified they will be shared lock, meaning multiple threads can access the queryables at the same time in a read only way.
- However if they are not const qualified the lock will be exclusive, waiting any thread reading (or writing) the queryable and then blocking any other thread to access it until unlocked.

This allows a generic thread safety while reducing thread contention as much as possible.

If you want to handle thread safety yourself you still have multiple options:

1. Get it your own way

   Just disable the above options and do what you want, but outside the queries because of encapsulation you will not have the same generic access.

2. Lock explicitly

   Keep the resources/storages as lockable and use the @ref Registry::queryEx (and Select.whereEx) methods or @ref AndEx, @ref OrEx, @ref NotEx modifiers where you can explicitly specify the AutoLock value.

3. AutoLock by default but disable when you know what you're doing

   I know sometimes ecstasy can lock multiple times the same mutex because it is hard to detect. But if you know it and it is sensitive systems (running lot of times per frame) you can use the \*Ex methods to UnLock explicitly the same way.

## Serializing your entities

Ecstasy has some built in serialization helpers in the @ref ecstasy::serialization namespace.

@warning
The entity serialization without specifying components is still WIP and will probably have a huge refactor.

The current available serializers are the following:

- RawSerializer: Custom binary serialization of components fields. Compact form but not related to any RFC.

If you need a missing serializer, you can write it yourself by inheriting the @ref ecstasy::serialization::Serializer class.
In case of a commonly used serializer type (json/yaml/NDR...) feel free to open an issue about it.

@warning
The RawSerializer uses a @ref std::stringstream. Therefore there are cursor positions, if you need to read twice the data, you need to reset the read cursor position.

### Using a serializer

Following examples below will be using the RawSerializer type and the example component Position:

```cpp
struct Position {
  public:
    float x;
    float y;
};
```

#### Common types

Serializer should support fundamental and container types by default but it depends on the serializer implementation.

| Serializer                                  | Fundamental (integers, floats, doubles...) | strings | containers |
| ------------------------------------------- | ------------------------------------------ | ------- | ---------- |
| @ref ecstasy::serialization::RawSerializer  | Ok                                         | OK      | Ok         |
| @ref ecstasy::serialization::JsonSerializer | Ok                                         | OK      | Ok         |

**1. Saving**

To save an object in a serializer, you can either call the method **save** or use the insertion operator **<<**

```cpp
RawSerializer serializer();

// Either with method save
serializer.save(42);
// Or with insertion operator
serializer << 42;
```

**2. Exporting/Importing**

You can export your serialized data using the _export\*_ methods.
And import them back with _import\*_ methods.

```cpp
RawSerializer serializer();

// Save all your variables...

// Export to a string
std::string serialized = serializer.exportBytes();
// Or to a file
serializer.exportFile("mydata.bin");
// Or to a stream
serializer.exportStream(std::cout);

/// Import back our data in the constructor
RawSerializer importSerializer(serialized);

// Or import from bytes on existing serializer
importSerializer.importBytes(serialized);
// Or from a file
importSerializer.importFile("mydata.bin");
// Or from a stream
importSerializer.importStream(std::cin);
```

**3. Updating**

To update an object from a serializer, you can either call the method **update** or use the extraction operator **>>**

```cpp
RawSerializer serializer(std::filesystem::path("mydata.bin"));

int my_number = 0;

// Either with method update
serializer.update(my_number);
// Or with extraction operator
serializer >> my_number;
```

**4. Loading**

To load an object from a serializer, you need to call the load template method.

@note
If there is no load method implemented for the type but the type is default constructible, it will be default constructed then updated.

```cpp
RawSerializer serializer(std::filesystem::path("mydata.bin"));

// Fill your serializer with previously saved data using import methods

int my_number = serializer.load<int>();
```

#### Custom types

If you are working with custom type (ie components) you can implement the extraction (**>>**) and insertion operator (**<<**) with the said serializer type for the _save_/_update_. And a constructor taking the serializer as parameter for the _load_.

@note
The operators doesn't have to be inside the class, you can define it outside the class if you don't have access to the class definition.
However it cannot work for the constructor.

```cpp
// Custom constructor for load method
Position(RawSerializer &serializer) : x(serializer.load<float>()), y(serializer.load<float>())
{
}

// Extraction operator for save
RawSerializer &operator>>(RawSerializer &serializer) const
{
    return serializer << x << y
}

// Insertion operator for update
Position &operator<<(RawSerializer &serializer)
{
    serializer >> x >> y;
    return *this;
}
```

If you need to (de)serialize the type from the save/update/loadEntity methods, you first need to register the type as serializable by the expected Serializer using the variadic macro @ref REGISTER_SERIALIZABLES

For example, if you want you type Position to be serializable by the RawSerializer and the (maybe to come) JsonSerializer:

```cpp
REGISTER_SERIALIZABLES(Position, RawSerializer, JsonSerializer)
```

#### Working with Entities

Since you can serialize any type, you can serialize entity components manually using the functions above.

You can save entity components explicitly using the templated saveEntity method, or every registered components with the classic saveEntity method.

@warning
To use the non templated `saveEntity` method, you need to register them using the @ref REGISTER_SERIALIZABLES macro (see below).

```cpp
RawSerializer serializer();

ecstasy::RegistryEntity entity(
        registry.entityBuilder().with<Position>(1.0f, -8456.0f).with<NPC>(Position(42.f, 0.f), "Steve").build(),
        registry);

// Save an entity explicit components
rawSerializer.saveEntity<NPC, Position>(entity);
// Or save the whole entity
rawSerializer.saveEntity(entity);

// Update an existing entity
rawSerializer.updateEntity(entity);

// Or create it entirely
ecstasy::RegistryEntity newEntity = rawSerializer.loadEntity(registry);
```
