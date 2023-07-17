// -*- coding: utf-8 -*-

#include "aievaluationwindows.h"
#include "aievaluator.h"
#include <QPushButton>

const int numRows = 4;
const int numCols =5;

AIEvaluationWindows::AIEvaluationWindows(QWidget *parent, QString qtableFilename)
    : QWidget(parent),
    AvgScore(0),
    currentTotalScore(0),
    currentCalTime(0),
    qTableFilename(qtableFilename)
{
//    gridLayout = new QGridLayout(this);
//    gridLayout->setSpacing(10); // 设置布局间距

    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < numCols; ++col) {
            AIEvaluator *aiEvaluator = new AIEvaluator(parent = this,qtableFilename = qtableFilename);
            aiEvaluators.append(aiEvaluator);
            connect(aiEvaluator, &AIEvaluator::evaluationFinished, this, &AIEvaluationWindows::updateEvaluationStatus);

//            gridLayout->addWidget(aiEvaluator->snakeGames[], row, col);
//            aiEvaluator->snakeGames->setVisible(false);
            aiEvaluator->start();
        }
    }

//    resize(aiEvaluators[0]->snakeGames[0]->width()*numCols+15*numCols, aiEvaluators[0]->snakeGames->height()*numRows+15*numRows);

//    setLayout(gridLayout);

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
