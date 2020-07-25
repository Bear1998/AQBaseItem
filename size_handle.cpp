#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>
#include <qdebug.h>
#include <QtWidgets>

#include "size_handle.h"
#include "graphics_obj.h"

SizeHandleRect::SizeHandleRect(QGraphicsItem* parent , int d, bool control):
    QGraphicsRectItem(-SELECTION_HANDLE_SIZE/2,
                       -SELECTION_HANDLE_SIZE/2,
                       SELECTION_HANDLE_SIZE,
                       SELECTION_HANDLE_SIZE, parent),
    m_dir(d),
    m_controlPoint(control),
    m_state(SelectionHandleOff),
    borderColor("white")
{
    this->setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemIgnoresTransformations,true);
    hide();
}


void SizeHandleRect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
    painter->setPen(Qt::SolidLine);
    painter->setBrush(QBrush(borderColor));

    painter->setRenderHint(QPainter::Antialiasing,false);

    if ( m_controlPoint  )
    {
        painter->setPen(QPen(Qt::red,Qt::SolidLine));
        painter->setBrush(Qt::green);
        painter->drawEllipse(rect().center(),3,3);
    }else
        painter->drawRect(rect());
    painter->restore();
}


void SizeHandleRect::setState(SelectionHandleState st)
{
    if (st == m_state)
        return;
    switch (st) {
    case SelectionHandleOff:
        hide();
        break;
    case SelectionHandleInactive:
    case SelectionHandleActive:
        show();
        break;
    }
    borderColor = Qt::white;
    m_state = st;
}

void SizeHandleRect::move(qreal x, qreal y)
{   
    setPos(x,y);
}

void SizeHandleRect::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    borderColor = Qt::white;
    update();
    QGraphicsRectItem::hoverLeaveEvent(event);
}

void SizeHandleRect::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    borderColor = Qt::blue;
    update();
    setCursor(Qt::OpenHandCursor);
    QGraphicsItem::hoverMoveEvent(event);
}

void SizeHandleRect::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
       m_isResizing_ = true;
       setCursor(Qt::ClosedHandCursor);
       c_down_ = event->scenePos();
       c_last_ = event->scenePos();
    }
    // TODO: delete
//    else if (event->button() == Qt::RightButton)
//    {
//        GraphicsItem *item = static_cast<GraphicsItem *>(parentItem());
//        static double angle = 0;
//        angle += 45;
//        if (angle > 360)
//            angle -= 360;
//        item->setTransformOriginPoint(item->mapFromScene(item->sceneBoundingRect().center()));
//        item->setRotation(angle);
//        item->updateCoordinate();
//    }
    else{
       QGraphicsItem::mousePressEvent(event);
    }
}

void SizeHandleRect::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    GraphicsItem *item = static_cast<GraphicsItem *>(parentItem());
    if (m_isResizing_) {
        c_last_ = event->scenePos();
        QPointF opposite = item->opposite(this->dir());
        QPointF new_delta = item->mapFromScene(c_last_) - opposite;
        QPointF init_delta = item->mapFromScene(c_down_) - opposite;
        double sx = new_delta.x() / init_delta.x();
        double sy = new_delta.y() / init_delta.y();
        item->stretch(this->dir(), sx, sy, opposite);
    }else {
        QGraphicsItem::mouseMoveEvent(event);
    }
}

void SizeHandleRect::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_isResizing_) {
        GraphicsItem *item = static_cast<GraphicsItem *>(parentItem());
        item->updateCoordinate();
        m_isResizing_ = false;
        setCursor(Qt::OpenHandCursor);
    } else {
        QGraphicsItem::mouseReleaseEvent(event);
    }
}

