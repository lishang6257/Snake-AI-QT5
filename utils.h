// -*- coding: utf-8 -*-

#ifndef UTILS_H
#define UTILS_H

#include "qobject.h"
#include <QPoint>

// 游戏模式枚举
enum class GameMode {
    Mode1, // Mannual
    Mode2, // A*
    Mode3, // BFS
    Mode4, // QLearning
    Mode5  // Replay
};

// 贪吃蛇移动方向枚举
enum class SnakeDirection {
    Up,
    Down,
    Left,
    Right
};

// 贪吃蛇动作枚举
enum class SnakeAction {
    ActionUp,
    ActionDown,
    ActionLeft,
    ActionRight
};

// 定义贪吃蛇的四个移动方向
static const QVector<QPoint> DirectionState = {QPoint(0, -1), QPoint(0, 1), QPoint(-1, 0), QPoint(1, 0)};

class SnakeGameSetting {
public:
    static const int UNIT_SIZE = 40; // 单位大小
    static const int UNIT_COUNT_X = 17; // 水平方向单元格数
    static const int UNIT_COUNT_Y = 17; // 垂直方向单元格数
};

class SnakeState;
// 自定义哈希函数，用于在 QLearningTable 中保存状态和对应的 Q 值
inline uint qHash(const SnakeState &state);

// 定义 QLearningTable 类型，用于保存状态和动作对应的 Q 值
typedef QHash<SnakeState, QHash<int, double>> QLearningTable;

// 重载数据流输出操作符，用于将 QLearningTable 写入数据流
QDataStream &operator<<(QDataStream &out, const QLearningTable &table);
// 重载数据流输入操作符，用于从数据流中读取 QLearningTable
QDataStream &operator>>(QDataStream &in, QLearningTable &table);

class SnakeState : QObject {
public:
    SnakeState();
    SnakeState(const SnakeState &other); // 自定义拷贝构造函数
    SnakeState& operator=(const SnakeState &other);
    bool operator==(const SnakeState &other) const;

    int UNIT_COUNT_X;
    int UNIT_COUNT_Y;
    int step;
    bool isGameStarted;
    GameMode currentGameMode;
    QVector<QPoint> snake;
    QPoint food;
    SnakeDirection currentDirection;

    // 将对象序列化为二进制数据
    QByteArray serialize() const;
    QByteArray serializeAppend() const;
    // 从二进制数据中反序列化对象
    static SnakeState deserialize(const QByteArray &data);

    // 将 QVector<SnakeState> 序列化为二进制数据
    QByteArray serialize(const QVector<SnakeState> &states) const;
    // 从二进制数据中反序列化 QVector<SnakeState> 对象
    static QVector<SnakeState> deserializeQVector(const QByteArray &data);
};

#endif // UTILS_H
