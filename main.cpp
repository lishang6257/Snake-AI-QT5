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
}
