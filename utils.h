// -*- coding: utf-8 -*-

#ifndef UTILS_H
#define UTILS_H

#include "qobject.h"
#include <QPoint>

enum class GameMode {
    Mode1,
    Mode2,
    Mode3
};

enum class Direction {
    Up,
    Down,
    Left,
    Right
};

const QVector<QPoint> DirectionState = {QPoint(0,-1),QPoint(0,1),QPoint(-1,0),QPoint(1,0)};

class SnakeGameSetting{
public:
    static const int UNIT_SIZE = 20;
    static const int UNIT_COUNT_X = 30;
    static const int UNIT_COUNT_Y = 20;
};

class SnakeState : QObject
{
public:
    SnakeState();
    SnakeState(const SnakeState &other); // 自定义拷贝构造函数
    SnakeState& operator=(const SnakeState &other);


//    SnakeGameSetting snakeGameSetting;
    int UNIT_COUNT_X;
    int UNIT_COUNT_Y;
    int step;
    bool isGameStarted;
    GameMode currentGameMode;
    QVector<QPoint> snake;
    QPoint food;
    Direction currentDirection;


    // 将对象序列化为二进制数据
    QByteArray serialize() const;
    // 从二进制数据中反序列化对象
    static SnakeState deserialize(const QByteArray &data);

    // 将 QVector<SnakeState> 序列化为二进制数
    QByteArray serialize(const QVector<SnakeState> &states) const;
    // 从二进制数据中反序列化 QVector<SnakeState> 对象
    static QVector<SnakeState> deserializeQVector(const QByteArray &data);

};



#endif // UTILS_H
