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

    //    gameTimer->start(50);
    startSnakeGameMode(currentMode, 1000);
}


SnakeGameWindows::SnakeGameWindows(QWidget *parent, GameMode gm)
    : QMainWindow(parent),
    gameTimer(new QTimer(this)),
    currentMode(gm),
    startTime(QDateTime::currentDateTime()),
    gameDuringTime(0),
    isGameStarted(false),
    replayStep(0)
{
    resize(WIDTH, HEIGHT);
    setWindowTitle("Snake Game");

    connect(gameTimer, &QTimer::timeout, this, &SnakeGameWindows::updateSnakeGame);

    //    replaySnakeGameState = getReplaySnakeGameState("SnakeGame_20230719_005522.dat");

    //    disconnect(gameTimer, &QTimer::timeout, this, &SnakeGameWindows::updateSnakeGame);
    //    connect(gameTimer, &QTimer::timeout, this, &SnakeGameWindows::playCurrentStepSnakeGameState);

    //    gameTimer->start(50);
    startSnakeGameMode(currentMode, 1);
}

SnakeGameWindows::SnakeGameWindows(QWidget *parent, QString qtableFilename)
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

    //    gameTimer->start(50);
    startSnakeGameMode(currentMode, 100);
    snakeGame.loadQLearningQTable(qtableFilename);
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
    painter.fillRect(rect(), Qt::gray);  // 填充整个窗口为灰色背景

    painter.setPen(Qt::NoPen);
    painter.setPen(Qt::black);

    // 绘制网格线
    for (int x = 0; x <= WIDTH; x += UNIT_SIZE)
    {
        painter.drawLine(x, 0, x, HEIGHT);  // 绘制竖直线
    }

    for (int y = 0; y <= HEIGHT; y += UNIT_SIZE)
    {
        painter.drawLine(0, y, WIDTH, y);  // 绘制水平线
    }

    // 绘制贪吃蛇身体
    for (int i = 1; i < snakeGame.snake.size(); ++i)
    {
        QPoint point = snakeGame.snake.at(i);  // 当前身体部分的点
        QPoint prePoint = snakeGame.snake.at(i - 1);  // 前一个身体部分的点

        QPoint snakeBodyPointPos = point * UNIT_SIZE;  // 当前身体部分在窗口中的坐标
        QPoint preSnakeBodyPointPos = prePoint * UNIT_SIZE;  // 前一个身体部分在窗口中的坐标
        QPoint dirPoint = prePoint - point;  // 蛇身行进的方向向量

        bool isPrePointSnakeHead = (prePoint == snakeGame.snake.first());  // 前一个身体部分是否为蛇头

        // 根据蛇身行进方向绘制蛇身体
        // 以矩形表示贪吃蛇的身体部分，并在前一个身体部分与当前身体部分之间绘制行进方向的箭头
        painter.setPen(QPen(Qt::black, 3));
        painter.setBrush(Qt::green);
        painter.drawRect(snakeBodyPointPos.x(), snakeBodyPointPos.y(), UNIT_SIZE, UNIT_SIZE);

        int toDeleteSpace = 3;
        if (dirPoint == DirectionState[(int)SnakeDirection::Left]) {
            // 向左行进
            if (isPrePointSnakeHead){
                painter.setPen(QPen(Qt::black, 3));
                painter.setBrush(Qt::green);
                painter.drawRect(snakeBodyPointPos.x() - UNIT_SIZE / 2, snakeBodyPointPos.y(), UNIT_SIZE / 2, UNIT_SIZE);

                drawSnakeHead(painter, SnakeDirection::Left);

            }

            painter.setPen(QPen(Qt::green, 3));
            painter.drawLine(QLine(snakeBodyPointPos.x(), snakeBodyPointPos.y() + toDeleteSpace,
                                   snakeBodyPointPos.x(), snakeBodyPointPos.y() + UNIT_SIZE - toDeleteSpace));

        }
        else if (dirPoint == DirectionState[(int)SnakeDirection::Right]) {
            // 向右行进
            painter.setPen(QPen(Qt::black, 3));
            painter.setBrush(Qt::green);
            if (isPrePointSnakeHead){
                painter.drawRect(snakeBodyPointPos.x() + UNIT_SIZE, snakeBodyPointPos.y(), UNIT_SIZE / 2, UNIT_SIZE);

                drawSnakeHead(painter, SnakeDirection::Right);
            }
            painter.setPen(QPen(Qt::green, 3));
            painter.drawLine(QLine(preSnakeBodyPointPos.x(), preSnakeBodyPointPos.y() + toDeleteSpace,
                                   preSnakeBodyPointPos.x(), preSnakeBodyPointPos.y() + UNIT_SIZE - toDeleteSpace));

        }
        else if (dirPoint == DirectionState[(int)SnakeDirection::Up]) {
            // 向上行进
            painter.setPen(QPen(Qt::black, 3));
            painter.setBrush(Qt::green);
            if (isPrePointSnakeHead){
                painter.drawRect(snakeBodyPointPos.x(), snakeBodyPointPos.y() - UNIT_SIZE / 2, UNIT_SIZE, UNIT_SIZE / 2);

                drawSnakeHead(painter, SnakeDirection::Up);
            }

            painter.setPen(QPen(Qt::green, 3));
            painter.drawLine(QLine(snakeBodyPointPos.x() + toDeleteSpace, snakeBodyPointPos.y(),
                                   snakeBodyPointPos.x() + UNIT_SIZE - toDeleteSpace, snakeBodyPointPos.y()));
        }
        else if (dirPoint == DirectionState[(int)SnakeDirection::Down]) {
            // 向下行进
            painter.setPen(QPen(Qt::black, 3));
            painter.setBrush(Qt::green);
            if (isPrePointSnakeHead){
                painter.drawRect(snakeBodyPointPos.x(), snakeBodyPointPos.y() + UNIT_SIZE, UNIT_SIZE, UNIT_SIZE / 2);

                drawSnakeHead(painter, SnakeDirection::Down);
            }

            painter.setPen(QPen(Qt::green, 3));
            painter.drawLine(QLine(preSnakeBodyPointPos.x() + toDeleteSpace, preSnakeBodyPointPos.y(),
                                   preSnakeBodyPointPos.x() + UNIT_SIZE - toDeleteSpace, preSnakeBodyPointPos.y()));
        }
    }


    // 绘制食物
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::red);
    QPointF foodPoints[3] = {
        QPointF(snakeGame.food.x() * UNIT_SIZE + UNIT_SIZE / 2, snakeGame.food.y() * UNIT_SIZE),
        QPointF(snakeGame.food.x() * UNIT_SIZE, snakeGame.food.y() * UNIT_SIZE + UNIT_SIZE),
        QPointF(snakeGame.food.x() * UNIT_SIZE + UNIT_SIZE, snakeGame.food.y() * UNIT_SIZE + UNIT_SIZE)
    };
    painter.drawPolygon(foodPoints, 3);

    // 根据当前游戏模式绘制辅助路径
    if (currentMode == GameMode::Mode1) {

    }
    else if (currentMode == GameMode::Mode2) {
        drawSnakePath(painter, snakeGame.AStarPath, Qt::red);
    }
    else if (currentMode == GameMode::Mode3) {
        drawSnakePath(painter, snakeGame.BFSPath, Qt::yellow);
    }


    // 绘制游戏模式和得分信息
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


    // 绘制游戏时间信息
    QString timeText = "Time: " + QString::number(gameDuringTime) + "s";
    painter.drawText(QRect(10, HEIGHT - 70, 200, 20), timeText);

}

void SnakeGameWindows::drawSnakeHead(QPainter &painter,SnakeDirection snakeDirection)
{
    if (!snakeGame.snake.empty())
    {
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setPen(Qt::NoPen);

        // 绘制蛇头身体，这里使用深绿色填充
        painter.setBrush(Qt::darkGreen);
        QPoint head = snakeGame.snake.first();
        painter.drawEllipse(head.x() * UNIT_SIZE, head.y() * UNIT_SIZE, UNIT_SIZE, UNIT_SIZE);

        // 计算蛇头中心位置
        QPointF headCenter(head.x() * UNIT_SIZE + UNIT_SIZE / 2.0, head.y() * UNIT_SIZE + UNIT_SIZE / 2.0);

        // 绘制两个小眼睛，这里使用白色填充
        painter.setBrush(Qt::white);

        QPolygonF nose;

        const qreal eyeRadius = UNIT_SIZE / 10.0;
        const qreal noseSize = UNIT_SIZE / 5.0;

        qreal leyeOffsetX = UNIT_SIZE / 4.0;
        qreal leyeOffsetY = UNIT_SIZE / 4.0;
        qreal reyeOffsetX = UNIT_SIZE / 4.0;
        qreal reyeOffsetY = UNIT_SIZE / 4.0;

        // 根据蛇头的移动方向来确定眼睛和鼻子的位置
        switch (snakeDirection)
        {
        case SnakeDirection::Up:
            //            (-1,-1) (1,-1)
            leyeOffsetX *= -1;
            leyeOffsetY *= -1;
            reyeOffsetX *= 1;
            reyeOffsetY *= -1;
            nose << headCenter + QPointF(0, noseSize / 2.0)
                 << headCenter + QPointF(-noseSize / 2.0, -noseSize / 2.0)
                 << headCenter + QPointF(noseSize / 2.0, -noseSize / 2.0);
            break;
        case SnakeDirection::Down:
            //            (-1,1) (1,1)
            leyeOffsetX *= -1;
            leyeOffsetY *= 1;
            reyeOffsetX *= 1;
            reyeOffsetY *= 1;
            nose << headCenter + QPointF(0, - noseSize / 2.0)
                 << headCenter + QPointF(-noseSize / 2.0, noseSize / 2.0)
                 << headCenter + QPointF(noseSize / 2.0, noseSize / 2.0);
            break;
        case SnakeDirection::Left:
            //            (-1,-1) (-1,1)
            leyeOffsetX *= -1;
            leyeOffsetY *= -1;
            reyeOffsetX *= -1;
            reyeOffsetY *= 1;
            nose << headCenter + QPointF(noseSize / 2.0,0)
                 << headCenter + QPointF(-noseSize / 2.0, -noseSize / 2.0)
                 << headCenter + QPointF(-noseSize / 2.0, noseSize / 2.0);
            break;
        case SnakeDirection::Right:
            //            (1,-1) (1,1)
            leyeOffsetX *= 1;
            leyeOffsetY *= -1;
            reyeOffsetX *= 1;
            reyeOffsetY *= 1;
            nose << headCenter + QPointF(-noseSize / 2.0,0)
                 << headCenter + QPointF(noseSize / 2.0, -noseSize / 2.0)
                 << headCenter + QPointF(noseSize / 2.0, noseSize / 2.0);
            break;
        default:
            break;
        }

        painter.drawEllipse(headCenter + QPointF(leyeOffsetX, leyeOffsetY), eyeRadius, eyeRadius);
        painter.drawEllipse(headCenter + QPointF(reyeOffsetX, reyeOffsetY), eyeRadius, eyeRadius);

        // 绘制三角形鼻子，这里使用红色填充
        painter.setBrush(Qt::red);

        painter.drawPolygon(nose);
    }
}

void SnakeGameWindows::drawSnakePath(QPainter &painter,QVector<QPoint> &path, QColor c)
{
    painter.setPen(c);
    painter.setBrush(Qt::NoBrush);
    for (int i = 0; i < path.size(); ++i) {
        QPoint point = path.at(i);

        if (i + 1 < path.size()) {
            QPoint nextPoint = path.at(i + 1);

            QPoint dirPoint = nextPoint - point;

//            painter.drawRect(point.x() * UNIT_SIZE, point.y() * UNIT_SIZE, UNIT_SIZE, UNIT_SIZE);

            if (dirPoint == DirectionState[(int)SnakeDirection::Left]) {
                // 向左行进，绘制左箭头
                QPolygon leftArrow;
                leftArrow << QPoint(point.x() * UNIT_SIZE + UNIT_SIZE / 2, point.y() * UNIT_SIZE + UNIT_SIZE / 4)
                          << QPoint(point.x() * UNIT_SIZE + UNIT_SIZE / 4, point.y() * UNIT_SIZE + UNIT_SIZE / 2)
                          << QPoint(point.x() * UNIT_SIZE + UNIT_SIZE / 2, point.y() * UNIT_SIZE + UNIT_SIZE * 3 / 4);
                painter.drawPolygon(leftArrow);
            } else if (dirPoint == DirectionState[(int)SnakeDirection::Right]) {
                // 向右行进，绘制右箭头
                QPolygon rightArrow;
                rightArrow << QPoint(point.x() * UNIT_SIZE + UNIT_SIZE / 2, point.y() * UNIT_SIZE + UNIT_SIZE / 4)
                           << QPoint(point.x() * UNIT_SIZE + UNIT_SIZE * 3 / 4, point.y() * UNIT_SIZE + UNIT_SIZE / 2)
                           << QPoint(point.x() * UNIT_SIZE + UNIT_SIZE / 2, point.y() * UNIT_SIZE + UNIT_SIZE * 3 / 4);
                painter.drawPolygon(rightArrow);
            } else if (dirPoint == DirectionState[(int)SnakeDirection::Up]) {
                // 向上行进，绘制上箭头
                QPolygon upArrow;
                upArrow << QPoint(point.x() * UNIT_SIZE + UNIT_SIZE / 4, point.y() * UNIT_SIZE + UNIT_SIZE / 2)
                        << QPoint(point.x() * UNIT_SIZE + UNIT_SIZE / 2, point.y() * UNIT_SIZE + UNIT_SIZE / 4)
                        << QPoint(point.x() * UNIT_SIZE + UNIT_SIZE * 3 / 4, point.y() * UNIT_SIZE + UNIT_SIZE / 2);
                painter.drawPolygon(upArrow);
            } else if (dirPoint == DirectionState[(int)SnakeDirection::Down]) {
                // 向下行进，绘制下箭头
                QPolygon downArrow;
                downArrow << QPoint(point.x() * UNIT_SIZE + UNIT_SIZE / 4, point.y() * UNIT_SIZE + UNIT_SIZE / 2)
                          << QPoint(point.x() * UNIT_SIZE + UNIT_SIZE / 2, point.y() * UNIT_SIZE + UNIT_SIZE * 3 / 4)
                          << QPoint(point.x() * UNIT_SIZE + UNIT_SIZE * 3 / 4, point.y() * UNIT_SIZE + UNIT_SIZE / 2);
                painter.drawPolygon(downArrow);
            }
        }
    }

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
            startSnakeGameMode(GameMode::Mode4);
            currentMode = GameMode::Mode4;
        }
        else if (currentMode == GameMode::Mode4){
            startSnakeGameMode(GameMode::Mode1,200);
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



