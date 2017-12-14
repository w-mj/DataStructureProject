#pragma once
#include <QWidget>
#include <QLayout>
class ParkingSpaceInfo: public QWidget
{
	Q_OBJECT
public:
	enum direction{N, S, E, W};
	ParkingSpaceInfo(QWidget* parent, const QString &dir);
	static QBoxLayout*  makeParkingSapceGroup(QWidget* parent, const QString& dir, int n, const QString &expend);
	void paintEvent(QPaintEvent* event) override;
	~ParkingSpaceInfo();
private:
	direction dir;
};

