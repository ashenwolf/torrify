#include "torrifywindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TorrifyWindow w;
    w.show();
    
    return a.exec();
}
