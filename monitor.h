#ifndef MONITOR_H
#define MONITOR_H

#include <QWidget>
#include <vector>
#include <QString>
#include "ParkingLotWidget.h"

namespace Ui {
    class Monitor;
}

class Monitor : public QWidget
{
    Q_OBJECT

public:
    explicit Monitor(QWidget *parent = 0);
    ~Monitor();

private:
    Ui::Monitor *ui;
};

#endif // MONITOR_H
