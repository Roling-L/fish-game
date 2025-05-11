#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include <QMediaPlayer>
#include <QAudioOutput>
#include "fish.h"

class GameScene : public QGraphicsScene {
    Q_OBJECT
public:
    GameScene(QMediaPlayer* bgmPlayer, QAudioOutput* audioOutput, QObject* parent = nullptr);
    void setGameSpeed(qreal speed);
    void startGame();
    void stopGame();
    void keyPressEvent(QKeyEvent* event) override;

signals:
    void gameOver(int score);
    void scoreChanged(int score);

private slots:
    void spawnFish();
    void updateScore();

private:
    PlayerFish* player;
    QGraphicsPixmapItem* background;
    QGraphicsTextItem* scoreText;
    QTimer* spawnTimer;
    QTimer* moveTimer;
    QMediaPlayer* eatSound;
    QMediaPlayer* gameOverSound;
    QMediaPlayer* bgmPlayer;
    QAudioOutput* audioOutput;
    int score;
    qreal gameSpeed;
    bool gameRunning;

    void checkCollisions();
    void gameEnd();
};

#endif // GAMESCENE_H