#include "qlearning.h"
#include <QFile>
#include <QDataStream>
#include <QJsonObject>
#include <QJsonDocument>
#include <QString>

QLearning::QLearning(QObject *parent)
{}

// QLearning算法的训练函数，用于训练智能体学习最佳策略
void QLearning::train(SnakeGame& game, int numEpisodes, double learningRate, double discountFactor, double explorationProb)
{
    QRandomGenerator randomGenerator(QRandomGenerator::global()->generate());
    randomGenerator.seed(QTime::currentTime().msec());
    //    randomGenerator.seed(0);

    for (int episode = 0; episode < numEpisodes; ++episode)
    {
        game.resetCurrentMode(); // 重置当前游戏模式，开始新的一局游戏

        while (!game.isGameOver()) // 在游戏没有结束的情况下进行训练
        {
            SnakeState currentState = game.getCurrentSnakeState(); // 获取当前贪吃蛇的状态

            int action;
            if (qTable.contains(currentState) && qTable[currentState].contains((int)SnakeAction::ActionUp))
            {
                // 如果当前状态已经在Q表中存在，则根据贪心策略或者随机策略选择动作
                if (randomGenerator.generateDouble() < explorationProb)
                {
                    // 随机选择动作
                    action = randomGenerator.bounded(4);
                }
                else
                {
                    // 根据贪心策略选择最优动作
                    action = qTable[currentState].key(qTable[currentState].values().at(0));
                }
            }
            else
            {
                // 如果当前状态在Q表中不存在，则随机选择动作
                action = randomGenerator.bounded(4);
            }

            double reward = game.executeQLearingAction(action); // 执行选择的动作并获取奖励

            SnakeState nextState = game.getCurrentSnakeState(); // 获取执行动作后的下一个状态

            if (!qTable.contains(currentState))
            {
                // 如果当前状态不在Q表中，则在Q表中新建该状态的记录
                qTable[currentState] = QHash<int, double>();
            }

            if (!qTable[nextState].contains((int)SnakeAction::ActionUp))
            {
                // 如果下一个状态不在Q表中，则在Q表中新建该状态的记录
                qTable[nextState] = QHash<int, double>();
            }

            double maxNextQValue = qTable[nextState].isEmpty() ? 0 : qTable[nextState].values().at(0); // 下一个状态的最大Q值
            double currentQValue = qTable[currentState][action]; // 当前状态执行当前动作的Q值
            double updatedQValue = currentQValue + learningRate * (reward + discountFactor * maxNextQValue - currentQValue); // 更新Q值
            qTable[currentState][action] = updatedQValue; // 更新Q表中的Q值
        }

        QString autoSaveFilename = "./qTable/SnakeGameQTable_E" + QString::number(episode) + ".dat";
        qDebug() << "epoch " + QString::number(episode);
        if((episode%500) == 0)saveQTableToFile(autoSaveFilename); // 每500个epoch自动保存Q表到文件
    }
}

// 从文件中加载Q表
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

// 将Q表保存到文件
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

// 根据Q表找到当前状态下的最优动作
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

    return SnakeDirection::Up;
}
