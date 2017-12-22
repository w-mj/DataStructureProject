#include "parkinglotgraph.h"
#include "Road.h"
#include <QVector>
#include <QtMath>
#include <QStack>
#include <QPen>
#include <QDebug>
#include <QWidget>
#include <queue>
#include <QPainter>

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

ParkingLotGraph::ParkingLotGraph(const ParkingLotWidget* pkl): pk(pkl)
{
    QVector<ParkingSpaceWidget*> spaces = pk->getSpaceList();
    QVector<Road*> roads = pk->getRoadList();
    QVector<Node*> unFinished;
    m_roadNodeList.resize(roads.size() * 2);  // 每一条路有两个节点，分别为2i和2i+1
    for (int i = 0; i < roads.size(); i++) {  // 初始化每条路的节点
        QPoint p;
        Road* r = roads.at(i);
        if (r->getDir() == Road::horizontal) {
            p.setX(r->pos().x() + 5);
            p.setY(r->pos().y() + r->height() / 2);
            m_roadNodeList[2*i] = addNode(p, Node::Type::road, r->getNumber());
            p.setX(r->pos().x() + r->width() - 5);
            m_roadNodeList[2*i + 1] = addNode(p, Node::Type::road, r->getNumber());
        } else {
            p.setY(r->pos().y() + 5);
            p.setX(r->pos().x() + r->width() / 2);
            m_roadNodeList[2*i] = addNode(p, Node::Type::road, r->getNumber());
            p.setY(r->pos().y() + r->height() - 5);
            m_roadNodeList[2*i+1] = addNode(p, Node::Type::road, r->getNumber());
        }
        if (r->getAction() != Road::Action::none) {
            m_roadNodeList[2*i + r->getActionPos() - 1]->setAction(r->getAction());  // 添加动作
             // 把有动作的节点添加到dict里，方便查找
            m_actionList.insert(m_roadNodeList[2*i + r->getActionPos() - 1]->getId(),
                    m_roadNodeList[2*i + r->getActionPos() - 1]);
        }
        m_roadNodeList[2*i]->addPath(m_roadNodeList[2*i + 1]);  // 一条路的两个端点连接起来
    }
    for (int i = 0; i < roads.size(); i++) {  // 再遍历路，找到路与路的交点
        Road* r = roads.at(i);
        QPoint p = r->pos();
        if (r->getDir() == Road::horizontal) {
            p.setY(p.y() + r->height() / 2);
            p.setX(p.x() - 30);  // 向外找
        } else {
            p.setX(p.x() + r->width() / 2);
            p.setY(p.y() - 30);  // 向外找
        }
        for (int j = 0; j < roads.size(); j++) {
            Road* r2 = roads.at(j);
            QPoint crossPoint = p;
            if (i != j && inRect(r2, p)) {
                m_roadNodeList[2*i]->addPath(m_roadNodeList[2*j]);
                m_roadNodeList[2*i]->addPath(m_roadNodeList[2*j + 1]);
                if (r2->getDir() == Road::horizontal)
                    crossPoint.setY(r2->pos().y() + r2->height() / 2);
                else
                    crossPoint.setX(r2->pos().x() + r2->width() / 2);
                unFinished.push_back(addNode(crossPoint, Node::Type::road, r2->getNumber()));
                break;
            }
        }
        if (r->getDir() == Road::horizontal) {
            p.setX(p.x() + r->width() + 60);  // 补偿x减掉的30
        } else {
            p.setY(p.y() + r->height() + 60);
        }
        for (int j = 0; j < roads.size(); j++) {
            Road* r2 = roads.at(j);
            QPoint crossPoint = p;
            if (i != j && inRect(r2, p)) {
                m_roadNodeList[2*i+1]->addPath(m_roadNodeList[2*j]);
                m_roadNodeList[2*i+1]->addPath(m_roadNodeList[2*j + 1]);
                if (r2->getDir() == Road::horizontal)
                    crossPoint.setY(r2->pos().y() + r2->height() / 2);
                else
                    crossPoint.setX(r2->pos().x() + r2->width() / 2);
                unFinished.push_back(addNode(crossPoint, Node::Type::road, r2->getNumber()));
                break;
            }
        }
    }

    // 连接岔路
    for (Node* cross: unFinished) {
        Node* endPoint = m_roadNodeList.at(2 * (cross->number - 1));
        for (Node* adj: endPoint->adjacent) {
           cross->addPath(adj);
        }
    }

    for(ParkingSpaceWidget* sp : spaces) {
        QPoint p = toCenter(sp), offset(0, 0);
        Node* spaceNode = addNode(p, Node::Type::space, sp->getNumber());
        m_spaceList.push_back(spaceNode);  // 保证按编号顺序
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

uint ParkingLotGraph::getNodeId(ParkingLotGraph::Node::Type t, int n) {
    if (t == Node::Type::space) {
        return  m_spaceList.at(n - 1)->getId();
    } else if(t == Node::Type::entry) {
        int k = 0;
        for (Node* node: m_actionList.values())
            if (node->action==Road::Action::entry)
                if (++k == n)
                    return node->id;
    }
    return -1;
}

qreal angle(const QPoint& p) {
    return qRadiansToDegrees(qAtan2(p.y(), p.x()));
}

// Dijkstra 求最短路径
Path *ParkingLotGraph::finaPath(ParkingLotGraph::Node::Type t1, int n1, ParkingLotGraph::Node::Type t2, int n2)
{
    struct HeapNode {
        uint d, u;//d为到该点的总路径，u为这一点
        HeapNode(uint a, uint b) :d(a), u(b) {}
        bool operator < (const HeapNode& rhs) const { return d > rhs.d;}//重载优先级队列的小于号
    };
    bool vis[m_all.size()] = {0};
    uint weight[m_all.size()];
    uint prev[m_all.size()] = {0};
    for (int i = 0; i < m_all.size(); i++)
        weight[i] = 0x3f3f3f; // inf

    n1 = qAbs(n1);
    n2 = qAbs(n2);

    uint id1 = getNodeId(t1, n1);  // 通过number和类型获得id
    uint id2 = getNodeId(t2, n2);

    weight[id1] = 0;

    std::priority_queue<HeapNode> queue;

    queue.push(HeapNode(0, id1));

    while (!queue.empty()) {
        HeapNode cur = queue.top();
        queue.pop();
        if (vis[cur.u])
            continue;
        vis[cur.u] = true;
        Node* node = m_all[cur.u];
        for (int i = 0; i < node->adjacent.size(); i++) {
            if (weight[cur.u] + node->weight[i] < weight[node->adjacent[i]->id]) {
                weight[node->adjacent[i]->id] = weight[cur.u] + node->weight[i];
                prev[node->adjacent[i]->id] = cur.u;
                queue.push(HeapNode(weight[node->adjacent[i]->id], node->adjacent[i]->id));
            }
        }
    }
    Path* path = new Path();
    uint n = id2;
    QPoint lastP;
    while (n != id1) {
        // 相邻两个点的xy都不想等，添加一个辅助顶点构成矩形
        if (m_all[n]->data.x() != m_all[prev[n]]->data.x() &&
                m_all[n]->data.y() != m_all[prev[n]]->data.y()) {
            QPoint tp;
            if (m_all[n]->type == Node::Type::space) {
                ParkingSpaceWidget* space = pk->getSpaceList().at(m_all[n]->number - 1);
                switch (space->getDir()) {
                case ParkingSpaceWidget::direction::E:
                case ParkingSpaceWidget::direction::W:
                    tp.setX(m_all[prev[n]]->data.x());
                    tp.setY(m_all[n]->data.y());
                    break;
                case ParkingSpaceWidget::direction::N:
                case ParkingSpaceWidget::direction::S:
                    tp.setX(m_all[n]->data.x());
                    tp.setY(m_all[prev[n]]->data.y());
                    break;
                }
            } else if (m_all[n]->type == Node::Type::road) {
                Road* road = pk->getRoadList().at(m_all[n]->number - 1);
                switch(road->getDir()) {
                case Road::direction::horizontal:
                    tp.setX(m_all[prev[n]]->data.x());
                    tp.setY(m_all[n]->data.y());
                    break;
                case Road::direction::vertical:
                    tp.setX(m_all[n]->data.x());
                    tp.setY(m_all[prev[n]]->data.y());
                    break;
                }
            }
            if (lastP.isNull())
                lastP = tp;

            qreal ang1 = angle(lastP - m_all[n]->data);
            qreal ang2 = angle(m_all[n]->data - tp);

            path->addPoint(0, PathPoint(QPointF(m_all[n]->data), ang1, m_all[n]->action, m_all[n]->getId()));  // 添加后一个点
            path->addPoint(0, PathPoint(QPointF(tp), ang2));  // 添加辅助点
            lastP = tp;
        } else {
            qreal ang = angle(m_all[n]->data - m_all[prev[n]]->data);
            path->addPoint(0, PathPoint(QPointF(m_all[n]->data), ang, m_all[n]->action, m_all[n]->getId()));
        }
        n = prev[n];
    }
    path->addPoint(0, PathPoint(QPointF(m_all[n]->data), angle(lastP - m_all[n]->data), m_all[n]->action, m_all[n]->getId())); // 把第一个点也加进去
    return path;
}

ParkingLotGraph::~ParkingLotGraph()
{
    for (Node* n: m_all)
        delete n;
    delete m_pixmap;
}

QPixmap *ParkingLotGraph::getPixmap()
{
    if (m_pixmap == nullptr)
        generatePixmap();
    return m_pixmap;
}

void ParkingLotGraph::generatePixmap()
{
    m_pixmap = new QPixmap(pk->size());
    m_pixmap->fill(Qt::transparent);
    QPainter painter(m_pixmap);
    QVector<Node*> vis(m_spaceList.size() + m_roadNodeList.size());
    QStack<Node*> stack;
    painter.setPen(Qt::red);
//    for (int i = 0; i < m_roadNodeList.size(); i++) {
//        qDebug() << i << "  " << m_roadNodeList.at(i)->adjacent.size();
//    }
    stack.push(m_spaceList.first());
    while (!stack.empty()) {
        Node* n = stack.pop();
        if (vis.indexOf(n) != -1)
            continue;
        vis.push_back(n);
        // qDebug() << "关联节点共有" << QString::number(n->adjacent.size());
        if (n->action != Road::Action::none) {
            painter.setBrush(Qt::red);
            painter.drawEllipse(n->data.x(), n->data.y(), 5, 5);
            painter.setBrush(Qt::NoBrush);
        } else
            painter.drawEllipse(n->data.x(), n->data.y(), 5, 5);
        for(Node* adj: n->adjacent) {
            if (vis.indexOf(adj) != -1)
                continue;
            stack.push(adj);
            painter.drawLine(n->data.x(), n->data.y(), adj->data.x(), adj->data.y());
        }
    }
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

Road::Action ParkingLotGraph::Node::getAction() const
{
    return action;
}

int ParkingLotGraph::Node::getId() const
{
    return id;
}
