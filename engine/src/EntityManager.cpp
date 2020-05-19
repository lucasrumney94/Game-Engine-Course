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

CollisionType EntityManager::CheckCollisions() const
{
    // Horrible Code taught in the course.
    for (auto& thisEntity: entities)
    {
        if(thisEntity->HasComponent<ColliderComponent>())
        {
            ColliderComponent* thisCollider = thisEntity->GetComponent<ColliderComponent>();
            for (auto& thatEntity: entities)
            {
                if (thisEntity->name.compare(thatEntity->name)!=0 && thatEntity->HasComponent<ColliderComponent>())
                {
                    ColliderComponent* thatCollider = thatEntity->GetComponent<ColliderComponent>();
                    if (Collision::CheckRectangleCollision(thisCollider->collider, thatCollider->collider))
                    {
                        if (thisCollider->colliderTag.compare("PLAYER")==0 && thatCollider->colliderTag.compare("ENEMY")==0)
                        {
                            return PLAYER_ENEMY_COLLISION;
                        }
                        if (thisCollider->colliderTag.compare("PLAYER")==0 && thatCollider->colliderTag.compare("PROJECTILE")==0)
                        {
                            return PLAYER_PROJECTILE_COLLISION;
                        }
                        if (thisCollider->colliderTag.compare("ENEMY")==0 && thatCollider->colliderTag.compare("FRIENDLY_PROJECTILE")==0)
                        {
                            return ENEMY_PROJECTILE_COLLISION;
                        }
                        if (thisCollider->colliderTag.compare("PLAYER")==0 && thatCollider->colliderTag.compare("LEVEL_COMPLETE")==0)
                        {
                            return PLAYER_LEVEL_COMPLETE_COLLISION;
                        }
                    }
                }
            }
        }
    }
    return NO_COLLISION;
}