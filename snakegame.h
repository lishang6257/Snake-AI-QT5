#ifndef SNAKEGAME_H
#define SNAKEGAME_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QTimer>
#include <QVector>
#include <QPoint>
#include <QMouseEvent>
#include <QDateTime>

#include "astar.h"
#include "bfs.h"
#include "utils.h"

class SnakeGameWindows;

class SnakeGame : public QObject
{
    Q_OBJECT

    friend class SnakeGameWindows;

public:
    explicit SnakeGame();

    void startMode(GameMode gm);
    int evaluateAutoAI();
    bool isGameOver();

signals:
    void gameOver();

private:
    enum class Direction {
        Up,
        Down,
        Left,
        Right
    };


    QVector<QPoint> snake;
    Direction snakeDirection;
    QPoint food;
    int score;
    GameMode currentMode;

    bool isGameStarted;

    AStar astar;
    QVector<QPoint> AStarPath; // 存储A*算法找到的路径

    BFS bfs;
    QVector<QPoint> BFSPath; // 存储A*算法找到的路径

    void updateGame();
    void generateFood();
    void BFSFindFood();
    void AStarFindFood();
    void AutoChangeSnakeDirection(const QVector<QPoint>& path);
    bool isGameOver(const QPoint& head);

};

#endif // SNAKEGAME_H
