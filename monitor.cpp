#include "monitor.h"
#include "ui_monitor.h"
#include <QObject>
#include <QDebug>
#include <mainwindow.h>
#include "parkingLotManager.h"
#include "carlist.h"
#include "logwindow.h"
#include "utils.h"

Monitor::Monitor(MainWindow *parent) :
    QWidget(parent),
    ui(new Ui::Monitor)
{

    l->append(FakeCar());

    adapter = new Adapter(this, l);

    ui->setupUi(this);
    QGraphicsScene *scene = new QGraphicsScene(this);  // 创建场景
    ParkingLotManager *pk = new ParkingLotManager(this, scene);
    QObject::connect(pk, &ParkingLotManager::enableUpButton, ui->upStairButton, &QPushButton::setEnabled);
    QObject::connect(pk, &ParkingLotManager::enableDownButton, ui->downStairButton, &QPushButton::setEnabled);
    QObject::connect(pk, &ParkingLotManager::setCapacity, ui->capacity, &QLabel::setText);
    QObject::connect(pk, &ParkingLotManager::setLoad, ui->load, &QLabel::setText);
    QObject::connect(pk, &ParkingLotManager::setLayerName, ui->layerName, &QLabel::setText);
    QObject::connect(ui->downStairButton, &QPushButton::clicked, pk, &ParkingLotManager::showDownStairFloor);
    QObject::connect(ui->upStairButton, &QPushButton::clicked, pk, &ParkingLotManager::showUpStairFloor);
    QObject::connect(ui->showMargin, &QCheckBox::toggled, pk, &ParkingLotManager::showMargin);
    QObject::connect(ui->showGraph, &QCheckBox::toggled, pk, &ParkingLotManager::showGraph);
    QObject::connect(ui->showLog, &QCheckBox::toggled, parent, &MainWindow::showLogWindow);
    QObject::connect(ui->showList, &QCheckBox::toggled, parent, &MainWindow::showListWindow);
    QObject::connect(ui->drawPath, &QPushButton::clicked, [=](){
        pk->drawPath(ui->placeNum1->text().toInt(), ui->placeNum2->text().toInt());
    });

    QObject::connect(ui->addCar, &QPushButton::clicked, [this]() {
        Log::i(checkPlate(this->ui->plateNumBox->text())==true?"true":"false");
    });
    QObject::connect(ui->random, &QPushButton::clicked, this, &Monitor::addCar);

    pk->showParkingLot();
    ui->view->setScene(scene);
    CarList::getInstance()->setAdapter(adapter);
}

void Monitor::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    if (!initialized) {
        QTimer *timer = new QTimer(this);
        timer->setSingleShot(true);
        connect(timer, &QTimer::timeout, [this]() {
            emit this->ui->showList->toggle();
            emit this->ui->showLog->toggle();
        });
        timer->start(100);  // 延时100ms，等待窗口创建完毕后执行，哈哈哈哈哈
        initialized = true;
    }
}

void Monitor::closeLogWindow()
{
    ui->showLog->setChecked(false);
}

void Monitor::closeListWindow()
{
    ui->showList->setChecked(false);
}

Monitor::~Monitor()
{
    delete ui;
}

void Monitor::addCar()
{
//    QModelIndex topLeft = adapter->index(0, 0);
//    QModelIndex bottomRight = adapter->index(adapter->rowCount(), 6);
//    emit adapter->dataChanged(topLeft, bottomRight);
    l->append(FakeCar());
}
