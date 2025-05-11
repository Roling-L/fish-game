#include "stdafx.h"
#include "fishgame.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    fishgame w;
    w.show();
    return a.exec();
}
