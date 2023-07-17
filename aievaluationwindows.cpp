//#include "aievaluationwindows.h"
//#include "aievaluator.h"
//#include <QPushButton>

//const int numRows = 1;
//const int numCols =10;

//AIEvaluationWindows::AIEvaluationWindows(QWidget *parent)
//    : QWidget(parent),
//    currentTotalScore(0),
//    currentCalTime(0),
//    AvgScore(0)
//{
//    gridLayout = new QGridLayout(this);
//    gridLayout->setSpacing(10); // 设置布局间距

//    for (int row = 0; row < numRows; ++row) {
//        for (int col = 0; col < numCols; ++col) {
//            AIEvaluator *aiEvaluator = new AIEvaluator();
//            aiEvaluators.append(aiEvaluator);
//            gridLayout->addWidget(aiEvaluator->snakeGames, row, col);
//            connect(aiEvaluator, &AIEvaluator::evaluationFinished, this, &AIEvaluationWindows::updateEvaluationStatus);
//            aiEvaluator->snakeGames->setVisible(false);
//            aiEvaluator->start();
//        }
//    }

//    resize(aiEvaluators[0]->snakeGames->width()*numCols+15*numCols, aiEvaluators[0]->snakeGames->height()*numRows+15*numRows);

//    setLayout(gridLayout);

//}



//void AIEvaluationWindows::updateEvaluationStatus(int score)
//{
//    qDebug() << QString("scc:") + QString::number(score);
//    currentTotalScore += score;
//    currentCalTime ++;
//    if(currentCalTime == numCols*numRows) {
//        qDebug() << QString("avg:") + QString::number(currentTotalScore/currentCalTime);
//        AvgScore = currentTotalScore/currentCalTime;
//        emit evaluateFinished();
//    }
//}
