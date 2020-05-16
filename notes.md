# General Setup
Could use Linux, Mac, Windows. Unix preferred. I will be using Windows Linux Subsystem/Ubuntu. If I use WSL I can use VSCode as an editor.

Using a make file that calls g++ compiler, and includes all of the lua and SDL libs.

## GNU Make
Important tool for building. There are Flags, rules, many many more features other than just building. 

# Game Engine Notes

## Game Loop
( Process Input -> Update Game -> Render )

Repeat Every Frame.

## Timesteps
Fixing our timestep is done through a deltaTime.
A frame deltaTime will allow us to stabilize our game.
Instead of just moving things every frame, tied to the framerate, 
we'd like the distance to be framerate independent. 

deltaTime is the amount of real time elapsed since the last frame.
`position.x += movement*deltaTime;`

## Rendering

### Double Buffer
- Back Buffer: Load this with information then wap
- Front Buffer: What's being shown

- Color Buffer: Colors of the screen pixels

### Process
1. Clear the back buffer
2. Draw all game objects on the back buffer
3. Swap the front and back buffers
4. Repeat


## ECS 

### Objects and Inheritance Approach
Player, Enemy, Obstacle all inherit from Object
Player and camera might have behaviour in common, but not in the object.
Complicated Inheritance Structures are not ideal for comprehension

### Entity-Component
Entities have attached components that are appropriate for their operation.
"Composition over Inheritance" approach.

Entity - Player
Component - Transform
Component - Collider
Component - Sprite

Entity - Enemy
Component - Transform
Component - Collider
Component - Sprite

Entity - DoorTrigger
Component - Transform
Component - Collider


### C++ Structure
Entity Class, that contains a list of components

Components need to know how to Update and/or Render themself
Specific components will inherit and be required to override these Update and Render functions

Game Holds the EntityManager
The EntityManager holds a list of the entities, 
-> and executes Update and Render for each entity
Each Entity holds a list of components
-> and executes Update and Render for each component

#### Pseudocode
```
class Entity 
{
    vector<Component*> components;

    AddComponent<T>(component);
    Update();
    Render();
}
```
```
class EntityManager
{
    vector<Entity*> entities;

    AddEntity(entity);
    GetEntity(string EntityName);
    Update();
    Render();
}
```
```
class Component 
{
    Entity* owner;

    virtual Update();
    virtual Render(); // virtual because they need to be overrode

}
```
```
class TransformComponent
{
    // data
    glm::vec2 position;
    float scale;

    Update() override 
    {

    }

    Render() override
    {

    }
}
```

## Using C++ Generics and Templates


```
// create a new entity
auto& newEntity(manager.AddEntity("Player"))

// can use templates to help
newEntity.AddComponent<TransformComponent>(10, 10, 40, 40, 40 , 40, 1);
newEntity.AddComponent<SpriteComponent>("tank.png");
```

```
entity.AddComponent<HereGoesTheComponentType>();
```

T is the Type 
TArgs are the types of the arguments
(can be named anything but this name is common)











