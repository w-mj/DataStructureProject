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
    int getNumber() const;

protected:
    void paintEvent(QPaintEvent* ) override;
private:
    bool m_showMargin = false;
    int number;
	direction dir;
    // QLabel *m_float_window = nullptr;
};

#endif
