#include "logwindow.h"
#include "ui_logwindow.h"

#include <QTime>
#include <QCloseEvent>
#include <QFile>


LogWindow* LogWindow::m_instance = nullptr;

LogWindow::LogWindow(QWidget *parent) :
    QWidget(parent, Qt::Dialog),
    ui(new Ui::LogWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    text = ui->text;
    setWindowTitle("日志窗口");
    setGeometry(parent->x() + parent->width(), y(), 400, 450);
    this->hide();
    setFocusPolicy(Qt::NoFocus);
    connect(ui->save, &QPushButton::clicked, this, &LogWindow::save);
    connect(ui->clear, &QPushButton::clicked, this, &LogWindow::clear);
}

void LogWindow::clear()
{
    ui->text->clear();
}

void LogWindow::save()
{
    QFile f(QString("Log_%1.log").arg(QTime::currentTime().toString("hh_mm_ss")));
    if (f.open(QFile::WriteOnly| QIODevice::Text)) {
        i("保存文件");
        f.write(buffer.toStdString().c_str());
        buffer.clear();
    }
    f.close();
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
    getInstance()->buffer.append(QTime::currentTime().toString("hh:mm:ss")+":"+text);
    getInstance()->buffer.append("\n");
}

void LogWindow::closeEvent(QCloseEvent *e)
{
    emit onCloseButtonClicked();
    e->accept();
}

//void LogWindow::i(char* text)
//{
//    getInstance()->text->append(QTime::currentTime().toString("hh:mm:ss")+":"+text);
//}

LogWindow *LogWindow::getInstance()
{
    return m_instance;
}
