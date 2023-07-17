// -*- coding: utf-8 -*-

#include "aievaluationwindows.h"
#include "aievaluator.h"
#include <QPushButton>

const int numRows = 4;
const int numCols =5;

AIEvaluationWindows::AIEvaluationWindows(QWidget *parent)
    : QWidget(parent),
    AvgScore(0),
    currentTotalScore(0),
    currentCalTime(0)
{
    for(int i=0;i <numCols*numRows;i ++){
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
    for(int i=0;i <numCols*numRows;i ++){
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
    for(int i=0;i <numCols*numRows;i ++){
        AIEvaluator *aiEvaluator = new AIEvaluator(this->thread(), qTableFilename);
        aiEvaluators.append(aiEvaluator);
        connect(aiEvaluator, &AIEvaluator::evaluationFinished, this, &AIEvaluationWindows::updateEvaluationStatus);
        aiEvaluator->start();
    }
}



void AIEvaluationWindows::setGridUI()
{
    gridLayout = new QGridLayout(this);
    gridLayout->setSpacing(10); // 设置布局间距

    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < numCols; ++col) {
            //            gridLayout->addWidget(aiEvaluator->snakeGames[], row, col);
            //            aiEvaluator->snakeGames->setVisible(false);
        }
    }

//    resize(aiEvaluators[0]->snakeGames[0]->width()*numCols+15*numCols, aiEvaluators[0]->snakeGames->height()*numRows+15*numRows);

    setLayout(gridLayout);
}

void AIEvaluationWindows::updateEvaluationStatus(int score)
{
    qDebug() << QString("scc:") + QString::number(score);
    currentTotalScore += score;
    currentCalTime ++;
    if(currentCalTime == numCols*numRows) {
        qDebug() << QString("avg:") + QString::number(currentTotalScore/currentCalTime);
        AvgScore = currentTotalScore/currentCalTime;
        emit evaluateFinished();
    }
}
