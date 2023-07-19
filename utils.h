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

// 定义贪吃蛇的动作
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

// 定义 Q 表的类型，用于保存状态和动作对应的 Q 值
typedef QHash<SnakeState, QHash<int, double>> QLearningTable;
QDataStream &operator<<(QDataStream &out, const QLearningTable &table);
QDataStream &operator>>(QDataStream &in, QLearningTable &table);
class SnakeState : QObject
{
public:
    SnakeState();
    SnakeState(const SnakeState &other); // 自定义拷贝构造函数
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

    // 哈希函数实现
    friend uint qHash(const SnakeState &state)
    {
        // 在这里，我们可以将SnakeState对象的各个属性拼接成一个字符串，并使用Qt提供的qHash函数进行哈希计算
        // 这里使用Qt的QByteArray作为中间数据来拼接属性
        QByteArray data;
        data.append(QByteArray::number(state.UNIT_COUNT_X));
        data.append(QByteArray::number(state.UNIT_COUNT_Y));
        data.append(QByteArray::number(state.isGameStarted));

        // 对蛇身体位置进行拼接
        for (const QPoint &point : state.snake) {
            data.append(QByteArray::number(point.x()));
            data.append(QByteArray::number(point.y()));
        }

        // 对食物位置进行拼接
        data.append(QByteArray::number(state.food.x()));
        data.append(QByteArray::number(state.food.y()));

        // 使用Qt提供的qHash函数计算哈希值
        return qHash(data);
    }

    // 将对象序列化为二进制数据
    QByteArray serialize() const;
    QByteArray serializeAppend() const;
    // 从二进制数据中反序列化对象
    static SnakeState deserialize(const QByteArray &data);

    // 将 QVector<SnakeState> 序列化为二进制数
    QByteArray serialize(const QVector<SnakeState> &states) const;
    // 从二进制数据中反序列化 QVector<SnakeState> 对象
    static QVector<SnakeState> deserializeQVector(const QByteArray &data);

};

inline uint qHash(const SnakeState &state);

#endif // UTILS_H
