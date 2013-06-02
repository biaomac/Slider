#ifndef SLIDERHANDLE_HPP
#define SLIDERHANDLE_HPP

#include <QGraphicsObject>

class QMenu;
class QAction;

class SliderHandle : public QGraphicsObject  {
    Q_OBJECT
public:
    explicit SliderHandle(QSize size, bool mark = false);
    ~SliderHandle();

    enum { Type = UserType + 1 };

    /**
     * @brief Get the identify value for this item.
     * @return An integer value.
     */
    int type() const;

    /**
     * @brief Set the normarlized value in the range of [0, 1].
     * @param normalizedX - a float value between 0 and 1.
     *      If the value is out of range 0 and 1 it will be clamped.
     */
    void setNormalizedX(qreal normalizedX);

    /**
     * @brief Get the handle's normalized value.
     * @return
     */
    qreal getNormalizedX() const;

    /**
     * @brief Get the handle's size.
     * @return An object of QSize.
     */
    QSize getSize() const;

    bool isMark() const;
    void setMark(bool mark);

    bool isMovable() const;

    /**
     * @brief Set the handle's size.
     * @param size An object of QSize.
     */
    void setSize(QSize size);

    // +----------------------------------------------------------------------+
    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

public slots:
    void setMovable(bool movable);
    void deleteHandle();

signals:
    void handleShouldBeDeleted(SliderHandle *handle);

protected:
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

private:
    QSize  size;
    bool   mark;
    bool   movable;
    qreal  normalizedX;

    QMenu   *menu;
    QAction *deleteAction;
    QAction *toggleMovableAction;
};

#endif // SLIDERHANDLE_HPP
