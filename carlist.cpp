#include "carlist.h"
#include "ui_carlist.h"
#include "QDebug"

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


Adapter::Adapter(QObject *parent, QList<Car*> *list): QAbstractTableModel(parent), m_list(list) {
    header << "车牌" << "车型" << "停放位置" << "驶入时间" << "已产生费用";
    timer = new QTimer(this);
    QObject::connect(timer, &QTimer::timeout, this, &Adapter::update);
    timer->start(1000);  // 每秒更新
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
        case 0: return m_list->at(index.row())->getPlateNumber();
        case 1: return m_list->at(index.row())->getColor();
        case 2: return m_list->at(index.row())->getPosition();
        case 3:
            if (m_list->at(index.row())->getStatus() == Car::waiting)
                return "正在等待";
            else
                return m_list->at(index.row())->getStartTime().toString("hh:mm");
        case 4: return m_list->at(index.row())->getFee();
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
