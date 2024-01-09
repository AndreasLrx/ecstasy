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
        for (auto [position, velocity] : registry.query<Position, Velocity>()) {
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
        auto builder = registry.entityBuilder().with<Position>(i * 2, i * 10);
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

To create a custom storage, you need to validate the @ref IsStorage concept. In addition you also need to implement the contains and emplace methods (see MapStorage).

## Making registry queries {#QueryTutorial}

When you populated your registry with entities and resources you can query them with the... @ref Registry::query method.
It will iterate on every entities having the requested components and return a reference of them.

The easiest syntax is to use tuple unpacking in a for loop:

```cpp
for (auto [position, velocity] : registry.query<Position, Velocity>()) {
    position.x += velocity.x;
    position.y += velocity.y;
}
```

You can also parallelized your query in multiple threads using the splitThreads query method:

```cpp
// Will make one thread for every 50 matching entities
registry.query<Position, Velocity>().splitThreads(50, [](auto components) {
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
for (auto [position, velocity] : registry.select<Position, Velocity>().where<Dynamic>()) {
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
   for (auto [position, velocity, mDensity] : registry.query<Position, Velocity, Maybe<Density>>()) {
       float multiplier = 1;
       // Density is a std::optional<std::reference_wrappper<Density&>>
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
   for (auto [position, velocity] : registry.select<Position, Velocity>().where<Not<Static>>()) {
       position.x += velocity.x;
       position.y += velocity.y;
   }
   ```

3. @ref ecstasy::Or / @ref ecstasy::XOr

   Performs a `Or`/`XOr` between the given components. Returned values are of type std::tuple<std::optional<Q1>, std::optional<Q2>...>.
   It works for at least two components but can take more than 2.
   I really don't know real use case of this but well that was fun to code.
   For an easier access on the tuple you could leave the Or in the where clause and add Maybe selects in the Select:

   ```cpp
   // Assuming positions is a std::vector<Position>
   for (auto [position, positions, velocity] : registry.select<Maybe<Position>, Maybe<Positions>, Velocity>().where<Or<Position, Positions>>()) {
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
       .select<Maybe<Position2D>, Maybe<Position3D>, Maybe<Velocity2D>, Maybe<Velocity3>>()
       .where<Or<And<Position2D, Velocity2D>, And<Position3D, Velocity3D>>>()) {
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
   for (auto [life] : registry.select<Life>().where<ecstasy::EqualTo<&Life::value, 0>>()) {
       // Condition equals: if (!(life.value == 0)) continue;
   }
   ```

2. @ref ecstasy::NotEqualTo

   Ensure the two operands are not equal for each matching entities.

   ```cpp
   for (auto [life] : registry.select<Life>().where<ecstasy::NotEqualTo<&Life::value, 0>>()) {
       // Condition equals: if (!(life.value != 0)) continue;
   }
   ```

3. @ref ecstasy::Less

   Ensure the first operand is strictly lower than the second operand.

   ```cpp
   for (auto [life] : registry.select<Life>().where<Shield, ecstasy::Less<&Life::value, &Shield::getValue>>()) {
       // Condition equals: if (!(life.value < shield.getValue())) continue;
   }
   ```

4. @ref ecstasy::LessEqual

   Ensure the first operand is lower than or equal to the second operand.

   ```cpp
   for (auto [life] : registry.select<Life>().where<Shield, ecstasy::LessEqual<&Life::value, &Shield::getValue>>()) {
       // Condition equals: if (!(life.value <= shield.getValue())) continue;
   }
   ```

5. @ref ecstasy::Greater

   Ensure the first operand is strictly greater than the second operand.

   ```cpp
   for (auto [life] : registry.select<Life>().where<Shield, ecstasy::Greater<&Life::value, &Shield::getValue>>()) {
       // Condition equals: if (!(life.value > shield.getValue())) continue;
   }
   ```

6. @ref ecstasy::GreaterEqual

   Ensure the first operand is greater than or equal to the second operand.

   ```cpp
   for (auto [life] : registry.select<Life>().where<Shield, ecstasy::GreaterEqual<&Life::value, &Shield::getValue>>()) {
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
        for (auto [position, velocity] : registry.query<Position, Velocity>()) {
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
