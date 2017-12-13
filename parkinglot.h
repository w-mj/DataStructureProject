#ifndef PARKINGLOT_H
#define PARKINGLOT_H
#include <QWidget>
#include <QString>
#include <QPainter>
#include <QBitmap>
#include <QDomDocument>
class ParkingLot
{
public:
    explicit ParkingLot(const QString& fname, QWidget *parent = nullptr);
    void creatMap();
    QPixmap creatMap2();
    QPixmap* getPixmap() const;
    ~ParkingLot();
private:
    bool vertical;
    QPixmap* map;
    QDomDocument* doc;
    QWidget* parent;
    QPixmap parseLayout(const QDomElement& layout) const;
};

#endif // PARKINGLOT_H
