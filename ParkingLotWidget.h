#pragma once
#include <QWidget>
#include <QString>
#include <QHBoxLayout>
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
	inline QHBoxLayout* parseHbox(const QDomElement& element);
	inline QVBoxLayout* parseVbox(const QDomElement& element);
	enum {horizontal, vertical} rootLayoutDirection;
	QBoxLayout *parseLayout(const QDomElement& element);
	QHBoxLayout *hBoxLayout = nullptr;
	QVBoxLayout *vBoxLayout = nullptr;
};

