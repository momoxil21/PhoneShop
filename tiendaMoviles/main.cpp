#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    // center windows on screen
    QSize availableSize = qApp->desktop()->availableGeometry().size();
    int width = availableSize.width() / 2 - w.width()/2;
    int height = availableSize.height() / 2 - w.height() / 2;
    w.move(width,height);


    w.show();

    return a.exec();
}
