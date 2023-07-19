CODECFORTR = UTF-8

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    qlearning.cpp \
    snakestate.cpp \
    aievaluationwindows.cpp \
    aievaluator.cpp \
    astar.cpp \
    bfs.cpp \
    main.cpp \
    snakegame.cpp \
    snakegamewindows.cpp

HEADERS += \
    aievaluationwindows.h \
    aievaluator.h \
    astar.h \
    bfs.h \
    qlearning.h \
    snakegame.h \
    snakegamewindows.h \
    utils.h \
    简介.h

FORMS += \
    snakegame.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
