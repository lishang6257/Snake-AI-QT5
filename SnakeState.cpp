// -*- coding: utf-8 -*-

#include <QIODevice>
#include <QDataStream>
#include <QHash>
#include "utils.h"

SnakeState::SnakeState()
    : QObject(),
    UNIT_COUNT_X(SnakeGameSetting::UNIT_COUNT_X),
    UNIT_COUNT_Y(SnakeGameSetting::UNIT_COUNT_Y),
    step(0),
    isGameStarted(false),
    currentGameMode(GameMode::Mode1),
    currentDirection(SnakeDirection::Right)
{}

SnakeState::SnakeState(const SnakeState &other)
    : QObject(),
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
    if (this == &other)
        return *this;

    UNIT_COUNT_X = other.UNIT_COUNT_X;
    UNIT_COUNT_Y = other.UNIT_COUNT_Y;
    isGameStarted = other.isGameStarted;
    snake = other.snake;
    food = other.food;

    currentGameMode = other.currentGameMode;
    currentDirection = other.currentDirection;
    step = other.step;


    return *this;
}

bool SnakeState::operator==(const SnakeState &other) const
{
    return UNIT_COUNT_X == other.UNIT_COUNT_X &&
           UNIT_COUNT_Y == other.UNIT_COUNT_Y &&
           isGameStarted == other.isGameStarted &&
           snake == other.snake &&
           food == other.food;
}

inline uint qHash(const SnakeState &state)
{
    QByteArray data;
    data.append(QByteArray::number(state.UNIT_COUNT_X));
    data.append(QByteArray::number(state.UNIT_COUNT_Y));
    data.append(QByteArray::number(state.isGameStarted));

    for (const QPoint &point : state.snake) {
        data.append(QByteArray::number(point.x()));
        data.append(QByteArray::number(point.y()));
    }

    data.append(QByteArray::number(state.food.x()));
    data.append(QByteArray::number(state.food.y()));

    return qHash(data);
}

QByteArray SnakeState::serialize() const
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_5_15);

    stream << UNIT_COUNT_X;
    stream << UNIT_COUNT_Y;
    stream << isGameStarted;
    stream << static_cast<int>(currentGameMode);
    stream << step;
    stream << snake;
    stream << food;
    stream << static_cast<int>(currentDirection);

    return data;
}

QByteArray SnakeState::serializeAppend() const
{
    QByteArray data = this->serialize();

    QByteArray data2;
    QDataStream stream2(&data2, QIODevice::WriteOnly);
    stream2.setVersion(QDataStream::Qt_5_15);
    stream2 << qint32(data.size());

    return data2+data;
}


QByteArray SnakeState::serialize(const QVector<SnakeState> &states) const
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_5_15);

    for (const SnakeState &state : states)
    {
        QByteArray stateData = state.serialize();
        stream << static_cast<qint32>(stateData.size());
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
    stream.setVersion(QDataStream::Qt_5_15);

    stream >> state.UNIT_COUNT_X;
    stream >> state.UNIT_COUNT_Y;
    stream >> state.isGameStarted;
    stream >> gameMode;
    stream >> state.step;
    stream >> state.snake;
    stream >> state.food;
    stream >> direction;

    state.currentDirection = static_cast<SnakeDirection>(direction);
    state.currentGameMode = static_cast<GameMode>(gameMode);

    return state;
}

QVector<SnakeState> SnakeState::deserializeQVector(const QByteArray &data)
{
    QVector<SnakeState> states;
    QDataStream stream(data);
    stream.setVersion(QDataStream::Qt_5_15);

    while(!stream.atEnd()){
        qint32 stateDataSize;
        stream >> stateDataSize;

        QByteArray stateData(stateDataSize, Qt::Uninitialized);
        stream.readRawData(stateData.data(), stateDataSize);
        SnakeState state = SnakeState::deserialize(stateData);

        states.append(state);
    }

    return states;
}

QDataStream &operator<<(QDataStream &out, const QLearningTable &table)
{
    out.setVersion(QDataStream::Qt_5_15);
    for (const auto &key : table.keys())
    {
        QByteArray stateData = key.serialize();
        out << static_cast<qint32>(stateData.size());

        out.writeRawData(stateData.data(), stateData.size());
        out << table.value(key);
    }
    return out;
}

QDataStream &operator>>(QDataStream &in, QLearningTable &table)
{
    table.clear();
    in.setVersion(QDataStream::Qt_5_15);
    while(!in.atEnd()){
        SnakeState key;
        QHash<int, double> value;

        qint32 stateDataSize;
        in >> stateDataSize;

        QByteArray stateData(stateDataSize, Qt::Uninitialized);
        in.readRawData(stateData.data(), stateDataSize);
        key = SnakeState::deserialize(stateData);

        in >> value;
        table.insert(key, value);
    }
    return in;
}

