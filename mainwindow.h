#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include "monitor.h"
#include "logwindow.h"

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
    ~MainWindow();

private:
    Ui::MainWindow *ui = nullptr;
    LogWindow* logWindow;
    QAction *action;
    Monitor *monitor;
};

#endif // MAINWINDOW_H
