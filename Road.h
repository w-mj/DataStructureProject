#pragma once
#include <QWidget>
#include "ParkingLotWidget.h"

class ParkingLotWidget;

class Road: public QWidget
{
	Q_OBJECT
public:
	enum direction{ horizontal, vertical};
    Road(ParkingLotWidget* parent, const int length, direction dir);
	void paintEvent(QPaintEvent* event) override;
    uint getNumber() const;
    int getLength() const;
    direction getDir() const;
    ~Road();

private:
    const int length;
	direction dir;
    uint number;
};

