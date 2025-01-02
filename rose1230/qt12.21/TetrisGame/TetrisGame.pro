QT += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TetrisGame
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    gameboard.cpp \
    tetrisblock.cpp

HEADERS += \
    mainwindow.h \
    gameboard.h \
    tetrisblock.h
