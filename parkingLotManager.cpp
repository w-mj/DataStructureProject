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
#include "carlist.h"

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
    Adapter *adapter = new Adapter(objectParent, &m_all_cars);  // 注册model
    CarList::getInstance()->setAdapter(adapter);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &ParkingLotManager::periodWork);
    QTimer* priceTimer = new QTimer(this);
    connect(priceTimer, &QTimer::timeout, this, &ParkingLotManager::requestPrice);
    priceTimer->start(1000);
}

void ParkingLotManager::showParkingLot(uint pos)
{
    if (pos >= m_num_of_layer) {
        qDebug() << "ParkingLotManager: 层数错误";
        return;
    }
    for (uint i = 0; i < m_num_of_layer; i++)
        m_widgets.at(i)->hide();
    qDebug()<<(QString("LINE:%1").arg(__LINE__));
    m_widgets.at(pos)->show();
    for (QGraphicsPixmapItem* p: m_graph_pixmap)
        if (m_scene->items().contains(p))
            m_scene->removeItem(p);
    if (m_showGraph) {
        qDebug()<<(QString("LINE:%1").arg(__LINE__));
        m_scene->addItem(m_graph_pixmap.at(m_current_floor));
    }
    for (auto c: m_all_cars) {
        if (c->getCurrentFloor() != static_cast<int>(pos))
            c->hide();
        else
            c->show();
    }
    m_scene->setSceneRect(m_widgets[pos]->rect());  // 固定位置
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
    if (n1 < -8 || n1 > 104 || n2 < -8 || n2 > 104)
        return ;
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

void ParkingLotManager::drawPath(Path* p)
{
    QPen pen(QColor(242, 23, 242));
    pen.setWidth(3);
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

bool ParkingLotManager::checkSame(const QString &plate)
{
    for (auto c: m_all_cars)
        if (c->getPlateNumber() == plate)
            return true;
    return false;
}

QString ParkingLotManager::getParkingLotName(int pos)
{
    return m_widgets.at(pos)->getName();
}

QPair<QString, int> ParkingLotManager::search(const QString& plate)
{
    for (auto c: m_all_cars) {
        if (c->getPlateNumber() == plate) {
            if (c->getStatus() == Car::Status::parking ||
                    c->getStatus() == Car::Status::moving)
                return qMakePair(m_widgets.at(c->getTargetFloor())->getName(),
                                 c->getNum());
            else if (c->getStatus() == Car::Status::waiting)
                return qMakePair(QString("0"), 0);
        }
    }
    return qMakePair(QString("-1"), -1);
}

bool comp2(const Car* c1, const Car* c2) {
    return c1->getPlateNumber() < c2->getPlateNumber();
}

int comp(const QString& p1, const Car* c2) {
    return strcmp(p1.toStdString().c_str(),  c2->getPlateNumber().toStdString().c_str());
}

QPair<QString, int> ParkingLotManager::biSearch(const QString &plate)
{
    sort(m_all_cars.begin(), m_all_cars.end(), &comp2);
    int left = 0, right = m_all_cars.size(), mid = m_all_cars.size() / 2;
    while (left != right) {
        int flag = comp(plate, m_all_cars.at(mid));
        if (flag == -1) {
            right = mid;
        } else if (flag == 1) {
            left = mid;
        } else if (flag == 0) {
            Car* c = m_all_cars.at(mid);
            if (c->getStatus() == Car::Status::parking ||
                    c->getStatus() == Car::Status::moving)
                return qMakePair(m_widgets.at(c->getTargetFloor())->getName(),
                                 c->getNum());
            else if (c->getStatus() == Car::Status::waiting)
                return qMakePair(QString("0"), 0);
        }
        mid = (right + left) / 2;
    }
    return qMakePair(QString("-1"), -1);
}

void ParkingLotManager::showMode(bool enable)
{
    if (enable)
        timer->start(1000);
    else
        timer->stop();
}

void ParkingLotManager::requestOut(Car *car, int exit)
{
    if (exit == -1)
        exit = rand() % m_num_of_entry + 1;
    Path *p;
    if (car->getCurrentFloor() == 1)
        p = m_graph[1]->findPath(NODE_TYPE::space, car->getNum(), NODE_TYPE::exit, exit);
    else
        p = m_graph[car->getCurrentFloor()]->
                findPath(NODE_TYPE::space, car->getNum(), NODE_TYPE::stair, exit);
    if (GET_SITUATION(car->getCurrentFloor(), car->getNum()-1) != BANNED)
        m_pool.append(qMakePair(car->getCurrentFloor(), car->getNum() - 1));
    m_widgets[car->getCurrentFloor()]->
            getSpaceList()[car->getNum() - 1]->setSituation(ParkingSpaceWidget::free);
    car->setNum(-exit);  // 负数代表出口
    car->setPath(p);
    car->setTargetFloor(1);
    car->setStatus(Car::moving);
    car->followPath();
}

void ParkingLotManager::requestStair(Car *car)
{
    int l = car->getTargetFloor();
    int n = car->getNum();
    int e = car->getEntryNum();
    Path *p;
    if (n > 0)
        p = m_graph[l]->findPath(NODE_TYPE::stair, e, NODE_TYPE::space, n);
    else
        p = m_graph[l]->findPath(NODE_TYPE::stair, e, NODE_TYPE::exit, -n);
    // drawPath(p);
    car->setPath(p);
    car->setCurrentFloor(l);
    if (l == static_cast<int>(m_current_floor))
        car->show();
    else
        car->hide();
    car->followPath();
}

void ParkingLotManager::requestBack(Car *car)
{
    Path* p = m_graph[1]->findPath(car, car->getEntryNum());
    car->setPath(p);
    car->followPath();
    m_all_cars.removeOne(car);
    m_waitting[car->getEntryNum() - 1].removeOne(car);
}

void ParkingLotManager::requestIn(Car* car)
{
    int entry = car->getEntryNum();
    car->requested = true;
    if (!m_pool.empty()) {
        uint l = m_pool.first().first;
        int n = m_pool.first().second + 1;
        if (m_widgets[l]->getSpaceList().at(n - 1)->getSituation() != FREE)
            return;
        // l = 0;
        m_pool.removeAt(0);
        Log::i(QString("分配第%1层%2号车位，剩余%3个空车位").arg(l).arg(n).arg(m_pool.size()));
        Path* p;
        if (l == 1)
            p = m_graph[1]->findPath(NODE_TYPE::queueHead, entry, NODE_TYPE::space, n);
        else {
            p = m_graph[1]->findPath(NODE_TYPE::queueHead, entry, NODE_TYPE::stair, entry);
        }
        // drawPath(p);
        m_waitting[entry - 1].pop_front();  // 移除等候
        m_cars[l][n - 1] = car;
        car->setPath(p);
        car->setTargetFloor(l);
        car->setNum(n);
        car->followPath();
        car->setStartTime(QTime::currentTime());
        car->setStatus(Car::parking);
        m_num_of_cars[l] += 1;
        m_widgets[l]->getSpaceList()[n - 1]->setSituation(ParkingSpaceWidget::occupied);
        qDebug()<<(QString("LINE:%1  %2").arg(__LINE__).arg(m_current_floor));
        emit setLoad(QString::number(m_capacity.at(m_current_floor) - m_num_of_cars[m_current_floor]));
    } else {
        Log::i("请求失败，车位已满");
    }
}

void ParkingLotManager::leave(Car* car)
{
    int l = car->getCurrentFloor();
    Log::i(QString("有车离开，已产生费用%1元，当前共有%2个空车位").arg(car->getFee()).arg(m_pool.size()));
    emit money(car->getFee().toDouble());
    callIn();
    car->hide();
    m_all_cars.removeOne(car);
    delete car;
    m_num_of_cars[l] -= 1;
    qDebug()<<(QString("LINE:%1").arg(__LINE__));
    emit setLoad(QString::number(m_capacity.at(m_current_floor) - m_num_of_cars[m_current_floor]));
}

void ParkingLotManager::addCarR()
{
    int entry = (rand() % m_num_of_entry);
    if (m_waitting[entry].size() > 10)
        return;

    Car *car = new Car(this);
    Log::i(QString("生成车:%1").arg(car->getPlateNumber()));
    car->setEntryNum(entry + 1);
    m_waitting[entry].append(car);
    Path *p = m_graph[1]->findPath(NODE_TYPE::entry, entry + 1, NODE_TYPE::queueHead, entry + 1);
    car->setPath(p);
    car->setCurrentFloor(1);  // 刚添加的车一定是在一层
    car->setTargetFloor(1);
    car->followPath();
    car->setStatus(Car::waiting);
    m_all_cars.append(car);
    m_scene->addItem(car);
    if (m_current_floor != 1)
        car->hide();
}

void ParkingLotManager::addCar(QString plate, int color, int entry)
{
    if (entry == -1)
        entry = (rand() % m_num_of_entry) + 1;
    if (m_waitting[entry - 1].size() > 10)
        return;
    Car *car = new Car(this, nullptr, 0, static_cast<Car::Color>(color));
    car->setPlateNumber(plate);
    car->setEntryNum(entry);
    m_waitting[entry-1].append(car);
    Path *p = m_graph[1]->findPath(NODE_TYPE::entry, entry, NODE_TYPE::queueHead, entry);
    car->setPath(p);
    car->setCurrentFloor(1);  // 刚添加的车一定是在一层
    car->setTargetFloor(1);
    car->followPath();
    car->setStatus(Car::waiting);
    m_all_cars.append(car);
    m_scene->addItem(car);
    Log::i(QString("生成车:%1").arg(car->getPlateNumber()));
    if (m_current_floor != 1)
        car->hide();
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

void ParkingLotManager::banSpace(bool banned, int l , int n)
{
    if (banned) {
        m_pool.removeOne(qMakePair(l, n - 1));
        Log::i(QString("禁用第%1层%2号车位，剩余%3个空车位").arg(l).arg(n).arg(m_pool.size()));
    } else {
        m_pool.append(qMakePair(l, n - 1));
        Log::i(QString("启用第%1层%2号车位，剩余%3个空车位").arg(l).arg(n).arg(m_pool.size()));
        callIn();
    }
}

void ParkingLotManager::generatePool(bool sequence, int max)
{
    m_pool.clear();
    if (max == -1)
        max = 65536;
    for (int l = m_num_of_layer - 1; l >= 0; l--) {
        for (int n = 0; n < m_capacity[l]; n++) {
            if (max-- == 0)
                goto OUT;
            if (GET_SITUATION(l, n) == FREE) {
                m_pool.append(qMakePair(l, n));
            }
        }
    }
OUT:
    if (!sequence)
        std::random_shuffle(m_pool.begin(), m_pool.end());
    Log::i(QString("生成车位池成功，当前共有") + QString::number(m_pool.size()) + "个空车位");
}

void ParkingLotManager::periodWork()
{
    int generateProbability = 80;
    if (rand() % 100 + 1 < generateProbability)
        addCarR();
    for (auto c: m_all_cars) {
        c->leaveProbability(2);
    }
    callIn();
}

void ParkingLotManager::callIn()
{
    for (uint i = 0 ; i < m_num_of_entry; i++) {
        if (m_waitting[lastInEntry].size() != 0) {
            Log::i(QString("从%1召唤车").arg(lastInEntry));
            Car *c = m_waitting[lastInEntry][0];
            if (c->requested)
                c->requestSpace();
        }
        lastInEntry = (lastInEntry + 1) % m_num_of_entry ;
    }
}

double ParkingLotManager::getYellow_price() const
{
    return yellow_price;
}

void ParkingLotManager::setYellow_price(double value)
{
    yellow_price = value;
}

double ParkingLotManager::getRed_price() const
{
    return red_price;
}

void ParkingLotManager::setRed_price(double value)
{
    red_price = value;
}

double ParkingLotManager::getPink_price() const
{
    return pink_price;
}

void ParkingLotManager::setPink_price(double value)
{
    pink_price = value;
}

void ParkingLotManager::setMax(int max)
{
    int n = m_max;
    int mmax = max;
    m_max = max;
    Log::i(QString("设置最大容量%1").arg(max));
    for (int l = m_num_of_layer - 1; l >= 0; l--) {
        for (int n = 0; n < m_capacity[l]; n++) {
            if (max > 0) {
                if (GET_SITUATION(l, n) != OCCUPIED) {
                    m_widgets.at(l)->getSpaceList().at(n)->setSituation(FREE);
                    m_widgets.at(l)->getSpaceList().at(n)->update();
                }
                max--;
            }
            else {
                m_widgets.at(l)->getSpaceList().at(n)->setSituation(BANNED);
                m_widgets.at(l)->getSpaceList().at(n)->update();
            }
        }
    }
    generatePool(m_sequence, mmax);
    if (max - n == 1)
        callIn();
    else if (max - n >= 2) {
        callIn();
        callIn();
    }
}

void ParkingLotManager::setSequence(bool sequence)
{
    m_sequence = sequence;
    generatePool(m_sequence, m_max);
}
