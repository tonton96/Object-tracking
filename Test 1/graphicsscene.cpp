#include "graphicsscene.h"
#include <QDebug>
#include <QGraphicsEllipseItem>
#include <QGraphicsPathItem>
#include <QPainterPath>
#include "qmath.h"

GraphicsScene::GraphicsScene(QObject *parent) :
    QGraphicsScene(parent)
{
    this->setBackgroundBrush(Qt::gray);
}

void GraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    if(this->main != nullptr){
        this->main->OnMoveMouse(mouseEvent);
    }
    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    if(this->main != nullptr){
        this->main->OnPressMouse(mouseEvent);
    }
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void GraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent * me)
{
    if(this->main!=nullptr){
        this->main->OnReleaseMouse(me);
    }
    QGraphicsScene::mouseReleaseEvent(me);
}

void GraphicsScene:: SetMain(MainWindow *qMain){
    this->main = qMain;
}
