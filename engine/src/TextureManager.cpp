#include <iostream>

#include "./TextureManager.h"

SDL_Texture* TextureManager::LoadTexture(const char* filePath)
{
    SDL_Surface* surface = IMG_Load(filePath);
    if (!surface)
    {
        std::cout<<"surface is null"<<std::endl;
    }
    std::cerr<<IMG_GetError()<<std::endl;
    std::cerr<<SDL_GetError()<<std::endl;
    SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
    SDL_FreeSurface(surface);
    return texture;

}

void TextureManager::Draw(SDL_Texture* texture, SDL_Rect sourceRectangle, SDL_Rect destinationRect, SDL_RendererFlip flip)
{
    SDL_RenderCopyEx(Game::renderer, texture, &sourceRectangle, &destinationRect, 0.0, NULL, flip);
}