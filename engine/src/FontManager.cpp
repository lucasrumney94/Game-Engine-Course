#include "./FontManager.h"

TTF_Font* FontManager::LoadFont(const char* filePath, int fontSize)
{
    return TTF_OpenFont(filePath, fontSize);
}

void FontManager::Draw(SDL_Texture *texture, SDL_Rect position)
{
    // Null uses the entire texture as a source
    SDL_RenderCopy(Game::renderer, texture, NULL, &position);
}   