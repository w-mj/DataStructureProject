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
    void closeEvent(QCloseEvent *e);
signals:
    void onCloseButtonClicked(void);

private:
    static LogWindow* m_instance;
    explicit LogWindow(QWidget *parent = 0);
    void clear(void);
    void save(void);

    Ui::LogWindow *ui;
    QTextEdit * text;
    QString buffer;
};

#endif // LOGWINDOW_H
