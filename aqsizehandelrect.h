#ifndef AQSIZEHANDELRECT_H
#define AQSIZEHANDELRECT_H

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
enum SelectionHandleState { SelectionHandleOff, SelectionHandleInactive, SelectionHandleActive };
enum HandlePosition{ Handle_None = 0 , LeftTop , Top, RightTop, Right, RightBottom, Bottom, LeftBottom, Left };
class AQSizeHandelRect : public QObject,public QGraphicsRectItem
{
    Q_OBJECT
public:
    explicit AQSizeHandelRect(QGraphicsItem *parent , int d);
    void setState(bool selected);
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
private:
    bool get_cursor(const QPointF &pos, QCursor &cursor);
signals:
    void sig_resize_point(int dir, QPointF point);
public slots:
    void slt_update_rotation(qreal angle);
private:
    const int dir_;
    bool m_isResizing_ = false;
    qreal angle_ = 0;
};

class AQRotateItem : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    explicit AQRotateItem(QGraphicsItem * parent = 0);
    void setState(bool selected);
    void set_center_point(QPointF point);
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
private:
    void item_rotate(QPointF current_point);

    double aq_anglea(QPointF a, QPointF b, QPointF c);
    double aq_square(double x);
    double aq_length(QPointF a, QPointF b);
    double aq_direction(QPointF a, QPointF b);
signals:
    void sig_update_retation(qreal angle);
private:
    bool m_isRotating_ = false;

    double press_rotate_;
    double previous_anglea_;
    QPointF previous_point_;
    QPointF center_point_;
    double current_rotate_ = 0;
    QPointF press_point_;
};

#endif // AQSIZEHANDELRECT_H
