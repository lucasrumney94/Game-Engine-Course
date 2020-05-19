#pragma once

#include <map>
#include <string>

#include <SDL2/SDL_ttf.h>

#include "./TextureManager.h"
#include "./FontManager.h"
#include "./EntityManager.h"

class AssetManager
{
    private:
        EntityManager* manager;
        std::map<std::string, SDL_Texture*> textures;
        std::map<std::string, TTF_Font*> fonts;
    public:
        AssetManager(EntityManager* manager);
        ~AssetManager();
        void ClearData();

        void AddTexture(std::string textureID, const char* filePath);
        SDL_Texture* GetTexture(std::string textureID);

        void AddFont(std::string fontID, const char* filePath, int fontSize);
        TTF_Font* GetFont(std::string fontID);
};