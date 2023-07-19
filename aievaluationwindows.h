// -*- coding: utf-8 -*-

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QObject>
#include "aievaluator.h"

class AIEvaluationWindows : public QWidget
{
    Q_OBJECT

public:
    explicit AIEvaluationWindows(QWidget *parent = nullptr);
    explicit AIEvaluationWindows(QWidget *parent , GameMode gameMode);
    explicit AIEvaluationWindows(QWidget *parent , QString qtableFilename);
    int AvgScore; // 平均得分

private slots:
    void updateEvaluationStatus(int score); // 更新评估状态
    void setGridUI(); // 设置网格界面

signals:
    void evaluateFinished(); // 评估完成信号

private:
    void onSubAISnakeGameDrawingFinished(QWidget* subQwidget, int id); // 子AI贪吃蛇游戏绘制完成槽函数
    QVector<AIEvaluator*> aiEvaluators; // AI评估器数组
    QGridLayout *gridLayout; // 网格布局
    int currentTotalScore; // 当前总得分
    int currentCalTime; // 当前计算次数

    QString qTableFilename; // Q表文件名

};

#endif // MAINWINDOW_H
