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
