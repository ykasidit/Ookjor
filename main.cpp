#include <QtGui/QApplication>
#include "ookjorwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    OokjorWindow w;
    w.show();
    return a.exec();
}
