#include "parkingLotManager.h"
#include "ParkingLotWidget.h"
#include "ParkingSpaceWidget.h"
#include <QDebug>
#include <QPainter>
#include <QPoint>
#include <QRect>
#include <QGraphicsScene>

//bool inRect(const QRect& r, const QPoint& p) {
//    if (p.x() > r.x() && p.x() < r.x() + r.width() &&
//        p.y() > r.y() && p.y() < r.y() + r.height())
//        return true;
//    else
//        return false;
//}

ParkingLotManager::ParkingLotManager(QObject* objectParent, QGraphicsScene* scene):
    QObject(objectParent), m_scene(scene)
{
    // 找到xml文件路径
    QString filePath = __FILE__;
    QStringList list = filePath.split("\\");
    QString result;
    for (int i = 0; i < list.size() - 1; i++)
        result.append(list.at(i)).append("\\");
    QStringList l = {"parkinglot_b1.xml", "parkinglot_f1.xml"};
    for (int i = 0; i < 2; i++) {
        QString t(result);
        m_xml_path.push_back(t.append(l.at(i)));
    }
    m_cars.resize(m_num_of_layer);  // 停在每一层的车
    m_waitting.resize(m_num_of_entry); // 在每个入口等待的车
    for (uint i = 0; i < m_num_of_layer; i++) {
        ParkingLotWidget* widget = new ParkingLotWidget(Q_NULLPTR, m_xml_path.at(i));  // 生成一个widget
        // m_pixmap.push_back(new QPixmap(widget->size()));  // 创建一层的pixmap
        // qDebug() << "widget size x=" << widget->size().width() << " y=" <<widget->size().height();
        // widget->render(m_pixmap[i]);  // 渲染pixmap
        m_capacity.push_back(widget->getCapacity());  // 获得这一层的最大容量
        m_cars[i].resize(m_capacity.at(i));  // 初始化一层的车位
        m_name.push_back(widget->getName());  // 这一层的名字
        m_num_of_cars.push_back(0);  // 初始停车数量为0
        widget->hide();
        m_widgets.push_back(widget);
        m_scene->addWidget(widget);
        if (i == 1)
            m_graph.push_back(new ParkingLotGraph(widget));
        else
            m_graph.push_back(nullptr);
    }
}

void ParkingLotManager::showParkingLot(uint pos)
{
    if (pos >= m_num_of_layer) {
        qDebug() << "ParkingLotManager: 层数错误";
        return;
    }
    for (uint i = 0; i < m_num_of_layer; i++)
        m_widgets.at(i)->hide();
    m_widgets.at(pos)->show();
    emit setCapacity(QString::number(m_capacity.at(pos)));
    emit setLoad(QString::number(m_capacity.at(pos) - m_num_of_cars.at(pos)));
    emit setLayerName(m_name.at(pos));
    if (pos == 1)
        m_graph[1]->paint(m_scene);
}

void ParkingLotManager::showParkingLot()
{
    showParkingLot(m_current_floor);
}

void ParkingLotManager::showDownStairFloor()
{
    if (m_current_floor == m_num_of_layer - 1)  // 当前是最顶层
        emit enableUpButton(true);
    if (m_current_floor == 1)
        emit enableDownButton(false);  // 到底
    if (m_current_floor > 0)
        --m_current_floor;
    showParkingLot(m_current_floor);
}

void ParkingLotManager::showUpStairFloor()
{
    if (m_current_floor == 0)
        emit enableDownButton(true);
    if (m_current_floor == m_num_of_layer - 2)
        emit enableUpButton(false);
    if (m_current_floor < m_num_of_layer - 1)
        ++ m_current_floor;
    showParkingLot(m_current_floor);
}

void ParkingLotManager::drawPath(uint n1, uint n2)
{
//    class Turtle {
//        QPoint p;
//        QGraphicsScene* s;
//    public:
//        Turtle(QPoint start, QGraphicsScene* scence): p(start), s(scence) {}
//        void goX(uint n) {
//            QPoint a = p;
//            a.setX(p.x() + n);
//            s->addLine(QLine(p, a));
//            p = a;
//        }
//        void goY(uint n) {
//            QPoint a = p;
//            a.setY(p.y() + n);
//            s->addLine(QLine(p, a));
//            p = a;
//        }
//        QPoint getP() const {return p;}
//        int x() const {return p.x();}
//        int y() const {return p.y();}
//    };
//    ParkingSpaceWidget* space1 = m_widgets.at(m_current_floor)->getSpaceList().at(n1 - 1);
//    ParkingSpaceWidget* space2 = m_widgets.at(m_current_floor)->getSpaceList().at(n2 - 1);
//    QPoint start = space1->pos(), end = space2->pos();
//    start.setX(start.x() + space1->width() / 2);
//    start.setY(start.y() + space1->height() / 2);  // 设定为车位的中点
//    end.setX(end.x() + space2->width() / 2);
//    end.setY(end.y() + space2->height() / 2);  // 设定为车位的中点
//    Turtle turtle(start, m_scene);
//    switch(space1->getDir()) {
//    case ParkingSpaceWidget::N: turtle.goY(-45); break;
//    case ParkingSpaceWidget::S: turtle.goY(45); break;
//    case ParkingSpaceWidget::E: turtle.goX(45); break;
//    case ParkingSpaceWidget::W: turtle.goX(-45); break;
//    }  // 从车位里走出来
//    int n = 0, Point;
//    while (turtle.x() != end.x() || turtle.y() != end.y()) {
//        Road* road;
//        for (Road* r: m_widgets.at(m_current_floor)->m_roadList)
//            if (inRect(*r, turtle.getP())) {
//                road = r;
//                break;
//            }  // 找到当前点在哪条路上

////        if (turtle.x() != end.x() && turtle.y() != end.y()) {
////            switch(space1->getDir()) {
////            case ParkingSpaceWidget::N:
////            case ParkingSpaceWidget::S: turtle.goX(end.x() - turtle.x()); break;
////            case ParkingSpaceWidget::E:
////            case ParkingSpaceWidget::W: turtle.goY(end.y() - turtle.y()); break;
////            }
////        } else if (turtle.x() == end.x())
////            turtle.goY(end.y() - turtle.y());
////        else
////            turtle.goX(end.x() - turtle.x());
////        if (n++ == 100)
////            break;
//    }
//    qDebug() << n;
}
