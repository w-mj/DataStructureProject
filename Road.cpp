#include "Road.h"
#include <QPainter>
#include <QPaintEvent>


Road::Road(ParkingLotWidget* parent, const int length, const direction dir)
		:QWidget(parent), length(length), dir(dir)
{
	switch(dir)
	{
	case horizontal:
		resize(length, 50);
		setMinimumSize(length, 50);
		setMaximumSize(length, 50);
		break;
	case vertical:
		resize(50, length);
		setMinimumSize(50, length);
		setMaximumSize(50, length);
		break;
	}
	// setContentsMargins(0, 0, 0, 0);
    number = parent->addRoad(this);
}

void Road::paintEvent(QPaintEvent * event)
{
    event->ignore();
	QPainter painter(this);
	/*painter.setPen(Qt::blue);
	painter.drawRect(0, 0, width() - 1, height() - 1);*/
	painter.setBrush(Qt::white);
	painter.setPen(Qt::white);
	auto x = 0, y = 0;
	if (dir == horizontal) {
		while (x < length) {
			painter.drawRect(x + 5, 20, 40, 10);
			x += 50;
		}
	}
	else {
		while (y < length) {
			painter.drawRect(20, y + 5, 10, 40);
			y += 50;
		}
	}
    //painter.setPen(Qt::red);
    //painter.setBrush(Qt::NoBrush);
    //painter.drawRect(0, 0, width() - 1, height() - 1);
}

Road::~Road()
{
}

uint Road::getNumber() const
{
    return number;
}

int Road::getLength() const
{
    return length;
}

Road::direction Road::getDir() const
{
    return dir;
}
