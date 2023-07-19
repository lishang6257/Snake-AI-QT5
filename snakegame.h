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

    void startMode(GameMode gm);

    SnakeState getCurrentSnakeState() const;
    void loadQLearningQTable(QString qtableFilename) const;

    bool isGameOver();
    void resetCurrentMode();
    double executeQLearingAction(int action);

private:

    QVector<QPoint> snake;
    SnakeDirection snakeDirection;
    QPoint food;
    int score;
    GameMode currentMode;
    int step;

    QString replayFilename;
    QString qTableFilename;

    bool isGameStarted;
    bool autoSave;
    QString autoSaveFilename;


    AStar astar;
    QVector<QPoint> AStarPath; // 存储A*算法找到的路径

    BFS bfs;
    QVector<QPoint> BFSPath; // 存储A*算法找到的路径

    QLearning *qlearning;
    QLearningTable qTable;

    void SnakeGameMoveToNextState();
    void updateGame();
    void generateFood();
    void BFSFindFood();
    void AStarFindFood();
    void QLearningFindFood();
    void AutoChangeSnakeDirection(const QVector<QPoint>& path);

    void saveSnakeToFile(const SnakeState ss, const QString filename);

    int evaluateAutoAI();
    bool isGameOver(const QPoint& head) const;

};

#endif // SNAKEGAME_H
