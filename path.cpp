#include "path.h"

Path::Path()
{

}

//用PathPoint数组初始化Path
Path::Path(PathPoint points[], int i)
{
    for(int j=0; j<i; j++)
        addPoint(points[j]);
}

PathPoint Path::getPoint(int i)
{
    return path.value(i);
}

void Path::addPoint(int i, const PathPoint &point)
{
    path.insert(i, point);
}

void Path::addPoint(const PathPoint &point)
{
    path.append(point);
}

bool Path::isEmpty()
{
    return path.isEmpty();
}

int Path::pointsCount()
{
    return path.length();
}

PathPoint &Path::getNext()
{
    PathPoint &tmp = path.first();
    path.removeFirst();
    return tmp;
}
