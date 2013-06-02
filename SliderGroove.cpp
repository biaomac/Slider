#include "SliderGroove.hpp"
#include "SliderHandle.hpp"
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QDebug>

SliderGroove::SliderGroove(QGraphicsItem *parent) : QGraphicsRectItem(parent) {
    handleGap = 3;
}

SliderGroove::~SliderGroove() {
}

void SliderGroove::contextMenuEvent(QGraphicsSceneContextMenuEvent * event) {
    insertHandleAt(normalizHandleX(event->pos().x()), true);
}

bool SliderGroove::sceneEventFilter(QGraphicsItem *watched, QEvent *event) {
    if (watched->type() == SliderHandle::Type) {
        SliderHandle *handle = dynamic_cast<SliderHandle *>(watched);

        if (event->type() == QEvent::GraphicsSceneMouseMove) {
            // Double click to add new handle.
            QGraphicsSceneMouseEvent *e = dynamic_cast<QGraphicsSceneMouseEvent *>(event);
            QPointF newPos = handle->mapToParent(e->pos());

            if (canMoveHandle(handle, newPos)) {
                moveHandle(handle, newPos);
            }

            return true;
        }
    }

    return QGraphicsRectItem::sceneEventFilter(watched, event);
}

void SliderGroove::deleteHandle(SliderHandle *handle) {
    handle->removeSceneEventFilter(this);
    handles.removeOne(handle);
    delete handle;
}

void SliderGroove::insertHandle(SliderHandle *handle) {
    handle->setParentItem(this);
    handle->installSceneEventFilter(this);
    handle->setNormalizedX(normalizHandleX(handle->pos().x()));

    connect(handle, SIGNAL(handleShouldBeDeleted(SliderHandle*)), this, SLOT(deleteHandle(SliderHandle*)));

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

    updateHandles();
}

void SliderGroove::insertHandleAt(qreal normalizedX, bool markHandle) {
    SliderHandle *handle = new SliderHandle(markHandle ? markHandleSize : roundHandleSize, markHandle);
    handle->setNormalizedX(normalizedX);

    int w = getSize().width();
    int x = w * handle->getNormalizedX() - w / 2;
    handle->setPos(QPointF(x, 0));
    insertHandle(handle);
}

QSizeF SliderGroove::getSize() const {
    return (shape().boundingRect()).size();
}

qreal SliderGroove::normalizHandleX(qreal x) const {
    qreal w = getSize().width();

    return (x + w / 2) / w;
}

bool SliderGroove::canMoveHandle(SliderHandle *handle, QPointF pos) const {
    if (!handle->isMovable()) { return false; }

    return handle->isMark() ? canMoveMarkHandle(handle, pos) : canMoveRoundHandle(handle, pos);
}

bool SliderGroove::canMoveMarkHandle(SliderHandle *handle, QPointF pos) const {
    int mx = getSize().width() / 2;
    int hx = handle->pos().x();
    bool moveLeft = pos.x() - handle->pos().x() < 0; // Judge of moving left or moving right.

    int prevIndex = findPreviousHandleIndex(handle, true);
    int nextIndex = findNextHandleIndex(handle, true);
    bool isFirst = prevIndex == -1;
    bool isLast  = nextIndex == -1;

    return moveLeft ?
           (isFirst ? (hx > -mx) : (hx > handles[prevIndex]->pos().x() + handleGap)):
           (isLast  ? (hx < +mx) : (hx < handles[nextIndex]->pos().x() - handleGap));
}

bool SliderGroove::canMoveRoundHandle(SliderHandle *handle, QPointF pos) const {
    int mx = getSize().width() / 2;
    int hx = handle->pos().x();
    bool moveLeft = pos.x() - handle->pos().x() < 0; // Judge of moving left or moving right.

    return moveLeft ? (hx > -mx ? true : false) : (hx < +mx ? true : false);
}

void SliderGroove::moveHandle(SliderHandle *handle, QPointF pos) {
    if (handle->isMark()) {
        moveMarkHandle(handle, pos);
    } else {
        moveRoundHandle(handle, pos);
    }
}

void SliderGroove::moveRoundHandle(SliderHandle *handle, QPointF pos) {
    int w2 = getSize().width() / 2;
    int x = pos.x();
    x = x < -w2 ? -w2 : x;
    x = x > +w2 ? +w2 : x;

    handle->setPos(x, handle->pos().y());
    handle->setNormalizedX(normalizHandleX(handle->pos().x()));
}

void SliderGroove::moveMarkHandle(SliderHandle *handle, QPointF pos) {
    int w2 = getSize().width() / 2;
    int x = pos.x();
    x = x < -w2 ? -w2 : x;
    x = x > +w2 ? +w2 : x;

    int prevIndex = findPreviousHandleIndex(handle, true);
    int nextIndex = findNextHandleIndex(handle, true);
    bool isFirst = prevIndex == -1;
    bool isLast  = nextIndex == -1;

    if (!isFirst) {
        int prevX = handles[prevIndex]->pos().x() + handleGap;
        x = x <= prevX ? prevX : x;
    }

    if (!isLast) {
        int nextX = handles[nextIndex]->pos().x() - handleGap;
        x = x >= nextX ? nextX : x;
    }

    handle->setPos(x, handle->pos().y());
    handle->setNormalizedX(normalizHandleX(handle->pos().x()));
}

int SliderGroove::findPreviousHandleIndex(SliderHandle *handle, bool markHandle) const {
    int index = handles.indexOf(handle);

    for (int i = index - 1; i >= 0; --i) {
        if (handles[i]->isMark() == markHandle) {
            return i;
        }
    }

    return -1;
}

int SliderGroove::findNextHandleIndex(SliderHandle *handle, bool markHandle) const {
    int index = handles.indexOf(handle);

    for (int i = index + 1; i < handles.size(); ++i) {
        if (handles[i]->isMark() == markHandle) {
            return i;
        }
    }

    return -1;
}

void SliderGroove::updateHandles() {
    qreal w = getSize().width();
    qreal h = getSize().height();
    qreal x = 0;

    foreach (SliderHandle *handle, handles) {
        x = handle->getNormalizedX() * w - w / 2;
        handle->setPos(x, handle->isMark() ? h / 2 : 0);
        handle->setSize(handle->isMark() ? markHandleSize : roundHandleSize);
    }
}

void SliderGroove::setRoundHandleSize(QSize size) {
    this->roundHandleSize = size;
}

QSize SliderGroove::getRoundHandleSize() const {
    return this->roundHandleSize;
}

void SliderGroove::setMarkHandleSize(QSize size) {
    this->markHandleSize = size;
}

QSize SliderGroove::getMarkHandleSize() const {
    return this->markHandleSize;
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
