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
    SnakeState(const SnakeState &other); // �Զ��忽�����캯��
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


    // ���������л�Ϊ����������
    QByteArray serialize() const;
    // �Ӷ����������з����л�����
    static SnakeState deserialize(const QByteArray &data);

    // �� QVector<SnakeState> ���л�Ϊ��������
    QByteArray serialize(const QVector<SnakeState> &states) const;
    // �Ӷ����������з����л� QVector<SnakeState> ����
    static QVector<SnakeState> deserializeQVector(const QByteArray &data);

};



#endif // UTILS_H
