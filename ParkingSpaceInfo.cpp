#include "ParkingSpaceInfo.h"
#include <QPainter>
#include <QDebug>

ParkingSpaceInfo::ParkingSpaceInfo(QWidget* parent, const direction dir) : QWidget(parent) 
{
	this->dir = dir;
	switch (dir)
	{
	case N:
	case S:
		resize(40, 60);
		setMinimumSize(40, 60);
		setMaximumSize(40, 60);
		break;
	case E:
	case W:
		resize(60, 40);
		setMinimumSize(60, 40);
		setMaximumSize(60, 40);
		break;
	}
}
ParkingSpaceInfo::ParkingSpaceInfo(QWidget* parent, const QString &dir)
{
	direction d = {};
	const char c = dir.at(0).unicode();
	switch (c)
	{
	case 'N':d = N; break;
	case 'E':d = E; break;
	case 'W':d = W; break;
	case 'S':d = S; break;
	default: qDebug() << this->objectName() << "´íÎóµÄ·½Ïò";
	}
	this->ParkingSpaceInfo::ParkingSpaceInfo(parent, d);
}

QBoxLayout *ParkingSpaceInfo::makeParkingSapceGroup(QWidget* parent, const QString& dir, int n, const QString & expend)
{
	QBoxLayout *layout;
	if (expend == "down")
		layout = new QVBoxLayout();
	else
		layout = new QHBoxLayout();
	for (int i = 0; i < n; i++)
	{
		ParkingSpaceInfo *pk = new ParkingSpaceInfo(parent, dir);
		layout->addWidget(pk);
	}
	layout->setMargin(0);
	layout->setSpacing(0);
	layout->setContentsMargins(0, 0, 0, 0);
	return layout;
}

void ParkingSpaceInfo::paintEvent(QPaintEvent * event)
{
	QPainter painter(this);
	painter.setPen(Qt::blue);
	// painter.drawRect(0, 0, this->width(), this->height());
	painter.setPen(Qt::red);
	painter.setBrush(Qt::white);
	painter.translate(30, 20);
	switch (dir) {
	case E:
		break;
	case S:
		painter.rotate(270);
		break;
	case W:
		painter.rotate(180);
		break;
	case N:
		painter.rotate(90);
		break;
	}
	painter.translate(-30, -20);
	painter.drawRect(0, 0, 60, 5);
	painter.drawRect(0, 5, 10, 35);
	painter.drawRect(10, 35, 50, 5);
	painter.end();
}


ParkingSpaceInfo::~ParkingSpaceInfo()
{
}
