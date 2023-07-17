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
    snake.append(QPoint(SnakeGameSetting::UNIT_COUNT_X / 2, SnakeGameSetting::UNIT_COUNT_Y / 2)); // ��ʼ��̰����λ��
}

void SnakeGame::BFSFindFood()
{
    // ��ȡ��ͷλ��
    QPoint head = snake.first();

    // ʹ�ù�����ȱ����㷨�ҵ�ʳ���λ��
    QVector<QPoint> obstacles = snake.toVector();
    if(!obstacles.empty()) obstacles.removeFirst(); // �Ƴ���ͷ
    if(!obstacles.empty()) obstacles.removeLast(); // �Ƴ���β
    BFSPath = bfs.findPath(head, food, obstacles);

    if (BFSPath.isEmpty()){
        BFSPath = bfs.findPath(head, snake.last(), obstacles);
    }
}

void SnakeGame::AStarFindFood()
{
    // ģʽ2����ʹ��A*Ѱ·�㷨�Զ�Ѱ��ʳ��
    // ʹ��A*Ѱ·�㷨�ҵ�·��
    QVector<QPoint> obstacles = snake.toVector();
    if(!obstacles.empty()) obstacles.removeFirst(); // �Ƴ���ͷ
    if(!obstacles.empty()) obstacles.removeLast(); // �Ƴ���β
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
    // �ƶ�
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
