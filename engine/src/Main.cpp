#include <iostream>
#include "./Constants.h"
#include "./Game.h"

int main(int argc, char *args[]) 
{
    std::cout << "Game Launched!" << std::endl;

    // Dynamically allocate memory for the game
    Game *game = new Game();

    // Initialize the game with a specific resolution
    game->Initialize(WINDOW_WIDTH, WINDOW_HEIGHT);

    while(game->IsRunning())
    {
        // Process Player Input
        game->ProcessInput();

        // Update Game State
        game->Update();

        // Render Game to Screen
        game->Render();
    }

    // Clean up
    game->Destroy();
    return 0;
}