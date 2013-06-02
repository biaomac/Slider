#include "Slider.hpp"
#include "SliderGroove.hpp"
#include "SliderHandle.hpp"

#include <QResizeEvent>
#include <QGraphicsScene>
#include <QList>
#include <QDebug>

Slider::Slider(QWidget *parent) : QGraphicsView(parent) {
    setRenderHint(QPainter::Antialiasing);

    scene = new QGraphicsScene();
    setScene(scene);

    groove = new SliderGroove();
    groove->setRect(0, 0, 10, 10);
    scene->addItem(groove);

    groove->setPos(0, 0);
    groove->insertHandleAt(0.5, false);
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

void Slider::setRoundHandleSize(QSize size) {
    groove->setRoundHandleSize(size);
}

QSize Slider::getRoundHandleSize() const {
    return groove->getRoundHandleSize();
}

QList<int> Slider::getValues() const {
    return groove->getHandleValues();
}

void Slider::setMarkHandleSize(QSize size) {
    return groove->setMarkHandleSize(size);
}

QSize Slider::getMarkHandleSize() const {
    return groove->getMarkHandleSize();
}

void Slider::hideSelectedHandles() {
    QList<QGraphicsItem *> items = scene->selectedItems();

    foreach (QGraphicsItem *i, items) {
        SliderHandle *handle = dynamic_cast<SliderHandle *>(i);
        if (handle->isMark() && handle->type() == SliderHandle::Type) {
            handle->hide();
        }
    }
}

void Slider::showAllHandles() {
    QList<QGraphicsItem *> items = scene->items();

    foreach (QGraphicsItem *i, items) {
        i->show();
    }
}
