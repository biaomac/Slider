#include "Slider.hpp"
#include "SliderGroove.hpp"
#include "SliderHandle.hpp"

#include <QResizeEvent>
#include <QGraphicsScene>
#include <QDebug>

Slider::Slider(QWidget *parent) : QGraphicsView(parent) {
    setRenderHint(QPainter::Antialiasing);

    scene = new QGraphicsScene();
    setScene(scene);

    groove = new SliderGroove();
    groove->setRect(0, 0, 10, 10);
    scene->addItem(groove);

    groove->setPos(0, 0);
    groove->insertHandleAt(0.5);
}

void Slider::resizeEvent(QResizeEvent *event) {
    QMargins margins = getMargins();
    QSize s = event->size();
    int sw = s.width();
    int sh = s.height();
    int gw = sw - margins.left() - margins.right();
    int gh = sh - margins.top() - margins.bottom();

    // Let the scene and groove fill the view window.
    scene->setSceneRect(-sw / 2, -sh / 2, sw, sh);
    groove->setRect(-gw / 2, -gh / 2, gw, gh);
    groove->setPos(0, 0);
    groove->updateHandles();
}

QMargins Slider::getMargins() const {
    return margins;
}

void Slider::setMargins(QMargins margins) {
    this->margins = margins;
}

void Slider::setHandleSize(QSize size) {
    groove->setHandleSize(size);
}

QSize Slider::getHandleSize() const {
    return groove->getHandleSize();
}

QList<int> Slider::getValues() const {
    return groove->getHandleValues();
}
