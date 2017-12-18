#include "boundary.h"
#include <QPainter>
#include <QEvent>

Boundary::Boundary(const QString &dir, int l, QWidget *parent): QWidget(parent), length(l)
{
    if (dir == "horizontal")
        vertical = false;
    else
        vertical = true;
    if (vertical) {
        resize(5, length);
        setMaximumSize(5, length);
        setMinimumSize(5, length);
    } else {
        resize(length, 5);
        setMaximumSize(length, 5);
        setMinimumSize(length, 5);
    }
}

void Boundary::paintEvent(QPaintEvent *) {
    QPainter p(this);
    p.setPen(Qt::black);
    p.setBrush(Qt::black);
    if (vertical) {
        p.drawRect(0, 0, 5, length);
    } else {
        p.drawRect(0, 0, length, 5);
    }
    p.setPen(Qt::red);
}
