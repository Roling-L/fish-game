
#include "fish.h"

Fish::Fish(const QPixmap& pixmap, qreal speed, QGraphicsItem* parent)
    : QGraphicsPixmapItem(pixmap, parent), speed(speed) {}

PlayerFish::PlayerFish(const QPixmap& pixmap, qreal speed, QGraphicsItem* parent)
    : Fish(pixmap, speed, parent), sizeLevel(1) {
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();
}

void PlayerFish::advance(int phase) {
    if (!phase) return;

    // Boundary checking
    if (x() < 0) setX(0);
    if (x() > scene()->width() - boundingRect().width())
        setX(scene()->width() - boundingRect().width());
    if (y() < 0) setY(0);
    if (y() > scene()->height() - boundingRect().height())
        setY(scene()->height() - boundingRect().height());
}

void PlayerFish::grow() {
    sizeLevel++;
    qreal scaleFactor = 1.0 + (sizeLevel - 1) * 0.2;
    setScale(scaleFactor);
    speed *= 0.95; // Slightly reduce speed when growing
}

int PlayerFish::getSizeLevel() const {
    return sizeLevel;
}

EnemyFish::EnemyFish(const QPixmap& pixmap, qreal speed, int sizeLevel, QGraphicsItem* parent)
    : Fish(pixmap, speed, parent), sizeLevel(sizeLevel) {
    qreal scaleFactor = 1.0 + (sizeLevel - 1) * 0.2;
    setScale(scaleFactor);
}

void EnemyFish::advance(int phase) {
    if (!phase) return;

    setPos(x() + speed, y());

    // Remove fish when it goes out of the scene
    if ((speed > 0 && x() > scene()->width()) ||
        (speed < 0 && x() + boundingRect().width() < 0)) {
        scene()->removeItem(this);
        delete this;
    }
}

int EnemyFish::getSizeLevel() const {
    return sizeLevel;
}