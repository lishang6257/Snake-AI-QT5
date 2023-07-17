#ifndef SNAKEGAMEWINDOWS_H
#define SNAKEGAMEWINDOWS_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QTimer>
#include <QVector>
#include <QPoint>
#include <QMouseEvent>
#include <QDateTime>

#include "utils.h"
#include "snakegame.h"


class SnakeGameWindows : public QMainWindow
{
    Q_OBJECT


public:
    explicit SnakeGameWindows(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void drawingFinished(QWidget *subEidgt);


private:
    QTimer *gameTimer;
    GameMode currentMode;
    QDateTime startTime;
    int gameDuringTime;
    bool isGameStarted;

    SnakeGame snakeGame;

    void startSnakeGameMode(GameMode gm,int gameTimeInterval = 10);
    void updateSnakeGame();

};


#endif // SNAKEGAMEWINDOWS_H
