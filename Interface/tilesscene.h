#pragma once

#include <QGraphicsScene>


class TilesScene : public QGraphicsScene
{
   Q_OBJECT

public:
   explicit TilesScene(QObject *parent = nullptr);
};
