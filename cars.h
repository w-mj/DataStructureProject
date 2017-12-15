#ifndef CARS_H
#define CARS_H

#include <QWidget>
#include <QPixmap>
#include <QString>
#include <QPaintEvent>

class Cars : public QWidget
{
    Q_OBJECT
public:
    enum Color { Pink, Red, Yellow };
    explicit Cars(QWidget *parent = nullptr);   //Ëæ»úÑÕÉ«
    explicit Cars(Color color, QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event);
private:
    Color m_color;
    QPixmap m_pixmap;
    QString m_number;
signals:

public slots:
};

#endif // CARS_H
