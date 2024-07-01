#pragma once

#include <stdint.h>

#include "tile.h"


class TileStore
{
public:

   TileStore(const QString& tilesDirPath);
   ~TileStore();

public:
   QString TilesDirPath() const;
   uint32_t NumTiles();
   Tile& GetTile(uint32_t i);
   uint32_t GetTileIndex(const QString& relFilePath);

private:
   QString tilesDirPath_;
   uint32_t numTiles_;
   Tile* tiles_;
   QString* tileNameMap_;
};

