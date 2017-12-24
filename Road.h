#pragma once
#include <QWidget>
#include "ParkingLotWidget.h"

class ParkingLotWidget;

class Road: public QWidget
{
	Q_OBJECT
public:
	enum direction{ horizontal, vertical};
    enum Action{entry = 1, stair, exit, queueHead, none};
    Road(ParkingLotWidget* parent, const int length, direction dir);
	void paintEvent(QPaintEvent* event) override;
    uint getNumber() const;
    int getLength() const;
    direction getDir() const;
    ~Road();

    Action getAction() const;
    void setAction(const Action &value);
    short getActionPos() const;
    void setActionPos(short value);

private:
    const int length;
    direction dir;
    uint number;
    Action action = none;
    short actionPos = 0;  // 1为左上端，2为右下端
    bool m_showMargin = false;
};

