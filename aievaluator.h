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
    explicit AIEvaluator(QObject *parent,QString qtableFilename);

signals:
    void evaluationFinished(int score);

protected:
    void run() override;


private:
    GameMode currentGameMode;
    int excuateTimes;
    QString qTableFilename;

public:
    QVector<SnakeGame *>snakeGames;
};

#endif // AIEVALUATOR_H
