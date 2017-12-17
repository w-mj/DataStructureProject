#include "monitor.h"
#include "ui_monitor.h"
#include <QObject>
#include <QDebug>

Monitor::Monitor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Monitor)
{
    ui->setupUi(this);
    QObject::connect(ui->downStairButton, QPushButton::clicked, [&]() {
        qDebug() << "下一层按钮";
        if (m_currentFloor > 0) {
            m_currentFloor -= 1;
            showParkinglot();
        }
        if (m_currentFloor == 0)
            ui->downStairButton->setEnabled(false);
        if (m_currentFloor == m_parkinglots.size() - 2)
            ui->upStairButton->setEnabled(true);
    });
    QObject::connect(ui->upStairButton, QPushButton::clicked, [&]() {
        qDebug() << "上一层按钮";
        if (m_currentFloor < m_parkinglots.size() - 1) {
            m_currentFloor += 1;
            showParkinglot();
        }
        if (m_currentFloor == m_parkinglots.size() - 1)
            ui->upStairButton->setEnabled(false);
        if (m_currentFloor == 1)
            ui->downStairButton->setEnabled(true);
    });
    if (m_currentFloor == 0)  // 如果默认楼层就是顶层或者最下一层，把相应的按钮disable
        ui->downStairButton->setEnabled(false);
    else if (m_currentFloor == m_parkinglots.size() - 1)
        ui->upStairButton->setEnabled(false);
    showParkinglot();
}

Monitor::~Monitor()
{
    delete ui;
}

void Monitor::showParkinglot()
{
    if (m_parkinglots.at(m_currentFloor) == nullptr)
        m_parkinglots[m_currentFloor] = new ParkingLotWidget(this, m_xmlPath.at(m_currentFloor));
    ui->currentFloor->setText("<center>" + m_parkinglots.at(m_currentFloor)->getName() + "</center>");
    for (auto p = m_parkinglots.begin(); p != m_parkinglots.end(); p++)
        if (*p != nullptr)
            (*p)->hide();
    m_parkinglots.at(m_currentFloor)->show();
    //ui->rootLayout->replaceWidget(ui->parkingLotView, m_parkinglots.at(m_currentFloor));
    //ui->parkingLotView = m_parkinglots.at(m_currentFloor);
}
