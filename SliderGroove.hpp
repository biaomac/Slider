#ifndef SLIDERGROOVE_HPP
#define SLIDERGROOVE_HPP

#include <QGraphicsRectItem>
#include <QList>

class SliderHandle;

class SliderGroove : public QGraphicsRectItem {
public:
    /**
     * @brief Contruct a SliderGroove instance.
     * @param parent
     */
    explicit SliderGroove(QGraphicsItem *parent = 0);

    /**
     * @brief Get the value of the handles, which are in range of [0, 100].
     * @return A list of integer values.
     */
    QList<int> getHandleValues() const;

    /**
     * @brief Set the handle's size.
     * @param size - The size of the handle.
     */
    void setHandleSize(QSize size);

    /**
     * @brief Get the handle's size.
     * @return An QSize object.
     */
    QSize getHandleSize() const;

    /**
     * @brief This method should be invoked when the groove's size is changed.
     */
    void updateHandles();

    /**
     * @brief Insert a handle at the position in the groove area.
     * @param normalizedX - A float value between [0, 1],
     *      the passed value will be clamped if outside the range.
     */
    void insertHandleAt(qreal normalizedX);

    // +----------------------------------------------------------------------+
    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    virtual bool sceneEventFilter(QGraphicsItem *watched, QEvent *event);

private:
    /**
     * @brief Get the width of the groove painting area.
     * @return A float value of the painting width.
     */
    qreal getWidth() const;

    /**
     * @brief Insert the passed handle pointer into the handles list.
     * @param handle - A pointer of the SliderHandle.
     */
    void insertHandle(SliderHandle *handle);

    /**
     * @brief Normalizing the handle's position between [0, 1] using its x coordinate.
     * @param handle - A pointer of the SliderHandle.
     * @return Return the normalized value in the range of [0, 1].
     */
    qreal normalizHandleX(SliderHandle *handle) const;

    /**
     * @brief To judge whether the handle can move to the new position.
     * @param handle - A pointer of the SliderHandle.
     * @param pos - The position the handle proposed to move.
     * @return - If can move return true, otherwise return false.
     */
    bool canHandleMove(SliderHandle *handle, QPointF pos);

    /**
     * @brief Move the specivied handle to the passed position.
     * @param handle - A pointer of the SliderHandle.
     * @param pos - The position the handle will move.
     */
    void moveHandle(SliderHandle *handle, QPointF pos);
    
    QList<SliderHandle *> handles;
    QSize handleSize;
    int handleGap;
};

#endif // SLIDERGROOVE_HPP
