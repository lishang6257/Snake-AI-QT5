// -*- coding: utf-8 -*-

#include <QIODevice>
#include <QDataStream>
#include "utils.h"
SnakeState::SnakeState()
    : QObject(), // 必须调用父类的拷贝构造函数
    UNIT_COUNT_X(SnakeGameSetting::UNIT_COUNT_X),
    UNIT_COUNT_Y(SnakeGameSetting::UNIT_COUNT_Y),
    step(0),
    isGameStarted(false),
    currentGameMode(GameMode::Mode1),
    currentDirection(Direction::Right)
{}


// 自定义拷贝构造函数
SnakeState::SnakeState(const SnakeState &other)
    : QObject(), // 必须调用父类的拷贝构造函数
    UNIT_COUNT_X(other.UNIT_COUNT_X),
    UNIT_COUNT_Y(other.UNIT_COUNT_Y),
    step(other.step),
    isGameStarted(other.isGameStarted),
    currentGameMode(other.currentGameMode),
    snake(other.snake),
    food(other.food),
    currentDirection(other.currentDirection)
{}

SnakeState& SnakeState::operator=(const SnakeState &other)
{
    if (this == &other) // 检查自我赋值
        return *this;

    // 调用父类的拷贝赋值运算符
    //    QObject::operator=(other);

    // 复制其他成员变量
    UNIT_COUNT_X = other.UNIT_COUNT_X;
    UNIT_COUNT_Y = other.UNIT_COUNT_Y;
    isGameStarted = other.isGameStarted;
    currentGameMode = other.currentGameMode;
    snake = other.snake;
    food = other.food;
    currentDirection = other.currentDirection;
    step = other.step;


    return *this;
}

QByteArray SnakeState::serialize() const
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_5_15); // 设置数据流的版本，确保兼容性

    // 将成员变量逐个写入数据流
    stream << UNIT_COUNT_X;
    stream << UNIT_COUNT_Y;
    stream << isGameStarted;
    stream << static_cast<int>(currentGameMode);
    stream << step;
    stream << snake;
    stream << food;
    stream << static_cast<int>(currentDirection);

    QByteArray data2;
    QDataStream stream2(&data2, QIODevice::WriteOnly);
    stream2.setVersion(QDataStream::Qt_5_15); // 设置数据流的版本，确保兼容性

    stream2 << qint32(data.size());

    return data2+data;
}

QByteArray SnakeState::serialize(const QVector<SnakeState> &states) const
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_5_15); // 设置数据流的版本，确保兼容性

    // 将每个 SnakeState 对象逐个写入数据流
    for (const SnakeState &state : states)
    {
        // 先写入 SnakeState 对象的序列化数据长度
        QByteArray stateData = state.serialize();
        stream << static_cast<qint32>(stateData.size());

        // 再写入 SnakeState 对象的序列化数据
        stream.writeRawData(stateData.data(), stateData.size());
    }

    return data;
}

SnakeState SnakeState::deserialize(const QByteArray &data)
{
    SnakeState state;
    int gameMode;
    int direction;
    QDataStream stream(data);
    stream.setVersion(QDataStream::Qt_5_15); // 设置数据流的版本，确保兼容性

    // 从数据流中逐个读取数据，并赋值给成员变量
    stream >> state.UNIT_COUNT_X;
    stream >> state.UNIT_COUNT_Y;
    stream >> state.isGameStarted;
    stream >> gameMode;
    stream >> state.step;
    stream >> state.snake;
    stream >> state.food;
    stream >> direction;

    state.currentDirection = static_cast<Direction>(direction);
    state.currentGameMode = static_cast<GameMode>(gameMode);

    return state;
}

QVector<SnakeState> SnakeState::deserializeQVector(const QByteArray &data)
{
    QVector<SnakeState> states;
    QDataStream stream(data);
    stream.setVersion(QDataStream::Qt_5_15); // 设置数据流的版本，确保兼容性

    // 从数据流中逐个读取 SnakeState 对象
    while(!stream.atEnd()){
        // 读取 SnakeState 对象的序列化数据长度
        qint32 stateDataSize;
        stream >> stateDataSize;

        // 读取 SnakeState 对象的序列化数据，并反序列化为 SnakeState 对象
        QByteArray stateData(stateDataSize, Qt::Uninitialized);
        stream.readRawData(stateData.data(), stateDataSize);
        SnakeState state = SnakeState::deserialize(stateData);

        // 将 SnakeState 对象添加到 QVector 中
        states.append(state);
    }

    return states;
}
//// 实现哈希函数
//uint AIState::qHash() const
//{
//    // 以头部位置和食物位置的哈希值为基础，再加上蛇身长度作为哈希值
//    return qHash(headPosition) ^ qHash(foodPosition) ^ snakeLength;
//}

