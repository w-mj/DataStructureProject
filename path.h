#ifndef PATH_H
#define PATH_H

#include<QList>
#include<QPoint>
#include "Road.h"

//路径上的点和车在该点的角度
struct PathPoint
{
    QPointF point;
    qreal   dir = 0;
    Road::Action action;
    int pointId;  // 为了做直角生成的辅助节点的id为-1
    PathPoint() {}
    PathPoint(QPointF p, qreal d, Road::Action a=Road::Action::none, int id=-1):
        point(p), dir(d), action(a), pointId(id) {}
    PathPoint(const PathPoint& another) {
        point = another.point;
        dir = another.dir;
    }
};

class Path
{
public:

    Path();
    Path(PathPoint points[], int i);
    PathPoint getPoint(int i);
    void addPoint(int i, const PathPoint &point);
    void addPoint(const PathPoint &point);
    bool isEmpty();
    int pointsCount();

private:
    QList<PathPoint> path;
    int len;
};

#endif // PATH_H
