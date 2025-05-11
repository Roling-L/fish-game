#include "gamescene.h"
#include <QKeyEvent>
#include <QTimer>
#include <QGraphicsTextItem>
#include <QFont>
#include <QRandomGenerator>

GameScene::GameScene(QMediaPlayer* bgmPlayer, QAudioOutput* audioOutput, QObject* parent)
    : QGraphicsScene(parent), score(0), gameSpeed(1.0), gameRunning(false), bgmPlayer(bgmPlayer), audioOutput(audioOutput) {
    setSceneRect(0, 0, 800, 600);

    // Load background
    background = new QGraphicsPixmapItem(QPixmap(":/resources/images/background.jpg").scaled(800, 600));
    addItem(background);
    background->setZValue(-1);

    // Load sounds
    eatSound = new QMediaPlayer(this);
    eatSound->setAudioOutput(new QAudioOutput(this));
    eatSound->setSource(QUrl("qrc:/resources/sounds/eat.wav"));

    gameOverSound = new QMediaPlayer(this);
    gameOverSound->setAudioOutput(new QAudioOutput(this));
    gameOverSound->setSource(QUrl("qrc:/resources/sounds/gameover.wav"));

    // Create player fish
    player = new PlayerFish(QPixmap(":/resources/images/player.png").scaled(50, 30), 5);
    addItem(player);
    player->setPos(width() / 2, height() / 2);

    // Score display
    scoreText = new QGraphicsTextItem("Score: 0");
    scoreText->setDefaultTextColor(Qt::white);
    scoreText->setFont(QFont("Arial", 16));
    addItem(scoreText);
    scoreText->setPos(10, 10);

    // Timers
    spawnTimer = new QTimer(this);
    connect(spawnTimer, &QTimer::timeout, this, &GameScene::spawnFish);

    moveTimer = new QTimer(this);
    connect(moveTimer, &QTimer::timeout, this, &GameScene::advance);
    connect(moveTimer, &QTimer::timeout, this, &GameScene::checkCollisions);
    connect(moveTimer, &QTimer::timeout, this, &GameScene::updateScore);
}

void GameScene::setGameSpeed(qreal speed) {
    gameSpeed = speed;
    if (spawnTimer->isActive()) {
        spawnTimer->setInterval(2000 / gameSpeed);
    }
}

void GameScene::startGame() {
    score = 0;
    scoreText->setPlainText("Score: 0");
    player->setPos(width() / 2, height() / 2);
    player->setScale(1.0);
    player->speed = 5;
    player->sizeLevel = 1;

    // Start timers
    spawnTimer->start(2000 / gameSpeed);
    moveTimer->start(16); // ~60 FPS

    gameRunning = true;

    // Play background music
    bgmPlayer->play();
}

void GameScene::stopGame() {
    spawnTimer->stop();
    moveTimer->stop();
    gameRunning = false;
    bgmPlayer->stop();
}

void GameScene::keyPressEvent(QKeyEvent* event) {
    if (!gameRunning) return;

    const qreal moveStep = player->speed;
    switch (event->key()) {
    case Qt::Key_Left:
        player->setX(player->x() - moveStep);
        break;
    case Qt::Key_Right:
        player->setX(player->x() + moveStep);
        break;
    case Qt::Key_Up:
        player->setY(player->y() - moveStep);
        break;
    case Qt::Key_Down:
        player->setY(player->y() + moveStep);
        break;
    default:
        QGraphicsScene::keyPressEvent(event);
    }
}

void GameScene::spawnFish() {
    int fishType = QRandomGenerator::global()->bounded(3);
    QPixmap pixmap;
    qreal speed;
    int sizeLevel;

    switch (fishType) {
    case 0:
        pixmap = QPixmap(":/resources/images/enemy1.png").scaled(40, 25);
        speed = 2.0 + QRandomGenerator::global()->bounded(2.0);
        sizeLevel = 1;
        break;
    case 1:
        pixmap = QPixmap(":/resources/images/enemy2.png").scaled(60, 35);
        speed = 1.5 + QRandomGenerator::global()->bounded(1.5);
        sizeLevel = 2;
        break;
    case 2:
        pixmap = QPixmap(":/resources/images/enemy3.png").scaled(80, 50);
        speed = 1.0 + QRandomGenerator::global()->bounded(1.0);
        sizeLevel = 3;
        break;
    }

    // Randomly decide direction (left to right or right to left)
    if (QRandomGenerator::global()->bounded(2) == 0) {
        speed = -speed;
    }

    EnemyFish* fish = new EnemyFish(pixmap, speed, sizeLevel);
    addItem(fish);

    // Set random position
    qreal yPos = QRandomGenerator::global()->bounded(height() - fish->boundingRect().height());
    if (speed > 0) {
        fish->setPos(-fish->boundingRect().width(), yPos);
    }
    else {
        fish->setPos(width(), yPos);
    }
}

void GameScene::checkCollisions() {
    if (!gameRunning) return;

    QList<QGraphicsItem*> collidingItems = player->collidingItems();
    for (QGraphicsItem* item : collidingItems) {
        EnemyFish* enemy = dynamic_cast<EnemyFish*>(item);
        if (enemy) {
            if (player->getSizeLevel() > enemy->getSizeLevel()) {
                // Player eats the enemy
                score += enemy->getSizeLevel() * 10;
                player->grow();
                scene()->removeItem(enemy);
                delete enemy;
                eatSound->play();
            }
            else {
                // Game over
                gameEnd();
                return;
            }
        }
    }
}

void GameScene::updateScore() {
    scoreText->setPlainTe