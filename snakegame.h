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


class SnakeGame : public QMainWindow
{
    Q_OBJECT

public:
    explicit SnakeGame(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    enum class Direction {
        Up,
        Down,
        Left,
        Right
    };

    enum class GameMode {
        Mode1,
        Mode2
    };

    QVector<QPoint> snake;
    Direction snakeDirection;
    QTimer *gameTimer;
    QPoint food;
    int score;
    GameMode currentMode;
    QDateTime startTime;
    AStar astar;
    QVector<QPoint> AStarPath; // 存储A*算法找到的路径

    void updateGame();
    void generateFood();
    bool isGameOver(const QPoint& head);
    void startMode1();
    void startMode2();
};

#endif // SNAKEGAME_H
