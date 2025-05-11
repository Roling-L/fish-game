#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_fishgame.h"

class fishgame : public QMainWindow
{
    Q_OBJECT

public:
    fishgame(QWidget *parent = nullptr);
    ~fishgame();

private:
    Ui::fishgameClass ui;
};
