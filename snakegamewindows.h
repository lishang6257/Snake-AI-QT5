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

    // 从文件中获取回放状态的贪吃蛇游戏
    QVector<SnakeState> getReplaySnakeGameState(QString filename);

    // 播放特定状态的贪吃蛇游戏
    void playSnakeGameState(const SnakeState &snakeState);

    // 播放当前步数的贪吃蛇游戏状态
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

    // 启动特定游戏模式的贪吃蛇游戏
    void startSnakeGameMode(GameMode gm, int gameTimeInterval = 10);

    // 更新贪吃蛇游戏状态
    void updateSnakeGame();
};

#endif // SNAKEGAMEWINDOWS_H
