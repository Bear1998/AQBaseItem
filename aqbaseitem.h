#ifndef AQBASEITEM_H
#define AQBASEITEM_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include "aqsizehandelrect.h"
const Qt::CursorShape handleCursors[] = {
    Qt::SizeFDiagCursor,
    Qt::SizeVerCursor,
    Qt::SizeBDiagCursor,
    Qt::SizeHorCursor,
    Qt::SizeFDiagCursor,
    Qt::SizeVerCursor,
    Qt::SizeBDiagCursor,
    Qt::SizeHorCursor,
};

typedef QVector<AQSizeHandelRect*> Handles;
class AQBaseItem : public QObject, public QGraphicsPathItem
{
    Q_OBJECT
public:
    AQBaseItem();
    QRectF boundingRect() const;
    QRectF rect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;

    void set_local_rect(QRectF rect);
    void set_painter_path(QPainterPath path);
private slots:
    void slt_update_rotation(qreal angle);
    void slt_resize_point(int handle_cnt, QPointF point);
private:

protected:
    int handle_cnt_ = -1;
    QRectF local_rect_;
private:
    void updateHandlesPos();
    void update_localrect(QPointF point);
private:
    Handles handles_;
    int handle_size = 10;
    AQRotateItem *rotate_item_ = Q_NULLPTR;

    QPainterPath painter_path_;
};


class PathItem : public AQBaseItem
{
public:
    PathItem();
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;

};
#endif // AQBASEITEM_H
