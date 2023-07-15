#include "snakegame.h"

#include <QRandomGenerator>
#include <QFontMetrics>
#include <QResizeEvent>
#include <QLine>

SnakeGame::SnakeGame(QWidget *parent)
    : QMainWindow(parent),
    snakeDirection(Direction::Right),
    gameTimer(new QTimer(this)),
    score(0)
{
    resize(WIDTH, HEIGHT);
    setWindowTitle("snake");

    snake.append(QPoint(UNIT_COUNT_X / 2, UNIT_COUNT_Y / 2)); // 初始化贪吃蛇位置

    gameTimer->setInterval(200); // 游戏速度
    connect(gameTimer, &QTimer::timeout, this, &SnakeGame::updateGame);
    gameTimer->start();

    startTime = QDateTime::currentDateTime(); // 记录游戏开始时间
}

void SnakeGame::paintEvent(QPaintEvent *event)
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


    for (int i = 1; i < snake.size(); ++i)
    {
        QPoint point = snake.at(i);
        painter.setPen(Qt::darkGreen);
        painter.setBrush(Qt::green);
        painter.drawRect(point.x() * UNIT_SIZE, point.y() * UNIT_SIZE, UNIT_SIZE, UNIT_SIZE);
        QPoint prePoint = snake.at(i - 1);
        if(prePoint == snake.first()) continue;
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

    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::darkGreen);
    QPoint head = snake.first();
    painter.drawEllipse(head.x() * UNIT_SIZE, head.y() * UNIT_SIZE, UNIT_SIZE, UNIT_SIZE);

    painter.setBrush(Qt::red);
    QPointF foodPoints[3] = {
        QPointF(food.x() * UNIT_SIZE + UNIT_SIZE / 2, food.y() * UNIT_SIZE),
        QPointF(food.x() * UNIT_SIZE, food.y() * UNIT_SIZE + UNIT_SIZE),
        QPointF(food.x() * UNIT_SIZE + UNIT_SIZE, food.y() * UNIT_SIZE + UNIT_SIZE)
    };
    painter.drawPolygon(foodPoints, 3);

    // 计算游戏运行时间
    qint64 elapsedSeconds = startTime.secsTo(QDateTime::currentDateTime());
    QString timeText = "Time:" + QString::number(elapsedSeconds) + "s";
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 14, QFont::Bold));
    int timeTextWidth = painter.fontMetrics().horizontalAdvance(timeText);
    int timeTextHeight = painter.fontMetrics().height();
    QPoint timeTextPos(10, HEIGHT - timeTextHeight - 10); // 时间文本位置
    painter.drawText(timeTextPos, timeText);

    QString scoreText = "Score:" + QString::number(score);
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 14, QFont::Bold));
    int scoreTextWidth = painter.fontMetrics().horizontalAdvance(scoreText);
    int scoreTextHeight = painter.fontMetrics().height();
    QPoint scoreTextPos(10, timeTextPos.y() - scoreTextHeight - 5); // 分数文本位置
    painter.drawText(scoreTextPos, scoreText);
}

void SnakeGame::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_W:
        if (snakeDirection != Direction::Down)
            snakeDirection = Direction::Up;
        break;
    case Qt::Key_S:
        if (snakeDirection != Direction::Up)
            snakeDirection = Direction::Down;
        break;
    case Qt::Key_A:
        if (snakeDirection != Direction::Right)
            snakeDirection = Direction::Left;
        break;
    case Qt::Key_D:
        if (snakeDirection != Direction::Left)
            snakeDirection = Direction::Right;
        break;
    }
}

void SnakeGame::updateGame()
{
    QPoint newHead = snake.first();
    switch (snakeDirection)
    {
    case Direction::Up:
        newHead.setY(newHead.y() - 1);
        break;
    case Direction::Down:
        newHead.setY(newHead.y() + 1);
        break;
    case Direction::Left:
        newHead.setX(newHead.x() - 1);
        break;
    case Direction::Right:
        newHead.setX(newHead.x() + 1);
        break;
    }

    if (newHead == food)
    {
        generateFood();
        score += 10;
    }
    else
    {
        snake.removeLast();
    }

    if (isGameOver(newHead))
    {
        gameTimer->stop();
        QMessageBox::information(this, "gameOver", "gameOver!");
        return;
    }

    snake.prepend(newHead);

    update();
}

void SnakeGame::generateFood()
{
    QVector<QPoint> occupiedPositions = snake.toVector();

    QPoint newFood;
    bool isOccupied = true;
    while (isOccupied) {
        int x = QRandomGenerator::global()->bounded(UNIT_COUNT_X);
        int y = QRandomGenerator::global()->bounded(UNIT_COUNT_Y);
        newFood = QPoint(x, y);

        isOccupied = false;
        for (const QPoint& bodyPart : occupiedPositions) {
            if (bodyPart == newFood) {
                isOccupied = true;
                break;
            }
        }
    }

    food = newFood;
}

bool SnakeGame::isGameOver(const QPoint& head)
{
    if (head.x() < 0 || head.x() >= UNIT_COUNT_X || head.y() < 0 || head.y() >= UNIT_COUNT_Y)
        return true;

    for (int i = 1; i < snake.size(); ++i)
    {
        if (head == snake.at(i))
            return true;
    }

    return false;
}
