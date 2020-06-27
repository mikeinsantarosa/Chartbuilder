// ++++++++++++++++++++++++++++++++++++++++++++ //
// File: main.cpp
// Description: Launches the program
//
//
// Original release date
// Date: 2019-03-07
//
//
// TODO:
// 1) Finish CI charts
// 2) Add Y Axis rescaler for
//    values < e-12
// ++++++++++++++++++++++++++++++++++++++++++++
#include "htcdataselector.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HTCDataSelector w;
   // w.move(20,20);
    w.show();

    // Setup application properties
    a.setApplicationName("ChartBuilder");



    return a.exec();
}
