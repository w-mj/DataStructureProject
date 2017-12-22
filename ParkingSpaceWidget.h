#ifndef PARKING_SPACE_WIDGET
#define PARKING_SPACE_WIDGET
#include <QWidget>
#include <QLayout>
#include <QLabel>
#include "ParkingLotWidget.h"
class ParkingLotWidget;

class ParkingSpaceWidget: public QWidget
{
	Q_OBJECT
public:
	enum direction{N, S, E, W};
    ParkingSpaceWidget(ParkingLotWidget* parent, const QString &dir);
    static QBoxLayout*  makeParkingSapceGroup(ParkingLotWidget* parent, const QString& dir, int n, const QString &expend);
	~ParkingSpaceWidget();
    direction getDir() const;
    void mouseDoubleClickEvent(QMouseEvent *event);
    int getNumber() const;

protected:
    void paintEvent(QPaintEvent* ) override;
private:
    bool m_showMargin = false;
    int number;
	direction dir;
    bool m_banned = false;
};

#endif
