#include "aqbaseitem.h"
#include <QStyle>
#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>

AQBaseItem::AQBaseItem( )
{
    setPos(0, 0);
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
        connect(rotate_item_, SIGNAL(sig_update_rotation(qreal)),size_handle, SLOT(slt_update_rotation(qreal)));
    }
    connect(rotate_item_, SIGNAL(sig_update_rotation(qreal)),this, SLOT(slt_update_rotation(qreal)));
}

void AQBaseItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(QPainter::Antialiasing, true);
    QPen pen;
    pen.setColor(Qt::black);
    painter->setPen(pen);
    painter->drawPolygon(polygon());
    painter->setPen(QPen(QColor(Qt::red), 1.0, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin));
    painter->drawPolygon(boundingRect());

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
//            AQSizeHandelRect *handle_2 = handles_.at(i-1);
//            painter->drawLine(handle_1->rect().center(),handle_2->rect().center());
        }
        handles_.at(0)->setState(true);
        rotate_item_->setState(true);
//        painter->drawLine(handles_.at(7)->rect().center(), handles_.at(0)->rect().center());

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
    QRectF b = sceneBoundingRect();
    QPointF p;
    qreal ratio_x = 1.0;
    qreal ratio_y = 1.0;
    QTransform transformations;

    // TODO: 边界判断
    // TODO: 换枚举类型
    switch (handle_cnt) {
    case 0:
        p = boundingRect().bottomRight();
        ratio_x = (b.right() - point.x()) / b.width();
        ratio_y = (b.bottom() - point.y()) / b.height();
        break;
    case 1:
        p = QPointF(boundingRect().center().x(), boundingRect().bottom());
        ratio_y = (b.bottom() - point.y()) / b.height();
        break;
    case 2:
        p = boundingRect().bottomLeft();
        ratio_x = (point.x() - b.left()) / b.width();
        ratio_y = (b.bottom() - point.y()) / b.height();
        break;
    case 3:
        p = QPointF(boundingRect().left(), boundingRect().center().y());
        ratio_x = (point.x() - b.left()) / b.width();
        break;
    case 4:
        p = boundingRect().topLeft();
        ratio_x = (point.x() - b.left()) / b.width();
        ratio_y = (point.y() - b.top()) / b.height();
        break;
    case 5:
        p = QPointF(boundingRect().center().x(), boundingRect().top());
        ratio_y = (point.y() - b.top()) / b.height();
        break;
    case 6:
        p = boundingRect().topRight();
        ratio_x = (b.right() - point.x()) / b.width();
        ratio_y = (point.y() - b.top()) / b.height();
        break;
    case 7:
        p = QPointF(boundingRect().right(), boundingRect().center().y());
        ratio_x = (point.x() - b.left()) / b.width();
        break;
    default:
        break;
    }
    transformations.translate(p.x(), p.y()).scale(ratio_x, ratio_y).translate(-p.x(), -p.y());
    setTransform(transformations, true);

    updateHandlesPos();
    update();
}

void AQBaseItem::slt_update_rotation(qreal angle)
{
    setTransformOriginPoint(boundingRect().center());
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
            handle->fromCircle(b.topLeft(), s);
            break;
        case 1:
            handle->fromCircle(QPointF(b.center().x(), b.top()), s);
            break;
        case 2:
            handle->fromCircle(b.topRight(), s);
            break;
        case 3:
            handle->fromCircle(QPointF(b.right(), b.center().y()), s);
            break;
        case 4:
            handle->fromCircle(b.bottomRight(), s);
            break;
        case 5:
            handle->fromCircle(QPointF(b.center().x(), b.bottom()), s);
            break;
        case 6:
            handle->fromCircle(b.bottomLeft(), s);
            break;
        case 7:
            handle->fromCircle(QPointF(b.left(), b.center().y()), s);
            break;
        default:
            break;
        }
    }
    rotate_item_->fromCircle(QPointF(b.center().x(), b.top()-10), s);
    rotate_item_->set_center_point(boundingRect().center());
}

PathItem::PathItem()
{
    QRectF rect(100, 100, 100, 100);
    QPolygonF polygon(rect);
    this->setPolygon(polygon);

    updateHandlesPos();
}

void PathItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    AQBaseItem::paint(painter,option, widget);
}
