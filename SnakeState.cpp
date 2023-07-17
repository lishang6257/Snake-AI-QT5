// -*- coding: utf-8 -*-

#include <QIODevice>
#include <QDataStream>
#include <QHash>
#include "utils.h"

SnakeState::SnakeState()
    : QObject(), // ������ø���Ŀ������캯��
    UNIT_COUNT_X(SnakeGameSetting::UNIT_COUNT_X),
    UNIT_COUNT_Y(SnakeGameSetting::UNIT_COUNT_Y),
    step(0),
    isGameStarted(false),
    currentGameMode(GameMode::Mode1),
    currentDirection(SnakeDirection::Right)
{}


// �Զ��忽�����캯��
SnakeState::SnakeState(const SnakeState &other)
    : QObject(), // ������ø���Ŀ������캯��
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
    if (this == &other) // ������Ҹ�ֵ
        return *this;

    // ���ø���Ŀ�����ֵ�����
    //    QObject::operator=(other);

    // ����������Ա����
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
bool SnakeState::operator==(const SnakeState &other) const
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

QByteArray SnakeState::serialize() const
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_5_15); // �����������İ汾��ȷ��������

    // ����Ա�������д��������
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
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_5_15); // �����������İ汾��ȷ��������

    // ����Ա�������д��������
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
    stream2.setVersion(QDataStream::Qt_5_15); // �����������İ汾��ȷ��������

    stream2 << qint32(data.size());

    return data2+data;
}


QByteArray SnakeState::serialize(const QVector<SnakeState> &states) const
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_5_15); // �����������İ汾��ȷ��������

    // ��ÿ�� SnakeState �������д��������
    for (const SnakeState &state : states)
    {
        // ��д�� SnakeState ��������л����ݳ���
        QByteArray stateData = state.serialize();
        stream << static_cast<qint32>(stateData.size());

        // ��д�� SnakeState ��������л�����
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
    stream.setVersion(QDataStream::Qt_5_15); // �����������İ汾��ȷ��������

    // ���������������ȡ���ݣ�����ֵ����Ա����
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
    stream.setVersion(QDataStream::Qt_5_15); // �����������İ汾��ȷ��������

    // ���������������ȡ SnakeState ����
    while(!stream.atEnd()){
        // ��ȡ SnakeState ��������л����ݳ���
        qint32 stateDataSize;
        stream >> stateDataSize;

        // ��ȡ SnakeState ��������л����ݣ��������л�Ϊ SnakeState ����
        QByteArray stateData(stateDataSize, Qt::Uninitialized);
        stream.readRawData(stateData.data(), stateDataSize);
        SnakeState state = SnakeState::deserialize(stateData);

        // �� SnakeState ������ӵ� QVector ��
        states.append(state);
    }

    return states;
}

// �� QLearningTable д�� QDataStream
QDataStream &operator<<(QDataStream &out, const QLearningTable &table)
{
    out.setVersion(QDataStream::Qt_5_15); // �����������İ汾��ȷ��������
    for (const auto &key : table.keys())
    {
        QByteArray stateData = key.serialize();
        out << static_cast<qint32>(stateData.size());

        // ��д�� SnakeState ��������л�����
        out.writeRawData(stateData.data(), stateData.size());
        out << table.value(key);
    }
    return out;
}

// �� QDataStream ��ȡ���ݵ� QLearningTable
QDataStream &operator>>(QDataStream &in, QLearningTable &table)
{
    table.clear();
    in.setVersion(QDataStream::Qt_5_15); // �����������İ汾��ȷ��������
    while(!in.atEnd()){
        SnakeState key;
        QHash<int, double> value;

        qint32 stateDataSize;
        in >> stateDataSize;

        // ��ȡ SnakeState ��������л����ݣ��������л�Ϊ SnakeState ����
        QByteArray stateData(stateDataSize, Qt::Uninitialized);
        in.readRawData(stateData.data(), stateDataSize);
        key = SnakeState::deserialize(stateData);

        in >> value;
        table.insert(key, value);
    }
    return in;
}

