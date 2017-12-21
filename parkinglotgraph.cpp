#include "parkinglotgraph.h"
#include "Road.h"
#include <QVector>
#include <QtMath>
#include <QStack>
#include <QPen>
#include <QDebug>
#include <QWidget>

bool inRect(const QWidget* r, const QPoint& p) {
    if (p.x() > r->pos().x() && p.x() < r->pos().x() + r->width() &&
        p.y() > r->pos().y() && p.y() < r->pos().y() + r->height())
        return true;
    else
        return false;
}

QPoint toCenter(const QWidget* w) {
    QPoint p;
    p.setX(w->pos().x() + w->width() / 2);
    p.setY(w->pos().y() + w->height() / 2);
    return p;
}

ParkingLotGraph::ParkingLotGraph(const ParkingLotWidget* pk)
{
    QVector<ParkingSpaceWidget*> spaces = pk->getSpaceList();
    QVector<Road*> roads = pk->getRoadList();
    m_roadNodeList.resize(roads.size() * 2);  // 每一条路有两个节点，分别为2i和2i+1
    for (int i = 0; i < roads.size(); i++) {  // 初始化每条路的节点
        QPoint p;
        Road* r = roads.at(i);
        if (r->getDir() == Road::horizontal) {
            p.setX(r->pos().x() + 5);
            p.setY(r->pos().y() + r->height() / 2);
            m_roadNodeList[2*i] = new Node(p, Node::Type::road, r->getNumber());
            p.setX(r->pos().x() + r->width() - 5);
            m_roadNodeList[2*i + 1] = new Node(p, Node::Type::road, r->getNumber());
        } else {
            p.setY(r->pos().y() + 5);
            p.setX(r->pos().x() + r->width() / 2);
            m_roadNodeList[2*i] = new Node(p, Node::Type::road, r->getNumber());
            p.setY(r->pos().y() + r->height() - 5);
            m_roadNodeList[2*i+1] = new Node(p, Node::Type::road, r->getNumber());
        }
    }
    for (int i = 0; i < roads.size(); i++) {  // 再遍历路，找到路与路的交点
        Road* r = roads.at(i);
        QPoint p = r->pos();
        if (r->getDir() == Road::horizontal) {
            p.setY(p.y() + r->height() / 2);
            p.setX(p.x() - 30);  // 向外找
            for (int j = 0; j < roads.size(); j++) {
                if (i != j && inRect(roads.at(j), p)) {
                    m_roadNodeList[2*i]->addPath(m_roadNodeList[2*j]);
                    m_roadNodeList[2*i]->addPath(m_roadNodeList[2*j + 1]);
                    break;
                }
            }
            p.setX(p.x() + r->width() + 60);  // 补偿x减掉的30
            for (int j = 0; j < roads.size(); j++) {
                if (i != j && inRect(roads.at(j), p)) {
                    m_roadNodeList[2*i+1]->addPath(m_roadNodeList[2*j]);
                    m_roadNodeList[2*i+1]->addPath(m_roadNodeList[2*j + 1]);
                    break;
                }
            }
        } else {
            p.setX(p.x() + r->width() / 2);
            p.setY(p.y() - 30);  // 向外找
            for (int j = 0; j < roads.size(); j++) {
                if (i != j && inRect(roads.at(j), p)) {
                    m_roadNodeList[2*i]->addPath(m_roadNodeList[2*j]);
                    m_roadNodeList[2*i]->addPath(m_roadNodeList[2*j + 1]);
                    break;
                }
            }
            p.setY(p.y() + r->height() + 60);
            for (int j = 0; j < roads.size(); j++) {
                if (i != j && inRect(roads.at(j), p)) {
                    m_roadNodeList[2*i+1]->addPath(m_roadNodeList[2*j]);
                    m_roadNodeList[2*i+1]->addPath(m_roadNodeList[2*j + 1]);
                    break;
                }
            }
        }
    }
    for(ParkingSpaceWidget* sp : spaces) {
        QPoint p = toCenter(sp), offset(0, 0);
        Node* spaceNode = new Node(p, Node::Type::space, sp->getNumber());
        m_spaceList.push_back(spaceNode);
        switch(sp->getDir()) {
        case ParkingSpaceWidget::N: offset.setY(-1);break;
        case ParkingSpaceWidget::S: offset.setY(1);break;
        case ParkingSpaceWidget::E: offset.setX(1);break;
        case ParkingSpaceWidget::W: offset.setX(-1);break;
        }
        while(inRect(sp, p)) p += offset;  // 直到走出这个widget
        p+=offset*5; // 再走两步
        for (int i = 0; i < roads.size(); i++) {  // 遍历所有路，找到与这个点相连的路，添加路径
            Road* r = roads.at(i);
            if (inRect(r, p)) {
                m_roadNodeList[2*i]->addPath(spaceNode);
                m_roadNodeList[2*i + 1]->addPath(spaceNode);
                break;
            }
        }
    }

}

void ParkingLotGraph::paint(QGraphicsScene *scene)
{
    QVector<Node*> vis(m_spaceList.size() + m_roadNodeList.size());
    QStack<Node*> stack;
    QPen pen(Qt::blue);
    qDebug() << "共有" << QString::number(m_spaceList.size()) << "个车位节点";
    qDebug() << "共有" << QString::number(m_roadNodeList.size()) << "个路节点";
    for (int i = 0; i < m_roadNodeList.size(); i++) {
        qDebug() << i << "  " << m_roadNodeList.at(i)->adjacent.size();
    }
    stack.push(m_spaceList.first());
    while (!stack.empty()) {
        Node* n = stack.pop();
        if (vis.indexOf(n) != -1)
            continue;
        vis.push_back(n);
        // qDebug() << "关联节点共有" << QString::number(n->adjacent.size());
        scene->addEllipse(n->data.x(), n->data.y(), 5, 5, pen);
        for(Node* adj: n->adjacent) {
            if (vis.indexOf(adj) != -1)
                continue;
            stack.push(adj);
            scene->addLine(n->data.x(), n->data.y(), adj->data.x(), adj->data.y(), pen);
        }
    }
}

ParkingLotGraph::~ParkingLotGraph()
{
    for (Node* n: m_spaceList)
        delete n;
    for (Node* n: m_roadNodeList)
        delete n;
}

void ParkingLotGraph::Node::addPath(ParkingLotGraph::Node *another)
{
    for (Node* adj: adjacent)
        if (adj == another)
            return;
    adjacent.push_back(another);
    another->adjacent.push_back(this);
    int dx = data.x() - another->data.x();
    int dy = data.y() - another->data.y();
    float w = qSqrt(dx * dx + dy * dy);
    weight.push_back(w);
    another->weight.push_back(w);
}
