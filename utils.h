// -*- coding: utf-8 -*-

#ifndef UTILS_H
#define UTILS_H

#include "qobject.h"
#include <QPoint>

enum class GameMode {
    Mode1,//Mannaul
    Mode2,//A*
    Mode3,//BFS
    Mode4,//QLearning
    Mode5 //replay
};

enum class SnakeDirection {
    Up,
    Down,
    Left,
    Right
};

// ����̰���ߵĶ���
enum class SnakeAction {
    ActionUp,
    ActionDown,
    ActionLeft,
    ActionRight
};

const QVector<QPoint> DirectionState = {QPoint(0,-1),QPoint(0,1),QPoint(-1,0),QPoint(1,0)};

class SnakeGameSetting{
public:
    static const int UNIT_SIZE = 20;
    static const int UNIT_COUNT_X = 30;
    static const int UNIT_COUNT_Y = 20;
};

class SnakeState;

// ���� Q ������ͣ����ڱ���״̬�Ͷ�����Ӧ�� Q ֵ
typedef QHash<SnakeState, QHash<int, double>> QLearingTable;

class SnakeState : QObject
{
public:
    SnakeState();
    SnakeState(const SnakeState &other); // �Զ��忽�����캯��
    SnakeState& operator=(const SnakeState &other);
    bool operator==(const SnakeState &other) const
    {
        // ��������ӱȽ���������ĳ�Ա�����Ƿ���ȵ��߼�
        // ע�⣺����ıȽ��߼�Ӧ�ú���������Ҫ�Ƚϵĳ�Ա����

        return UNIT_COUNT_X == other.UNIT_COUNT_X &&
               UNIT_COUNT_Y == other.UNIT_COUNT_Y &&
               step == other.step &&
               isGameStarted == other.isGameStarted &&
               currentGameMode == other.currentGameMode &&
               snake == other.snake &&
               food == other.food &&
               currentDirection == other.currentDirection;
    }


//    SnakeGameSetting snakeGameSetting;
    int UNIT_COUNT_X;
    int UNIT_COUNT_Y;
    int step;
    bool isGameStarted;
    GameMode currentGameMode;
    QVector<QPoint> snake;
    QPoint food;
    SnakeDirection currentDirection;

    friend uint qHash(const SnakeState &state) {
        return qHash(state.snake.first()) ^ qHash(state.food) ^ state.step;
    }

    // ���������л�Ϊ����������
    QByteArray serialize() const;
    // �Ӷ����������з����л�����
    static SnakeState deserialize(const QByteArray &data);

    // �� QVector<SnakeState> ���л�Ϊ��������
    QByteArray serialize(const QVector<SnakeState> &states) const;
    // �Ӷ����������з����л� QVector<SnakeState> ����
    static QVector<SnakeState> deserializeQVector(const QByteArray &data);

};

inline uint qHash(const SnakeState &state);

#endif // UTILS_H
