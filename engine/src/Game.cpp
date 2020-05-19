#include <iostream>

#include "../lib/glm/glm.hpp"

#include "./Constants.h"
#include "./Game.h"
#include "./AssetManager.h"
#include "./Map.h"
#include "./Components/TransformComponent.h"
#include "./Components/SpriteComponent.h"
#include "./Components/KeyboardControlComponent.h"
#include "./Components/ColliderComponent.h"
#include "./Components/TextLabelComponent.h"
#include "./Components/ProjectileEmitterComponent.h"

EntityManager manager;
AssetManager* Game::assetManager = new AssetManager(&manager);
SDL_Renderer* Game::renderer;
SDL_Event Game::event;
SDL_Rect Game::camera = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
Entity* mainPlayer = NULL;
Map* map;

Game::Game(): ticksLastFrame{0}
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

void Game::Initialize(int width, int height)
{
    // Ask SDL to initialize, and if its nonzero return, we have an error
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cerr << "Error Initializing SDL." << std::endl;
        return;
    }
    if (TTF_Init() != 0)
    {
        std::cerr << "Error Initializing SDL TTF" << std::endl;
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

    LoadLevel(1);

    isRunning = true;
    return;
}

// Horrible?
Entity& player(manager.AddEntity("chopper", PLAYER_LAYER));

void Game::LoadLevel(int levelNumber)
{
    sol::state lua;
    //get base, os, and math libraries
    lua.open_libraries(sol::lib::base, sol::lib::os, sol::lib::math);

    std::string levelName = "Level" + std::to_string(levelNumber);
    lua.script_file("./assets/scripts/" + levelName + ".lua");

    //////// LOAD ASSETS FROM LUA FILE /////////////
    sol::table levelData = lua[levelName];
    sol::table levelAssets = levelData["assets"];

    unsigned int assetIndex = 0;
    while(true)
    {
        sol::optional<sol::table> existsAssetIndexNode = levelAssets[assetIndex];
        if (existsAssetIndexNode == sol::nullopt)
        {
            break;
        }
        else
        {
            sol::table asset = levelAssets[assetIndex];
            std::string assetType = asset["type"];

            // TEXTURES
            if (assetType.compare("texture")==0)
            {
                std::string assetID = asset["id"];
                std::string assetFile = asset["file"];
                assetManager->AddTexture(assetID, assetFile.c_str());
            }

            // TTF FONTS
            if (assetType.compare("font") == 0)
            {
                std::string assetID = asset["id"];
                std::string assetFile = asset["file"];
                int fontSize = asset["fontSize"];
                assetManager->AddFont(assetID, assetFile.c_str(), fontSize);
            }
        }
        assetIndex++;
    }
    ////////////////////////////////////////////////
    //////// LOAD MAP FROM LUA FILE ///////////////
    sol::table levelMap = levelData["map"];
    std::string mapTextureID = levelMap["textureAssetId"];
    std::string mapFile = levelMap["file"];

    map = new Map(
        mapTextureID, 
        static_cast<int>(levelMap["scale"]),
        static_cast<int>(levelMap["tileSize"])
    );

    map->LoadMap(
        mapFile,
        static_cast<int>(levelMap["mapSizeX"]),
        static_cast<int>(levelMap["mapSizeY"])
    );
    ////////////////////////////////////////////////

    ////////////////////////////////////////////////
    //////// LOAD ENTITIES FROM LUA FILE ///////////
    sol::table levelEntities = levelData["entities"];
    unsigned int entityIndex = 0;
    while (true) {
        sol::optional<sol::table> existsEntityIndexNode = levelEntities[entityIndex];
        if (existsEntityIndexNode == sol::nullopt) {
            break;
        }
        else {
            sol::table entity = levelEntities[entityIndex];
            std::string entityName = entity["name"];
            LayerType entityLayerType = static_cast<LayerType>(static_cast<int>(entity["layer"]));

            // Add new entity
            auto& newEntity(manager.AddEntity(entityName, entityLayerType));

            // Add Transform Component
            sol::optional<sol::table> existsTransformComponent = entity["components"]["transform"];
            if (existsTransformComponent != sol::nullopt) {
                newEntity.AddComponent<TransformComponent>(
                    static_cast<int>(entity["components"]["transform"]["position"]["x"]),
                    static_cast<int>(entity["components"]["transform"]["position"]["y"]),
                    static_cast<int>(entity["components"]["transform"]["velocity"]["x"]),
                    static_cast<int>(entity["components"]["transform"]["velocity"]["y"]),
                    static_cast<int>(entity["components"]["transform"]["width"]),
                    static_cast<int>(entity["components"]["transform"]["height"]),
                    static_cast<int>(entity["components"]["transform"]["scale"])
                );
            }

            // Add sprite component
            sol::optional<sol::table> existsSpriteComponent = entity["components"]["sprite"];
            if (existsSpriteComponent != sol::nullopt) {
                std::string textureId = entity["components"]["sprite"]["textureAssetId"];
                bool isAnimated = entity["components"]["sprite"]["animated"];
                if (isAnimated) {
                    newEntity.AddComponent<SpriteComponent>(
                        textureId,
                        static_cast<int>(entity["components"]["sprite"]["frameCount"]),
                        static_cast<int>(entity["components"]["sprite"]["animationSpeed"]),
                        static_cast<bool>(entity["components"]["sprite"]["hasDirections"]),
                        static_cast<bool>(entity["components"]["sprite"]["fixed"])
                    );
                } else {
                    newEntity.AddComponent<SpriteComponent>(textureId, false);
                }
            }

            // Add input control component
            sol::optional<sol::table> existsInputComponent = entity["components"]["input"];
            if (existsInputComponent != sol::nullopt) {
                sol::optional<sol::table> existsKeyboardInputComponent = entity["components"]["input"]["keyboard"];
                if (existsKeyboardInputComponent != sol::nullopt) {
                    std::string upKey = entity["components"]["input"]["keyboard"]["up"];
                    std::string rightKey = entity["components"]["input"]["keyboard"]["right"];
                    std::string downKey = entity["components"]["input"]["keyboard"]["down"];
                    std::string leftKey = entity["components"]["input"]["keyboard"]["left"];
                    std::string shootKey = entity["components"]["input"]["keyboard"]["shoot"];
                    newEntity.AddComponent<KeyboardControlComponent>(upKey, rightKey, downKey, leftKey, shootKey);
                }
            }

            // Add collider component
            sol::optional<sol::table> existsColliderComponent = entity["components"]["collider"];
            if (existsColliderComponent != sol::nullopt) {
                std::string colliderTag = entity["components"]["collider"]["tag"];
                newEntity.AddComponent<ColliderComponent>(
                    colliderTag,
                    static_cast<int>(entity["components"]["transform"]["position"]["x"]),
                    static_cast<int>(entity["components"]["transform"]["position"]["y"]),
                    static_cast<int>(entity["components"]["transform"]["width"]),
                    static_cast<int>(entity["components"]["transform"]["height"])
                );
            }

            // Add projectile emitter component
            sol::optional<sol::table> existsProjectileEmitterComponent = entity["components"]["projectileEmitter"];
            if (existsProjectileEmitterComponent != sol::nullopt) {
                int parentEntityXPos = entity["components"]["transform"]["position"]["x"];
                int parentEntityYPos = entity["components"]["transform"]["position"]["y"];
                int parentEntityWidth = entity["components"]["transform"]["width"];
                int parentEntityHeight = entity["components"]["transform"]["height"];
                int projectileWidth = entity["components"]["projectileEmitter"]["width"];
                int projectileHeight = entity["components"]["projectileEmitter"]["height"];
                int projectileSpeed = entity["components"]["projectileEmitter"]["speed"];
                int projectileRange = entity["components"]["projectileEmitter"]["range"];
                int projectileAngle = entity["components"]["projectileEmitter"]["angle"];
                bool projectileShouldLoop = entity["components"]["projectileEmitter"]["shouldLoop"];
                std::string textureAssetId = entity["components"]["projectileEmitter"]["textureAssetId"];
                Entity& projectile(manager.AddEntity("projectile", PROJECTILE_LAYER));
                projectile.AddComponent<TransformComponent>(
                    parentEntityXPos + (parentEntityWidth / 2),
                    parentEntityYPos + (parentEntityHeight / 2),
                    0,
                    0,
                    projectileWidth,
                    projectileHeight,
                    1
                );
                projectile.AddComponent<SpriteComponent>(textureAssetId);
                projectile.AddComponent<ProjectileEmitterComponent>(
                    projectileSpeed,
                    projectileAngle,
                    projectileRange,
                    projectileShouldLoop
                );
                projectile.AddComponent<ColliderComponent>(
                    "PROJECTILE",
                    parentEntityXPos,
                    parentEntityYPos,
                    projectileWidth,
                    projectileHeight
                );
            }
        }
        entityIndex++;
    }

    mainPlayer = manager.GetEntityByName("player");

    ////////////////////////////////////////////////


    // Load all assets into the assetManager's map
    // assetManager->AddTexture(
    //     "tank-image", 
    //     std::string("./assets/images/tank-big-right.png").c_str()
    // );
    // assetManager->AddTexture(
    //     "chopper-image",
    //     std::string("./assets/images/chopper-spritesheet.png").c_str()
    // );
    // assetManager->AddTexture(
    //     "radar-image",
    //     std::string("./assets/images/radar.png").c_str()
    // );
    // assetManager->AddTexture(
    //     "heliport-image",
    //     std::string("./assets/images/heliport.png").c_str()
    // );
    // assetManager->AddTexture(
    //     "projectile-image",
    //     std::string("./assets/images/bullet-enemy.png").c_str()
    // );
    // assetManager->AddTexture(
    //     "jungle-tiletexture",
    //     std::string("./assets/tilemaps/jungle.png").c_str()
    // );
    // assetManager->AddFont(
    //     "charriot-font",
    //     std::string("./assets/fonts/charriot.ttf").c_str(),
    //     14
    // );

    // // Load the map
    // map = new Map("jungle-tiletexture", 2, 32);
    // map->LoadMap("./assets/tilemaps/jungle.map", 25, 20);
    
    // // add entities and components to the entities
    // // create and add a new Entity named tank
    // Entity& tankEntity(manager.AddEntity("tank", ENEMY_LAYER));
    // tankEntity.AddComponent<TransformComponent>(150, 495, 2, 0, 32, 32, 1);
    // tankEntity.AddComponent<SpriteComponent>("tank-image");
    // tankEntity.AddComponent<ColliderComponent>("ENEMY", 150, 495, 32, 32);

    // Entity& projectile(manager.AddEntity("projectile", PROJECTILE_LAYER));
    // projectile.AddComponent<TransformComponent>(150+16, 495+16, 0, 0, 4, 4, 1);
    // projectile.AddComponent<SpriteComponent>("projectile-image");
    // projectile.AddComponent<ColliderComponent>("PROJECTILE", 150+16, 495+16, 4, 4);
    // projectile.AddComponent<ProjectileEmitterComponent>(50, 270, 200, true);

    // player.AddComponent<TransformComponent>(240, 106, 0, 0, 32, 32, 1);
    // player.AddComponent<SpriteComponent>("chopper-image", 2, 90, true, false);
    // player.AddComponent<KeyboardControlComponent>("up", "right", "down", "left", "space");
    // player.AddComponent<ColliderComponent>("PLAYER", 240, 106, 32, 32);
    
    // Entity& radarEntity(manager.AddEntity("Radar", UI_LAYER));
    // radarEntity.AddComponent<TransformComponent>(720, 15, 0, 0, 64, 64, 1);
    // radarEntity.AddComponent<SpriteComponent>("radar-image", 8, 150, false, true);
  
    // Entity& heliportEntity(manager.AddEntity("Heliport", VEGETATION_LAYER));
    // heliportEntity.AddComponent<TransformComponent>(600, 512, 0, 0, 64, 64, 1);
    // heliportEntity.AddComponent<SpriteComponent>("heliport-image");
    // heliportEntity.AddComponent<ColliderComponent>("LEVEL_COMPLETE", 600, 512, 32, 32);

    // Entity& levelLabelName(manager.AddEntity("LabelLevelName", UI_LAYER));
    // levelLabelName.AddComponent<TextLabelComponent>(10, 10, "First Level...", "charriot-font", WHITE_COLOR);

    std::cout<<"Level " << levelNumber <<" Loaded!"<<std::endl;
}

void Game::ProcessInput()
{
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

    // this will call all the entity's update functions
    manager.Update(deltaTime);

    HandleCameraMovement();
    CheckCollisions();
}

void Game::Render()
{
    // Set the background draw color to grey
    SDL_SetRenderDrawColor(renderer,21,21,21,255);

    // clear the back buffer
    SDL_RenderClear(renderer);

    if (manager.HasNoEntities())
    {
        return;
    }
    
    manager.Render();

    // Swap Front and Back Buffers 
    // (displaying the drawings that were made on the back buffer)
    SDL_RenderPresent(renderer);
}

void Game::HandleCameraMovement()
{
    if (mainPlayer)
    {
        TransformComponent* mainPlayerTransform = mainPlayer->GetComponent<TransformComponent>();
        camera.x = mainPlayerTransform->position.x - (WINDOW_WIDTH / 2);
        camera.y = mainPlayerTransform->position.y - (WINDOW_HEIGHT / 2);

        // clamp camera to bounds
        camera.x = camera.x < 0 ? 0 : camera.x;
        camera.y = camera.y < 0 ? 0 : camera.y;
        camera.x = camera.x > camera.w ? camera.w : camera.x;
        camera.y = camera.y > camera.h ? camera.h : camera.y;
    }
}

void Game::CheckCollisions()
{
    CollisionType collisionType = manager.CheckCollisions();
    if (collisionType == PLAYER_ENEMY_COLLISION)
    {
        ProcessGameOver();
    }
    if (collisionType == PLAYER_LEVEL_COMPLETE_COLLISION)
    {
        ProcessNextLevel(1);
    }
    if (collisionType == PLAYER_PROJECTILE_COLLISION)
    {
        ProcessGameOver();
    }
}

void Game::ProcessNextLevel(int levelNumber)
{
    std::cout<<"Next Level!"<<std::endl;
    isRunning = false;
}

void Game::ProcessGameOver()
{
    std::cout<<"Next Level!"<<std::endl;
    isRunning = false;
}

void Game::Destroy()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

