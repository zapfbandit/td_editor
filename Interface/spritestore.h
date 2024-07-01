#pragma once

#include <stdint.h>

#include <QDir>

#include "sprite.h"


class SpriteStore
{
public:

   SpriteStore(const QString& spritesDirPath);
   ~SpriteStore();

public:

   void GetNumSprites(const QString& dirPath);
   void LoadSprites(const QString& dirPath);

public:

   QString SpritesDirPath() const;
   uint32_t NumSprites();
   Sprite& GetSprite(uint32_t i);
   uint32_t GetSpriteIndex(const QString& relFilePath);

private:

   QString spritesDirPath_;
   QDir spritesDir_;

   uint32_t numSprites_;
   uint32_t offset_;
   Sprite* sprites_;
   QString* spriteNameMap_;
};

