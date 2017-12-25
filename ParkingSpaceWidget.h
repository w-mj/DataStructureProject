#ifndef PARKING_SPACE_WIDGET
#define PARKING_SPACE_WIDGET
#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <QMouseEvent>
#include "ParkingLotWidget.h"
class ParkingLotWidget;

class ParkingSpaceWidget: public QWidget
{
	Q_OBJECT
public:
	enum direction{N, S, E, W};
    enum Situation{banned, occupied, free};
    ParkingSpaceWidget(ParkingLotWidget* parent, const QString &dir);
    static QBoxLayout*  makeParkingSapceGroup(ParkingLotWidget* parent, const QString& dir, int n, const QString &expend);
	~ParkingSpaceWidget();
    direction getDir() const;
    void mouseDoubleClickEvent(QMouseEvent *event);
    int getNumber() const;

    Situation getSituation() const;
    void setSituation(const Situation &value);

protected:
    void paintEvent(QPaintEvent* ) override;
private:
    bool m_showMargin = false;
    int number;
	direction dir;
    Situation m_situation = free;

signals:
    void ban(bool, int);
};

#endif
