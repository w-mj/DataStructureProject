#include "monitor.h"
#include "ui_monitor.h"
#include <QObject>
#include <QDebug>
#include <mainwindow.h>
#include "parkingLotManager.h"
#include "carlist.h"
#include "logwindow.h"
#include "utils.h"
#include <QMessageBox>

Monitor::Monitor(MainWindow *parent) :
    QWidget(parent),
    ui(new Ui::Monitor)
{
    ui->setupUi(this);
    ui->selectColor->addItem("粉车");
    ui->selectColor->addItem("红车");
    ui->selectColor->addItem("黄车");
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
    QObject::connect(ui->showMode, &QCheckBox::toggled, pk, &ParkingLotManager::showMode);
    ui->sequence->hide();
    QObject::connect(ui->addCar, &QPushButton::clicked, [this, pk]() {
        QString p = this->ui->plateNumBox->text();
        if(checkPlate(p) == false) {
            QMessageBox::warning(this, "警告", "车牌不合规则");
        } else if (pk->checkSame(p)) {
            QMessageBox::warning(this, "警告", "车牌重复");
        } else {
            pk->addCar(p, ui->selectColor->currentIndex(), -1);
        }
    });
    QObject::connect(ui->random, &QPushButton::clicked, pk, &ParkingLotManager::addCarR);
    QObject::connect(ui->sequence, &QCheckBox::clicked, pk, &ParkingLotManager::setSequence);
    QObject::connect(ui->confirm_max, &QPushButton::clicked, [this, pk]() {
        pk->setMax(ui->max_number->value());
    });
    pk->showParkingLot();
    ui->view->setScene(scene);
    ui->max_number->setValue(208);

    QObject::connect(ui->searchButton, &QPushButton::clicked, [this, pk]() {
        QPair<QString, int> p = pk->search(this->ui->searchBox->text());
        if (p.first == "-1" && p.second == -1)
            QMessageBox::information(this, "搜索结果", QString("未找到%1")
                                     .arg(this->ui->searchBox->text()));
        else if (p.first == "0" && p.second == 0)
            QMessageBox::information(this, "搜索结果",
                                     QString("%1正在等候中").arg(this->ui->searchBox->text()));
        else
            QMessageBox::information(this, "搜索结果",
                                     QString("%1在%2%3号").arg(this->ui->searchBox->text())
                                     .arg(p.first).arg(p.second));
    });
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

}
