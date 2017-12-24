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
#include "carlist.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    logWindow = LogWindow::newInstance(this);
    list = CarList::newInstance(this);

    Monitor *monitor = new Monitor(this);
    resize(monitor->size());
    installEventFilter(this);
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
        logWindow->setGeometry(x()+width() + 15, y() + 35, 400, 450);
        logWindow->show();
        activateWindow();
    } else
        logWindow->hide();
}

void MainWindow::showListWindow(bool enable)
{
    if (enable) {
        list->setGeometry(x()+width() + 15, y() + width() / 2, 400, 450);
        list->show();
        activateWindow();
    } else
        list->hide();
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
       if (event->type() == QEvent::Move) {
           logWindow->setGeometry(x()+width() + 15, y() + 35, 400, 450);
           list->setGeometry(x()+width() + 15, y() + width() / 2, 400, 450);
       }
       return QWidget::eventFilter(obj, event);
   }
