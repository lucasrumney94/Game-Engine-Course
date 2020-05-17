#include<iostream>
#include<string>

#include "./Entity.h"

// Define the constructor of Entity
// and take care of setting the manager 
Entity::Entity(EntityManager& manager): manager(manager)
{
    this->isActive = true;
}

// Define the second constructor of Entity that takes a string
// and take care of setting the manager and name
Entity::Entity(EntityManager& manager, std::string name, LayerType layer)
    : manager(manager), name(name), layer(layer)
{
    this->isActive = true;
}

void Entity::Update(float deltaTime)
{
    // call all of the components update functions
    for (auto& component: components)
    {
        component->Update(deltaTime);
    }
}

void Entity::Render()
{
    // call all of the components update functions
    for (auto& component: components)
    {
        component->Render();
    }
}

void Entity::Destroy()
{
    // TODO: Actually destroy instead of disabling
    this->isActive = false;
}

bool Entity::IsActive() const
{
    return this->isActive;
}