#ifndef SLIDER_HPP
#define SLIDER_HPP

#include <QGraphicsView>
#include <QMargins>
#include <QList>

class QGraphicsScene;
class SliderGroove;


class Slider : public QGraphicsView {
    Q_OBJECT
public:
    explicit Slider(QWidget *parent = 0);

    QList<int> getValues() const;

    /**
     * @brief Set the margins around the groove.
     *      Because the setContentsMargins method dosen't work for QGraphicsView
     *      that's the reason we define this method.
     * @param margins - An QMargins instance.
     */
    void setMargins(QMargins margins);

    /**
     * @brief Get the margins around the groove.
     * @return
     */
    QMargins getMargins() const;

    /**
     * @brief Set the handle's size.
     * @param size - An QSize object.
     */
    void setRoundHandleSize(QSize size);

    /**
     * @brief Get the size of handle.
     * @return An QSize object.
     */
    QSize getRoundHandleSize() const;

    void setMarkHandleSize(QSize size);
    QSize getMarkHandleSize() const;

    void hideSelectedHandles();
    void showAllHandles();

protected:
    /**
     * @brief When the view's size changed, we should update the scene's size and
     *      groove's size since updateing the handle's position.
     * @param event
     */
    virtual void resizeEvent(QResizeEvent *event);
    
public:
    QGraphicsScene *scene;
    SliderGroove   *groove;
    QMargins        margins;
};

#endif // SLIDER_HPP
