#include "Road.h"
#include <QPainter>

Road::Road(QWidget* parent, const int length, const direction dir)
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
}

void Road::paintEvent(QPaintEvent * event)
{
	QPainter painter(this);
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
}

Road::~Road()
{
}
