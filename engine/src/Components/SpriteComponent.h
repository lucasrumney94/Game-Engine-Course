#pragma once

#include <SDL2/SDL.h>

#include "../TextureManager.h"
#include "../AssetManager.h"
//#include "./TransformComponent.h"

class SpriteComponent: public Component
{
    private: 
        // keep track of the transform (Required Component)
        TransformComponent* transform;
        SDL_Texture* texture;
        SDL_Rect sourceRectangle;
        SDL_Rect destinationRectangle;

    public:
        SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;
    
        SpriteComponent(const char* filePath)
        {
            SetTexture(filePath);
        }

        void SetTexture(std::string assetTextureID)
        {
            texture = Game::assetManager->GetTexture(assetTextureID);
        }

        void Initialize() override
        {
            transform = owner->GetComponent<TransformComponent>();
            sourceRectangle.x = 0;
            sourceRectangle.y = 0;
            sourceRectangle.w = transform->width;
            sourceRectangle.h = transform->height;
        }

        void Update(float deltaTime) override
        {
            destinationRectangle.x = static_cast<int>(transform->position.x);
            destinationRectangle.y = static_cast<int>(transform->position.y);
            destinationRectangle.w = transform->width*transform->scale;
            destinationRectangle.h = transform->height*transform->scale;            
        }

        void Render() override 
        {
            TextureManager::Draw(texture, sourceRectangle, destinationRectangle, spriteFlip);
        }
};