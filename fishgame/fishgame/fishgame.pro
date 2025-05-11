QT += core gui multimedia widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FishGame
TEMPLATE = app

SOURCES += main.cpp \
           gamewindow.cpp \
           gamescene.cpp \
           fish.cpp

HEADERS += gamewindow.h \
           gamescene.h \
           fish.h

RESOURCES += resources.qrc