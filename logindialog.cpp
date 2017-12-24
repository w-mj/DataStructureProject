#include "logindialog.h"
#include "ui_logindialog.h"
#include "logwindow.h"
#include <QTextStream>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    QFile f("QCore.dll");
    if (f.open(QFile::ReadOnly| QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&f);
        in >> id;
        in >> psd;
        if (id.size() != 0 && psd.size() != 0)
            first = false;
    }
    hide();
    if (first) {
        setWindowTitle("第一次使用，请注册");
        ui->repeat->show();
        ui->repeat_label->show();
    }
    else {
        setWindowTitle("登陆");
        ui->repeat->hide();
        ui->repeat_label->hide();
    }
    QPalette p(QPalette::WindowText,Qt::red);
    ui->hint->setPalette(p);
    setModal(true);
    connect(ui->ok, &QPushButton::clicked, this, &LoginDialog::ok);
    connect(ui->cancel, &QPushButton::clicked, this, &LoginDialog::cancel);
}


bool LoginDialog::getPermission()
{
    if (login)
        return true;
    return exec();
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::ok()
{
    if (first) {
        if (!(ui->id->text().size() && ui->psd->text().size() && ui->repeat->text().size()))
            ui->hint->setText("所有选项均必填");
        else if (ui->psd->text() == ui->repeat->text()) {
            Log::i("注册登陆成功");
            id = ui->id->text();
            psd = ui->psd->text();
            login = true;
            accept();
            QFile f("QCore.dll");
            if (f.open(QFile::WriteOnly)) {
                f.write(id.toStdString().c_str());
                f.write("\n");
                f.write(psd.toStdString().c_str());
                first = false;
            }
        } else {
            ui->hint->setText("两次密码不匹配");
        }
    } else {
        if (!(ui->id->text().size() && ui->psd->text().size()))
            ui->hint->setText("所有选项均必填");
        else if (ui->id->text()!=id || ui->psd->text() != psd)
            ui->hint->setText("用户名或密码不匹配");
        else {
            Log::i("登陆成功");
            login = true;
            accept();
        }
    }
}

void LoginDialog::cancel()
{
    reject();
}
