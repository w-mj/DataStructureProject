#ifndef BOUNDARY_H
#define BOUNDARY_H

#include <QWidget>
#include <QString>

class Boundary : public QWidget
{
    Q_OBJECT
public:
    explicit Boundary(const QString& dir, int length, QWidget *parent = nullptr);

private:
    bool vertical;
    int length;
protected:
    void paintEvent(QPaintEvent *);
};

#endif // BOUNDARY_H
