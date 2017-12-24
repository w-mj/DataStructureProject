#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include "monitor.h"
#include "logwindow.h"
#include "carlist.h"

class Monitor;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void open(void);
    void showLogWindow(bool enable);
    void showListWindow(bool enable);
    bool eventFilter(QObject *obj, QEvent *event);
    ~MainWindow();

private:
    Ui::MainWindow *ui = nullptr;
    LogWindow* logWindow;
    CarList *list;
    QAction *action;
    Monitor *monitor;
};

#endif // MAINWINDOW_H
