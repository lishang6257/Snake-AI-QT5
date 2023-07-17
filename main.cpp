#include "snakegamewindows.h"
#include "aievaluationwindows.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SnakeGameWindows w;
    w.show();


//    AIEvaluationWindows AIw;
//    while(true){
//        connect(&AIw,AIEvaluationWindows::evaluateFinished(),this,Q);
//    }
//    AIw.show();

    return a.exec();
}
