#include "ParkingLotWidget.h"
#include "ParkingSpaceWidget.h"
#include "boundary.h"
#include <QFile>
#include <QMessageBox>
#include <QDomDocument>
#include <QDebug>
#include "Road.h"
#include <QPainter>
#include <QPaintEvent>

ParkingLotWidget::ParkingLotWidget(QWidget *parent, const QString& xml) : QWidget(parent)
{
	parse_xml(xml);
    // resize(783, 879);
    // resize(640, 480);
    setLayout(layout);
    adjustSize();  // 调整大小至刚刚可以装下所有元素
}


int ParkingLotWidget::addSpace(ParkingSpaceWidget *sp)
{
    m_spaceList.push_back(sp);
    return m_spaceList.size();
}

int ParkingLotWidget::addRoad(Road *r)
{
    m_roadList.push_back(r);
    return m_roadList.size();
}

ParkingLotWidget::~ParkingLotWidget()
{
}

void ParkingLotWidget::parse_xml(const QString & xml)
{
	QFile file(xml);
	if (!file.open(QFile::ReadOnly | QFile::Text)) {
		QMessageBox::critical(this, QObject::tr("Error"),
			QObject::tr("Cannot read file %1").arg(xml));
        return;
	}

	QString errorStr;
	int errorLine;
	int errorColumn;
	QDomDocument doc;
	if (!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn)) {
		QMessageBox::critical(this, QObject::tr("Error"),
			QObject::tr("Parse error at line %1, column %2: %3")
			.arg(errorLine).arg(errorColumn).arg(errorStr));
        return;
	}
    QDomElement element = doc.documentElement();
    if (element.tagName() != "parkinglot") {
        qDebug() << this -> objectName() << "布局文件错误: 根标签不是parkinglot";
        return;
    }
    this->name = element.attribute("name");
    QDomElement childElement = element.firstChildElement();
    QString name = childElement.tagName();
    if (childElement.tagName().contains("Layout"))
	{
        layout = parseLayout(childElement);
	} else
	{
        qDebug() << this -> objectName() << "布局文件错误: 没有布局标签";
        return;
	}
}

QBoxLayout * ParkingLotWidget::parseLayout(const QDomElement & element)
{
	QBoxLayout* layout;
	if (element.tagName() == "hLayout")
		layout = new QHBoxLayout;
	else
		layout = new QVBoxLayout;
    layout->setSizeConstraint(QLayout::SetFixedSize);
    // qDebug() << "处理" << element.tagName();
	
	QDomElement child = element.firstChild().toElement();
	while (!child.isNull())
	{
		QString name = child.tagName();
		if (name == "road")
		{
            // qDebug() << "处理road";
            Road *r;
			const int l = child.attribute("length").toInt();
			if (child.attribute("direction") == "vertical")
                r = new Road(this, l, Road::vertical);
			else
                r = new Road(this, l, Road::horizontal);
            QString action = child.attribute("action", "none");
            if (action == "entry")
                r->setAction(Road::Action::entry);
            else if (action == "stair")
                r->setAction(Road::Action::stair);
            else if (action == "exit")
                r->setAction(Road::Action::exit);
            else
                r->setAction(Road::Action::none);
            r->setActionPos(child.attribute("actionPos", "0").toShort());
            layout->addWidget(r);
		}
		else if (name == "room")
		{
            // qDebug() << "处理room";
			const auto n = child.attribute("number", "1").toInt();
			layout->addLayout(ParkingSpaceWidget::makeParkingSapceGroup(
				this, child.attribute("direction"), n, child.attribute("expendDirection")));
		}
		else if ((name.at(0) == 'v' ||  name.at(0) == 'h' )&& name.contains("Layout"))
			layout->addLayout(parseLayout(child));
        else if (name == "space") {
            // qDebug() << "处理space";
            QSpacerItem* sw = new QSpacerItem(0, 0);
            if (element.tagName() == "hLayout")
                sw->changeSize(child.attribute("length").toInt(), 0, QSizePolicy::Fixed, QSizePolicy::Minimum);
            else
                sw->changeSize(0, child.attribute("length").toInt(), QSizePolicy::Minimum, QSizePolicy::Fixed);
            layout->addSpacerItem(sw);
        } else if (name == "boundary") {
            // qDebug() << "处理boundary";
            int l = child.attribute("length").toInt();
            Boundary *b = new Boundary(child.attribute("direction"), l, this);
            layout->addWidget(b);
        }
        else
			qDebug() << this->objectName() << "错误的xml标签";
		child = child.nextSiblingElement();
	}
	layout->addStretch(); // 添加伸缩，实现居上或居左对齐
	layout->setMargin(0);
	layout->setSpacing(0);
	return layout;
}

QString ParkingLotWidget::getName() const
{
    return name;
}

const QVector<ParkingSpaceWidget *>& ParkingLotWidget::getSpaceList() const
{
    return m_spaceList;
}

const QVector<Road *>& ParkingLotWidget::getRoadList() const
{
    return m_roadList;
}

void ParkingLotWidget::showMarginSlot(bool b)
{
    // qDebug() << "Parkinglot Widget: show margin " << b;
    emit showMargain(b);
}
