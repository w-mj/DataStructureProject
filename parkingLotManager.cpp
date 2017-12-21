#include "parkingLotManager.h"
#include "ParkingLotWidget.h"
#include "ParkingSpaceWidget.h"
#include "path.h"
#include "MyEvent.h"
#include <QDebug>
#include <QPainter>
#include <QPoint>
#include <QRect>
#include <QGraphicsScene>
#include <QApplication>

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
    for (uint i = 0; i < m_num_of_layer; i++) {  // 全部为饿汉模式
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
        m_graph.push_back(new ParkingLotGraph(widget));
        m_graph_pixmap.push_back(new QGraphicsPixmapItem(*m_graph.last()->getPixmap()));
        QObject::connect(this, &ParkingLotManager::showMarginSignal, widget, &ParkingLotWidget::showMarginSlot);
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
    for (QGraphicsPixmapItem* p: m_graph_pixmap)
        if (m_scene->items().contains(p))
            m_scene->removeItem(p);
    if (m_showGraph) {
        m_scene->addItem(m_graph_pixmap.at(m_current_floor));
    }
    emit setCapacity(QString::number(m_capacity.at(pos)));
    emit setLoad(QString::number(m_capacity.at(pos) - m_num_of_cars.at(pos)));
    emit setLayerName(m_name.at(pos));
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

ParkingLotGraph::Node::Type getType(int n) {
    if (n > 0)
        return ParkingLotGraph::Node::Type::space;
    else
        return ParkingLotGraph::Node::Type::entry;
}

void ParkingLotManager::drawPath(int n1, int n2)
{
    Path *p = m_graph[1]->finaPath(getType(n1), n1, getType(n2), n2);
    for(int i = 1; i < p->pointsCount(); i++) {
        QLineF line(p->getPoint(i-1).point, p->getPoint(i).point);
        m_scene->addLine(line);
    }
}

void ParkingLotManager::showMargin(bool enable)
{
    // qDebug() << "show margin" << enable;
    emit showMarginSignal(enable);
}

void ParkingLotManager::showGraph(bool enable)
{
    // qDebug() << "show graph" << enable;
    m_showGraph = enable;
    showParkingLot();
}

void ParkingLotManager::clearScene()
{
    qDebug() << "清空scene";
    m_scene->clear();
}
