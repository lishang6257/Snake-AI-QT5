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

#include "astar.h"
#include "bfs.h"
#include "utils.h"

class SnakeGameWindows;
class AIEvaluator;
class SnakeState;

class SnakeGame : public QObject
{
    Q_OBJECT

    friend class AIEvaluator;
    friend class SnakeGameWindows;
    friend class SnakeState;

public:
    explicit SnakeGame();

    void startMode(GameMode gm);
    int evaluateAutoAI();
    bool isGameOver();
    SnakeState getCurrentSnakeState();

private:

    QVector<QPoint> snake;
    Direction snakeDirection;
    QPoint food;
    int score;
    GameMode currentMode;
    int step;

    bool isGameStarted;
    bool autoSave;
    QString autoSaveFilename;


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

    void saveSnakeToFile(const SnakeState ss, const QString filename);

};

#endif // SNAKEGAME_H
