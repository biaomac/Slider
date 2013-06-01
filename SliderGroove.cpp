#include "SliderGroove.hpp"
#include "SliderHandle.hpp"
#include <QGraphicsSceneMouseEvent>
#include <QPainter>

SliderGroove::SliderGroove(QGraphicsItem *parent) : QGraphicsRectItem(parent) {
    handleGap = 3;
}

void SliderGroove::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event) {
    SliderHandle *handle = new SliderHandle(handleSize);
    handle->setPos(QPointF(event->pos().x(), 0));
    insertHandle(handle);
}

bool SliderGroove::sceneEventFilter(QGraphicsItem *watched, QEvent *event) {
    if (watched->type() == SliderHandle::Type) {
        SliderHandle *handle = dynamic_cast<SliderHandle *>(watched);

        if (event->type() == QEvent::GraphicsSceneMouseMove) {
            // Double click to add new handle.
            QGraphicsSceneMouseEvent *e = dynamic_cast<QGraphicsSceneMouseEvent *>(event);
            QPointF newPos = handle->mapToParent(e->pos());

            if (canHandleMove(handle, newPos)) {
                moveHandle(handle, newPos);
            }

            return true;
        } else if (event->type() == QEvent::GraphicsSceneContextMenu) {
            // Context menu event.
            if (handles.size() > 1) {
                handle->removeSceneEventFilter(this);
                handles.removeOne(handle);
                delete handle;
            }

            return true;
        }
    }

    return QGraphicsRectItem::sceneEventFilter(watched, event);
}

void SliderGroove::insertHandle(SliderHandle *handle) {
    handle->setParentItem(this);
    handle->installSceneEventFilter(this);
    handle->setNormalizedX(normalizHandleX(handle));

    int index = 0;

    foreach (SliderHandle *t, handles) {
        if (t->pos().x() < handle->pos().x()) {
            ++index;
        } else {
            break;
        }
    }

    handles.insert(index, handle);

    for (int i = 0; i < handles.size(); ++i) {
        handles[i]->setZValue(i);
    }
}

void SliderGroove::insertHandleAt(qreal normalizedX) {
    SliderHandle *handle = new SliderHandle(handleSize);
    handle->setNormalizedX(normalizedX);

    int x = getWidth() * handle->getNormalizedX() - getWidth() / 2;
    handle->setPos(QPointF(x, 0));
    insertHandle(handle);
}

qreal SliderGroove::getWidth() const {
    return (shape().boundingRect()).width();
}

qreal SliderGroove::normalizHandleX(SliderHandle *handle) const {
    qreal w = getWidth();
    qreal tp = handle->pos().x() + w / 2;

    return tp / w;
}

bool SliderGroove::canHandleMove(SliderHandle *handle, QPointF pos) {
    int mx = getWidth() / 2;
    int hx = handle->pos().x();
    int index = handles.indexOf(handle);
    bool moveLeft = pos.x() - handle->pos().x() < 0; // Judge of moving left or moving right.

    return moveLeft ?
           ((index == 0) ?                  (hx > -mx) : (hx > handles[index - 1]->pos().x() + handleGap)):
           ((index == handles.size() - 1) ? (hx < +mx) : (hx < handles[index + 1]->pos().x() - handleGap));
}

void SliderGroove::moveHandle(SliderHandle *handle, QPointF pos) {
    int w2 = getWidth() / 2;
    int x = pos.x();
    x = x < -w2 ? -w2 : x;
    x = x > +w2 ? +w2 : x;

    int index = handles.indexOf(handle);

    if (index > 0) {
        int prevX = handles[index - 1]->pos().x();
        x = x <= prevX + handleGap ? prevX + handleGap : x;
    }

    if (index < handles.size() - 1) {
        int nextX = handles[index + 1]->pos().x();
        x = x >= nextX - handleGap ? nextX - handleGap : x;
    }

    handle->setPos(x, 0);
    handle->setNormalizedX(normalizHandleX(handle));
}

void SliderGroove::updateHandles() {
    qreal w = getWidth();
    qreal x = 0;

    foreach (SliderHandle *h, handles) {
        x = h->getNormalizedX() * w - w / 2;
        h->setPos(x, 0);
        h->setSize(handleSize);
    }
}

void SliderGroove::setHandleSize(QSize size) {
    this->handleSize = size;
}

QSize SliderGroove::getHandleSize() const {
    return this->handleSize;
}

QList<int> SliderGroove::getHandleValues() const {
    QList<int> values;
    int v = 0;

    foreach (SliderHandle *h, handles) {
        v = (int) (qRound(h->getNormalizedX() * 100));
        v = v <= 0   ? 0   : v;
        v = v >= 100 ? 100 : v;

        values.append(v);
    }

    return values;
}

QRectF SliderGroove::boundingRect() const {
    return rect();
}

QPainterPath SliderGroove::shape() const {
    QPainterPath path;
    path.addRoundedRect(rect(), 4, 4);

    return path;
}

void SliderGroove::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->fillPath(shape(), Qt::gray);
}
