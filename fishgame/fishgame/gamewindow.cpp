#include "gamewindow.h"
#include "gamescene.h"
#include <QVBoxLayout>
#include <QGraphicsView>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QFont>

GameWindow::GameWindow(QWidget* parent)
    : QMainWindow(parent) {
    // Setup audio
    bgmPlayer = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    bgmPlayer->setAudioOutput(audioOutput);
    bgmPlayer->setSource(QUrl("qrc:/resources/sounds/bgm.mp3"));
    audioOutput->setVolume(50);

    // Setup UI
    setupUI();

    // Create game scene
    scene = new GameScene(bgmPlayer, audioOutput, this);
    connect(scene, &GameScene::gameOver, this, &GameWindow::showGameOver);

    QGraphicsView* view = new QGraphicsView(scene);
    view->setRenderHint(QPainter::Antialiasing);
    view->setBackgroundBrush(Qt::black);
    view->setCacheMode(QGraphicsView::CacheBackground);
    view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    view->setDragMode(QGraphicsView::NoDrag);

    centralWidget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);
    layout->addWidget(view);
    layout->setContentsMargins(0, 0, 0, 0);

    setCentralWidget(centralWidget);
    resize(800, 600);
    setWindowTitle("大鱼吃小鱼");

    showMenu();
}

void GameWindow::setupUI() {
    // Title label
    titleLabel = new QLabel("大鱼吃小鱼", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setFont(QFont("Arial", 32, QFont::Bold));
    titleLabel->setStyleSheet("color: white;");

    // Start button
    startButton = new QPushButton("开始游戏", this);
    startButton->setFont(QFont("Arial", 18));
    startButton->setFixedSize(200, 50);
    connect(startButton, &QPushButton::clicked, this, &GameWindow::startGame);

    // Difficulty combo box
    difficultyCombo = new QComboBox(this);
    difficultyCombo->addItem("简单");
    difficultyCombo->addItem("中等");
    difficultyCombo->addItem("困难");
    difficultyCombo->setFont(QFont("Arial", 14));
    difficultyCombo->setFixedSize(200, 30);
    connect(difficultyCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, &GameWindow::setDifficulty);

    // Game over labels
    gameOverLabel = new QLabel("游戏结束", this);
    gameOverLabel->setAlignment(Qt::AlignCenter);
    gameOverLabel->setFont(QFont("Arial", 32, QFont::Bold));
    gameOverLabel->setStyleSheet("color: white;");
    gameOverLabel->hide();

    finalScoreLabel = new QLabel(this);
    finalScoreLabel->setAlignment(Qt::AlignCenter);
    finalScoreLabel->setFont(QFont("Arial", 24));
    finalScoreLabel->setStyleSheet("color: white;");
    finalScoreLabel->hide();
}

void GameWindow::startGame() {
    hideMenu();
    scene->startGame();
}

void GameWindow::showGameOver(int score) {
    finalScoreLabel->setText(QString("最终得分: %1").arg(score));
    finalScoreLabel->show();
    gameOverLabel->show();

    QTimer::singleShot(3000, this, [this]() {
        finalScoreLabel->hide();
        gameOverLabel->hide();
        showMenu();
        });
}

void GameWindow::setDifficulty(int index) {
    qreal speed = 1.0 + index * 0.5; // 1.0 for easy, 1.5 for medium, 2.0 for hard
    scene->setGameSpeed(speed);
}

void GameWindow::showMenu() {
    titleLabel->setGeometry(width() / 2 - 150, height() / 2 - 150, 300, 60);
    startButton->setGeometry(width() / 2 - 100, height() / 2 - 30, 200, 50);
    difficultyCombo->setGeometry(width() / 2 - 100, height() / 2 + 40, 200, 30);

    titleLabel->show();
    startButton->show();
    difficultyCombo->show();
}

void GameWindow::hideMenu() {
    titleLabel->hide();
    startButton->hide();
    difficultyCombo->hide();
    finalScoreLabel->hide();
    gameOverLabel->hide();
}