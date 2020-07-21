#include "aqsizehandelrect.h"
#include "aqbaseitem.h"
#include <QPainter>
#include <QDebug>
#include <QCursor>
AQSizeHandelRect::AQSizeHandelRect(QGraphicsItem *parent, int d) : QGraphicsRectItem(parent),dir_(d)
{
    this->setAcceptHoverEvents(true);
    hide();
}

void AQSizeHandelRect::setState(bool selected)
{
    if(selected){
        show();
    }else {
        hide();
    }
}

void AQSizeHandelRect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
    painter->setPen(Qt::SolidLine);

    painter->setRenderHint(QPainter::Antialiasing, true);
//    static QPixmap pixmapScale(":/resource/scale.png");
//    painter->setRenderHints(QPainter::SmoothPixmapTransform);
//    painter->drawPixmap(this->rect().toRect(),pixmapScale);

    QPen pen(Qt::black, 1, Qt::SolidLine);
    pen.setBrush(Qt::black);
    painter->setPen(pen);
    painter->drawEllipse(rect());
    painter->restore();

}

void AQSizeHandelRect::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    QCursor cursor;
    if (m_isResizing_ || (get_cursor(event->pos(), cursor))){
        setCursor(cursor);
    }else{
        setCursor(Qt::ArrowCursor);
    }
    QGraphicsItem::hoverMoveEvent(event);
}

void AQSizeHandelRect::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    static qreal z = 0.0;
    setZValue(z += 1.0);
    QCursor cursor;
    if (event->button() == Qt::LeftButton && get_cursor(event->pos(), cursor)) {
       m_isResizing_ = true;
    }else{
       QGraphicsItem::mousePressEvent(event);
    }
}

void AQSizeHandelRect::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_isResizing_) {
        prepareGeometryChange();
        emit sig_resize_point(dir_, event->pos());
    }else {
        QGraphicsItem::mouseMoveEvent(event);
    }
}

void AQSizeHandelRect::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_isResizing_) {
        m_isResizing_ = false;
    } else {
        QGraphicsItem::mouseReleaseEvent(event);
    }
}

bool AQSizeHandelRect::get_cursor(const QPointF &pos, QCursor &cursor)
{
    int num = dir_;
    if(this->rect().contains(pos)){
        if(angle_){
            int change = round(angle_ / 45);
            num = (dir_ + change) % 8;
        }
        cursor = handleCursors[num];
        return true;
    }
    return false;
}

void AQSizeHandelRect::slt_update_rotation(qreal angle)
{
    angle_ = angle;
}


AQRotateItem::AQRotateItem(QGraphicsItem *parent): QGraphicsRectItem(parent)
{
    this->setAcceptHoverEvents(true);
    hide();
}

void AQRotateItem::setState(bool selected)
{
    if(selected){
        show();
    }else {
        hide();
    }
}

void AQRotateItem::set_center_point(QPointF point)
{
    center_point_ = point;
}

void AQRotateItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
    painter->setPen(Qt::SolidLine);

    painter->setRenderHint(QPainter::Antialiasing, true);

    QPen pen(Qt::black, 1, Qt::SolidLine);
    pen.setBrush(Qt::black);
    painter->setPen(pen);
    painter->drawEllipse(rect());
    painter->restore();
}

void AQRotateItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if (m_isRotating_ || this->rect().contains(event->pos())){
        setCursor(Qt::ClosedHandCursor);
    }else{
        setCursor(Qt::ArrowCursor);
    }
    QGraphicsItem::hoverMoveEvent(event);
}

void AQRotateItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    press_point_ = event->pos();
    static qreal z = 0.0;
    setZValue(z += 1.0);
    if (event->button() == Qt::LeftButton && this->rect().contains(event->pos())) {
        m_isRotating_ = true;
        press_rotate_ = current_rotate_;
        previous_anglea_ = 360.0;
        previous_point_ = press_point_;
    }else{
       QGraphicsItem::mousePressEvent(event);
    }
}

void AQRotateItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_isRotating_){
        prepareGeometryChange();
        item_rotate(event->pos());
    }else {
        QGraphicsItem::mouseMoveEvent(event);
    }
}

void AQRotateItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_isRotating_) {
        m_isRotating_ = false;
    } else {
        QGraphicsItem::mouseReleaseEvent(event);
    }
}

void AQRotateItem::item_rotate(QPointF current_point)
{
    if(press_point_ == current_point){
        return;
    }
    double current_anglea = aq_anglea(center_point_, press_point_, current_point);
    qDebug()<<center_point_<<press_point_<<current_point;

    if(current_anglea < previous_anglea_){
        press_point_ = previous_point_;
        press_rotate_ = current_rotate_;

        current_anglea = aq_anglea(center_point_, press_point_, current_point);
    }

    double current_direction = aq_direction(previous_point_, current_point);
    current_rotate_ = press_rotate_ + current_anglea * current_direction;

    while(current_rotate_ > 360.0)
        current_rotate_ -= 360.0;
    while(current_rotate_ < 0.0)
        current_rotate_ += 360.0;

    previous_point_ = current_point;
    previous_anglea_ = current_anglea;

    sig_update_retation(current_rotate_);
}

double AQRotateItem::aq_anglea(QPointF a, QPointF b, QPointF c)
{
    double ab = aq_length(a, b);
    double ac = aq_length(a, c);
    double bc = aq_length(b, c);

    double cos_a = (ab * ab + ac * ac - bc * bc) / (2 * ab * ac);

    return acos(cos_a) * (180.0 / 3.141592653);
}

double AQRotateItem::aq_square(double x)
{
    return x * x;
}

double AQRotateItem::aq_length(QPointF a, QPointF b)
{
    return sqrt(aq_square(a.x() - b.x()) + aq_square(a.y() - b.y()));
}

double AQRotateItem::aq_direction(QPointF a, QPointF b)
{
    QPointF oa(center_point_.x() - a.x(), center_point_.y() - a.y());
    QPointF ob(center_point_.x() - b.x(), center_point_.y() - b.y());

    double ans = oa.x() * ob.y()- oa.y() * ob.x();

    if(ans > 0)
        return 1.0;
    else
        return -1.0;
}
