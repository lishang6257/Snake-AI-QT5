// -*- coding: utf-8 -*-

#include "snakegamewindows.h"
#include <QPainter>
#include <QKeyEvent>
#include <QRandomGenerator>
#include <QMessageBox>
#include <QFile>
#include <QThread>

const int WIDTH = SnakeGameSetting::UNIT_SIZE * SnakeGameSetting::UNIT_COUNT_X;
const int HEIGHT = SnakeGameSetting::UNIT_SIZE * SnakeGameSetting::UNIT_COUNT_Y;

const int UNIT_SIZE = SnakeGameSetting::UNIT_SIZE;

SnakeGameWindows::SnakeGameWindows(QWidget *parent)
    : QMainWindow(parent),
    gameTimer(new QTimer(this)),
    currentMode(GameMode::Mode4),
    startTime(QDateTime::currentDateTime()),
    gameDuringTime(0),
    isGameStarted(false),
    replayStep(0)
{
    resize(WIDTH, HEIGHT);
    setWindowTitle("Snake Game");

    connect(gameTimer, &QTimer::timeout, this, &SnakeGameWindows::updateSnakeGame);

//    replaySnakeGameState = getReplaySnakeGameState("SnakeGame_20230717_143654.dat");

//    disconnect(gameTimer, &QTimer::timeout, this, &SnakeGameWindows::updateSnakeGame);
//    connect(gameTimer, &QTimer::timeout, this, &SnakeGameWindows::playCurrentStepSnakeGameState);

    gameTimer->start(50);
}


void SnakeGameWindows::startSnakeGameMode(GameMode gm, int timeInterval)
{
    startTime = QDateTime::currentDateTime();
    gameDuringTime = 0;
    snakeGame.startMode(gm);
    gameTimer->setInterval(timeInterval);
    gameTimer->start();
}

void SnakeGameWindows::updateSnakeGame()
{
    snakeGame.updateGame();
    if(snakeGame.isGameStarted)
        gameDuringTime = startTime.secsTo(QDateTime::currentDateTime());
    update();
}

void SnakeGameWindows::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillRect(rect(), Qt::gray);

    painter.setPen(Qt::NoPen);
    painter.setPen(Qt::black);

    // 绘制垂直线条
    for (int x = 0; x <= WIDTH; x += UNIT_SIZE)
    {
        painter.drawLine(x, 0, x, HEIGHT);
    }

    // 绘制水平线条
    for (int y = 0; y <= HEIGHT; y += UNIT_SIZE)
    {
        painter.drawLine(0, y, WIDTH, y);
    }

    // 绘制贪吃蛇躯体
    for (int i = 1; i < snakeGame.snake.size(); ++i)
    {
        QPoint point = snakeGame.snake.at(i);
        painter.setPen(Qt::black);
        painter.setBrush(Qt::green);
        painter.drawRect(point.x() * UNIT_SIZE, point.y() * UNIT_SIZE, UNIT_SIZE, UNIT_SIZE);
        QPoint prePoint = snakeGame.snake.at(i - 1);
        if(prePoint == snakeGame.snake.first()) continue;
        QPoint dirPoint = point - prePoint;
        painter.setPen(Qt::white);
        if(dirPoint.x() == 1){
            painter.drawLine(QLine(point.x() * UNIT_SIZE,point.y() * UNIT_SIZE,point.x() * UNIT_SIZE,(point.y()+1) * UNIT_SIZE));
        }
        else if(dirPoint.x() == -1){
            painter.drawLine(QLine(prePoint.x() * UNIT_SIZE,prePoint.y() * UNIT_SIZE,prePoint.x() * UNIT_SIZE,(prePoint.y()+1) * UNIT_SIZE));
        }
        else if(dirPoint.y() == 1){
            painter.drawLine(QLine(point.x() * UNIT_SIZE,point.y() * UNIT_SIZE,(point.x()+1) * UNIT_SIZE,point.y() * UNIT_SIZE));
        }
        else if(dirPoint.y() == -1){
            painter.drawLine(QLine(prePoint.x() * UNIT_SIZE,prePoint.y() * UNIT_SIZE,(prePoint.x()+1) * UNIT_SIZE,prePoint.y() * UNIT_SIZE));
        }

    }
    if (currentMode == GameMode::Mode1) {

    }
    else if (currentMode == GameMode::Mode2) {
        // 绘制A*寻路路径
        painter.setPen(Qt::red);
        painter.setBrush(Qt::NoBrush);
        for (const QPoint& point : snakeGame.AStarPath) {
            painter.drawRect(point.x() * UNIT_SIZE, point.y() * UNIT_SIZE, UNIT_SIZE, UNIT_SIZE);
        }

    }
    else if (currentMode == GameMode::Mode3) {
        // 绘制BFS寻路路径
        painter.setPen(Qt::darkYellow);
        painter.setBrush(Qt::NoBrush);
        for (const QPoint& point : snakeGame.BFSPath) {
            painter.drawRect(point.x() * UNIT_SIZE, point.y() * UNIT_SIZE, UNIT_SIZE, UNIT_SIZE);
        }
    }
    if(!snakeGame.snake.empty())
    {
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::darkGreen);
        QPoint head = snakeGame.snake.first();
        painter.drawEllipse(head.x() * UNIT_SIZE, head.y() * UNIT_SIZE, UNIT_SIZE, UNIT_SIZE);
    }
    painter.setBrush(Qt::red);
    QPointF foodPoints[3] = {
        QPointF(snakeGame.food.x() * UNIT_SIZE + UNIT_SIZE / 2, snakeGame.food.y() * UNIT_SIZE),
        QPointF(snakeGame.food.x() * UNIT_SIZE, snakeGame.food.y() * UNIT_SIZE + UNIT_SIZE),
        QPointF(snakeGame.food.x() * UNIT_SIZE + UNIT_SIZE, snakeGame.food.y() * UNIT_SIZE + UNIT_SIZE)
    };
    painter.drawPolygon(foodPoints, 3);

    QString modeText;
    if (currentMode == GameMode::Mode1)
        modeText = "Mode: 1 (Manual)";
    else if (currentMode == GameMode::Mode2)
        modeText = "Mode: 2 (Auto-AStar)";
    else if (currentMode == GameMode::Mode3)
        modeText = "Mode: 3 (Auto-BFS)";
    else if (currentMode == GameMode::Mode4)
        modeText = "Mode: 4 (QLearning)";

    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 14, QFont::Bold));
    painter.drawText(QRect(10, HEIGHT - 30, 200, 20), modeText);
    painter.drawText(QRect(10, HEIGHT - 50, 200, 20), "Score: " + QString::number(snakeGame.score));

    QString timeText = "Time: " + QString::number(gameDuringTime) + "s";
    painter.drawText(QRect(10, HEIGHT - 70, 200, 20), timeText);

}

void SnakeGameWindows::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_W:
        if (snakeGame.snakeDirection != SnakeDirection::Down)
            snakeGame.snakeDirection = SnakeDirection::Up;
        break;
    case Qt::Key_S:
        if (snakeGame.snakeDirection != SnakeDirection::Up)
            snakeGame.snakeDirection = SnakeDirection::Down;
        break;
    case Qt::Key_A:
        if (snakeGame.snakeDirection != SnakeDirection::Right)
            snakeGame.snakeDirection = SnakeDirection::Left;
        break;
    case Qt::Key_D:
        if (snakeGame.snakeDirection != SnakeDirection::Left)
            snakeGame.snakeDirection = SnakeDirection::Right;
        break;
    }
}

void SnakeGameWindows::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {

        if (currentMode == GameMode::Mode1){
            startSnakeGameMode(GameMode::Mode2);
            currentMode = GameMode::Mode2;
        }
        else if (currentMode == GameMode::Mode2)
        {
            startSnakeGameMode(GameMode::Mode3);
            currentMode = GameMode::Mode3;
        }
        else if (currentMode == GameMode::Mode3){
            startSnakeGameMode(GameMode::Mode1, 200);
            currentMode = GameMode::Mode1;
        }
    }
}

void SnakeGameWindows::playSnakeGameState(const SnakeState &snakeState)
{
    currentMode = snakeState.currentGameMode;
    isGameStarted = snakeState.isGameStarted;
    currentMode = snakeState.currentGameMode;
    snakeGame.snake = snakeState.snake;
    snakeGame.food = snakeState.food;
    snakeGame.snakeDirection = snakeState.currentDirection;

    update();
}

void SnakeGameWindows::playCurrentStepSnakeGameState()
{
    if(replayStep >= replaySnakeGameState.size()) replayStep--;

    playSnakeGameState(replaySnakeGameState[replayStep]);
    replayStep ++;

    update();
}


QVector<SnakeState> SnakeGameWindows::getReplaySnakeGameState(QString filename)
{
    // 从文件中读取 QVector<SnakeState> 对象
    QFile file(filename);
    QVector<SnakeState> states;
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray data = file.readAll();
        states = SnakeState::deserializeQVector(data);
        file.close();
    }
    return states;
}



