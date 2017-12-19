#ifndef CARS_H
#define CARS_H

#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QString>
#include <QPoint>
#include <QtMath>

//车身长宽
#define M_LEN   (200)
#define M_WID   (130)

class Cars : public QGraphicsPixmapItem
{
public:
    enum Color { Pink, Red, Yellow, RANDOM };
    explicit Cars(QGraphicsItem *parent = Q_NULLPTR, int dir = 0, Cars::Color color = RANDOM);
    void Forward(int vel = 3);
    void Backward(int vel = 3);
    void moveLeft(int vel = 3);
    void moveRight(int vel = 3);
    void Rotate(int ang = 30);
    void turnLeft(int r, int ang);
    void turnRight(int r, int ang);

    Color getColor();
    qreal getDir();

private:
    Color m_color;      //车身颜色 N为0， 顺时针递增
    QPixmap m_pic;
    QString m_number;
    QPointF  m_pos;

};

#endif // CARS_H
