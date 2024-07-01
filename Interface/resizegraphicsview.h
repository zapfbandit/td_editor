#pragma once

#include <QGraphicsView>


class ResizeGraphicsView : public QGraphicsView
{
   Q_OBJECT

public:
   ResizeGraphicsView(QWidget* parent);

protected:
   virtual void resizeEvent(QResizeEvent *event) override;
   virtual void mousePressEvent(QMouseEvent *event) override;

signals:
   void Resize();
   void Press(const QPoint& pos);
};

