#ifndef PARKING_LOT_WIDGET
#define PARKING_LOT_WIDGET
#include <QWidget>
#include <QString>
#include <QVBoxLayout>
#include <QDomDocument>
#include <QVector>
#include "Road.h"
#include "ParkingSpaceWidget.h"
class ParkingSpaceWidget;  // 先声明类，防止循环include
class Road;

class ParkingLotWidget: public QWidget
{
	Q_OBJECT
public:
	explicit ParkingLotWidget(QWidget* parent, const QString& xml);
    int addSpace(ParkingSpaceWidget* sp);
    int addRoad(Road* r);
    int getCapacity() {return m_spaceList.size(); }
	~ParkingLotWidget();
    QString getName() const;
    const QVector<ParkingSpaceWidget *>& getSpaceList() const;
    const QVector<Road *>& getRoadList() const;
    void showMarginSlot(bool b);

private:
    QVector<ParkingSpaceWidget*> m_spaceList;
    QVector<Road*> m_roadList;
    void parse_xml(const QString& xml);
	QBoxLayout *parseLayout(const QDomElement& element);
	QBoxLayout *layout = nullptr;
    QString name;

signals:
    void showMargain(bool);
};

#endif
