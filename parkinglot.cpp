#include "parkinglot.h"
#include <QDomDocument>
#include <QFile>
#include <QMessageBox>
#include <QPainter>
#include <QWidget>
#include <QDebug>
#include <algorithm>

ParkingLot::ParkingLot(const QString &fname, QWidget* parent)
{
    map = new QPixmap(1000, 1000);
    this->parent = parent;
    QFile file(fname);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::critical(parent, QObject::tr("Error"),
                             QObject::tr("Cannot read file %1").arg(fname));
    }

    QString errorStr;
    int errorLine;
    int errorColumn;
    doc = new QDomDocument;
    if (!doc -> setContent(&file, false, &errorStr, &errorLine, &errorColumn)) {
    QMessageBox::critical(parent, QObject::tr("Error"),
                          QObject::tr("Parse error at line %1, column %2: %3")
                          .arg(errorLine).arg(errorColumn).arg(errorStr));
    }
    // map->resize(1000, 1000);
    // painter = new QPainter(map);
}

QPixmap ParkingLot::creatMap2()
{
    QDomElement root =doc -> documentElement();
    if (root.tagName() != "parkinglot") {
        QMessageBox::critical(parent, QObject::tr("Error"),
                              QObject::tr("Not a parkinglot file"));
    }
    qDebug() << "!!!!";
    return parseLayout(root.firstChild().toElement());
}

QPixmap *ParkingLot::getPixmap()
{
    return map;
}

ParkingLot::~ParkingLot()
{
    // delete painter;
    delete doc;
    delete map;
}

QPixmap drawRoad(const QDomElement& roadXML) {
    QPixmap result(800, 600);
    if (roadXML.tagName() != "road") {
        qDebug() << "XML is not a road";
        return result.copy(0, 0, 0, 0);
    }
    int x = 0, y = 0;
    QPainter painter(&result);
    painter.setBrush(Qt::white);
    painter.setPen(Qt::white);
    enum {vertical, horizontal} direction;
    const int length = roadXML.attribute("length").toInt();
    qDebug() << "length:" << length;
    if (roadXML.attribute("direction") == "horizontal")
        direction = horizontal;
    else
        direction = vertical;
    if (direction == horizontal) {
        while (x < length) {
            painter.drawRect(x + 5, 20, 40, 10);
            x += 50;
        }
    } else {
        while (y < length) {
            painter.drawRect(20, y + 5, 10, 40);
            y += 50;
        }
    }
    if (direction == horizontal)
        return result.copy(0, 0, length, 50);
    else
        return result.copy(0, 0, 50, length);
}

QPixmap drawRoom(const QDomElement& roomXML) {
    QPixmap result(800, 600);
    if (roomXML.tagName() != "room") {
        qDebug() << "XML节点不是车位";
        return result.copy(0, 0, 0, 0);
    }
    QPixmap temp(60, 40);
    QPainter painter(&result), tPainter(&temp);
    painter.setPen(Qt::white);
    painter.setBrush(Qt::white);
    tPainter.setPen(Qt::white);
    tPainter.setBrush(Qt::white);

    ushort direction = roomXML.attribute("direction").at(0).unicode();
    int n = roomXML.attribute("number").toInt();
    int x = 0, y = 0;
    enum {down, right} expendDirection;
    if (roomXML.attribute("expendDirection") == "down")
        expendDirection = down;
    else
        expendDirection = right;
    tPainter.drawRect(0, 0, 60, 5);
    tPainter.drawRect(0, 5, 10, 35);
    tPainter.drawRect(10, 35, 50, 5);
    tPainter.end();
    switch(direction) {
    case 'E':
        // temp = temp;
        break;
    case 'S':
        temp = temp.transformed(QMatrix().rotate(270));
        break;
    case 'W':
        temp = temp.transformed(QMatrix().rotate(180));
        break;
    case 'N':
        temp = temp.transformed(QMatrix().rotate(90));
        break;
    default:
        qDebug() << "未知方向" << direction;
        break;
    }
    // tPainter.begin(&temp);
    for (int i = 0; i < n; i++) {
        painter.drawPixmap(x, y, temp.width(), temp.height(), temp);
        if (expendDirection == down)
            y += temp.height();
        else
            x += temp.width();
    }
    if (expendDirection == down)
        return result.copy(0, 0, temp.width(), y);
    else
        return result.copy(0, 0, temp.height(), x);
}

QPixmap ParkingLot::parseLayout(const QDomElement& layout)
{
    QPixmap result(2000, 2000), temp;
    QPainter painter(&result);
    painter.setPen(Qt::black);
    int x = 0, y = 0, maxX = 0, maxY = 0;
    enum {vertical, horizontal} direction;
    if (layout.tagName() == "vLayout")
        direction = vertical;
    else if (layout.tagName() == "hLayout")
        direction = horizontal;
    else {
        qDebug() << "错误的布局名称" << layout.tagName();
        return result;
    }

    QDomElement child = layout.firstChild().toElement();
    while(!child.isNull()) {
        QString name = child.tagName();
        if (name == "road") {
            temp = drawRoad(child);
        } else if (name == "room") {
            temp = drawRoom(child);
        } else if (name == "vLayout" || name == "hLayout")
            temp = parseLayout(child);
        painter.drawPixmap(x, y, temp.width(), temp.height(), temp);
        if (direction == horizontal) {
            x += temp.width();
            maxY = std::max(maxY, temp.height());
        } else {
            y += temp.height();
            maxX = std::max(maxX, temp.width());
        }
        child = child.nextSibling().toElement();
    }
    if (direction == horizontal) {
        return result.copy(0, 0, x, maxY);
    } else {
        return result.copy(0, 0, maxX, y);
    }
}
