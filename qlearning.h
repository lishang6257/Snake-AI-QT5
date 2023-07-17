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
    QLearingTable qTable;

public:
    explicit QLearning(QObject *parent = nullptr);


    void saveQTableToFile(const QString& filename) const;
    QLearingTable loadQTableFromFile(const QString& filename);

    // Q 学习函数
    void train(SnakeGame& game, int numEpisodes, double learningRate, double discountFactor, double explorationProb);

    SnakeDirection findPath(const SnakeState& currentState);



};

#endif // QLEARNING_H
