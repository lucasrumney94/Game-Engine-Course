#pragma once

#include <vector>

#include "./Entity.h"
#include "./Component.h"

class EntityManager 
{
    private:
        std::vector<Entity*> entities;

    public:
        void ClearData();
        void Update(float deltaTime);
        void Render();
        bool HasNoEntities();
        Entity& AddEntity(std::string entityName);

        // not going to change them, but just return the list
        std::vector<Entity*> GetEntities() const;
        unsigned int GetEntityCount();

};