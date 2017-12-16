#include "ParkingSpaceWidget.h"
#include <QPainter>
#include <QDebug>
#include "ParkingLotWidget.h"

ParkingSpaceWidget::ParkingSpaceWidget(ParkingLotWidget* parent, const QString &dir): QWidget(parent)
{
	direction d = {};
	const char c = dir.at(0).unicode();
	switch (c)
	{
	case 'N':d = N; break;
	case 'E':d = E; break;
	case 'W':d = W; break;
	case 'S':d = S; break;
	default: qDebug() << this->objectName() << "错误的方向";
	}
	this->dir = d;
	switch (this->dir)
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
    number = parent->addSpace(this);
}

QBoxLayout *ParkingSpaceWidget::makeParkingSapceGroup(ParkingLotWidget* parent, const QString& dir, int n, const QString & expend)
{
	QBoxLayout *layout;
	if (expend == "down")
		layout = new QVBoxLayout();
	else
		layout = new QHBoxLayout();
	for (int i = 0; i < n; i++)
	{
		ParkingSpaceWidget *pk = new ParkingSpaceWidget(parent, dir);
		layout->addWidget(pk);
	}
	layout->addStretch();  // 居上或居左对齐
	layout->setMargin(0);
	layout->setSpacing(0);
	return layout;
}

void ParkingSpaceWidget::paintEvent(QPaintEvent * event)
{
	QPainter painter(this);
    painter.setPen(Qt::black);
    painter.drawText(20, 25, QString::number(number));
	painter.setPen(Qt::white);
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
	/*painter.setPen(Qt::blue);
	painter.setBrush(Qt::NoBrush);
	painter.drawRect(0, 0, this->width() - 1, this->height() - 1);*/
	painter.end();
}


ParkingSpaceWidget::~ParkingSpaceWidget()
{
}
