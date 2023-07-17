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


class SnakeGame : public QMainWindow
{
    Q_OBJECT


public:
    explicit SnakeGame(QWidget *parent = nullptr);

    enum class GameMode {
        Mode1,
        Mode2,
        Mode3
    };
    void startMode(GameMode gm,int gameTimeInterval = 10);
    int evaluateAutoAI();
    bool isGameOver();

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void drawingFinished(QWidget *subEidgt);

private:
    enum class Direction {
        Up,
        Down,
        Left,
        Right
    };


    QVector<QPoint> snake;
    QVector<QPoint> boarderObstacles;
    Direction snakeDirection;
    QTimer *gameTimer;
    QPoint food;
    int score;
    GameMode currentMode;
    QDateTime startTime;

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
