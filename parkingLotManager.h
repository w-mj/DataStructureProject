#ifndef PARKINGLOTITEM_H
#define PARKINGLOTITEM_H

#include <QPixmap>
#include <QVector>
#include <QQueue>
#include <QGraphicsScene>
#include "car.h"
#include "ParkingLotWidget.h"
#include "parkinglotgraph.h"

class Car;

class ParkingLotManager: public QObject
{
    Q_OBJECT
public:
    explicit ParkingLotManager(QObject* objectParent, QGraphicsScene* scene);
    struct RequestSpace {
        bool success = false;
        uint layer = 0;
        uint number = 0;
        Path* path = Q_NULLPTR;
        RequestSpace() {}
        RequestSpace(uint l, uint n, Path* p):success(true), layer(l), number(n), path(p)  {}
        RequestSpace(const RequestSpace& another) = default;
    };
    void showParkingLot(uint pos);
    void showParkingLot(void);
    void showDownStairFloor(void);
    void showUpStairFloor(void);
    void drawPath(int n1, int n2);

public slots:
    void requestIn(Car* car);
    void requestOut(Car* car, int exit = -1);
    void requestStair(Car* car);
    void leave(Car* car);
    void addCar(int entry = -1);

    void showMargin(bool enable);
    void showGraph(bool enable);
    void banSpace(bool banned, int layer, int number);

private:
    QGraphicsScene* m_scene;
    QVector<ParkingLotWidget*> m_widgets;  // 每一层的widget
    QVector<int> m_capacity;  // 每一层的容量
    QVector<int> m_num_of_cars;  // 每一层当前停的车数量
    QVector<QVector<Car*> > m_cars;  // 停在每一层的车
    QVector<QList<Car*> >m_waitting; // 在每个入口等待的车
    QList<Car*> m_all_cars;
    QVector<ParkingLotGraph*> m_graph;
    QVector<QGraphicsPixmapItem*> m_graph_pixmap;
    QStringList m_xml_path;  // 每一层xml文件地址
    QStringList m_name;  // 每一层的名字
    QList<QPair<int, int>> m_pool;  // 车位池
    const uint m_num_of_layer = 2;  // 总层数
    const uint m_num_of_entry = 2;  // 入口数
    uint m_current_floor = 1; // 当前楼层，默认为1楼（按照从下往上递增，值与实际楼层无关）
    bool m_showGraph = false;  // 是否显示图

    void generatePool(bool sequence);

signals:
    void enableUpButton(bool);
    void enableDownButton(bool);
    void setCapacity(const QString&);
    void setLoad(const QString&);
    void setLayerName(const QString&);
    void showMarginSignal(bool);

    void carLeave(void);
};

#endif // PARKINGLOTITEM_H
