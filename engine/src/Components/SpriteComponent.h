#pragma once

#include <SDL2/SDL.h>

#include "../TextureManager.h"
#include "../AssetManager.h"
#include "../Animation.h"

class SpriteComponent: public Component
{
    private: 
        // keep track of the transform (Required Component)
        TransformComponent* transform;
        SDL_Texture* texture;
        SDL_Rect sourceRectangle;
        SDL_Rect destinationRectangle;
        bool isAnimated;
        int numFrames;
        int animationSpeed;
        // always stay in same position (fixed) in screenspace
        bool isFixed; 
        std::map<std::string, Animation> animations;
        std::string currentAnimationName;
        unsigned int animationIndex = 0;

    public:
        SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;
    
        SpriteComponent(const char* filePath)
        {
            isAnimated = false;
            isFixed = false;
            SetTexture(filePath);
        }

        SpriteComponent(std::string assetTextureId) 
        {
            this->isAnimated = false;
            this->isFixed = false;
            SetTexture(assetTextureId);
        }

        SpriteComponent(std::string assetTextureId, bool isFixed) 
        {
            this->isAnimated = false;
            this->isFixed = isFixed;
            SetTexture(assetTextureId);
        }

        SpriteComponent(std::string animationID, int numFrames, int animationSpeed, bool hasDirections, bool isFixed)
        {
            isAnimated = true;
            this->numFrames = numFrames;
            this->animationSpeed = animationSpeed;
            this->isFixed = isFixed;

            if (hasDirections)
            {
                // create animations
                Animation downAnimation = Animation(0, numFrames, animationSpeed);
                Animation rightAnimation = Animation(1, numFrames, animationSpeed);
                Animation leftAnimation = Animation(2, numFrames, animationSpeed);
                Animation upAnimation = Animation(3, numFrames, animationSpeed);

                // add animations
                animations.emplace("DownAnimation", downAnimation);
                animations.emplace("RightAnimation", rightAnimation);
                animations.emplace("LeftAnimation", leftAnimation);
                animations.emplace("UpAnimation", upAnimation);

                // default to down
                this->animationIndex = 0;
                this->currentAnimationName = "DownAnimation";     
            }
            else
            {
                Animation singleAnimation = Animation(0, numFrames, animationSpeed);
                animations.emplace("SingleAnimation", singleAnimation);
                this->animationIndex = 0;
                this->currentAnimationName = "SingleAnimation";
            }

            Play(this->currentAnimationName);
            SetTexture(animationID);
        }

        void Play(std::string animationName)
        {
            // get the number of frames for the given animation
            numFrames = animations[animationName].numFrames;
            animationIndex = animations[animationName].index;
            animationSpeed = animations[animationName].animationSpeed;
            currentAnimationName = animationName;
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
            if (isAnimated)
            {
                // update the x by the width of the sourceRectangle, dependent on time.
                sourceRectangle.x = sourceRectangle.w * static_cast<int>((SDL_GetTicks() / animationSpeed) % numFrames);
            }

            // update the y by the index height
            sourceRectangle.y = animationIndex * transform->height;
            destinationRectangle.x = static_cast<int>(transform->position.x)-(isFixed ? 0 : Game::camera.x);
            destinationRectangle.y = static_cast<int>(transform->position.y)-(isFixed ? 0 : Game::camera.y);
            destinationRectangle.w = transform->width*transform->scale;
            destinationRectangle.h = transform->height*transform->scale; 
        }

        void Render() override 
        {
            TextureManager::Draw(texture, sourceRectangle, destinationRectangle, spriteFlip);
        }
};