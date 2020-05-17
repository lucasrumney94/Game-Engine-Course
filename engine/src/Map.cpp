#include <fstream>

#include "./Game.h"
#include "./Map.h"
#include "./EntityManager.h"
#include "./Components/TileComponent.h"

// externally defined manager
extern EntityManager manager;

Map::Map(std::string textureID, int scale, int tileSize)
{
    this->textureID = textureID;
    this->scale = scale;
    this->tileSize = tileSize;
}

void Map::LoadMap(std::string filePath, int mapSizeX, int mapSizeY)
{
    // read the map file
    std::fstream mapFile;
    mapFile.open(filePath);

    for (int y = 0; y < mapSizeY; y++)
    {
        for (int x = 0; x < mapSizeX; x++)
        {
            char ch;
            
            //first character in pair is Y
            mapFile.get(ch);
            int sourceRectY = atoi(&ch)*tileSize;

            //second character in pair is x
            mapFile.get(ch);
            int sourceRectX = atoi(&ch)*tileSize;

            AddTile(sourceRectX, sourceRectY, x*scale*tileSize, y*scale*tileSize);

            //ignore the comma
            mapFile.ignore();
        } 
    }

    mapFile.close();
}

void Map::AddTile(int sourceRectX, int sourceRectY, int x, int y)
{
    // add new tile to game scene.
    Entity& newTile(manager.AddEntity("Tile"));
    newTile.AddComponent<TileComponent>(sourceRectX, sourceRectY, x, y, tileSize, scale, textureID);
}