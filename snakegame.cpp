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
    autoSave(true),
    astar(new AStar()),
    bfs(new BFS()),
    qlearning(new QLearning())
{
    snake.append(QPoint(SnakeGameSetting::UNIT_COUNT_X / 2, SnakeGameSetting::UNIT_COUNT_Y / 2));
}

SnakeGame::SnakeGame(const SnakeGame&copySnakeGame)
    :snake(copySnakeGame.snake),
    snakeDirection(copySnakeGame.snakeDirection),
    food(copySnakeGame.food),
    score(copySnakeGame.score),
    currentMode(copySnakeGame.currentMode),
    step(copySnakeGame.step),
    replayFilename(copySnakeGame.replayFilename),
    qTableFilename(copySnakeGame.qTableFilename),
    isGameStarted(copySnakeGame.isGameStarted),
    autoSave(copySnakeGame.autoSave),
    autoSaveFilename(copySnakeGame.autoSaveFilename),
    AStarPath(copySnakeGame.AStarPath),
    BFSPath(copySnakeGame.BFSPath),
    qlearning(copySnakeGame.qlearning),
    astar(copySnakeGame.astar),
    bfs(copySnakeGame.bfs)
{
}

SnakeGame::SnakeGame(QString qTableFilename)
    :snakeDirection(SnakeDirection::Right),
    score(0),
    currentMode(GameMode::Mode1),
    step(0),
    qTableFilename(qTableFilename),
    isGameStarted(false),
    autoSave(true),
    qlearning(new QLearning()),
    astar(new AStar()),
    bfs(new BFS())
{
    snake.append(QPoint(SnakeGameSetting::UNIT_COUNT_X / 2, SnakeGameSetting::UNIT_COUNT_Y / 2));
}
// 寻找食物的BFS算法
void SnakeGame::BFSFindFood()
{
    QPoint head = snake.first();

    QVector<QPoint> obstacles = snake.toVector();
    if (!obstacles.empty()) obstacles.removeFirst();
//    if (!obstacles.empty()) obstacles.removeLast();
    BFSPath = bfs->findPath(head, food, obstacles);

    if (BFSPath.isEmpty()) {
        BFSPath = bfs->findPath(head, snake.last(), obstacles);
    }
}

// 寻找食物的AStar算法
void SnakeGame::AStarFindFood()
{
    QVector<QPoint> obstacles = snake.toVector();  // 将蛇身转换为障碍物集合
    if (!obstacles.empty()) obstacles.removeFirst();  // 移除蛇头位置，防止AStar寻路算法无法处理
    if (!obstacles.empty()) obstacles.removeLast();   // 移除蛇尾位置，防止AStar寻路算法无法处理

    // 使用AStar算法寻找蛇头到食物的路径
    QVector<QPoint> SnakeToFood = astar->findPath(snake.first(), food, obstacles);

    // 判断是否满足特殊条件，可直接得到食物
    bool flag = (SnakeGameSetting::UNIT_COUNT_X % 2 == 1) && (SnakeGameSetting::UNIT_COUNT_Y % 2 == 1);
    if (flag && (snake.length() >= SnakeGameSetting::UNIT_COUNT_X * SnakeGameSetting::UNIT_COUNT_Y - 1)) {
        AStarPath = SnakeToFood;  // 直接得到食物，路径即为蛇头到食物的路径
    } else {
        SnakeGame virtualSnakeGame(*this);
        virtualSnakeGame.autoSave = false;  // 不自动保存虚拟蛇游戏的状态

        QVector<QPoint> tmpSnakeToFood = SnakeToFood;
        while (!tmpSnakeToFood.empty()) {
            virtualSnakeGame.AutoChangeSnakeDirection(tmpSnakeToFood);  // 自动调整虚拟蛇游戏的移动方向
            virtualSnakeGame.SnakeGameMoveToNextState();  // 虚拟蛇游戏移动到下一个状态
            tmpSnakeToFood.removeFirst();
        }

        QVector<QPoint> vobstacles = virtualSnakeGame.snake.toVector();  // 获取虚拟蛇游戏的障碍物集合
        if (!vobstacles.empty()) vobstacles.removeFirst();  // 移除虚拟蛇游戏的蛇头位置，防止AStar寻路算法无法处理
        if (!vobstacles.empty()) vobstacles.removeLast();   // 移除虚拟蛇游戏的蛇尾位置，防止AStar寻路算法无法处理
        QVector<QPoint> virtualSnakeToTail = astar->findPath(virtualSnakeGame.snake.first(), virtualSnakeGame.snake.last(), vobstacles);  // 寻找虚拟蛇游戏蛇头到蛇尾的路径

        // 若蛇存在直接路径到食物并且虚拟蛇游戏存在直接路径到蛇尾，则直接吃掉食物
        if (!SnakeToFood.empty() && !virtualSnakeToTail.empty()) {
            AStarPath = SnakeToFood;
        } else {
//            if (!obstacles.empty()) obstacles.removeLast();
            AStarPath = astar->findPath(snake.first(), snake.last(), obstacles, true);  // 否则使用AStar算法寻找蛇头到蛇尾的路径
        }
    }
}


// 根据寻路结果调整蛇的移动方向
void SnakeGame::AutoChangeSnakeDirection(const QVector<QPoint>& path)
{
    if (!path.isEmpty()) {
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

// 蛇移动到下一个状态
void SnakeGame::SnakeGameMoveToNextState()
{
    step += 1;
    QPoint newHead = snake.first();
    switch (snakeDirection) {
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

    if (isGameOver(newHead)) {
        isGameStarted = false;
    } else {
        if (newHead == food) {
            snake.prepend(newHead);
            generateFood();
            score += 10;
        } else {
            snake.removeLast();
            snake.prepend(newHead);
        }
        if (autoSave)
            saveSnakeToFile(getCurrentSnakeState(), autoSaveFilename);
    }
}

// 更新游戏状态
void SnakeGame::updateGame()
{
    if (currentMode == GameMode::Mode2) {
        AStarFindFood();
        AutoChangeSnakeDirection(AStarPath);
    } else if (currentMode == GameMode::Mode3) {
        BFSFindFood();
        AutoChangeSnakeDirection(BFSPath);
    } else if (currentMode == GameMode::Mode4) {
        snakeDirection = qlearning->findPath(getCurrentSnakeState());
    }
    qDebug() << QString::number((int)snakeDirection);
    SnakeGameMoveToNextState();
}

// 生成食物
void SnakeGame::generateFood()
{
    QSet<QPoint> occupiedPositions(snake.begin(), snake.end());

    if (occupiedPositions.size() == SnakeGameSetting::UNIT_COUNT_X * SnakeGameSetting::UNIT_COUNT_Y) {
        isGameStarted = false;
        return;
    }

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

// 判断游戏是否结束
bool SnakeGame::isGameOver(const QPoint& head) const
{
    if (head.x() < 0 || head.x() >= SnakeGameSetting::UNIT_COUNT_X || head.y() < 0 || head.y() >= SnakeGameSetting::UNIT_COUNT_Y) {
        return true;
    }
    for(int i = 1;i < snake.size() - 1;i ++) if(head == snake.at(i)) return true;
    return false;

}

// 开始指定模式的游戏
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

// 重置当前模式的游戏
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

// 获取当前游戏状态
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

// 判断游戏是否结束
bool SnakeGame::isGameOver()
{
    return !isGameStarted;
}

// 将当前游戏状态保存到文件
void SnakeGame::saveSnakeToFile(const SnakeState state, const QString filename)
{
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly | QIODevice::Append)) {
        QByteArray data = state.serializeAppend();
        file.write(data);
        file.close();
    }
}

// 加载QLearning的Q表
void SnakeGame::loadQLearningQTable(QString qtableFilename) const
{
    if (currentMode == GameMode::Mode4) {
        qlearning->loadQTableFromFile(qtableFilename);
    }
}

// 执行QLearning的动作
double SnakeGame::executeQLearingAction(int action)
{
    QPoint preFood = food;

    switch (action) {
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

    SnakeGameMoveToNextState();

    double reward = 0.0;
    if (snake.first() == preFood) {
        reward = 10.0;
    } else if (!isGameStarted) {
        reward = -10.0;
    }

    return reward;
}

// 获取当前游戏得分
int SnakeGame::getCurrentSankeGameScore()
{
    return score;
}
