#include "cars.h"


Cars::Cars(QGraphicsItem *parent, int dir, Cars::Color color) : QGraphicsPixmapItem(parent)
{
    m_pos = this->pos();
    this->setRotation(dir);
    if(color==RANDOM)
        color = Cars::Color(rand()%3);  //车身颜色一定是确定的
    m_color = color;
    switch (m_color) {
    case Pink:
        m_pic = QPixmap(":/cars/pink");
        break;
    case Red:
        m_pic = QPixmap(":/cars/red");
        break;
    case Yellow:
        m_pic = QPixmap(":/cars/yellow");
        break;
    default:
        m_pic = QPixmap(":/cars/red");
        break;
    }
    m_pic = m_pic.scaled(M_WID, M_LEN);
    this->setPixmap(m_pic);
    setTransformOriginPoint(M_WID/2, M_LEN/2);  //设置旋转中心
}

void Cars::Forward(qreal vel)
{
    qreal ang = qDegreesToRadians(rotation());
    this->moveBy(+vel*qSin(ang), -vel*qCos(ang));
}

void Cars::Backward(qreal vel)
{
    qreal ang = qDegreesToRadians(rotation());
    this->moveBy(-vel*qSin(ang), +vel*qCos(ang));
}

void Cars::moveLeft(qreal vel)
{
    qreal ang = qDegreesToRadians(rotation());
    moveBy(-vel*qCos(ang), -vel*qSin(ang));
}

void Cars::moveRight(qreal vel)
{
    qreal ang = qDegreesToRadians(rotation());
    moveBy(vel*qCos(ang), vel*qSin(ang));
}

//ang为旋转角度（弧度），正数为顺时针，负数逆时针
void Cars::Rotate(qreal ang)
{
    setRotation(rotation() + ang);
}

void Cars::turnLeft(int r, int ang)
{
    Forward(r*qSin(ang));
    Rotate(-ang);
    moveLeft(r*(1-qCos(ang)));
}

void Cars::turnRight(int r, int ang)
{
    Forward(r*qSin(ang));
    Rotate(ang);
    moveRight(r*(1-qCos(ang)));
}

Cars::Color Cars::getColor()
{
    return this->m_color;
}

qreal Cars::getDir()
{
    return this->rotation();
}
