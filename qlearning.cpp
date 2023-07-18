#include "qlearning.h"
#include <QFile>
#include <QDataStream>
#include <QJsonObject>
#include <QJsonDocument>
#include <QString>

QLearning::QLearning(QObject *parent)
{}
// 实现 QLearning 类中的 train() 函数
void QLearning::train(SnakeGame& game, int numEpisodes, double learningRate, double discountFactor, double explorationProb)
{
    // 初始化随机数种子
    // 创建一个新的 QRandomGenerator 实例
    QRandomGenerator randomGenerator(QRandomGenerator::global()->generate());
    randomGenerator.seed(QTime::currentTime().msec());
//    randomGenerator.seed(0);

    // 开始执行 Q 学习算法
    for (int episode = 0; episode < numEpisodes; ++episode)
    {
        // 重置游戏状态
        game.resetCurrentMode();

        while (!game.isGameOver())
        {
            // 获取当前游戏状态
            SnakeState currentState = game.getCurrentSnakeState();

            // 选择动作
            int action;
            if (qTable.contains(currentState) && qTable[currentState].contains((int)SnakeAction::ActionUp))
            {
                // 根据贪婪策略选择动作
                if (randomGenerator.generateDouble() < explorationProb)
                {
                    // 探索性动作
                    action = randomGenerator.bounded(4);
                }
                else
                {
                    // 贪婪动作
                    action = qTable[currentState].key(qTable[currentState].values().at(0));
                }
            }
            else
            {
                // 随机选择动作
                action = randomGenerator.bounded(4);
            }

            // 执行动作并获取奖励
            double reward = game.executeQLearingAction(action);

            // 获取下一个状态
            SnakeState nextState = game.getCurrentSnakeState();

            // 更新 Q 表
            if (!qTable.contains(currentState))
            {
                qTable[currentState] = QHash<int, double>();
            }

            if (!qTable[nextState].contains((int)SnakeAction::ActionUp))
            {
                qTable[nextState] = QHash<int, double>();
            }

            double maxNextQValue = qTable[nextState].isEmpty() ? 0 : qTable[nextState].values().at(0);
            double currentQValue = qTable[currentState][action];
            double updatedQValue = currentQValue + learningRate * (reward + discountFactor * maxNextQValue - currentQValue);
            qTable[currentState][action] = updatedQValue;
        }
        QString autoSaveFilename = "./qTable/SnakeGameQTable_E" + QString::number(episode) + ".dat";
        qDebug() << "epoch " + QString::number(episode);
        if((episode%500) == 0)saveQTableToFile(autoSaveFilename);
    }
}

QLearningTable QLearning::loadQTableFromFile(const QString &filename)
{
    QLearningTable table;
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly))
    {
        QDataStream stream(&file);
        stream >> table;
        file.close();
    }
    qTable = table;
    return table;
}

void QLearning::saveQTableToFile(const QString& filename) const
{
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly))
    {
        QDataStream stream(&file);
        stream << qTable;
        file.close();
    }
}


SnakeDirection QLearning::findPath(const SnakeState& currentState)
{
    if (qTable.contains(currentState) && !qTable[currentState].isEmpty())
    {
        int bestAction = -1;
        double maxQValue = - std::numeric_limits<double>::max();

        for (const int action : qTable[currentState].keys())
        {
            double qValue = qTable[currentState][action];
            if (qValue > maxQValue)
            {
                maxQValue = qValue;
                bestAction = action;
            }
        }

        return (SnakeDirection)bestAction;
    }

    // 如果 Q 表中没有当前状态的数据，随机选择一个动作
    return SnakeDirection::Up;
}
