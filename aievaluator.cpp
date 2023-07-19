// -*- coding: utf-8 -*-

#include "aievaluator.h"
#include "utils.h"

AIEvaluator::AIEvaluator(QObject *parent)
    : QThread(parent),
    currentGameMode(GameMode::Mode2),
    excuateTimes(1)
{
    // 创建一个 SnakeGame 实例，并使用指定的游戏模式开始游戏
    for (int i = 0; i < excuateTimes; i++) {
        snakeGames.append(new SnakeGame());
        snakeGames[i]->startMode(currentGameMode);
    }
}

AIEvaluator::AIEvaluator(QObject *parent, GameMode gameMode)
    : QThread(parent),
    currentGameMode(gameMode),
    excuateTimes(1)
{
    // 创建一个 SnakeGame 实例，并使用指定的游戏模式开始游戏
    for (int i = 0; i < excuateTimes; i++) {
        snakeGames.append(new SnakeGame());
        snakeGames[i]->startMode(currentGameMode);
    }
}

AIEvaluator::AIEvaluator(QObject *parent, QString qtableFilename)
    : QThread(parent),
    currentGameMode(GameMode::Mode4),
    excuateTimes(1),
    qTableFilename(qtableFilename)
{
    // 创建一个 SnakeGame 实例，并使用指定的 Q 表文件名开始游戏
    for (int i = 0; i < excuateTimes; i++) {
        snakeGames.append(new SnakeGame(qTableFilename = qtableFilename));
        snakeGames[i]->startMode(currentGameMode);
    }
}

// 评估线程执行的任务
void AIEvaluator::run()
{
    int score = 0;
    // 对每个 SnakeGame 实例进行游戏，并统计总得分
    for (int i = 0; i < excuateTimes; i++) {
        while (snakeGames[i]->isGameStarted) {
            snakeGames[i]->updateGame();
        }
        score += snakeGames[i]->score;
    }
    // 发送评估完成的信号，传递平均得分
    emit evaluationFinished(score / excuateTimes);
}
