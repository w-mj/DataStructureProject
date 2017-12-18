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
    // TODO: 动态修改
    std::vector<ParkingLotWidget*> m_parkinglots = {nullptr, nullptr};  // lazy
    std::vector<QString> m_xmlPath;// = {"parkinglot_b1.xml", "parkinglot_f1.xml"};
    uint m_currentFloor = 1;  // 默认为地上一层

    void showParkinglot(void);
};

#endif // MONITOR_H
