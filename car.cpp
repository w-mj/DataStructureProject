#include "car.h"
#include "logwindow.h"
#include <QTimer>
#include <QDebug>
#include <QString>

QString reaomPlate() {
    QString province = "京津冀晋蒙辽吉黑沪苏浙皖闽赣鲁豫鄂湘粤桂琼川贵云渝藏陕甘青宁新港澳台";
    QString result;
    result.append(province.at(rand() % province.size()));
    result.append('A' + (rand() % 26));
    result.append('0' + (rand() % 10));
    result.append('0' + (rand() % 10));
    result.append('0' + (rand() % 10));
    result.append('0' + (rand() % 10));
    return result;
}


Car::Car(ParkingLotManager * manager, QGraphicsItem *parent, int dir, Car::Color color) :
    QObject(0),
    QGraphicsPixmapItem(parent),
    posAni(new QPropertyAnimation(this,"m_pos")),
    m_target(PathPoint(this->pos(),this->rotation())),
    m_current(PathPoint(this->pos(),this->rotation())),
    crash(true)
{
    m_manager = manager;
    m_pos = this->pos();
    this->setRotation(dir);
    if(color==RANDOM)
        color = Car::Color(rand()%3);  //车身颜色一定是确定的
    m_color = color;
    switch (m_color) {
    case Pink:
        m_pic = QPixmap(":/cars/pink");
        break;
    case Red:
        m_pic = QPixmap(":/cars/red");
        break;
    case Yellow:
        m_pic = QPixmap(":/cars/yellow");
        break;
    default:
        m_pic = QPixmap(":/cars/red");
        break;
    }
    m_pic = m_pic.scaled(M_WID, M_LEN);
    this->setPixmap(m_pic);
    setTransformOriginPoint(M_WID/2, M_LEN/2);  //设置旋转中心
    connect(posAni,QPropertyAnimation::finished, this, followPath);
    connect(this, &Car::queueHead, manager, &ParkingLotManager::requestIn);
    connect(this, &Car::exit, manager, &ParkingLotManager::leave);
    connect(this, &Car::stair, manager, &ParkingLotManager::requestStair);
    connect(this, &Car::out, manager, &ParkingLotManager::requestOut);
    connect(this, &Car::back, manager, &ParkingLotManager::requestBack);
    // connect(this, &Car::entry, manager, &ParkingLotManager::leave);
    m_number = reaomPlate();

}

void Car::Forward(qreal vel)
{
    qreal ang = qDegreesToRadians(rotation());
    this->moveBy(+vel*qSin(ang), -vel*qCos(ang));
}

void Car::Backward(qreal vel)
{
    qreal ang = qDegreesToRadians(rotation());
    this->moveBy(-vel*qSin(ang), +vel*qCos(ang));
}

void Car::moveLeft(qreal vel)
{
    qreal ang = qDegreesToRadians(rotation());
    moveBy(-vel*qCos(ang), -vel*qSin(ang));
}

void Car::moveRight(qreal vel)
{
    qreal ang = qDegreesToRadians(rotation());
    moveBy(vel*qCos(ang), vel*qSin(ang));
}

//ang为旋转角度（弧度），正数为顺时针，负数逆时针
void Car::Rotate(qreal ang)
{
    setRotation(rotation() + ang);
}

//r为弯道半径，ang为转过的角度
void Car::turnLeft(int r, double ang)
{
    qreal rad = qDegreesToRadians(ang);
    Forward(r*qSin(rad));
    moveLeft(r*(1-qCos(rad)));
    Rotate(-ang);
}

//r为弯道半径，ang为转过的角度
void Car::turnRight(int r, double ang)
{
    qreal rad = qDegreesToRadians(ang);
    Forward(r*qSin(rad));
    moveRight(r*(1-qCos(rad)));
    Rotate(ang);
}

void Car::setPath(Path *path)
{
    m_path = path;
    m_target = path->getNext();
    m_target.point = m_target.point - QPoint(20, 30);
    m_current = m_target;
    m_target.action = Road::none;
    this->setPos(m_target.point);
    this->setRotation(m_target.dir+90);
}

int Car::getNum()
{
    return num;
}

int Car::getEntryNum()
{
    return entryNum;
}

void Car::moveTo(QPointF target)
{
    qreal dx = m_current.point.x()-m_target.point.x();
    qreal dy = m_current.point.y()-m_target.point.y();
    qreal dis = qSqrt(qPow(dx,2)+qPow(dy,2));

    qreal dur = dis/0.2;

    posAni->setDuration(dur);
    posAni->setEndValue(target);
    posAni->start();
}

void Car::followPath()
{
    setRotation(m_target.dir+90);
    switch (m_target.action)
    {
    case Road::entry :
        Log::i("发送entry信号");
        emit entry(this);
        break;
    case Road::stair:
        Log::i("发送楼梯信号");
        emit stair(this);
        break;
    case Road::exit:
        Log::i("发送离开信号");
        emit exit(this);
        break;
    case Road::queueHead:
        Log::i("发送请求车位信号");
        emit queueHead(this);
        crash=false;
        // emit back(this);
        break;
    case Road::none:
        break;
    }
    if(m_path && !m_path->isEmpty())
    {
        m_current = m_target;
        m_target = m_path->getNext();
        m_target.point = m_target.point - QPoint(20, 30);
        moveTo(m_target.point);
    } else {
        delete m_path;
        m_path = nullptr;
    }
}

QPointF Car::getmPos()
{
    return this->pos();
}

void Car::setmPos(QPointF pos)
{
    this->setPos(pos);
    this->m_pos = pos;
}

Car::Color Car::getColor()
{
    return this->m_color;
}

qreal Car::getDir()
{
    return this->rotation();
}

QString Car::getPosition() const
{
    if (m_status == moving) {
        return "正在移动";
    } else if (m_status == waiting) {
        return "正在等候";
    } else  {
        return QString("%1%2号").arg(m_manager->getParkingLotName(targetFloor)).arg(num);
    }
}

QString Car::getPlateNumber() const
{
    return m_number;
}

QString Car::getFee() const
{
    if (m_status == waiting)
        return "0";
    int elapsed = startTime.msecsTo(QTime::currentTime());
    switch (m_color) {
    case Color::Pink:
        elapsed *= 1;
        break;
    case Color::Red:
        elapsed *= 2;
        break;
    case Color::Yellow:
        elapsed *= 3;
        break;
    default:
        break;
    }
    return QString::number(elapsed / 1000);
}

Car::Status Car::getStatus() const
{
    return m_status;
}

void Car::setStatus(const Status &value)
{
    m_status = value;
}

QTime Car::getStartTime() const
{
    return startTime;
}

void Car::setStartTime(const QTime &value)
{
    startTime = value;
}


void Car::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(crash)
    {
        QList<QGraphicsItem *> items = collidingItems();
        if(!items.isEmpty())
        {
            for(auto item:items)
            {
                if(item->type()==Car::Type)
                {
                    Car *car = qgraphicsitem_cast<Car*>(item);
                    if (car->getStatus() == Car::parking)
                        continue;
                    if(qAbs(car->getDir()-270)<20&&qAbs(this->getDir()-270)<20&&car->getmPos().x()<this->getmPos().x())
                    {
                        posAni->stop();
                        break;
                    }
                    else if(qAbs(car->getDir()-90)<20&&qAbs(this->getDir()-90)<20&&car->getmPos().x()>this->getmPos().x())
                    {
                        posAni->stop();
                        break;
                    }
                }
                if(this->posAni->currentTime()!=this->posAni->totalDuration())
                    posAni->start();
            }
        }
        else
        {
            if(this->posAni->currentTime()!=this->posAni->totalDuration())
                this->posAni->start();
        }
    }
    QGraphicsPixmapItem::paint(painter, option, widget);
}

int Car::type() const
{
    return Type;
}
void Car::leaveProbability(int p)
{
    if (m_status == parking) {
        if (rand() % 100 + 1 < p)
            emit out(this, -1);
    }
    else if (m_status == waiting)
        if (rand() % 100 + 1 < 10) {
            m_status = parking;
            emit back(this);
        }
}


void Car::setPlateNumber(const QString &number) {
    this->m_number = number;
}

void Car::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_status==parking&&event->button()==Qt::LeftButton)
        emit out(this, -1);
    else if (m_status == waiting && event->button() == Qt::LeftButton)
        emit back(this);
    event->accept();
}

void Car::requestSpace()
{
    emit queueHead(this);
}

void Car::go()
{
    if (m_status == parking)
        emit out(this, -1);
    else if (m_status == waiting) {
        m_status = parking;
        emit back(this);
    }
}

int Car::getTargetFloor() const
{
    return targetFloor;
}

void Car::setTargetFloor(int value)
{
    targetFloor = value;
}

int Car::getCurrentFloor() const
{
    return currentFloor;
}

void Car::setCurrentFloor(int value)
{
    currentFloor = value;
}

void Car::setEntryNum(int value)
{
    entryNum = value;
}

void Car::setNum(int value)
{
    num = value;
}
