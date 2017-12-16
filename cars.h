#ifndef CARS_H
#define CARS_H

#include <QWidget>
#include <QPixmap>
#include <QString>
#include <QPaintEvent>
#include <QPoint>

//车身长宽
#define M_LEN   (200)
#define M_WID   (130)
#define PI      (3.1415926f)

class Cars : public QWidget
{
    Q_OBJECT
public:
    enum Color { Pink, Red, Yellow, RANDOM };
    explicit Cars(QWidget *parent = nullptr, int dir = 0, Cars::Color color = RANDOM);
    void paintEvent(QPaintEvent *event);
    void Forward(int vel);
    void Backward(int vel);
private:
    Color m_color;      //车身颜色
    int m_dir;          //车头朝向 方向角 N为0， 顺时针递增
    QPixmap m_pixmap;
    QString m_number;
    QPoint  m_pos;

signals:

public slots:
};

#endif // CARS_H
