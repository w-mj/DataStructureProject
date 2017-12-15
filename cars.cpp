#include "cars.h"
#include <QtGlobal>
#include <QPainter>

Cars::Cars(QWidget *parent) : QWidget(parent)
{
    Cars(Color(rand() % 3), parent);
}

Cars::Cars(Cars::Color color, QWidget *parent) : QWidget(parent)
{
    m_color = color;
    resize(200, 130);
    switch (m_color) {
    case Pink:
        m_pixmap = QPixmap(":/cars/pink");
        break;
    case Red:
        m_pixmap = QPixmap(":/cars/red");
        break;
    case Yellow:
        m_pixmap = QPixmap(":/cars/yellow");
        break;
    default:
        m_pixmap = QPixmap(":/cars/red");
        break;
    }
}

void Cars::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.drawPixmap(0, 0, 200, 130, m_pixmap);
    painter.setPen(QPen(Qt::red, 5));
    painter.drawRect(0, 0, 200, 130);
    event->ignore();
}
