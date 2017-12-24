#include "carlist.h"
#include "ui_carlist.h"
#include "QDebug"

int FakeCar::n = 0;
CarList* CarList::m_instance = nullptr;

CarList::CarList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CarList)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    setWindowTitle("车辆列表");
    setGeometry(parent->x() + parent->width(), y() + parent->height() / 2, 400, 450);
    hide();
}

CarList::~CarList()
{
    delete ui;
}
void CarList::setAdapter(Adapter *adapter)
{
    ui->tableView->setModel(adapter);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();
}

CarList *CarList::newInstance(QWidget *parent)
{
    if (m_instance == nullptr)
        m_instance = new CarList(parent);
    return m_instance;
}

CarList *CarList::getInstance()
{
    return m_instance;
}

void CarList::closeEvent(QCloseEvent *e)
{
    emit onCloseButtonClicked();
    e->accept();
}


FakeCar::FakeCar()
{
    plateNum = QString("吉H%1").arg(++n, 4, 10, QLatin1Char('0'));
    startTime = QTime::currentTime();
    pos = QString("一层%1号").arg(n);
    type = rand() % 3;
}

Adapter::Adapter(QObject *parent, QList<FakeCar> *list): QAbstractTableModel(parent), m_list(list) {
    header << "车牌" << "车型" << "停放位置" << "驶入时间" << "已产生费用";
    timer = new QTimer(this);
    QObject::connect(timer, &QTimer::timeout, this, &Adapter::update);
    timer->start(1000);
}

int Adapter::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 5;
}

int Adapter::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_list->size();
}

QVariant Adapter::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole) {
        switch(index.column()) {
        case 0: return m_list->at(index.row()).plateNum;
        case 1: return m_list->at(index.row()).type;
        case 2: return m_list->at(index.row()).pos;
        case 3: return m_list->at(index.row()).startTime.toString("hh:mm");
        case 4: return QTime::currentTime().second() - m_list->at(index.row()).startTime.second();
        default: return QVariant();
        }
    }
    return QVariant();
}

QVariant Adapter::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
        return header.at(section);
    return QVariant();
}

void Adapter::update()
{
    beginResetModel();
    endResetModel();
}
