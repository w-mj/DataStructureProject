#include "parkingLotManager.h"
#include "ParkingLotWidget.h"
#include "ParkingSpaceWidget.h"
#include "path.h"
#include <QDebug>
#include <QPainter>
#include <QPoint>
#include <QRect>
#include <QGraphicsScene>
#include <QApplication>
#include "logwindow.h"
#include "car.h"

#define GET_SITUATION(l, n) (m_widgets[l]->getSpaceList().at(n)->getSituation())
#define BANNED (ParkingSpaceWidget::Situation::banned)
#define OCCUPIED (ParkingSpaceWidget::Situation::occupied)
#define FREE (ParkingSpaceWidget::Situation::free)

#define NODE_TYPE ParkingLotGraph::Node::Type


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
        QObject::connect(widget, &ParkingLotWidget::banParkingSpace, [i, this](bool b, int n){banSpace(b, i, n);});
    }
    generatePool(true);
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
    emit setLoad(QString::number(m_capacity.at(pos) - m_num_of_cars[pos]));
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
        return ParkingLotGraph::Node::Type::act;
}

void ParkingLotManager::drawPath(int n1, int n2)
{
    QPen pen(QColor(242, 23, 242));
    pen.setWidth(3);
    Path *p = m_graph[m_current_floor]->findPath(getType(n1), n1, getType(n2), n2);
    for(int i = 1; i < p->pointsCount(); i++) {
        QLineF line(p->getPoint(i-1).point, p->getPoint(i).point);
        QPointF dir(qCos(qDegreesToRadians(p->getPoint(i-1).dir)) * 10,
                    qSin(qDegreesToRadians(p->getPoint(i-1).dir)) * 10);
        QLineF dirLine(p->getPoint(i-1).point, p->getPoint(i-1).point + dir);
        m_scene->addLine(line);
        m_scene->addLine(dirLine, pen);
        m_scene->addText(QString::number(p->getPoint(i-1).dir))->moveBy(p->getPoint(i-1).point.x(),
                                                                        p->getPoint(i-1).point.y());
    }
}

void ParkingLotManager::requestOut(Car *car, int exit)
{
    if (exit != -1)
        exit = rand() % m_num_of_entry;
    Path *p; // TODO：请求出口
}

void ParkingLotManager::requestIn(Car* car)
{
    int entry = car->getEntryNum();
    if (!m_pool.empty()) {
        uint l = m_pool.first().first;
        int n = m_pool.first().second + 1;
        m_pool.removeAt(0);
        Path* p;
        if (l == m_current_floor)
            p = m_graph[l]->findPath(NODE_TYPE::queueHead, entry, NODE_TYPE::space, n);
        else {
            p = m_graph[m_current_floor]->findPath(NODE_TYPE::queueHead, entry, NODE_TYPE::stair, entry);
        }
        m_waitting[entry - 1].removeOne(car);  // 移除等候
        m_cars[l][n] = car;
        car->setPath(p);
        car->setFloor(l);
        car->setNum(n);
        car->followPath();
        Log::i(QString("分配第%1层%2号车位，剩余%3个空车位").arg(l).arg(n).arg(m_pool.size()));
    } else {
        Log::i("请求失败，车位已满");
    }
}

void ParkingLotManager::leave(Car* car)
{
    int l = car->getFloor();
    int n = car->getNum();
    m_pool.append(qMakePair(l, n));
    Log::i(QString("有车离开，当前共有%1个空车位").arg(m_pool.size()));
    emit carLeave();
    emit setLoad(QString::number(m_capacity.at(l) - m_num_of_cars[l]));  // 更新剩余车位
}

void ParkingLotManager::addCar(int entry)
{
    Log::i("生成车");
    if (entry == -1)
        entry = (rand() % m_num_of_entry);
    Car *car = new Car(this);
    car->setEntryNum(entry + 1);
    m_waitting[entry].append(car);
    Path *p = m_graph[1]->findPath(NODE_TYPE::entry, entry + 1, NODE_TYPE::queueHead, entry + 1);
    car->setPath(p);
    car->followPath();
    m_all_cars.append(car);
    m_scene->addItem(car);
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

void ParkingLotManager::banSpace(bool banned, int layer, int number)
{
    if (banned) {
        m_pool.removeOne(qMakePair(layer, number));
        Log::i(QString("禁用成功，当前共有") + QString::number(m_pool.size()) + "个空车位");
    } else {
        m_pool.append(qMakePair(layer, number));
        Log::i(QString("解禁成功，当前共有") + QString::number(m_pool.size()) + "个空车位");
    }
}

void ParkingLotManager::generatePool(bool sequence)
{
    for (int l = m_num_of_layer - 1; l >= 0; l--) {
        for (int n = 0; n < m_capacity[l]; n++) {
            if (GET_SITUATION(l, n) == FREE) {
                m_pool.append(qMakePair(l, n));
            }
        }
    }
    if (!sequence)
        std::random_shuffle(m_pool.begin(), m_pool.end());
    Log::i(QString("生成车位池成功，当前共有") + QString::number(m_pool.size()) + "个空车位");
}
