#include "./AssetManager.h"

AssetManager::AssetManager(EntityManager* manager): manager(manager)
{

}

void AssetManager::ClearData()
{
    textures.clear();
}

void AssetManager::AddTexture(std::string textureID, const char* filePath)
{
    textures.emplace(textureID, TextureManager::LoadTexture(filePath));
}

SDL_Texture* AssetManager::GetTexture(std::string textureID)
{
    return textures[textureID];
}