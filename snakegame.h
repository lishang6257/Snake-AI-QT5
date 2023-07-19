// -*- coding: utf-8 -*-

#ifndef SNAKEGAME_H
#define SNAKEGAME_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QTimer>
#include <QVector>
#include <QPoint>
#include <QMouseEvent>
#include <QDateTime>

#include "utils.h"
#include "astar.h"
#include "bfs.h"

class SnakeGameWindows;
class AIEvaluator;
class SnakeState;
class QLearning;


class SnakeGame : public QObject
{
    Q_OBJECT

    friend class AIEvaluator;
    friend class SnakeGameWindows;
    friend class SnakeState;

public:
    explicit SnakeGame();
    explicit SnakeGame(const SnakeGame&copySnakeGame);
    explicit SnakeGame(QString qTableFilename);

    // 启动特定的游戏模式
    void startMode(GameMode gm);

    // 获取当前贪吃蛇的状态
    SnakeState getCurrentSnakeState() const;

    // 加载 Q 学习的 Q 表
    void loadQLearningQTable(QString qtableFilename) const;

    // 判断游戏是否结束
    bool isGameOver();

    // 重置当前游戏模式
    void resetCurrentMode();

    // 执行 Q 学习的动作并返回奖励
    double executeQLearingAction(int action);

private:
    QVector<QPoint> snake;         // 贪吃蛇的身体坐标
    SnakeDirection snakeDirection; // 贪吃蛇当前移动方向
    QPoint food;                   // 食物的坐标
    int score;                     // 得分
    GameMode currentMode;          // 当前游戏模式
    int step;                      // 记录游戏进行的步数

    QString replayFilename;        // 回放文件名
    QString qTableFilename;        // Q 表文件名

    bool isGameStarted;            // 标识游戏是否已开始
    bool autoSave;                 // 是否自动保存游戏状态
    QString autoSaveFilename;      // 自动保存文件名


    AStar *astar;                  // A* 算法对象
    QVector<QPoint> AStarPath;     // A* 算法找到的路径

    BFS *bfs;                      // BFS 算法对象
    QVector<QPoint> BFSPath;       // BFS 算法找到的路径

    QLearning *qlearning;          // Q 学习对象
    QLearningTable qTable;         // Q 学习的 Q 表

    // 贪吃蛇移动到下一个状态
    void SnakeGameMoveToNextState();

    // 更新游戏状态
    void updateGame();

    // 生成新的食物
    void generateFood();

    // 使用 BFS 算法寻找食物
    void BFSFindFood();

    // 使用 A* 算法寻找食物
    void AStarFindFood();

    // 使用 Q 学习算法寻找食物
    void QLearningFindFood();

    // 根据路径自动改变贪吃蛇的移动方向
    void AutoChangeSnakeDirection(const QVector<QPoint>& path);

    // 将贪吃蛇的状态保存到文件中
    void saveSnakeToFile(const SnakeState ss, const QString filename);

    int getCurrentSankeGameScore();

    // 判断游戏是否结束
    bool isGameOver(const QPoint& head) const;
};

#endif // SNAKEGAME_H
