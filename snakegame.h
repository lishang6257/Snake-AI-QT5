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
    static constexpr int WIDTH = 400;         // ��Ϸ���ڿ��
    static constexpr int HEIGHT = 400;        // ��Ϸ���ڸ߶�
    static constexpr int UNIT_SIZE = 20;      // ��Ԫ���С
    static constexpr int UNIT_COUNT_X = WIDTH / UNIT_SIZE;   // ��Ԫ��������ˮƽ����
    static constexpr int UNIT_COUNT_Y = HEIGHT / UNIT_SIZE;  // ��Ԫ����������ֱ����

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
