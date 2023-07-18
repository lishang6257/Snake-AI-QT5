// -*- coding: utf-8 -*-

#include "snakegame.h"
#include "qlearning.h"
#include <QPainter>
#include <QKeyEvent>
#include <QRandomGenerator>
#include <QMessageBox>
#include <QFile>

SnakeGame::SnakeGame()
    :snakeDirection(SnakeDirection::Right),
    score(0),
    currentMode(GameMode::Mode1),
    step(0),
    isGameStarted(false),
    autoSave(false),
    qlearning(new QLearning())
{
    snake.append(QPoint(SnakeGameSetting::UNIT_COUNT_X / 2, SnakeGameSetting::UNIT_COUNT_Y / 2));
}

SnakeGame::SnakeGame(QString qTableFilename)
    :snakeDirection(SnakeDirection::Right),
    score(0),
    currentMode(GameMode::Mode1),
    step(0),
    qTableFilename(qTableFilename),
    isGameStarted(false),
    autoSave(false),
    qlearning(new QLearning())
{
    snake.append(QPoint(SnakeGameSetting::UNIT_COUNT_X / 2, SnakeGameSetting::UNIT_COUNT_Y / 2));
}

void SnakeGame::BFSFindFood()
{
    QPoint head = snake.first();

    QVector<QPoint> obstacles = snake.toVector();
    if(!obstacles.empty()) obstacles.removeFirst();
    if(!obstacles.empty()) obstacles.removeLast();
    BFSPath = bfs.findPath(head, food, obstacles);

    if (BFSPath.isEmpty()){
        BFSPath = bfs.findPath(head, snake.last(), obstacles);
    }
}

void SnakeGame::AStarFindFood()
{
    QVector<QPoint> obstacles = snake.toVector();
    if(!obstacles.empty()) obstacles.removeFirst();
    if(!obstacles.empty()) obstacles.removeLast();
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
            snakeDirection = SnakeDirection::Left;
        else if (nextPoint.x() > head.x())
            snakeDirection = SnakeDirection::Right;
        else if (nextPoint.y() < head.y())
            snakeDirection = SnakeDirection::Up;
        else if (nextPoint.y() > head.y())
            snakeDirection = SnakeDirection::Down;
    }
}

void SnakeGame::SnakeGameMoveToNextState()
{
    step += 1;
    QPoint newHead = snake.first();
    switch (snakeDirection)
    {
    case SnakeDirection::Up:
        newHead.setY(newHead.y() - 1);
        break;
    case SnakeDirection::Down:
        newHead.setY(newHead.y() + 1);
        break;
    case SnakeDirection::Left:
        newHead.setX(newHead.x() - 1);
        break;
    case SnakeDirection::Right:
        newHead.setX(newHead.x() + 1);
        break;
    }

    if (isGameOver(newHead))
    {
        isGameStarted = false;
    }
    else{
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
        if(autoSave)
            saveSnakeToFile(getCurrentSnakeState(), autoSaveFilename);
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
    }else if (currentMode == GameMode::Mode4) {
        snakeDirection = qlearning->findPath(getCurrentSnakeState());
    }
    qDebug() << QString::number((int)snakeDirection);
    SnakeGameMoveToNextState();
}

void SnakeGame::generateFood()
{
    QSet<QPoint> occupiedPositions(snake.begin(), snake.end());
    QPoint newFood;
    bool isOccupied = true;

    QRandomGenerator randomGenerator(QRandomGenerator::global()->generate());
//    randomGenerator.seed(0);
    randomGenerator.seed(QTime::currentTime().msec());


    while (isOccupied) {
        int x = randomGenerator.bounded(SnakeGameSetting::UNIT_COUNT_X);
        int y = randomGenerator.bounded(SnakeGameSetting::UNIT_COUNT_Y);
        newFood = QPoint(x, y);
        isOccupied = occupiedPositions.contains(newFood);
    }
    food = newFood;
}

bool SnakeGame::isGameOver(const QPoint& head) const
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

    QDateTime now = QDateTime::currentDateTime();
    QString timestamp = now.toString("_yyyyMMdd_hhmmss");
    autoSaveFilename = "SnakeGame" + timestamp + ".dat";
    QFile file(autoSaveFilename);
}

void SnakeGame::resetCurrentMode()
{
    snake.clear();
    snake.append(QPoint(SnakeGameSetting::UNIT_COUNT_X / 2, SnakeGameSetting::UNIT_COUNT_Y / 2));
    score = 0;
    generateFood();
    isGameStarted = true;

    QDateTime now = QDateTime::currentDateTime();
    QString timestamp = now.toString("_yyyyMMdd_hhmmss");
    autoSaveFilename = "SnakeGame" + timestamp + ".dat";
    QFile file(autoSaveFilename);
}

SnakeState SnakeGame::getCurrentSnakeState() const
{
    SnakeState state;
    state.UNIT_COUNT_X = SnakeGameSetting::UNIT_COUNT_X;
    state.UNIT_COUNT_Y = SnakeGameSetting::UNIT_COUNT_Y;
    state.currentGameMode = currentMode;
    state.currentDirection = snakeDirection;
    state.isGameStarted = isGameStarted;
    state.snake = snake;
    state.food = food;
    state.step = step;
    return state;
}

bool SnakeGame::isGameOver()
{
    return !isGameStarted;
}

void SnakeGame::saveSnakeToFile(const SnakeState state,const QString filename)
{
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        QByteArray data = state.serializeAppend();
        file.write(data);
        file.close();
    }
}

void SnakeGame::loadQLearningQTable(QString qtableFilename) const
{
    if(currentMode == GameMode::Mode4){
        qlearning->loadQTableFromFile(qtableFilename);
    }
}

double SnakeGame::executeQLearingAction(int action)
{
    // 备份当前贪吃蛇的位置，用于后面判断是否吃到食物
    QPoint preFood = food;
    double preDisFromHeadToFood = (snake.first() - food).manhattanLength();

    // 更新贪吃蛇的移动方向
    switch (action)
    {
    case (int)SnakeAction::ActionUp:
        snakeDirection = SnakeDirection::Up;
        break;
    case (int)SnakeAction::ActionDown:
        snakeDirection = SnakeDirection::Down;
        break;
    case (int)SnakeAction::ActionLeft:
        snakeDirection = SnakeDirection::Left;
        break;
    case (int)SnakeAction::ActionRight:
        snakeDirection = SnakeDirection::Right;
        break;
    }

    // 更新游戏状态，使贪吃蛇向当前方向移动一步
    SnakeGameMoveToNextState();

    // 判断是否吃到了食物，计算奖励值
    double reward = 0.0;
    if (snake.first() == preFood)
    {
        // 如果贪吃蛇吃到了食物，奖励值为正数，增加得分
        reward = 10.0;
    }
    else if (!isGameStarted)
    {
        // 如果游戏结束，奖励值为负数，表示游戏结束
        reward = -10.0;
    }

    //靠近食物奖励
    double curDisFromHeadToFood = (snake.first() - preFood).manhattanLength();

//    reward += 0.01 * 1./(preDisFromHeadToFood - curDisFromHeadToFood);

//    if(curDisFromHeadToFood < preDisFromHeadToFood)
//    {
//        reward += 0.001;
//    }
//    else
//    {

//        reward -= 0.0001;
//    }

    // 返回奖励值
    return reward;
}

int SnakeGame::evaluateAutoAI()
{
    return score;
}
