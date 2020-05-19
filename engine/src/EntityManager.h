#pragma once

#include <vector>

#include "./Entity.h"
#include "./Component.h"
#include "./Constants.h"

class EntityManager 
{
    private:
        std::vector<Entity*> entities;

    public:
        void ClearData();
        void Update(float deltaTime);
        void Render();
        bool HasNoEntities();
        Entity& AddEntity(std::string entityName, LayerType layer);

        // not going to change them, but just return the list
        std::vector<Entity*> GetEntities() const;
        std::vector<Entity*> GetEntitiesByLayer(LayerType layer) const;
        unsigned int GetEntityCount();

        CollisionType CheckCollisions() const;
        void DestroyInactiveEntities();

};