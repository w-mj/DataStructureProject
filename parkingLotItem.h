#ifndef PARKINGLOTITEM_H
#define PARKINGLOTITEM_H

#include <QGraphicsItem>
#include <QPixmap>
#include <QVector>
#include <QQueue>
#include "car.h"

class ParkingLotItem: public QGraphicsPixmapItem
{
public:
    ParkingLotItem(QGraphicsPixmapItem* parent);
    void showParkingLot(uint pos);
private:
    QVector<QPixmap*> m_pixmap;  // 每一层的pixmap
    QVector<int> m_max_capacity;  // 每一层的容量
    QVector<int> m_num_of_cars;  // 每一层当前停的车数量
    QVector<QVector<Car*> > m_cars;  // 停在每一层的车
    QVector<QQueue<Car*> >m_waitting; // 在每个入口等待的车
    QStringList m_xml_path;  // 每一层xml文件地址
    QStringList m_name;  // 每一层的名字
    const uint m_num_of_layer = 2;  // 总层数
    const uint m_num_of_entry = 2;  // 入口数
};

#endif // PARKINGLOTITEM_H
