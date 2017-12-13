#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include "monitor.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void open(void);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QAction *action;
    Monitor *monitor;
};

#endif // MAINWINDOW_H
