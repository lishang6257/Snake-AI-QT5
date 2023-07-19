// -*- coding: utf-8 -*-

#ifndef QLEARNING_H
#define QLEARNING_H

#include <QObject>
#include <QHash>
#include <QVector>
#include <QTime>
#include <QRandomGenerator>
#include "utils.h"
#include "snakegame.h"

class QLearning : public QObject
{
    Q_OBJECT

private:
    // 初始化 Q 表
    QLearningTable qTable;

public:
    explicit QLearning(QObject *parent = nullptr);

    // 将 Q 表保存到文件中
    void saveQTableToFile(const QString& filename) const;

    // 从文件中加载 Q 表
    QLearningTable loadQTableFromFile(const QString& filename);

    // Q 学习函数
    void train(SnakeGame& game, int numEpisodes, double learningRate, double discountFactor, double explorationProb);

    // 根据当前状态找到贪吃蛇的移动方向
    SnakeDirection findPath(const SnakeState& currentState);
};

#endif // QLEARNING_H
