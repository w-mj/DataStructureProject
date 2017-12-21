#ifndef PARKINGLOTGRAPH_H
#define PARKINGLOTGRAPH_H

#include "Road.h"
#include "ParkingSpaceWidget.h"
#include "path.h"
#include <QVector>
#include <QPoint>
#include <QGraphicsScene>

class ParkingLotGraph
{
public:
    struct Node{
    public:
        enum Type{road, space, entry};
        Node(const QPoint& p, Type t, uint n, uint idd) :type(t), data(p), number(n), id(idd) {}
        void addPath(Node* another);
        uint number;
        uint id;
        Type type;
        QPoint data;
        QVector<Node*> adjacent;
        QVector<float> weight;
        uint getId() const;
    };
    ParkingLotGraph(const ParkingLotWidget*pkl);
    Node* addNode(QPoint p, Node::Type t, uint n) {
        Node* no = new Node(p, t, n, m_all.size());
        m_all.push_back(no);
        return no;
    }
    void paint(QGraphicsScene* scene);
    Path* finaPath(Node::Type t1, uint n1, Node::Type t2, uint n2);
    ~ParkingLotGraph();
private:
   QVector<Node* > m_all;
   QVector<Node* > m_spaceList;
   QVector<Node* > m_roadNodeList;
   const ParkingLotWidget* pk;
};

#endif // PARKINGLOTGRAPH_H
