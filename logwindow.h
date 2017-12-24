#ifndef LOGWINDOW_H
#define LOGWINDOW_H

#include <QWidget>
#include <QTextEdit>

#define Log LogWindow

namespace Ui {
class LogWindow;
}

class LogWindow : public QWidget
{
    Q_OBJECT

public:
    ~LogWindow();
    static LogWindow* getInstance();
    static LogWindow* newInstance(QWidget* parent);
    static void i(QString text);
    // static void i(char* text);

private:
    static LogWindow* m_instance;
    explicit LogWindow(QWidget *parent = 0);

    Ui::LogWindow *ui;
    QTextEdit * text;
};

#endif // LOGWINDOW_H
