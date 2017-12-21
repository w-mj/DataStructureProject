#include "monitor.h"
#include "ui_monitor.h"
#include <QObject>
#include <QDebug>
#include "parkingLotManager.h"

Monitor::Monitor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Monitor)
{
    ui->setupUi(this);
    QGraphicsScene *scene = new QGraphicsScene(this);  // 创建场景
    ParkingLotManager *pk = new ParkingLotManager(this, scene);
    QObject::connect(ui->downStairButton, &QPushButton::clicked, pk, &ParkingLotManager::showDownStairFloor);
    QObject::connect(ui->upStairButton, &QPushButton::clicked, pk, &ParkingLotManager::showUpStairFloor);
    QObject::connect(pk, &ParkingLotManager::enableUpButton, ui->upStairButton, &QPushButton::setEnabled);
    QObject::connect(pk, &ParkingLotManager::enableDownButton, ui->downStairButton, &QPushButton::setEnabled);
    QObject::connect(pk, &ParkingLotManager::setCapacity, ui->capacity, &QLabel::setText);
    QObject::connect(pk, &ParkingLotManager::setLoad, ui->load, &QLabel::setText);
    QObject::connect(pk, &ParkingLotManager::setLayerName, ui->layerName, &QLabel::setText);

    QObject::connect(ui->drawPath, &QPushButton::clicked, [=](){
        pk->drawPath(ui->placeNum1->text().toInt(), ui->placeNum2->text().toInt());
    });
    pk->showParkingLot();
    ui->view->setScene(scene);
    // scene->addItem(pk);
}

Monitor::~Monitor()
{
    delete ui;
}
