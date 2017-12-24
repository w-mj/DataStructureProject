#ifndef CARLIST_H
#define CARLIST_H

#include <QDialog>
#include <QTime>
#include <QTimer>
#include <QList>
#include <QRunnable>
#include <QAbstractTableModel>

namespace Ui {
class CarList;
}

struct FakeCar {
    QString plateNum;
    QTime startTime;
    QString pos;
    int type;
    static int n;
    FakeCar();
};

class Adapter: public QAbstractTableModel {
public:
    explicit Adapter(QObject* parent, QList<FakeCar>* list);
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    void update();
private:
    QStringList header;
    QList<FakeCar>* m_list;
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
signals:
    void onCloseButtonClicked(void);

private:
    static CarList* m_instance;
    explicit CarList(QWidget *parent = 0);
    QList<FakeCar>* m_adapter;
    Ui::CarList *ui;
};

#endif // CARLIST_H
