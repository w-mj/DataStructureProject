#pragma once
#include <QWidget>
#include <QString>
#include <QVBoxLayout>
#include <QDomDocument>
class ParkingLotWidget: public QWidget
{
	Q_OBJECT
public:
	explicit ParkingLotWidget(QWidget* parent, const QString& xml);
	void paintEvent(QPaintEvent* event) override;
	~ParkingLotWidget();
private:
	void parse_xml(const QString& xml);
	QBoxLayout *parseLayout(const QDomElement& element);
	QBoxLayout *layout = nullptr;
};

