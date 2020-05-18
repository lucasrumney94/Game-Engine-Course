#pragma once

#include <vector>
#include <string>
#include <map>

#include "./EntityManager.h"
#include "./Component.h"
#include "./Constants.h"

// Forward Declarations 
class EntityManager;
class Component;

class Entity
{
    private:
        EntityManager& manager;
        bool isActive;
        std::vector<Component*> components;
        std::map<const std::type_info*, Component*> componentTypeMap;

    public:
        std::string name;
        LayerType layer;
        Entity(EntityManager& manager);
        Entity(EntityManager&, std::string name, LayerType layer);
        void Update(float deltaTime);
        void Render();
        void Destroy();
        bool IsActive() const;

        // generics are defined in h file
        // returns a reference because we know it won't be null
        template<typename T, typename... TArgs>
        T& AddComponent(TArgs&&... args)
        {
            T* newComponent(new T(std::forward<TArgs>(args)...));
            newComponent->owner = this;
            components.emplace_back(newComponent);
            componentTypeMap[&typeid(*newComponent)] = newComponent;
            newComponent->Initialize();
            return *newComponent;
        }

        template<typename T>
        bool HasComponent() const
        {
            return componentTypeMap.count(&typeid(T));
        }


        // needs to return a pointer, not a reference, 
        // because the result could be null!
        template<typename T>
        T* GetComponent()
        {
            return static_cast<T*>(componentTypeMap[&typeid(T)]);
        }



};