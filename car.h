#ifndef CAR_H
#define CAR_H

#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QString>
#include <QPoint>
#include <QtMath>
#include <QPropertyAnimation>
#include <QObject>
#include <QTime>
#include "path.h"
#include "parkingLotManager.h"
class ParkingLotManager;

//车身长宽
#define M_LEN   (60)
#define M_WID   (40)

class Car : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_PROPERTY(QPointF m_pos READ getmPos WRITE setmPos)
    Q_PROPERTY(int m_dir READ getDir WRITE setRotation)
public:
    enum Status {waiting, moving, parking};
    enum Color { Pink, Red, Yellow, RANDOM };
    explicit Car(ParkingLotManager * manager, QGraphicsItem *parent = Q_NULLPTR,
                 int dir = 0, Car::Color color = RANDOM);
    //基础移动
    void Forward(qreal vel = 3);    //前进
    void Backward(qreal vel = 3);   //后退
    void moveLeft(qreal vel = 3);   //向左平移
    void moveRight(qreal vel = 3);  //向右平移
    void Rotate(qreal ang = 30);    //原地旋转
    void turnLeft(int r, double ang);  //左转
    void turnRight(int r, double ang); //右转
    void setPath(Path *path);

    int getNum();
    void setNum(int value);
    int getEntryNum();
    void setEntryNum(int value);
    int getTargetFloor() const;
    void setTargetFloor(int value);
    int getCurrentFloor() const;
    void setCurrentFloor(int value);

    //动画
    void moveTo(QPointF target);
    void followPath();

    //为了Property
    QPointF getmPos();
    void    setmPos(QPointF pos);

    Color getColor();
    qreal getDir();

    QPropertyAnimation *posAni;

    QString getPosition() const;
    QString getPlateNumber() const;
    QString getFee() const;
    Status getStatus() const;
    void setStatus(const Status &value);

    QTime getStartTime() const;
    void setStartTime(const QTime &value);

private:
    Status m_status;
    Color   m_color;      //车身颜色 N为0， 顺时针递增
    QPixmap m_pic;
    QString m_number;
    QPointF m_pos;
    qreal   m_dir;
    Path    *m_path;
    PathPoint m_target;
    PathPoint m_current;
    int entryNum;
    int targetFloor;
    int currentFloor;
    int num;
    QTime startTime;
    ParkingLotManager* m_manager;
signals:
    void entry(Car* car);
    void stair(Car* car);
    void exit(Car* car);
    void queueHead(Car* car);
};

#endif // CAR_H
