#ifndef CAR_H
#define CAR_H

#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QString>
#include <QPoint>
#include <QtMath>

//车身长宽
#define M_LEN   (200)
#define M_WID   (130)

class Car : public QGraphicsPixmapItem
{
public:
    enum Color { Pink, Red, Yellow, RANDOM };
    explicit Car(QGraphicsItem *parent = Q_NULLPTR, int dir = 0, Car::Color color = RANDOM);
    void Forward(qreal vel = 3);    //前进
    void Backward(qreal vel = 3);   //后退
    void moveLeft(qreal vel = 3);   //向左平移
    void moveRight(qreal vel = 3);  //向右平移
    void Rotate(qreal ang = 30);    //原地旋转
    void turnLeft(int r, double ang);  //左转
    void turnRight(int r, double ang); //右转

    Color getColor();
    qreal getDir();

private:
    Color m_color;      //车身颜色 N为0， 顺时针递增
    QPixmap m_pic;
    QString m_number;
    QPointF  m_pos;

};

#endif // CAR_H
