#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "monitor.h"
#include <QDialog>
#include <QLabel>
#include <QImage>
#include <QPainter>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    resize(1800, 800);
    //ui->setupUi(this);

    montior = new Monitor(this);
    QPixmap pix(":/cars/yellow");
    QPainter painter(this);
    painter.drawPixmap(0,0,pix);
    QPixmap pix2=pix.scaled(QSize(50,50));
    painter.drawPixmap(0,0,pix2);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete montior;
}

void MainWindow::open(void) {
    QDialog dialog;
    dialog.setWindowTitle(tr("Hello, dialog!"));
    dialog.exec();
}
