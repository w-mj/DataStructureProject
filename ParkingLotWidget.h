#ifndef PARKING_LOT_WIDGET
#define PARKING_LOT_WIDGET
#include <QWidget>
#include <QString>
#include <QVBoxLayout>
#include <QDomDocument>
#include <vector>
#include "ParkingSpaceWidget.h"
class ParkingSpaceWidget;  // 先声明类，防止循环include

class ParkingLotWidget: public QWidget
{
	Q_OBJECT
public:
	explicit ParkingLotWidget(QWidget* parent, const QString& xml);
    int addSpace(ParkingSpaceWidget* sp);
	~ParkingLotWidget();
    QString getName() const;

protected:
    void paintEvent(QPaintEvent* event) override;
private:
    std::vector<ParkingSpaceWidget*> m_spaceList;
    void parse_xml(const QString& xml);
	QBoxLayout *parseLayout(const QDomElement& element);
	QBoxLayout *layout = nullptr;
    QString name;
};

#endif
