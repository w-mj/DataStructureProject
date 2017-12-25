#ifndef MONITOR_H
#define MONITOR_H

#include <QWidget>
#include <vector>
#include <QString>
#include <mainwindow.h>
#include "ParkingLotWidget.h"
#include "carlist.h"

class MainWindow;

namespace Ui {
    class Monitor;
}

class Monitor : public QWidget
{
    Q_OBJECT

public:
    explicit Monitor(MainWindow *parent = 0);
    void showEvent(QShowEvent *event);
    void closeLogWindow();
    void closeListWindow();
    ~Monitor();

private:
    Ui::Monitor *ui;
    void addCar();
    Adapter *adapter;
    bool initialized = false;
};

#endif // MONITOR_H
