#include "cars.h"
#include <QtGlobal>

Cars::Cars(QWidget *parent) : QWidget(parent)
{
    m_color = rand() % 3;
}


