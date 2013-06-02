#include "SliderHandle.hpp"
#include <QPainter>
#include <QPainterPath>
#include <QGraphicsSceneContextMenuEvent>

#include <QAction>
#include <QMenu>

SliderHandle::SliderHandle(QSize size, bool mark) {
    this->size = size;
    this->mark = mark;

    this->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);

    menu = new QMenu();
    deleteAction = menu->addAction("Delete");
    toggleMovableAction = menu->addAction("Toggle movable");
    toggleMovableAction->setCheckable(true);

    setMovable(true);

    connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteHandle()));
    connect(toggleMovableAction, SIGNAL(toggled(bool)), this, SLOT(setMovable(bool)));
}

SliderHandle::~SliderHandle() {
}

QRectF SliderHandle::boundingRect() const {
    int w = size.width();
    int h = size.height();

    return QRectF(-w / 2 - 2, -h / 2 - 2, w + 4, h + 4);
}

int SliderHandle::type() const {
    return Type;
}

void SliderHandle::setNormalizedX(qreal normalizedX) {
    normalizedX = normalizedX <= 0 ? 0 : normalizedX;
    normalizedX = normalizedX >= 1 ? 1 : normalizedX;

    this->normalizedX = normalizedX;
}

qreal SliderHandle::getNormalizedX() const {
    return this->normalizedX;
}

QSize SliderHandle::getSize() const {
    return size;
}

void SliderHandle::setSize(QSize size) {
    this->size = size;
}

bool SliderHandle::isMark() const {
    return mark;
}

void SliderHandle::setMark(bool mark) {
    this->mark = mark;
}

bool SliderHandle::isMovable() const {
    return this->movable;
}

void SliderHandle::setMovable(bool movable) {
    this->movable = movable;
    this->toggleMovableAction->setChecked(movable);
}

void SliderHandle::deleteHandle() {
    emit handleShouldBeDeleted(this);
}

void SliderHandle::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {
    if (isMark()) {
        menu->popup(event->screenPos());
    }
}

QPainterPath SliderHandle::shape() const {
    QPainterPath path;
    int w = size.width();
    int h = size.height();

    if (mark) {
        path.moveTo(0,     -h / 2 - 2);
        path.lineTo(-w / 2, h / 2 - 2);
        path.lineTo( w / 2, h / 2 - 2);
        path.closeSubpath();
    } else {
        path.addRoundedRect(QRectF(-w / 2, -h / 2, w - 1, h - 1), w / 2, h / 2);
    }

    return path;
}

void SliderHandle::paint(QPainter *painter,
                        const QStyleOptionGraphicsItem *option,
                        QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QRadialGradient rg(0, 0, size.width() * 0.55);
    rg.setColorAt(0, Qt::black);
    rg.setColorAt(0.8, Qt::red);
    painter->fillPath(shape(), rg);

    painter->setPen(isSelected() ? Qt::black : Qt::darkGray);
    painter->drawPath(shape());
}
