// -*- coding: utf-8 -*-

#ifndef AIEVALUATOR_H
#define AIEVALUATOR_H

#include <QThread>
#include "snakegame.h"

class AIEvaluator : public QThread
{
    Q_OBJECT

public:
    explicit AIEvaluator(QObject *parent = nullptr);
    explicit AIEvaluator(QObject *parent, GameMode gameMode);
    explicit AIEvaluator(QObject *parent, QString qtableFilename);

signals:
    void evaluationFinished(int score); // 评估完成信号，发送评估得分

protected:
    void run() override; // 线程运行函数，用于评估AI性能

private:
    GameMode currentGameMode; // 当前游戏模式
    int excuateTimes; // 执行次数
    QString qTableFilename; // Q表文件名

public:
    QVector<SnakeGame *>snakeGames; // 贪吃蛇游戏数组

};

#endif // AIEVALUATOR_H
