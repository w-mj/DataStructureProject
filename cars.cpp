#include "cars.h"
#include <QtGlobal>
#include <QPainter>

Cars::Cars(QWidget *parent) : QWidget(parent)
{
    m_color = Color(rand() % 3);
    resize(200, 130);
}

void Cars::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QPixmap pix(":/cars/yellow");
    painter.drawPixmap(0, 0, 200, 130, pix);
    painter.setPen(QPen(Qt::red, 5));
    painter.drawRect(0, 0, 200, 130);
}
