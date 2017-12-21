#ifndef PARKINGLOTGRAPH_H
#define PARKINGLOTGRAPH_H

#include "Road.h"
#include "ParkingSpaceWidget.h"
#include <QVector>
#include <QPoint>
#include <QGraphicsScene>

class ParkingLotGraph
{
public:
    struct Node{
        enum Type{road, space, entry};
        uint number;
        Type type;
        QPoint data;
        QVector<Node*> adjacent;
        QVector<float> weight;
        Node(QPoint p, Type t, uint n):type(t), data(p), number(n){}
        void addPath(Node* another);
    };
    ParkingLotGraph(const ParkingLotWidget*);
    void paint(QGraphicsScene* scene);
    ~ParkingLotGraph();
private:
   QVector<Node* > m_spaceList;
   QVector<Node* > m_roadNodeList;
};

#endif // PARKINGLOTGRAPH_H
