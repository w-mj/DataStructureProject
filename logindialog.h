#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QString>
#include <QFile>

#define REQUIRE_PREMISSION (LoginDialog::getInstance().getPermission())

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    static LoginDialog& getInstance(QWidget* parent = 0) {  // 返回引用防止被delete
        Q_UNUSED(parent);
        static LoginDialog instance(0);  // 利用静态变量实现线程安全的单例模式
        return instance;
    }
    bool getPermission();
    ~LoginDialog();

private slots:
    void ok();
    void cancel();

private:
    explicit LoginDialog(QWidget *parent = 0);
    explicit LoginDialog(const LoginDialog& another) = delete;
    LoginDialog& operator=(const LoginDialog& another) = delete;
    Ui::LoginDialog *ui;
    QString id, psd;
    bool first = true;
    bool login = false;
};

#endif // LOGINDIALOG_H
