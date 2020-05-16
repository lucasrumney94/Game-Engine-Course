#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

class Game 
{
    private:
        bool isRunning;
        SDL_Window *window;
        SDL_Renderer *renderer;

    public:
        // Constructor
        Game();
        // Destructor
        ~Game();

        int ticksLastFrame;
        // This will not change anything in class, its a getter so set it as const
        bool IsRunning() const;
        void Initialize(int width, int height);
        void ProcessInput();
        void Update();
        void Render();
        void Destroy();
};