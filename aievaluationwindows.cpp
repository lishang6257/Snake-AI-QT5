// -*- coding: utf-8 -*-

#include "aievaluationwindows.h"
#include "aievaluator.h"
#include <QPushButton>

const int numRows = 4;
const int numCols = 5;

AIEvaluationWindows::AIEvaluationWindows(QWidget *parent)
    : QWidget(parent),
    AvgScore(0),
    currentTotalScore(0),
    currentCalTime(0)
{
    // 创建 numCols*numRows 个 AIEvaluator 实例，并进行评估
    for (int i = 0; i < numCols * numRows; i++) {
        AIEvaluator *aiEvaluator = new AIEvaluator();
        aiEvaluators.append(aiEvaluator);
        connect(aiEvaluator, &AIEvaluator::evaluationFinished, this, &AIEvaluationWindows::updateEvaluationStatus);
        aiEvaluator->start();
    }
}

AIEvaluationWindows::AIEvaluationWindows(QWidget *parent, GameMode gameMode)
    : QWidget(parent),
    AvgScore(0),
    currentTotalScore(0),
    currentCalTime(0)
{
    // 创建 numCols*numRows 个带有指定 GameMode 的 AIEvaluator 实例，并进行评估
    for (int i = 0; i < numCols * numRows; i++) {
        AIEvaluator *aiEvaluator = new AIEvaluator(this->thread(), gameMode);
        aiEvaluators.append(aiEvaluator);
        connect(aiEvaluator, &AIEvaluator::evaluationFinished, this, &AIEvaluationWindows::updateEvaluationStatus);
        aiEvaluator->start();
    }
}

AIEvaluationWindows::AIEvaluationWindows(QWidget *parent, QString qtableFilename)
    : QWidget(parent),
    AvgScore(0),
    currentTotalScore(0),
    currentCalTime(0),
    qTableFilename(qtableFilename)
{
    // 创建 numCols*numRows 个带有指定 qtableFilename 的 AIEvaluator 实例，并进行评估
    for (int i = 0; i < numCols * numRows; i++) {
        AIEvaluator *aiEvaluator = new AIEvaluator(this->thread(), qTableFilename);
        aiEvaluators.append(aiEvaluator);
        connect(aiEvaluator, &AIEvaluator::evaluationFinished, this, &AIEvaluationWindows::updateEvaluationStatus);
        aiEvaluator->start();
    }
}

// 设置评估窗口的网格布局
void AIEvaluationWindows::setGridUI()
{
    gridLayout = new QGridLayout(this);
    gridLayout->setSpacing(10);

    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < numCols; ++col) {
            // 这里可能有部分代码未完成，需要根据具体情况添加对应的逻辑
            // gridLayout->addWidget(aiEvaluator->snakeGames[], row, col);
            // aiEvaluator->snakeGames->setVisible(false);
        }
    }

    // 这里可能有部分代码未完成，需要根据具体情况添加对应的逻辑
    // resize(aiEvaluators[0]->snakeGames[0]->width()*numCols+15*numCols, aiEvaluators[0]->snakeGames->height()*numRows+15*numRows);

    setLayout(gridLayout);
}

// 更新评估状态，计算平均得分并发出评估完成的信号
void AIEvaluationWindows::updateEvaluationStatus(int score)
{
    qDebug() << QString("Score:") + QString::number(score);
    currentTotalScore += score;
    currentCalTime++;
    if (currentCalTime == numCols * numRows) {
        qDebug() << QString("Average Score:") + QString::number(currentTotalScore / currentCalTime);
        AvgScore = currentTotalScore / currentCalTime;
        emit evaluateFinished();
    }
}
