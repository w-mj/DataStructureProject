#include "cars.h"
#include <QPainter>

Cars::Cars(QWidget *parent, int dir, Cars::Color color) : QWidget(parent)
{
    if(color==RANDOM)
        color = Cars::Color(rand()%3);  //车身颜色一定是确定的
    m_color = color;
    m_dir = dir;
    m_pos = this->pos();
    resize(M_WID, M_LEN);
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
    m_pos = this->pos();
    painter.translate(M_WID/2, M_LEN/2);    //转移远点，设置旋转中心
    painter.rotate(m_dir);                  //旋转
    painter.translate(-M_WID/2, -M_LEN/2);  //移回远点
    painter.drawPixmap(0, 0, M_WID, M_LEN, m_pixmap);   //绘制小车
    painter.setPen(QPen(Qt::red, 5));
    painter.drawRect(0, 0, M_WID, M_LEN);
    event->ignore();
}

void Cars::Forward(int vel)
{
    float ang = m_dir*PI/180;
    this->move(m_pos.x()+vel*sin(ang), m_pos.y()-vel*cos(ang));
}

void Cars::Backward(int vel)
{
    float ang = m_dir*PI/180;
    this->move(m_pos.x()-vel*sin(ang), m_pos.y()+vel*cos(ang));
}
