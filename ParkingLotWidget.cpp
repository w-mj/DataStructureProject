#include "ParkingLotWidget.h"
#include "ParkingSpaceWidget.h"
#include <QFile>
#include <QMessageBox>
#include <QDomDocument>
#include <QDebug>
#include "Road.h"
#include <QPainter>
#include <QPaintEvent>

ParkingLotWidget::ParkingLotWidget(QWidget *parent, const QString& xml) : QWidget(parent)
{
	resize(1000, 1000);
	parse_xml(xml);
	setLayout(layout);
}


void ParkingLotWidget::paintEvent(QPaintEvent * event)
{
    event->ignore();
}

int ParkingLotWidget::addSpace(ParkingSpaceWidget *sp)
{
    m_spaceList.push_back(sp);
    return m_spaceList.size();
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
	}

	QString errorStr;
	int errorLine;
	int errorColumn;
	QDomDocument doc;
	if (!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn)) {
		QMessageBox::critical(this, QObject::tr("Error"),
			QObject::tr("Parse error at line %1, column %2: %3")
			.arg(errorLine).arg(errorColumn).arg(errorStr));
	}
	QDomElement element = doc.documentElement().firstChild().toElement();
	QString name = element.tagName();
	if (element.tagName().contains("Layout"))
	{
		layout = parseLayout(element);
	} else
	{
		qDebug() << this -> objectName() << "布局文件错误";
	}
}

QBoxLayout * ParkingLotWidget::parseLayout(const QDomElement & element)
{
	QBoxLayout* layout;
	if (element.tagName() == "hLayout")
		layout = new QHBoxLayout;
	else
		layout = new QVBoxLayout;
	
	QDomElement child = element.firstChild().toElement();
	while (!child.isNull())
	{
		QString name = child.tagName();
		if (name == "road")
		{
			const int l = child.attribute("length").toInt();
			if (child.attribute("direction") == "vertical")
				layout->addWidget(new Road(this, l, Road::vertical));
			else
				layout->addWidget(new Road(this, l, Road::horizontal));
		}
		else if (name == "room")
		{
			const auto n = child.attribute("number", "1").toInt();
			layout->addLayout(ParkingSpaceWidget::makeParkingSapceGroup(
				this, child.attribute("direction"), n, child.attribute("expendDirection")));
		}
		else if ((name.at(0) == 'v' ||  name.at(0) == 'h' )&& name.contains("Layout"))
			layout->addLayout(parseLayout(child));
		else
			qDebug() << this->objectName() << "错误的xml标签";
		child = child.nextSiblingElement();
	}
	layout->addStretch(); // 添加伸缩，实现居上或居左对齐
	layout->setMargin(0);
	layout->setSpacing(0);
	return layout;
}
