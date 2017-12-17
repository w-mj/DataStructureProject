#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cars.h"
#include <QDialog>
#include <QLabel>
#include <QScrollArea>
#include "ParkingLotWidget.h"
#include "monitor.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    Monitor *monitor = new Monitor(this);
    resize(monitor->size());
}

MainWindow::~MainWindow()
{
    delete ui;
    // delete monitor;
}

void MainWindow::open(void) {
    QDialog dialog;
    dialog.setWindowTitle(tr("Hello, dialog!"));
    dialog.exec();
}
