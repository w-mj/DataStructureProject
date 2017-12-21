#ifndef PARKINGLOTGRAPH_H
#define PARKINGLOTGRAPH_H

#include "Road.h"
#include "ParkingSpaceWidget.h"
#include "path.h"
#include <QVector>
#include <QPoint>
#include <QGraphicsScene>
#include <QMap>
#include "Road.h"

class ParkingLotGraph
{
public:
    struct Node{
    public:
        enum Type{road, space, entry};
        Node(const QPoint& p, Type t, uint n, uint idd) : number(n), id(idd), type(t), data(p) {}
        void addPath(Node* another);
        uint number;
        uint id;
        Type type;
        QPoint data;
        Road::Action action = Road::Action::none;
        QVector<Node*> adjacent;
        QVector<float> weight;
        uint getId() const;
        Road::Action getAction() const;
        void setAction(const Road::Action &value) {
            action = value;
        }
    };
    ParkingLotGraph(const ParkingLotWidget*pkl);
    Node* addNode(QPoint p, Node::Type t, uint n) {
        Node* no = new Node(p, t, n, m_all.size());
        m_all.push_back(no);
        return no;
    }
    void paint(QGraphicsScene* scene);
    Path* finaPath(Node::Type t1, int n1, Node::Type t2, int n2);
    uint getNodeId(Node::Type t, int n);
    ~ParkingLotGraph();
private:
   QVector<Node* > m_all;
   QVector<Node* > m_spaceList;
   QVector<Node* > m_roadNodeList;
   QMap<uint, Node*> m_actionList;
   const ParkingLotWidget* pk;
};

#endif // PARKINGLOTGRAPH_H
