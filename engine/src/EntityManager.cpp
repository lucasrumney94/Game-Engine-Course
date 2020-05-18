#include <string>

#include "./EntityManager.h"
#include "./Collision.h"
#include "./Components/ColliderComponent.h"

void EntityManager::ClearData()
{
    for (auto& entity: entities)
    {
        entity->Destroy();
    }
}

bool EntityManager::HasNoEntities()
{
    return entities.size() == 0;
}

void EntityManager::Update(float deltaTime)
{
    for (auto& entity: entities)
    {
        entity->Update(deltaTime);
    }
}

void EntityManager::Render()
{
    // for (auto& entity: entities)
    // {
    //     entity->Render();
    // }
    for (int layerNumber = 0; layerNumber<NUM_LAYERS; layerNumber++)
    {
        // render entities by painter's algorithm back to front
        for (auto& entity: GetEntitiesByLayer(static_cast<LayerType>(layerNumber)))
        {
            entity->Render();
        }
    }
}

Entity& EntityManager::AddEntity(std::string entityName, LayerType layer)
{
    // create a new entity and assign THIS manager as its manager
    Entity *entity = new Entity(*this, entityName, layer);
    // add the newly created entity to the vector of entities
    entities.emplace_back(entity);
    return *entity;
}

std::vector<Entity*> EntityManager::GetEntities() const 
{
    return entities;
}

std::vector<Entity*> EntityManager::GetEntitiesByLayer(LayerType layer) const 
{
    std::vector<Entity*> selectedEntities;
    for (auto& entity: entities)
    {
        if (entity->layer == layer)
        {
            selectedEntities.emplace_back(entity);
        }
    }
    return selectedEntities;
}

unsigned int EntityManager::GetEntityCount()
{
    return entities.size();
}

std::string EntityManager::CheckEntityCollisions(Entity& myEntity) const
{
    ColliderComponent* myCollider = myEntity.GetComponent<ColliderComponent>();
    for (auto& entity: entities)
    {
        if (entity->name.compare(myEntity.name) != 0 && entity->name.compare("Tile"))
        {
            if (entity->HasComponent<ColliderComponent>())
            {
                ColliderComponent* otherCollider = entity->GetComponent<ColliderComponent>();
                if(Collision::CheckRectangleCollision(myCollider->collider, otherCollider->collider))
                {
                    return otherCollider->colliderTag;
                }
            }
        }
    }
    return std::string();
}