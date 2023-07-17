#include "aievaluator.h"

AIEvaluator::AIEvaluator(QObject *parent)
    : QThread(parent),
    currentGameMode(SnakeGame::GameMode::Mode2),
    excuateTimes(1)
{
    snakeGames = new SnakeGame();
    snakeGames->startMode(SnakeGame::GameMode::Mode2, 10);

}

void AIEvaluator::run()
{

    while(true){
//        qDebug() << "running " << QThread::currentThreadId();
        // 发送评估结果信号
        if(snakeGames->isGameOver())
        {
            emit evaluationFinished(snakeGames->evaluateAutoAI());
            return;
        }
    }
}
