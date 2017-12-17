#include "mainwindow.h"
#include <QApplication>
#include <QString>
#include <QCoreApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qDebug() << QCoreApplication::applicationDirPath();
    MainWindow w;
    w.show();

    return a.exec();
}
