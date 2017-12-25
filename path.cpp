#include "path.h"
#include "logwindow.h"

Path::Path()
{
    // path.append(PathPoint());
}

Path::Path(int)
{
    path.append(PathPoint());
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

PathPoint Path::getNext()
{
    PathPoint tmp = path.first();
    path.removeFirst();
    // Log::i(QString("(%1,%2)").arg(tmp.point.x()).arg(tmp.point.y()));
    return tmp;
}

bool inl(const QPointF& p1, const QPointF& p2, const QPointF& p3) {
    if (p3.x() == p2.x() && p2.x() == p1.x())
        return true;
    else if (p3.x() == p2.x() || p2.x() == p1.x())
        return false;
    float k1 = (p3.y() - p2.y()) / (p3.x() - p2.x());
    float k2 = (p2.y() - p1.y()) / (p2.x() - p1.x());
    return k2 == k1;
}

void Path::regularize()
{
    QVector<PathPoint> delist;
    for (int n = 0; n  < path.size() - 2; n++) {
        if ( inl(path[n].point, path[n+1].point, path[n+2].point))
            delist.append(path[n + 1]);
    }
    for (auto n: delist)
        path.removeOne(n);
}

void PathPoint::operator=(const PathPoint &another)
{
    point = another.point;
    dir = another.dir;
    pointId = another.pointId;
    action = another.action;
}
