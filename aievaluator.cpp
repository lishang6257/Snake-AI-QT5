// -*- coding: utf-8 -*-

#include "aievaluator.h"
#include "utils.h"

AIEvaluator::AIEvaluator(QObject *parent)
    : QThread(parent),
    currentGameMode(GameMode::Mode2),
    excuateTimes(1)
{
    for(int i=0;i < excuateTimes;i ++){
        snakeGames.append(new SnakeGame());
        snakeGames[i]->startMode(currentGameMode);
    }
}

AIEvaluator::AIEvaluator(QObject *parent,GameMode gameMode)
    : QThread(parent),
    currentGameMode(gameMode),
    excuateTimes(1)
{
    for(int i=0;i < excuateTimes;i ++){
        snakeGames.append(new SnakeGame());
        snakeGames[i]->startMode(currentGameMode);
    }
}

AIEvaluator::AIEvaluator(QObject *parent,QString qtableFilename)
    : QThread(parent),
    currentGameMode(GameMode::Mode4),
    excuateTimes(1),
    qTableFilename(qtableFilename)
{
    for(int i=0;i < excuateTimes;i ++){
        snakeGames.append(new SnakeGame(qTableFilename = qtableFilename));
        snakeGames[i]->startMode(currentGameMode);
    }
}

void AIEvaluator::run()
{
    int score = 0;
    for(int i = 0;i < excuateTimes;i ++){
        while(snakeGames[i]->isGameStarted){
            snakeGames[i]->updateGame();
        }
        score += snakeGames[i]->score;
    }
    emit evaluationFinished(score/excuateTimes);
}
