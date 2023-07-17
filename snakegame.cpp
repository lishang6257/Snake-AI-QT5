#include "snakegame.h"
#include <QPainter>
#include <QKeyEvent>
#include <QRandomGenerator>
#include <QMessageBox>


SnakeGame::SnakeGame()
    :snakeDirection(Direction::Right),
    score(0),
    currentMode(GameMode::Mode1),
    isGameStarted(false)
{
    snake.append(QPoint(SnakeGameSetting::UNIT_COUNT_X / 2, SnakeGameSetting::UNIT_COUNT_Y / 2)); // 初始化贪吃蛇位置
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
        isGameStarted = false;
        emit gameOver();
        return;
    }

    if (newHead == food)
    {
        snake.prepend(newHead);
        generateFood();
        score += 10;
    }
    else
    {
        snake.removeLast();
        snake.prepend(newHead);
    }

}

void SnakeGame::generateFood()
{
    QSet<QPoint> occupiedPositions(snake.begin(), snake.end());
    QPoint newFood;
    bool isOccupied = true;
    while (isOccupied) {
        int x = QRandomGenerator::global()->bounded(SnakeGameSetting::UNIT_COUNT_X);
        int y = QRandomGenerator::global()->bounded(SnakeGameSetting::UNIT_COUNT_Y);
        newFood = QPoint(x, y);
        isOccupied = occupiedPositions.contains(newFood);
    }
    food = newFood;
}

bool SnakeGame::isGameOver(const QPoint& head)
{
    if (head.x() < 0 || head.x() >= SnakeGameSetting::UNIT_COUNT_X || head.y() < 0 || head.y() >= SnakeGameSetting::UNIT_COUNT_Y)
    {
        return true;
    }
    for (int i = 1; i < snake.size()-1; ++i)
    {
        if (head == snake.at(i))
        {
            return true;
        }
    }

    return false;
}

void SnakeGame::startMode(GameMode gm)
{
    currentMode = gm;
    snake.clear();
    snake.append(QPoint(SnakeGameSetting::UNIT_COUNT_X / 2, SnakeGameSetting::UNIT_COUNT_Y / 2));
    score = 0;
    generateFood();
    isGameStarted = true;
}

int SnakeGame::evaluateAutoAI()
{
    return score;
}

bool SnakeGame::isGameOver()
{
    return !isGameStarted;
}
