#include "snakegamewindows.h"
#include <QPainter>
#include <QKeyEvent>
#include <QRandomGenerator>
#include <QMessageBox>


const int WIDTH = SnakeGameSetting::UNIT_SIZE * SnakeGameSetting::UNIT_COUNT_X;
const int HEIGHT = SnakeGameSetting::UNIT_SIZE * SnakeGameSetting::UNIT_COUNT_Y;

const int UNIT_SIZE = SnakeGameSetting::UNIT_SIZE;

SnakeGameWindows::SnakeGameWindows(QWidget *parent)
    : QMainWindow(parent),
    gameTimer(new QTimer(this)),
    currentMode(GameMode::Mode1),
    startTime(QDateTime::currentDateTime()),
    gameDuringTime(0),
    isGameStarted(false)
{
    resize(WIDTH, HEIGHT);
    setWindowTitle("Snake Game");

    connect(gameTimer, &QTimer::timeout, this, &SnakeGameWindows::updateSnakeGame);
//    connect(snakeGame, SIGNAL(snakeGame.gameOver()),this, [](int value) {
//        gameDuringTime =
//    });

    startSnakeGameMode(currentMode, 200);

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

    // ���ƴ�ֱ����
    for (int x = 0; x <= WIDTH; x += UNIT_SIZE)
    {
        painter.drawLine(x, 0, x, HEIGHT);
    }

    // ����ˮƽ����
    for (int y = 0; y <= HEIGHT; y += UNIT_SIZE)
    {
        painter.drawLine(0, y, WIDTH, y);
    }

    // ����̰��������
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
        // ����A*Ѱ··��
        painter.setPen(Qt::red);
        painter.setBrush(Qt::NoBrush);
        for (const QPoint& point : snakeGame.AStarPath) {
            painter.drawRect(point.x() * UNIT_SIZE, point.y() * UNIT_SIZE, UNIT_SIZE, UNIT_SIZE);
        }

    }
    else if (currentMode == GameMode::Mode3) {
        // ����BFSѰ··��
        painter.setPen(Qt::darkYellow);
        painter.setBrush(Qt::NoBrush);
        for (const QPoint& point : snakeGame.BFSPath) {
            painter.drawRect(point.x() * UNIT_SIZE, point.y() * UNIT_SIZE, UNIT_SIZE, UNIT_SIZE);
        }
    }

    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::darkGreen);
    QPoint head = snakeGame.snake.first();
    painter.drawEllipse(head.x() * UNIT_SIZE, head.y() * UNIT_SIZE, UNIT_SIZE, UNIT_SIZE);

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
        if (snakeGame.snakeDirection != SnakeGame::Direction::Down)
            snakeGame.snakeDirection = SnakeGame::Direction::Up;
        break;
    case Qt::Key_S:
        if (snakeGame.snakeDirection != SnakeGame::Direction::Up)
            snakeGame.snakeDirection = SnakeGame::Direction::Down;
        break;
    case Qt::Key_A:
        if (snakeGame.snakeDirection != SnakeGame::Direction::Right)
            snakeGame.snakeDirection = SnakeGame::Direction::Left;
        break;
    case Qt::Key_D:
        if (snakeGame.snakeDirection != SnakeGame::Direction::Left)
            snakeGame.snakeDirection = SnakeGame::Direction::Right;
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
