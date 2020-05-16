#include <iostream>
#include "./Constants.h"
#include "./Game.h"


Game::Game()
{
    this->isRunning = false;
}

Game::~Game()
{

}

bool Game::IsRunning() const
{
    return this->isRunning;
}

// Projectile Variables to see something on screen
float projectilePositionX = 0.0f;
float projectilePositionY = 0.0f;
float projectileVelX = 10.0f;
float projectileVelY = 10.0f;

void Game::Initialize(int width, int height)
{
    // Ask SDL to initialize, and if its nonzero return, we have an error
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cerr << "Error Initializing SDL." << std::endl;
        return;
    }

     // CREATE WINDOW
    window = SDL_CreateWindow(
        "Game Engine", 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        width, 
        height,
        // flag that removes the border from the window
        SDL_WINDOW_BORDERLESS
    );
    // If the SDL window didn't properly initialize (non-zero exit)
    if (!window)
    {
        std::cerr << "Error creating SDL window." << std::endl;
        return;
    }

    // CREATE RENDERER
    // -1 gets the default driver (display), 0 is no flags
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer)
    {
        std::cerr << "Error creating SDL renderer." << std::endl;
        return;
    }

    isRunning = true;
    return;
}

void Game::ProcessInput()
{
    // create an event
    SDL_Event event;
    // poll the event reference
    SDL_PollEvent(&event);

    switch(event.type)
    {
        case SDL_QUIT:
        {
            isRunning = false;
            break;
        }
        case SDL_KEYDOWN:
        {
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                isRunning = false;
            }
            break;
        }
        default:
        {
            break;
        }
    }
}

void Game::Update()
{
    // update the position in pixel space
    projectilePositionX += projectileVelX;
    projectilePositionY += projectileVelY;

}

void Game::Render()
{
    // Set the background draw color to grey
    SDL_SetRenderDrawColor(renderer,21,21,21,255);

    // clear the back buffer
    SDL_RenderClear(renderer);

    // initialize the projectile as a rectangle (struct)
    SDL_Rect projectile {
        // cast the floats projectilePositionX,Y to ints
        static_cast<int>(projectilePositionX),
        static_cast<int>(projectilePositionY),
        10, // width  
        10 // height
    };

    // Change Draw color to white
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // Draw the rectangle by passing a reference
    SDL_RenderFillRect(renderer, &projectile);

    // Show the frame
    SDL_RenderPresent(renderer);
}

void Game::Destroy()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}