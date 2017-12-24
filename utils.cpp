#include "utils.h"

bool checkPlate(QString plate)
{
    QString province = "京津冀晋蒙辽吉黑沪苏浙皖闽赣鲁豫鄂湘粤桂琼川贵云渝藏陕甘青宁新港澳台";
    if (!province.contains(plate.at(0)))
        return false;
    if (plate.size() != 6)
        return false;
    for (auto c = plate.begin() + 1; c != plate.end(); c++)
        if (!c->isLetterOrNumber())
            return false;
    return true;
}
