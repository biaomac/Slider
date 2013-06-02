#ifndef SLIDERGROOVE_HPP
#define SLIDERGROOVE_HPP

#include <QGraphicsRectItem>
#include <QList>
#include <QObject>

class SliderHandle;

class SliderGroove : public QObject, public QGraphicsRectItem {
    Q_OBJECT
public:
    /**
     * @brief Contruct a SliderGroove instance.
     * @param parent
     */
    explicit SliderGroove(QGraphicsItem *parent = 0);

    ~SliderGroove();

    /**
     * @brief Get the value of the handles, which are in range of [0, 100].
     * @return A list of integer values.
     */
    QList<int> getHandleValues() const;

    /**
     * @brief Set the round handle's size.
     * @param size - The size of the handle.
     */
    void setRoundHandleSize(QSize size);

    /**
     * @brief Get the round handle's size.
     * @return An QSize object.
     */
    QSize getRoundHandleSize() const;

    /**
     * @brief Set the mark handle's size.
     * @param size - The size of the handle.
     */
    void setMarkHandleSize(QSize size);

    /**
     * @brief Get the mark handle's size.
     * @return An QSize object.
     */
    QSize getMarkHandleSize() const;

    /**
     * @brief This method should be invoked when the groove's size is changed.
     */
    void updateHandles();

    /**
     * @brief Insert a handle at the position in the groove area.
     * @param normalizedX - A float value between [0, 1],
     *      the passed value will be clamped if outside the range.
     * @param markHandle - Indicate whether the handle is mark.
     */
    void insertHandleAt(qreal normalizedX, bool markHandle);

    // +----------------------------------------------------------------------+
    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent * event);
    virtual bool sceneEventFilter(QGraphicsItem *watched, QEvent *event);

private slots:
    void deleteHandle(SliderHandle *handle);

private:
    /**
     * @brief Get the size of the groove painting area.
     * @return An size object of the painting area.
     */
    QSizeF getSize() const;

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
    qreal normalizHandleX(qreal x) const;

    /**
     * @brief To judge whether the handle can move to the new position.
     * @param handle - A pointer of the SliderHandle.
     * @param pos - The position the handle proposed to move.
     * @return - If can move return true, otherwise return false.
     */
    bool canMoveHandle(SliderHandle *handle, QPointF pos) const;
    bool canMoveMarkHandle(SliderHandle *handle, QPointF pos) const;
    bool canMoveRoundHandle(SliderHandle *handle, QPointF pos) const;

    /**
     * @brief Move the specivied handle to the passed position.
     * @param handle - A pointer of the SliderHandle.
     * @param pos - The position the handle will move.
     */
    void moveHandle(SliderHandle *handle, QPointF pos);
    void moveMarkHandle(SliderHandle *handle, QPointF pos);
    void moveRoundHandle(SliderHandle *handle, QPointF pos);

    int findPreviousHandleIndex(SliderHandle *handle, bool markHandle) const;
    int findNextHandleIndex(SliderHandle *handle, bool markHandle) const;
    
    QList<SliderHandle *> handles;
    QSize roundHandleSize;
    QSize markHandleSize;
    int handleGap;
};

#endif // SLIDERGROOVE_HPP
