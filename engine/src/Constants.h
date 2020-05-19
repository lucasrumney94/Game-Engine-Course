#pragma once

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 800;

// target frames per second
const unsigned int FPS = 60;
// target frame time in whole milliseconds
const unsigned int FRAME_TARGET_TIME = 1000 / FPS; 

enum CollisionType 
{
    NO_COLLISION,
    PLAYER_ENEMY_COLLISION, 
    PLAYER_PROJECTILE_COLLISION, 
    ENEMY_PROJECTILE_COLLISION,
    PLAYER_VEGETATION_COLLISION,
    PLAYER_LEVEL_COMPLETE_COLLISION
};

enum LayerType
{
    TILEMAP_LAYER = 0,
    VEGETATION_LAYER = 1,
    ENEMY_LAYER = 2,
    PLAYER_LAYER = 3,
    PROJECTILE_LAYER = 4,
    UI_LAYER = 5
};
const unsigned int NUM_LAYERS = 6;
