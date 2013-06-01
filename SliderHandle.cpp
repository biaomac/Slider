#include "SliderHandle.hpp"
#include <QPainter>
#include <QPainterPath>

SliderHandle::SliderHandle(QSize size) {
    this->size = size;

    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
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

QPainterPath SliderHandle::shape() const {
    int w = size.width();
    int h = size.height();

    QPainterPath path;
    path.addRoundedRect(QRectF(-w / 2, -h / 2, w - 1, h - 1), w / 2, h / 2);

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
