#include "ParkingSpaceWidget.h"
#include <QPainter>
#include <QDebug>
#include <QEvent>
#include <QLabel>
#include <QEvent>
#include <QHoverEvent>
#include <QObject>
#include <QMenu>
#include <QCursor>
#include "ParkingLotWidget.h"
#include "logindialog.h"
#include "logwindow.h"

ParkingSpaceWidget::ParkingSpaceWidget(ParkingLotWidget* parent, const QString &dir): QWidget(parent)
{
    direction d = {};
    const char c = dir.at(0).unicode();
    switch (c)
    {
    case 'N':d = N; break;
    case 'E':d = E; break;
    case 'W':d = W; break;
    case 'S':d = S; break;
    default: qDebug() << this->objectName() << "错误的方向";
    }
    this->dir = d;
    switch (this->dir)
    {
    case N:
    case S:
        resize(40, 60);
        setMinimumSize(40, 60);
        setMaximumSize(40, 60);
        break;
    case E:
    case W:
        resize(60, 40);
        setMinimumSize(60, 40);
        setMaximumSize(60, 40);
        break;
    }
    number = parent->addSpace(this);
    setToolTip(QString::number(number));
    QObject::connect(parent, &ParkingLotWidget::showMargain, [this](bool b){
        this->m_showMargin = b;
        update();
    });
    QObject::connect(this, &ParkingSpaceWidget::ban, parent, &ParkingLotWidget::banParkingSpace);
}

QBoxLayout *ParkingSpaceWidget::makeParkingSapceGroup(ParkingLotWidget* parent, const QString& dir, int n, const QString & expend)
{
    QBoxLayout *layout;
    if (expend == "down")
        layout = new QVBoxLayout();
    else
        layout = new QHBoxLayout();
    for (int i = 0; i < n; i++)
    {
        ParkingSpaceWidget *pk = new ParkingSpaceWidget(parent, dir);
        layout->addWidget(pk);
    }
    layout->addStretch();  // 居上或居左对齐
    // layout->setMargin(0);
    layout->setSpacing(0);
    return layout;
}

void ParkingSpaceWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(Qt::black);
    painter.drawText(20, 25, QString::number(number));
    QString p;
    p.append(QString::number(pos().x()));
    p.append(",");
    p.append(QString::number(pos().y()));

    painter.setPen(Qt::white);
    painter.setBrush(Qt::white);
    //painter.save();
    painter.translate(30, 20);
    switch (dir) {
    case E:
        break;
    case S:
        painter.rotate(270);
        break;
    case W:
        painter.rotate(180);
        break;
    case N:
        painter.rotate(90);
        break;
    }
    painter.translate(-30, -20);
    painter.drawRect(0, 0, 60, 5);
    painter.drawRect(0, 5, 10, 35);
    painter.drawRect(10, 35, 50, 5);
    if (m_showMargin) {
        painter.setPen(Qt::blue);
        painter.setBrush(Qt::NoBrush);
        painter.drawRect(0, 0, width() - 1, height() - 1);
    }
    if (m_situation == banned) {
        QPen p;
        p.setBrush(Qt::black);
        p.setWidth(4);
        p.setCapStyle(Qt::RoundCap);
        painter.setPen(p);
        painter.setBrush(QColor(240, 172, 47));
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.drawEllipse(15, 5, 26, 26);
        painter.drawRect(17, 17, 24, 2);
    }

    //painter.restore();
    //painter.setPen(Qt::black);
    //painter.drawText(0, 25, p);
    painter.end();
}

ParkingSpaceWidget::Situation ParkingSpaceWidget::getSituation() const
{
    return m_situation;
}

void ParkingSpaceWidget::setSituation(const Situation &value)
{
    m_situation = value;
}

int ParkingSpaceWidget::getNumber() const
{
    return number;
}

ParkingSpaceWidget::direction ParkingSpaceWidget::getDir() const
{
    return dir;
}

void ParkingSpaceWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        if (REQUIRE_PREMISSION) {
            Log::i("请求权限成功");
            if (m_situation == banned) {
                qDebug() << "禁用解除" << number;
                emit ban(false, number);
                m_situation = free;
            } else if (m_situation == free) {
                qDebug() << "禁用车位" << number;
                emit ban(true, number);
                m_situation = banned;
            } else {
                qDebug() << "车位已被占用";
            }
            update();
        } else {
            Log::i("请求权限失败");
        }
    }
}

ParkingSpaceWidget::~ParkingSpaceWidget()
{
}
