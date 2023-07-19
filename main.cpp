// -*- coding: utf-8 -*-

#include "snakegamewindows.h"
#include "aievaluationwindows.h"
#include "qlearning.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SnakeGameWindows w(nullptr, GameMode::Mode2);
    w.show();

//    //    AIEvaluationWindows AIw(nullptr, GameMode::Mode1);

//    //    AIEvaluationWindows AIw(nullptr, "SnakeGameQTable_E5000.dat");
//    //    AIw.show();

    return a.exec();

//    // 创建一个 SnakeGame 对象
//    SnakeGame game;
//    // 定义 Q 学习的参数
//    int numEpisodes = 10001;        // 训练的周期数
//    double learningRate = 0.1;    // 学习率
//    double discountFactor = 0;  // 折扣因子
//    double explorationProb = 0.2; // 探索概率
//    // 创建 QLearning 对象并开始训练
//    QLearning qLearning;
//    qLearning.train(game, numEpisodes, learningRate, discountFactor, explorationProb);



}
