#ifndef SNAKEGAME_H
#define SNAKEGAME_H

#include <QMainWindow>
#include <QTimer>
#include <QVector>
#include <QPoint>
#include <QKeyEvent>
#include <QDateTime>
#include <QMessageBox>
#include <QPainter>
#include <QPaintEvent>

class SnakeGame : public QMainWindow
{
    Q_OBJECT

public:
    explicit SnakeGame(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void updateGame();

private:
    static constexpr int WIDTH = 400;         // 游戏窗口宽度
    static constexpr int HEIGHT = 400;        // 游戏窗口高度
    static constexpr int UNIT_SIZE = 20;      // 单元格大小
    static constexpr int UNIT_COUNT_X = WIDTH / UNIT_SIZE;   // 单元格数量（水平方向）
    static constexpr int UNIT_COUNT_Y = HEIGHT / UNIT_SIZE;  // 单元格数量（垂直方向）

    QVector<QPoint> snake;
    enum class Direction {
        Up,
        Down,
        Left,
        Right
    };
    Direction snakeDirection;
    QPoint food;
    QTimer* gameTimer;
    int score;
    QDateTime startTime;

    void generateFood();
    bool isGameOver(const QPoint& head);
};

#endif // SNAKEGAME_H
