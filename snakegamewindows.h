// -*- coding: utf-8 -*-

#ifndef SNAKEGAMEWINDOWS_H
#define SNAKEGAMEWINDOWS_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QTimer>
#include <QVector>
#include <QPoint>
#include <QMouseEvent>
#include <QDateTime>

#include "utils.h"
#include "snakegame.h"


class SnakeGameWindows : public QMainWindow
{
    Q_OBJECT


public:
    explicit SnakeGameWindows(QWidget *parent = nullptr);
    explicit SnakeGameWindows(QWidget *parent, GameMode gm);
    explicit SnakeGameWindows(QWidget *parent, QString qtableFilename);
    QVector<SnakeState> getReplaySnakeGameState(QString filename);
    void playSnakeGameState(const SnakeState &snakeState);
    void playCurrentStepSnakeGameState();

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void drawingFinished(QWidget *subEidgt);


private:
    QTimer *gameTimer;
    GameMode currentMode;
    QDateTime startTime;
    int gameDuringTime;
    bool isGameStarted;

    SnakeGame snakeGame;

    QVector<SnakeState> replaySnakeGameState;
    int replayStep;

    void startSnakeGameMode(GameMode gm,int gameTimeInterval = 10);
    void updateSnakeGame();

};


#endif // SNAKEGAMEWINDOWS_H
