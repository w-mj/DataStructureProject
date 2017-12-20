#include "monitor.h"
#include "ui_monitor.h"
#include <QObject>
#include <QDebug>
#include "parkingLotItem.h"

Monitor::Monitor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Monitor)
{
    ui->setupUi(this);
    ParkingLotItem *pk = new ParkingLotItem(Q_NULLPTR, this);
    QObject::connect(ui->downStairButton, &QPushButton::clicked, pk, &ParkingLotItem::showDownStairFloor);
    QObject::connect(ui->upStairButton, &QPushButton::clicked, pk, &ParkingLotItem::showUpStairFloor);
    QObject::connect(pk, &ParkingLotItem::enableUpButton, ui->upStairButton, &QPushButton::setEnabled);
    QObject::connect(pk, &ParkingLotItem::enableDownButton, ui->downStairButton, &QPushButton::setEnabled);
    QObject::connect(pk, &ParkingLotItem::setCapacity, ui->capacity, &QLabel::setText);
    QObject::connect(pk, &ParkingLotItem::setLoad, ui->load, &QLabel::setText);

    pk->showParkingLot();
    QGraphicsScene *scene = new QGraphicsScene(this);  // 创建场景
    ui->view->setScene(scene);
    scene->addItem(pk);
}

Monitor::~Monitor()
{
    delete ui;
}
