#ifndef AIEVALUATOR_H
#define AIEVALUATOR_H

#include <QThread>
#include "snakegame.h"

class AIEvaluator : public QThread
{
    Q_OBJECT

public:
    explicit AIEvaluator(QObject *parent = nullptr);

signals:
    void evaluationFinished(int score);

protected:
    void run() override;

private:
    SnakeGame::GameMode currentGameMode;
    int excuateTimes;
public:
    SnakeGame *snakeGames;

};

#endif // AIEVALUATOR_H
