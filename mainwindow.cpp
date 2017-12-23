#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "car.h"
#include <QDialog>
#include <QLabel>
#include <QScrollArea>
#include <logwindow.h>
#include <QDebug>
#include "ParkingLotWidget.h"
#include "monitor.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    logWindow = LogWindow::getInstance(this);
    Monitor *monitor = new Monitor(this);
    resize(monitor->size());
}

MainWindow::~MainWindow()
{
    delete ui;
    // delete monitor;
    // delete logWindow;
}

void MainWindow::open(void) {
    QDialog dialog;
    dialog.setWindowTitle(tr("Hello, dialog!"));
    dialog.exec();
}

void MainWindow::showLogWindow(bool enable)
{
    if (enable) {
        qDebug() << x() << "  " << y() << "  " << width();
        logWindow->setGeometry(x()+width() + 15, y() + 35, 400, 450);
        logWindow->show();
        setFocus();
    } else
        logWindow->hide();
}
