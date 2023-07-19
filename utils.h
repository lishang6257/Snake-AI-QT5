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
    static const int UNIT_SIZE = 40;
    static const int UNIT_COUNT_X = 29;
    static const int UNIT_COUNT_Y = 29;
};

class SnakeState;

// ���� Q ������ͣ����ڱ���״̬�Ͷ�����Ӧ�� Q ֵ
typedef QHash<SnakeState, QHash<int, double>> QLearningTable;
QDataStream &operator<<(QDataStream &out, const QLearningTable &table);
QDataStream &operator>>(QDataStream &in, QLearningTable &table);
class SnakeState : QObject
{
public:
    SnakeState();
    SnakeState(const SnakeState &other); // �Զ��忽�����캯��
    SnakeState& operator=(const SnakeState &other);
    bool operator==(const SnakeState &other) const;

//    SnakeGameSetting snakeGameSetting;
    int UNIT_COUNT_X;
    int UNIT_COUNT_Y;
    int step;
    bool isGameStarted;
    GameMode currentGameMode;
    QVector<QPoint> snake;
    QPoint food;
    SnakeDirection currentDirection;

    // ��ϣ����ʵ��
    friend uint qHash(const SnakeState &state)
    {
        // ��������ǿ��Խ�SnakeState����ĸ�������ƴ�ӳ�һ���ַ�������ʹ��Qt�ṩ��qHash�������й�ϣ����
        // ����ʹ��Qt��QByteArray��Ϊ�м�������ƴ������
        QByteArray data;
        data.append(QByteArray::number(state.UNIT_COUNT_X));
        data.append(QByteArray::number(state.UNIT_COUNT_Y));
        data.append(QByteArray::number(state.isGameStarted));

        // ��������λ�ý���ƴ��
        for (const QPoint &point : state.snake) {
            data.append(QByteArray::number(point.x()));
            data.append(QByteArray::number(point.y()));
        }

        // ��ʳ��λ�ý���ƴ��
        data.append(QByteArray::number(state.food.x()));
        data.append(QByteArray::number(state.food.y()));

        // ʹ��Qt�ṩ��qHash���������ϣֵ
        return qHash(data);
    }

    // ���������л�Ϊ����������
    QByteArray serialize() const;
    QByteArray serializeAppend() const;
    // �Ӷ����������з����л�����
    static SnakeState deserialize(const QByteArray &data);

    // �� QVector<SnakeState> ���л�Ϊ��������
    QByteArray serialize(const QVector<SnakeState> &states) const;
    // �Ӷ����������з����л� QVector<SnakeState> ����
    static QVector<SnakeState> deserializeQVector(const QByteArray &data);

};

inline uint qHash(const SnakeState &state);

#endif // UTILS_H
