#include "snakegame.h"
#include <QPainter>
#include <QKeyEvent>
#include <QRandomGenerator>
#include <QMessageBox>

const int UNIT_SIZE = 20;
extern const int UNIT_COUNT_X = 30;
extern const int UNIT_COUNT_Y = 20;
const int WIDTH = UNIT_SIZE * UNIT_COUNT_X;
const int HEIGHT = UNIT_SIZE * UNIT_COUNT_Y;

SnakeGame::SnakeGame(QWidget *parent)
    : QMainWindow(parent),
    snakeDirection(Direction::Right),
    gameTimer(new QTimer(this)),
    score(0),
    currentMode(GameMode::Mode1),
    startTime(QDateTime::currentDateTime())
{
    resize(WIDTH, HEIGHT);
    setWindowTitle("Snake Game");

    snake.append(QPoint(UNIT_COUNT_X / 2, UNIT_COUNT_Y / 2)); // 初始化贪吃蛇位置

    gameTimer->setInterval(200); // 游戏速度
    connect(gameTimer, &QTimer::timeout, this, &SnakeGame::updateGame);
    gameTimer->start();
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

    // 绘制贪吃蛇躯体
    for (int i = 1; i < snake.size(); ++i)
    {
        QPoint point = snake.at(i);
        painter.setPen(Qt::black);
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
    if (currentMode == GameMode::Mode1) {

    }
    else if (currentMode == GameMode::Mode2) {
        // 绘制A*寻路路径
        painter.setPen(Qt::red);
        painter.setBrush(Qt::NoBrush);
        for (const QPoint& point : AStarPath) {
            painter.drawRect(point.x() * UNIT_SIZE, point.y() * UNIT_SIZE, UNIT_SIZE, UNIT_SIZE);
        }

    }
    else if (currentMode == GameMode::Mode3) {
        // 绘制BFS寻路路径
        painter.setPen(Qt::darkYellow);
        painter.setBrush(Qt::NoBrush);
        for (const QPoint& point : BFSPath) {
            painter.drawRect(point.x() * UNIT_SIZE, point.y() * UNIT_SIZE, UNIT_SIZE, UNIT_SIZE);
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
    painter.drawText(QRect(10, HEIGHT - 50, 200, 20), "Score: " + QString::number(score));

    qint64 elapsedSeconds = startTime.secsTo(QDateTime::currentDateTime());
    QString timeText = "Time: " + QString::number(elapsedSeconds) + "s";
    painter.drawText(QRect(10, HEIGHT - 70, 200, 20), timeText);
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

void SnakeGame::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QPoint clickPos = event->pos() / UNIT_SIZE;
        if (clickPos == food) {
            return;
        }
        else {
            if (currentMode == GameMode::Mode1)
                startMode(GameMode::Mode2);
            else if (currentMode == GameMode::Mode2)
                startMode(GameMode::Mode3);
            else if (currentMode == GameMode::Mode3)
                startMode(GameMode::Mode1, 200);
        }
    }
}

void SnakeGame::BFSFindFood()
{
    // 获取蛇头位置
    QPoint head = snake.first();

    // 使用广度优先遍历算法找到食物的位置
    QVector<QPoint> obstacles = snake.toVector();
    if(!obstacles.empty()) obstacles.removeFirst(); // 移除蛇头
    if(!obstacles.empty()) obstacles.removeLast(); // 移除蛇尾
    BFSPath = bfs.findPath(head, food, obstacles);

    if (BFSPath.isEmpty()){
        BFSPath = bfs.findPath(head, snake.last(), obstacles);
    }
}

void SnakeGame::AStarFindFood()
{
    // 模式2：蛇使用A*寻路算法自动寻找食物
    // 使用A*寻路算法找到路径
    QVector<QPoint> obstacles = snake.toVector();
    if(!obstacles.empty()) obstacles.removeFirst(); // 移除蛇头
    if(!obstacles.empty()) obstacles.removeLast(); // 移除蛇尾
    AStarPath = astar.findPath(snake.first(), food, obstacles);

    if (AStarPath.isEmpty()){
        AStarPath = astar.findPath(snake.first(), snake.last(), obstacles);
    }


}

void SnakeGame::AutoChangeSnakeDirection(const QVector<QPoint>& path)
{
    if (!path.isEmpty())
    {
        QPoint nextPoint = path[0];
        QPoint head = snake.first();
        if (nextPoint.x() < head.x())
            snakeDirection = Direction::Left;
        else if (nextPoint.x() > head.x())
            snakeDirection = Direction::Right;
        else if (nextPoint.y() < head.y())
            snakeDirection = Direction::Up;
        else if (nextPoint.y() > head.y())
            snakeDirection = Direction::Down;
    }
}

void SnakeGame::updateGame()
{
    if (currentMode == GameMode::Mode2) {
        AStarFindFood();
        AutoChangeSnakeDirection(AStarPath);
    }
    else if (currentMode == GameMode::Mode3) {
        BFSFindFood();
        AutoChangeSnakeDirection(BFSPath);
    }
    // 移动
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

    if (isGameOver(newHead))
    {
        gameTimer->stop();
        QMessageBox::information(this, "Game Over", "Game Over!");
        return;
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

    snake.prepend(newHead);


    update();
}

void SnakeGame::generateFood()
{
    QSet<QPoint> occupiedPositions(snake.begin(), snake.end());
    QPoint newFood;
    bool isOccupied = true;
    while (isOccupied) {
        int x = QRandomGenerator::global()->bounded(UNIT_COUNT_X);
        int y = QRandomGenerator::global()->bounded(UNIT_COUNT_Y);
        newFood = QPoint(x, y);
        isOccupied = occupiedPositions.contains(newFood);
    }
    food = newFood;
}

bool SnakeGame::isGameOver(const QPoint& head)
{
    if (head.x() < 0 || head.x() >= UNIT_COUNT_X || head.y() < 0 || head.y() >= UNIT_COUNT_Y)
        return true;

    for (int i = 1; i < snake.size()-1; ++i)
    {
        if (head == snake.at(i))
            return true;
    }

    return false;
}

void SnakeGame::startMode(GameMode gm, int gameTimeInterval)
{
    currentMode = gm;
    snake.clear();
    snake.append(QPoint(UNIT_COUNT_X / 2, UNIT_COUNT_Y / 2));
    score = 0;
    generateFood();
    gameTimer->start();
    gameTimer->setInterval(gameTimeInterval);
    startTime = QDateTime::currentDateTime();
    update();
}

int SnakeGame::evaluateAutoAI(GameMode gm, int times,int gameTimeInterval)
{
    int totalScore = 0;
    if(gm == GameMode::Mode1) return 0;
    for(int i = 0;i < times;i ++){
        startMode(gm, gameTimeInterval);
        totalScore += score;
    }
    return totalScore/times;

}
