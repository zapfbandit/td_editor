#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>

class Settings : public QSettings
{
public:
   Settings();

public:
   bool ShowGrid() const;
   void ShowGrid(const bool showGrid);

   QString MapPath() const;
   void MapPath(const QString& mapPath);

   uint32_t MapWidth() const;
   void MapWidth(const uint32_t mapWidth);

   uint32_t MapHeight() const;
   void MapHeight(const uint32_t mapHeight);

   QString TilesPath() const;
   void TilesPath(const QString& tilesPath);

   QString SpritesPath() const;
   void SpritesPath(const QString& spritesPath);

   bool IgnoreIndexWarning() const;
   void IgnoreIndexWarning(const bool ignoreIndexWarning);

   QByteArray WindowState() const;
   void WindowState(const QByteArray& windowState);

   QByteArray WindowGeometry() const;
   void WindowGeometry(const QByteArray& windowGeometry);

};

#endif // SETTINGS_H
