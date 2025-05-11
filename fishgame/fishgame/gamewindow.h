#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QAudioOutput>

class GameScene;

class GameWindow : public QMainWindow {
    Q_OBJECT
public:
    GameWindow(QWidget* parent = nullptr);

private slots:
    void startGame();
    void showGameOver(int score);
    void setDifficulty(int index);

private:
    GameScene* scene;
    QMediaPlayer* bgmPlayer;
    QAudioOutput* audioOutput;
    QWidget* centralWidget;
    QPushButton* startButton;
    QComboBox* difficultyCombo;
    QLabel* titleLabel;
    QLabel* gameOverLabel;
    QLabel* finalScoreLabel;

    void setupUI();
    void showMenu();
    void hideMenu();
};

#endif // GAMEWINDOW_H