#ifndef FISH_H
#define FISH_H

#include <QGraphicsPixmapItem>
#include <QPixmap>

class Fish : public QGraphicsPixmapItem {
public:
    Fish(const QPixmap& pixmap, qreal speed, QGraphicsItem* parent = nullptr);
    qreal speed;
};

class PlayerFish : public Fish {
public:
    PlayerFish(const QPixmap& pixmap, qreal speed, QGraphicsItem* parent = nullptr);
    void advance(int phase) override;
    void grow();
    int getSizeLevel() const;

protected:
    int sizeLevel;
};

class EnemyFish : public Fish {
public:
    EnemyFish(const QPixmap& pixmap, qreal speed, int sizeLevel, QGraphicsItem* parent = nullptr);
    void advance(int phase) override;
    int getSizeLevel() const;

private:
    int sizeLevel;
};

#endif // FISH_H