#pragma once

#include <vector>
#include <string>

#include "./EntityManager.h"
#include "./Component.h"

// Forward Declarations 
class EntityManager;
class Component;

class Entity
{
    private:
        EntityManager& manager;
        bool isActive;
        std::vector<Component*> components;

    public:
        std::string name;
        Entity(EntityManager& manager);
        Entity(EntityManager&, std::string name);
        void Update(float deltaTime);
        void Render();
        void Destroy();
        bool IsActive() const;

        // generics are defined in h file
        template<typename T, typename... TArgs>
        T& AddComponent(TArgs&&... args){
            T* newComponent(new T(std::forward<TArgs>(args)...));
            newComponent->owner = this;
            components.emplace_back(newComponent);
            newComponent->Initialize();
            return *newComponent;
        }
};