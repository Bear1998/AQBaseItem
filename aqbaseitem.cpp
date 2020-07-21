#include "aqbaseitem.h"
#include <QStyle>
#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>

AQBaseItem::AQBaseItem( )
{
    setPos(0,0);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);
    setAcceptHoverEvents(true);

    rotate_item_ = new AQRotateItem(this);
    for (int i = 0;i< 8;i++)
    {
        AQSizeHandelRect *size_handle = new AQSizeHandelRect(this, i);
        handles_.push_back(size_handle);
        connect(size_handle,SIGNAL(sig_resize_point(int,QPointF)),this, SLOT(slt_resize_point(int, QPointF)));
        connect(rotate_item_, SIGNAL(sig_update_retation(qreal)),size_handle, SLOT(slt_update_rotation(qreal)));
    }
    connect(rotate_item_, SIGNAL(sig_update_retation(qreal)),this, SLOT(slt_update_rotation(qreal)));
}

QRectF AQBaseItem::boundingRect() const
{
    qreal size = handle_size / 2;
    return this->rect().adjusted(-size, -size, size, size);

}

QRectF AQBaseItem::rect() const
{
    return local_rect_;
}

void AQBaseItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(QPainter::Antialiasing, true);
    QPen pen;
    pen.setColor(Qt::black);
    painter->setPen(pen);
    painter->drawPath(this->path());
    painter->setPen(QPen(QColor(Qt::red), 1.0, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin));
    painter->drawRect(local_rect_);

    if (option->state & QStyle::State_Selected) {
        painter->setRenderHint(QPainter::Antialiasing, true);
        if (option->state & QStyle::State_HasFocus)
            painter->setPen(Qt::yellow);
        else
            painter->setPen(Qt::white);
        painter->setPen(QPen(QColor(Qt::black), 1.0, Qt::DotLine, Qt::SquareCap, Qt::MiterJoin));
        for (int i = 1;i<handles_.size();i++){
            AQSizeHandelRect *handle_1 = handles_.at(i);
            handle_1->setState(true);
            AQSizeHandelRect *handle_2 = handles_.at(i-1);
            painter->drawLine(handle_1->rect().center(),handle_2->rect().center());
        }
        handles_.at(0)->setState(true);
        rotate_item_->setState(true);
        painter->drawLine(handles_.at(7)->rect().center(), handles_.at(0)->rect().center());

        painter->setRenderHint(QPainter::Antialiasing, false);
    }else{
        for (int i = 0;i<handles_.size();i++){
            handles_.at(i)->setState(false);
        }
        rotate_item_->setState(false);
    }
}

void AQBaseItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);
}

void AQBaseItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsItem::hoverMoveEvent(event);
}

void AQBaseItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
}

void AQBaseItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
}

void AQBaseItem::slt_resize_point(int handle_cnt, QPointF point)
{
    QRectF b = rect();
    QTransform transformations;
    switch (handle_cnt) {
    case 0:
        local_rect_.setTopLeft(point);
        break;
    case 1:
        local_rect_.setTop(point.y());
        break;
    case 2:
        local_rect_.setTopRight(point);
        break;
    case 3:
        transformations.translate(-b.topLeft().x(), 0);
        transformations.scale((point.x() - b.left()) / b.width(), 1.0);
        transformations.translate(b.topLeft().x(), 0);
        local_rect_.setRight(point.x());
        break;
    case 4:
        local_rect_.setBottomRight(point);
        break;
    case 5:
        local_rect_.setBottom(point.y());
        break;
    case 6:
        local_rect_.setBottomLeft(point);
        break;
    case 7:
        local_rect_.setLeft(point.x());
        break;
    default:
        break;
    }
    this->setTransform(transformations);

    updateHandlesPos();
}

void AQBaseItem::set_local_rect(QRectF rect)
{
    local_rect_ = rect;
    updateHandlesPos();
}

void AQBaseItem::set_painter_path(QPainterPath path)
{
    painter_path_ = path;
}

void AQBaseItem::slt_update_rotation(qreal angle)
{
    setRotation(angle);
}

void AQBaseItem::updateHandlesPos()
{
    qreal s = handle_size;
    QRectF b = boundingRect();
    for(int i = 0;i<handles_.size(); i++){
        AQSizeHandelRect *handle = handles_.at(i);
        switch (i) {
        case 0:
            handle->setRect(QRectF(b.left(), b.top(), s, s));
            break;
        case 1:
            handle->setRect(QRectF(b.center().x() - s / 2, b.top(), s, s));
            break;
        case 2:
            handle->setRect(QRectF(b.right() - s, b.top(), s, s));
            break;
        case 3:
            handle->setRect(QRectF(b.right() - s, b.center().y() - s / 2, s, s));     
            break;
        case 4:
            handle->setRect(QRectF(b.right() - s, b.bottom() - s, s, s));
            break;
        case 5:
            handle->setRect(QRectF(b.center().x() - s / 2, b.bottom() - s, s, s));
            break;
        case 6:
            handle->setRect(QRectF(b.left(), b.bottom() - s, s, s));
            break;
        case 7:
            handle->setRect(QRectF(b.left(), b.center().y() - s / 2, s, s));
            break;
        default:
            break;
        }
    }
    rotate_item_->setRect(QRectF(b.center().x() - s / 2, b.top() -10, s, s));
    rotate_item_->set_center_point(local_rect_.center());
    setTransformOriginPoint(local_rect_.center());
}

PathItem::PathItem()
{
    QPen pen(Qt::yellow, 1, Qt::SolidLine);
    pen.setBrush(Qt::yellow);
    QPainterPath path;
    path.addRect(QRect(100,100, 100, 100));
    set_local_rect(path.boundingRect());
//    this->set_painter_path(path);
    this->setPath(path);

}

void PathItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    AQBaseItem::paint(painter,option, widget);
}
