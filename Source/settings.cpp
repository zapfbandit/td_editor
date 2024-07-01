#include "settings.h"

#include <QDir>


Settings::Settings():
   QSettings("TD Editor Settings.ini",
             QSettings::Format::IniFormat)
{
   ShowGrid(ShowGrid());
   MapPath(MapPath());
   MapWidth(MapWidth());
   MapHeight(MapHeight());
   TilesPath(TilesPath());
   SpritesPath(SpritesPath());
   IgnoreIndexWarning(IgnoreIndexWarning());
   WindowState(WindowState());
   WindowGeometry(WindowGeometry());
}


bool Settings::ShowGrid() const
{
   return value("map/showGrid", true).toBool();
}


void Settings::ShowGrid(const bool showGrid)
{
   setValue("map/showGrid", showGrid);

   sync();
}


QString Settings::MapPath() const
{
   return value("map/path", "").toString();
}


void Settings::MapPath(const QString& mapPath)
{
   setValue("map/path", QDir().relativeFilePath(mapPath));

   sync();
}


uint32_t Settings::MapWidth() const
{
   return value("map/width", "8").toUInt();
}


void Settings::MapWidth(const uint32_t mapWidth)
{
   setValue("map/width", mapWidth);

   sync();
}


uint32_t Settings::MapHeight() const
{
   return value("map/height", "8").toUInt();
}


void Settings::MapHeight(const uint32_t mapHeight)
{
   setValue("map/height", mapHeight);

   sync();
}


QString Settings::TilesPath() const
{
   return value("tiles/path", "Tiles").toString();
}


void Settings::TilesPath(const QString& tilesPath)
{
   setValue("tiles/path", QDir().relativeFilePath(tilesPath));

   sync();
}


QString Settings::SpritesPath() const
{
   return value("sprites/path", "Sprites").toString();
}


void Settings::SpritesPath(const QString& spritesPath)
{
   setValue("sprites/path", QDir().relativeFilePath(spritesPath));

   sync();
}


bool Settings::IgnoreIndexWarning() const
{
   return value("dialogs/ignoreIndexWarning", "false").toBool();
}


void Settings::IgnoreIndexWarning(const bool ignoreIndexWarning)
{
   setValue("dialogs/ignoreIndexWarning", ignoreIndexWarning);

   sync();
}


QByteArray Settings::WindowState() const
{
   return value("window/state", "").toByteArray();
}


void Settings::WindowState(const QByteArray& windowState)
{
   setValue("window/state", windowState);

   sync();
}


QByteArray Settings::WindowGeometry() const
{
   return value("window/geometry", "").toByteArray();
}


void Settings::WindowGeometry(const QByteArray& windowGeometry)
{
   setValue("window/geometry", windowGeometry);

   sync();
}
