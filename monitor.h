#ifndef MONITOR_H
#define MONITOR_H

#include <QWidget>

class Monitor : public QWidget
{
    Q_OBJECT
public:
    explicit Monitor(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *);

signals:

public slots:
};

#endif // MONITOR_H
