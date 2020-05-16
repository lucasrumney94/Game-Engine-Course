#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "./Entity.h"
#include "./Component.h"
#include "./EntityManager.h"

class Game 
{
    private:
        bool isRunning;
        SDL_Window *window;

    public:
        // Constructor
        Game();
        // Destructor
        ~Game();

        // static renderer so its accessible from any render function
        static SDL_Renderer *renderer;

        int ticksLastFrame;

        // This will not change anything in class, its a getter so set it as const
        bool IsRunning() const;
        void LoadLevel(int levelNumber);
        void Initialize(int width, int height);
        void ProcessInput();
        void Update();
        void Render();
        void Destroy();
};