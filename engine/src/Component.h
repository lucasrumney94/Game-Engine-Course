#pragma once

#include "./Entity.h"

// forward declaration
class Entity;

class Component 
{
    public: 
        Entity* owner;
        virtual ~Component() {}
        virtual void Initialize() {}
        virtual void Update(float deltaTime){}
        virtual void Render() {}
};