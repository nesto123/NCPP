#include "mainwindow.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    /////////////////
    a.setWindowIcon(QIcon(":/window_logo.png"));
    ///////////////////
    return a.exec();
}
