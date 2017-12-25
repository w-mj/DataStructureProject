#ifndef CARLIST_H
#define CARLIST_H

#include <QDialog>
#include <QTime>
#include <QTimer>
#include <QList>
#include <QRunnable>
#include <QAbstractTableModel>
#include "car.h"

namespace Ui {
class CarList;
}

class Adapter: public QAbstractTableModel {
public:
    explicit Adapter(QObject* parent, QList<Car*>* list);
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QList<Car*>* getCarList();
    void update();
private:
    QStringList header;
    QList<Car*>* m_list;
    QTimer *timer;
};

class CarList : public QDialog
{
    Q_OBJECT

public:
    ~CarList();
    void setAdapter(Adapter* adapter);
    static CarList* newInstance(QWidget* parent);
    static CarList* getInstance();
    void closeEvent(QCloseEvent *e);
    void mouseEvent(const QModelIndex& index);
signals:
    void onCloseButtonClicked(void);

private:
    static CarList* m_instance;
    explicit CarList(QWidget *parent = 0);
    QList<Car*>* m_carList;
    Ui::CarList *ui;
};

#endif // CARLIST_H
