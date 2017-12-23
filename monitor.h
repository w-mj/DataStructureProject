#ifndef MONITOR_H
#define MONITOR_H

#include <QWidget>
#include <vector>
#include <QString>
#include <mainwindow.h>
#include "ParkingLotWidget.h"

class MainWindow;

namespace Ui {
    class Monitor;
}

class Monitor : public QWidget
{
    Q_OBJECT

public:
    explicit Monitor(MainWindow *parent = 0);
    ~Monitor();

private:
    Ui::Monitor *ui;
};

#endif // MONITOR_H
