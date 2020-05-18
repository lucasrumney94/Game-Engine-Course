#pragma once

#include <SDL2/SDL.h>

class Collision
{
    public: 
        // Axis-Aligned Bounding Box (AABB) Collision Check (AAAAABCCCCCCC)
        static bool CheckRectangleCollision(const SDL_Rect& rectangleA, const SDL_Rect& rectangleB);
};