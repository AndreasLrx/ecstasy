# Events integration

@tableofcontents

## Description

The [event integration](@ref ecstasy::integration::event) contains helpers to manage events.
**It does not detect the events** but it allows the use of associated listeners.

### How to get feedback

There is two ways of getting event feedbacks:

- [Event Listeners](@ref ecstasy::integration::event::EventListener): Callback lambdas
- Input Resources ([Keyboard](@ref ecstasy::integration::event::Keyboard), [Mouse](@ref ecstasy::integration::event::Mouse), [Gamepads](@ref ecstasy::integration::event::Gamepads)): State tracking

Listeners are callbacks (usually lambdas) attached to an entity as a component. They are called with the registry, the entity and the event references as parameters.

Input resources are optional resources you can use to keep track of the inputs states.
It allows to have access to input states at any moments, for example to check whether a particular key is still being pressed.

### Event flow

1. Register your listeners and resources

   Create your listeners components and input resources.

2. Detect the event

   Using system checks, or third party libraries like SDL2 or SFML whatever, the goal of this integration is to help handling events, not detecting them. However there is an [sfml integration](@ref ecstasy::integration::sfml) with included event detection system.

3. Calls [EventsManager::handleEvent()](@ref ecstasy::integration::event::EventsManager::handleEvent)

   Convert the detected event to an [ecstasy event](@ref ecstasy::integration::event::Event) and feed it to the event manager.

4. Event dispatch

   The manager will first call the associated listeners if any.

   Then if the registry contains an input associated resource, it will update them.

   Finally if the [user action integration](@ref ecstasy::integration::user_action) is enabled, and there is a [user_action::Users](@ref ecstasy::integration::user_action::Users) resource, it will call its handleEvent method to detect and handle actions.

## Example

These examples are mostly extracted from the SFML integration demo working with events, because it is made for!

### Creating listeners components

@note You can find the different listeners types in the [listeners](@ref src/ecstasy/integrations/event/listeners) folder.

@note You can have multiple entities listening for the same event

```cpp
using event = ecstasy::integration::event;

int main() {
   ecstasy::Registry registry;

   // Creating an entity listening only to mouse events
   registry.entityBuilder()
      .with<event::MouseButtonListener>(
            [](ecstasy::Registry &r, ecstasy::Entity entity, const event::MouseButtonEvent &e) {
               (void)r;
               (void)entity;
               std::cout << "Mouse button " << static_cast<int>(e.button) << " event, pressed = " << e.pressed
                        << std::endl;
            });

   // Or creating an entity with multiple listeners
   registry.entityBuilder()
      .with<event::MouseButtonListener>(
         [](ecstasy::Registry &r, ecstasy::Entity entity, const event::MouseButtonEvent &e) {
               (void)r;
               (void)entity;
               std::cout << "Mouse button " << static_cast<int>(e.button) << " event, pressed = " << e.pressed
                        << std::endl;
         })
      .with<event::MouseWheelScrollListener>(
         [](ecstasy::Registry &r, ecstasy::Entity entity, const event::MouseWheelScrollEvent &e) {
               (void)r;
               (void)entity;
               std::cout << "Mouse wheel " << static_cast<int>(e.wheel) << " event, delta = " << e.delta << std::endl;
         })
      .with<event::MouseMoveListener>(
         [](ecstasy::Registry &r, ecstasy::Entity entity, const event::MouseMoveEvent &e) {
               (void)r;
               (void)entity;
               std::cout << "Mouse move to (" << e.x << ", " << e.y << ")" << std::endl;
         });
}

```

### Sending an event

@note You can find the different event types in the [events](@ref src/ecstasy/integrations/event/events) folder.

@note This example detect its events from an SFML window.

```cpp
using event = ecstasy::integration::event;

void detect_events(ecstasy::Registry &registry, sf::RenderWindow &window)
{
   sf::Event event;

   while (window.pollEvent(event)) {
      switch (event.type) {
            /// Mouse events
            case sf::Event::MouseButtonPressed:
            case sf::Event::MouseButtonReleased:
               event::EventsManager::handleEvent(
                  registry,
                  event::MouseButtonEvent(
                     static_cast<event::Mouse::Button>(event.mouseButton.button),
                     event.type == sf::Event::MouseButtonPressed) // Second parameter is whether the button is pressed
                  );
               break;
            case sf::Event::MouseWheelScrolled:
               event::EventsManager::handleEvent(
                  registry,
                  event::MouseWheelScrollEvent(
                        event.mouseWheelScroll.delta,
                        static_cast<event::Mouse::Wheel>(event.mouseWheelScroll.wheel)
                  ));
               break;
            case sf::Event::MouseMoved:
               // Simply calls handleEvent with an event constructed easily with its constructor
               event::EventsManager::handleEvent(
                  registry,
                  event::MouseMoveEvent(event.mouseMove.x, event.mouseMove.y));
               break;
      }
   }
}
```

### Accessing an input state

If you don't want to have listeners or if you need to be able to check an input state, here is how you can do it.

@note You can find the different input types in the [inputs](@ref src/ecstasy/integrations/event/inputs) folder.

```cpp
using event = ecstasy::integration::event;


int main()
{
   ecstasy::Registry registry;

   // Resource must be registered if you want it accessible
   registry.addResource<event::Keyboard>();

   while true {
      // Fetch your events here and dispatch them with handleEvent()
      // mySuperFetchEvents()

      // Always fetch resources const qualified if possible
      if (registry.getResource<const event::Keyboard>()->isKeyDown(event::Keyboard::Key::Escape))
         break;
   }
}

```
