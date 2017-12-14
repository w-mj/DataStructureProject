#pragma once
#include <QWidget>
class Road: public QWidget
{
	Q_OBJECT
public:
	enum direction{ horizontal, vertical};
	Road(QWidget* parent, const int length, direction dir);
	void paintEvent(QPaintEvent* event) override;
	~Road();
private:
	const int length;
	direction dir;
};

