#include "car.h"


Car::Car(QGraphicsItem *parent, int dir, Car::Color color) : QGraphicsPixmapItem(parent)
{
    m_pos = this->pos();
    this->setRotation(dir);
    if(color==RANDOM)
        color = Car::Color(rand()%3);  //车身颜色一定是确定的
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

void Car::Forward(qreal vel)
{
    qreal ang = qDegreesToRadians(rotation());
    this->moveBy(+vel*qSin(ang), -vel*qCos(ang));
}

void Car::Backward(qreal vel)
{
    qreal ang = qDegreesToRadians(rotation());
    this->moveBy(-vel*qSin(ang), +vel*qCos(ang));
}

void Car::moveLeft(qreal vel)
{
    qreal ang = qDegreesToRadians(rotation());
    moveBy(-vel*qCos(ang), -vel*qSin(ang));
}

void Car::moveRight(qreal vel)
{
    qreal ang = qDegreesToRadians(rotation());
    moveBy(vel*qCos(ang), vel*qSin(ang));
}

//ang为旋转角度（弧度），正数为顺时针，负数逆时针
void Car::Rotate(qreal ang)
{
    setRotation(rotation() + ang);
}

//r为弯道半径，ang为转过的角度
void Car::turnLeft(int r, int ang)
{
    qreal rad = qDegreesToRadians(ang);
    Forward(r*qSin(rad));
    moveLeft(r*(1-qCos(rad)));
    Rotate(-ang);
}

//r为弯道半径，ang为转过的角度
void Car::turnRight(int r, int ang)
{
    qreal rad = qDegreesToRadians(ang);
    Forward(r*qSin(rad));
    moveRight(r*(1-qCos(rad)));
    Rotate(ang);
}

Car::Color Car::getColor()
{
    return this->m_color;
}

qreal Car::getDir()
{
    return this->rotation();
}
