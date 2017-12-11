#ifndef CARS_H
#define CARS_H

#include <QWidget>
#include <QPixmap>
#include <QString>

class Cars : public QWidget
{
    Q_OBJECT
private:
    enum Color m_color;
    QPixmap m_pixmap;
    QString m_number;
public:
    enum Color { Pink, Red, Yellow }
    explicit Cars(QWidget *parent = nullptr);
    explicit Cars(Color color, QWidget *parenet = nullptr);
signals:

public slots:
};

#endif // CARS_H
