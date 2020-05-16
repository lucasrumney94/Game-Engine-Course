#include <iostream>
#include "./Constants.h"
#include "./Game.h"
#include "../lib/glm/glm.hpp"


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
// TODO: Move these to a proper place
glm::vec2 projectilePos = glm::vec2(0.0f, 0.0f);
glm::vec2 projectileVel = glm::vec2(20.0f, 30.0f);

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
    // cache current ticks
    uint32_t ticksCurrent = SDL_GetTicks();
    int timeToWait = FRAME_TARGET_TIME - (ticksCurrent-ticksLastFrame);

    // Wait until TARGET FRAME TIME (~16.6ms for 60fps) has elapsed since the last frame
    // while loops are processor instructions, consumes 100% of cpu core ~ bad!
    //while(!SDL_TICKS_PASSED(SDL_GetTicks(), ticksLastFrame+FRAME_TARGET_TIME));

    // Using SDL_Delay is preferable and doesn't absolutely consume the cpu
    // only wait if we are faster than the target frame time
    if (timeToWait > 0 && timeToWait<FRAME_TARGET_TIME)
    {
        SDL_Delay(timeToWait); // Delay one ms
    }

    // frame deltaTime in seconds
    float deltaTime = (ticksCurrent - ticksLastFrame) / 1000.0f;

    // clamp the value of deltaTime to avoid pause/background/debug induced jumps in position
    // caused by large deltaTime values
    // conditional operator
    deltaTime = (deltaTime>0.0f) ? 0.05f : deltaTime;


    // how many ms since the last SDL library initialized
    // to be used in the next pass
    ticksLastFrame = ticksCurrent;

    // update the position scaled by deltaTime
    projectilePos = glm::vec2(
        projectilePos.x + projectileVel.x * deltaTime,
        projectilePos.y + projectileVel.y * deltaTime
    );

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
        static_cast<int>(projectilePos.x),
        static_cast<int>(projectilePos.y),
        10, // width  
        10 // height
    };

    // Change Draw color to white
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // Draw the rectangle by passing a reference
    SDL_RenderFillRect(renderer, &projectile);

    // Swap Front and Back Buffers 
    // (displaying the drawings that were made on the back buffer)
    SDL_RenderPresent(renderer);
}

void Game::Destroy()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}