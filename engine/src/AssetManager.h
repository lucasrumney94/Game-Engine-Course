#pragma once

#include <map>
#include <string>

#include "./TextureManager.h"
#include "./EntityManager.h"

class AssetManager
{
    private:
        EntityManager* manager;
        std::map<std::string, SDL_Texture*> textures;
    public:
        AssetManager(EntityManager* manager);
        ~AssetManager();
        void ClearData();
        void AddTexture(std::string textureID, const char* filePath);
        SDL_Texture* GetTexture(std::string textureID);
};