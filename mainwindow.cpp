#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cars.h"
#include <QDialog>
#include <QLabel>
#include <QScrollArea>
#include "parkinglot.h"
#include "ParkingLotWidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    resize(1800, 800);
    // ui->setupUi(this);
    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    scrollArea->setWidgetResizable(true);
    // scrollArea->resize(1800, 800);
    // monitor = new Monitor(this);
    // Cars *car = new Cars(this);
    ParkingLotWidget *pk = new ParkingLotWidget(this, "D:\\wming\\Documents\\DataStructureProject\\parkinglot_f1.xml");
	// pk->show();
    // pk->creatMap();
    /*QLabel *label = new QLabel(this);
    label->resize(1000, 1000);
    label->setPixmap(pk->creatMap2());*/
	scrollArea->resize(pk->width(), this->height());
    scrollArea->setWidget(pk);
}

MainWindow::~MainWindow()
{
    delete ui;
    // delete monitor;
}

void MainWindow::open(void) {
    QDialog dialog;
    dialog.setWindowTitle(tr("Hello, dialog!"));
    dialog.exec();
}
