#include "logwindow.h"
#include "ui_logwindow.h"

#include <QTime>


LogWindow* LogWindow::m_instance = nullptr;

LogWindow::LogWindow(QWidget *parent) :
    QWidget(parent, Qt::Dialog),
    ui(new Ui::LogWindow)
{
    ui->setupUi(this);
    text = ui->text;
    setWindowTitle("日志窗口");
    setGeometry(parent->x() + parent->width(), y(), 400, 450);
    this->hide();
    setFocusPolicy(Qt::NoFocus);
}

LogWindow::~LogWindow()
{
    delete ui;
}

LogWindow *LogWindow::newInstance(QWidget *parent)
{
    if (m_instance == nullptr)
        m_instance = new LogWindow(parent);
    return m_instance;
}

void LogWindow::i(QString text)
{
    getInstance()->text->append(QTime::currentTime().toString("hh:mm:ss")+":"+text);
}

LogWindow *LogWindow::getInstance()
{
    return m_instance;
}
