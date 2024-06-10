# User Actions integration

@tableofcontents

## Description

The [user actions integration](@ref ecstasy::integration::user_action) allows to map events to an action.
Actions can be mapped to keyboard, mouse or gamepad, making the handle of different inputs easier since you don't have to deal with the different event types between keyboard/gamepad.
It also helps dealing with local multiplayer as an action can be linked to a specific user.
Finally it has included toml serialization functions to save/load the user keybinds.

If you don't really understand what is an Action here is an example:
You play a 2D plateformer game where you can _move left_, _move right_ and _jump_, these are your 3 actions.
If you have local multiplayer you will need these actions for **each player**, that's why "user actions", action binding are attached to a specific player.

@warning
This integration requires the event integration

## Usage

We'll see how to implement listeners for the example of a 2D platformer.
Move actions will be configured with arrows and ZQSD on keyboard, and left axis and left/right triggers on gamepad.

### Register your actions and bindings

An action is simply and identifier.
An action binding is a link between an action identifier, a user identifier and an event (multiple bindings can be linked to the same action/user id).

The easier to define your action identifiers is simply to use an enum.

```cpp
enum class Actions : size_t { MoveLeft, MoveRight, MoveHorizontal, Jump, Count };
```

Now to create the bindings follow these steps:

- Add a [Users](@ref ecstasy::integrations::user_action::Users) resource to the registry
- Add your [Action bindings](@ref ecstasy::integrations::user_action::ActionBinding) to your users
- Notify the user resources bindings have changed

```cpp
using eua = ecstasy::integration::user_action;
using event = ecstasy::integration::event;

void setupActions(ecstasy::Registry &registry)
{
   // We need this resource to store the action bindings and handle the various users profiles
   eua::Users &users = registry.addResource<eua::Users>();

   // Get a reference to the bindings vector of the first user (getUserProfile has a parameter to set the user ID, default to 0)
   // I admit this is a little verbose
   auto &binds = users.getUserProfile().getActionBindings().getBindings();
   // Run backwards !
   binds.emplace_back(Actions::MoveLeft, event::Keyboard::Key::Q);
   binds.emplace_back(Actions::MoveLeft, event::Keyboard::Key::Left);
   binds.emplace_back(Actions::MoveLeft, event::Gamepad::Axis::TriggerLeft);
   // Run forward !
   binds.emplace_back(Actions::MoveRight, event::Keyboard::Key::D);
   binds.emplace_back(Actions::MoveRight, event::Keyboard::Key::Right);
   binds.emplace_back(Actions::MoveRight, event::Gamepad::Axis::TriggerRight);
   // RUN, need a specific action because gamepad axis are from range -1 to 1
   binds.emplace_back(Actions::MoveHorizontal, event::Gamepad::Axis::LeftX);
   // Jump !
   binds.emplace_back(Actions::Jump, event::Keyboard::Key::Space);
   binds.emplace_back(Actions::Jump, event::Gamepad::Button::FaceDown);

   // This is important to notice the users the internal bindings vector has changed
   users.updateBindings();
}
```

### Listen to your actions

There are 3 component types to have action listeners:

- [ActionListener](@ref ecstasy::integration::user_action::ActionListener): Single action listener, easy to create but limited to a single function since entity can't have multiple instances of the same component.
- [ActionListeners](@ref ecstasy::integration::user_action::ActionListeners): Map between Action id <-> Action Listener, verbose to instantiate but allows to listen to every action type in different callbacks
- [ActionIdListener](@ref ecstasy::integration::user_action::ActionIdListener): Templated listener to take advantage of the ecs query system, requires another resource/system to works

@note
They can be used together, either on different entities or on the same entity.

@note
Listeners can be targeted toward an action type (identifier), if you want to match all actions you can use the special identifier [Actions::All](@ref ecstasy::integration::user_action::Action::All)

#### ActionListener

[ActionListener](@ref ecstasy::integration::user_action::ActionListener) are the easiest listeners, but you can attach only one of them to each entities.

```cpp
using eua = ecstasy::integration::user_action;

void setupListeners(ecstasy::Registry &registry)
{
   // This entity will listen to every action change
   registry.entityBuilder()
      .with<eua::ActionListener>([](ecstasy::Registry &r, ecstasy::Entity e, eua::Action a) {
         std::cout<<"Action "<<a.id<<" has value "<<a.value<<std::endl;
         }
      )
      .build();
   // Or only to jump
   registry.entityBuilder()
      .with<eua::ActionListener>([](ecstasy::Registry &r, ecstasy::Entity e, eua::Action a) {
         std::cout<<"Jump action ("<<a.id<<") has value "<<a.value<<std::endl;
         },
         Actions::Jump
      )
      .build();
}

```

#### ActionListeners

[ActionListeners](@ref ecstasy::integration::user_action::ActionListeners) allows to handle all actions independently in a single component, but it is quite verbose to instantiate.

@warning
Using Action::All does not work with this component

```cpp
using eua = ecstasy::integration::user_action;

void setupListeners(ecstasy::Registry &registry)
{
   // This entity will listen to every action change, and have also special behaviors for the different actions
   // The internal map has eua::Action::Id as keys (size_t) and enum cannot be converted implicitly so casts are required.
   registry.entityBuilder()
      .with<eua::ActionListeners>({
         {static_cast<size_t>(Actions::MoveLeft), [&values](ecstasy::Registry &r, ecstasy::Entity e, eua::Action a) {
            std::cout<<"Move to the left at speed "<<a.value<<std::endl;
            }
         },
         {static_cast<size_t>(Actions::MoveRight), [&values](ecstasy::Registry &r, ecstasy::Entity e, eua::Action a) {
            std::cout<<"Move to the right at speed "<<a.value<<std::endl;
            }
         },
         {static_cast<size_t>(Actions::Jump), [&values](ecstasy::Registry &r, ecstasy::Entity e, eua::Action a) {
            std::cout<<"Jump! "<<a.value<<std::endl;
            }
         }
      })
      .build();
}

```

#### ActionIdListener

[ActionIdListener](@ref ecstasy::integration::user_action::ActionIdListener) are templated listeners bound to a specific action.
It is mostly the same as basic ActionListener, but since they are templated you can have multiple instances on the same entity.

To make them works you need to add a resource and a system first:

- [PendingActions Resource](@ref ecstasy::integration::user_action::PendingActions): A queue to store the detected actions not yet distributed to listeners
- [PollActions System](@ref ecstasy::integration::user_action::PollActions): Poll the actions from the queue and dispatch them to the templated ActionIdListener components

@note
You should run the PollActions system after the events have been detected. If you are using the SFML integration it means after the PollEvents system.

```cpp

using eua = ecstasy::integration::user_action;

void setupListeners(ecstasy::Registry &registry)
{
   // Add the actions queue
   registry.addResource<eua::PendingActions>();
   // Add the actions polling system
   // It takes as template parameters an integer sequence where each entry is supposed to match an action id, and therefore an ActionIdListener storage
   registry.addSystem<eua::PollActions<std::make_index_sequence<static_cast<size_t>(Actions::Count)>>>();


   // This entity will listen to every action change, and have also special behaviors for the different actions
   // The templated type expects eua::Action::Id (size_t) and enum cannot be converted implicitly so casts are required.
   registry.entityBuilder()
      .with<eua::ActionIdListener<static_cast<size_t>(Actions::MoveLeft)>>([&values](ecstasy::Registry &r, ecstasy::Entity e, eua::Action a) {
         std::cout<<"Move to the left at speed "<<a.value<<std::endl;
      })
      .with<eua::ActionIdListener<static_cast<size_t>(Actions::MoveRight)>>([&values](ecstasy::Registry &r, ecstasy::Entity e, eua::Action a) {
         std::cout<<"Move to the right at speed "<<a.value<<std::endl;
      })
      .with<eua::ActionIdListener<static_cast<size_t>(Actions::Jump)>>([&values](ecstasy::Registry &r, ecstasy::Entity e, eua::Action a) {
         std::cout<<"Jump! "<<a.value<<std::endl;
      })
      .build();
}

```

### Using intermediate actions

You may have noticed the `MoveHorizontal` action in the register part but not in the listener.
It is because this action is different because it expects an input between -1 and 1, as would be the value produced from a gamepad axis (joystick).

But the buttons/key return either 1 or 0 (pressed or not) so they are not compatible with a gamepad axis.
What we can do however to make them compatible is use `MoveLeft` and `MoveRight` as intermediate actions, and use `MoveHorizontal` for your game logic.

```cpp
using eua = ecstasy::integration::user_action;

void setupListeners(ecstasy::Registry &registry)
{
   // We'll use this entity to convert our actions
   registry.entityBuilder()
      .with<eua::ActionIdListener<static_cast<size_t>(Actions::MoveLeft)>>([&values](ecstasy::Registry &r, ecstasy::Entity e, eua::Action a) {
         a.id = Actions::MoveHorizontal;
         a.value = -a.value; // We are in the negative way
         // Send it back to the appropriate listeners
         r.getResource<eua::Users>().callActionListeners(r, a)
      })
      .with<eua::ActionIdListener<static_cast<size_t>(Actions::MoveRight)>>([&values](ecstasy::Registry &r, ecstasy::Entity e, eua::Action a) {
         a.id = Actions::MoveHorizontal;
         // Send it back to the appropriate listeners
         r.getResource<eua::Users>().callActionListeners(r, a)
      })
      .build();

   // And this is our real entity watching the move action
   registry.entityBuilder()
      .with<eua::ActionIdListener<static_cast<size_t>(Actions::MoveHorizontal)>>([&values](ecstasy::Registry &r, ecstasy::Entity e, eua::Action a) {
         std::cout<<"Moving at speed "<<a.value<<std::endl;
      })
      .with<eua::ActionIdListener<static_cast<size_t>(Actions::Jump)>>([&values](ecstasy::Registry &r, ecstasy::Entity e, eua::Action a) {
         std::cout<<"Jump! "<<a.value<<std::endl;
      })
      .build();
}

```

### Saving and loading the bindings

You can convert [UserProfile](@ref ecstasy::integration::user_action::UserProfile) bindings to/from toml format using `dump()` and `load()` methods.

Examples are much better so here it is!

#### Saving

```cpp
using eua = ecstasy::integration::user_action;

void saveProfile(ecstasy::Registry &registry, eua::UserProfile::Id userId = 0)
{
   // Consider we called the setupActions from above
   toml::table out = registry.getResource<eua::Users>().getUserProfile(userId).dump();

   // Just replace this with a filestream if you want to save to a file
   std::cout<< out <<std::endl;
}

```

@note
The action are registered by their ids until there is an easy way to serialize them. For the moment ecstasy only has size_t values when serializing so it is serialized as it.
(Comments with action names are manually added for better comprehension)

```toml
id = 0

[bindings]
# Move Left
Action-0 = [ 'Key->Q', 'Key->Left', 'GamePadAxis->TriggerLeft' ]
# Move Right
Action-1 = [ 'Key->D', 'Key->Right', 'GamePadAxis->TriggerRight' ]
# Move Horizontal
Action-2 = [ 'GamepadAxis->LeftX' ]
# Jump
Action-3 = [ 'Key->Space', 'GamepadButton->FaceDown' ]
```

#### Loading

```cpp
using eua = ecstasy::integration::user_action;

void loadProfile(ecstasy::Registry &registry, std::string_view bindings)
{
   // Consider we called the setupActions from above
   toml::table in = toml::parse(bindings);

   auto &profile = registry.getResource<eua::Users>().getUserProfile(static_cast<UserProfile::Id>(in.get("id")->as_integer()->get()));
   profile.load(in);
}

```
