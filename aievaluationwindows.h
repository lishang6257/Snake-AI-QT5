#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include "aievaluator.h"



class AIEvaluationWindows : public QWidget
{
    Q_OBJECT

public:
    explicit AIEvaluationWindows(QWidget *parent = nullptr);
    int AvgScore;

private slots:
    void updateEvaluationStatus(int score);

signals:
    void evaluateFinished();


private:
    void onSubAISnakeGameDrawingFinished(QWidget* subQwidget, int id);
    QVector<AIEvaluator*> aiEvaluators;
    QGridLayout *gridLayout;
    int currentTotalScore;
    int currentCalTime;

};

#endif // MAINWINDOW_H
