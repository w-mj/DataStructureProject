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
    Road::Action action = Road::Action::none;
    int pointId;  // 为了做直角生成的辅助节点的id为-1
    PathPoint(): point(QPoint(0, 0)), pointId(-2) {}
    PathPoint(QPointF p, qreal d, Road::Action a=Road::Action::none, int id=-1):
        point(p), action(a), pointId(id) {
        while (d < 0 || d > 360) {
            if (d < 0)
                d += 360;
            else if (d > 360)
                d -= 360;
        }
        dir = d;
    }
    PathPoint(const PathPoint& another) {
        point = another.point;
        dir = another.dir;
        pointId = another.pointId;
        action = another.action;
    }
    void operator=(const PathPoint& another);
    bool operator==(const PathPoint& another) const {
        return (point == another.point && dir == another.dir);
    }
    bool operator==(const PathPoint& another) {
        return (point == another.point && dir == another.dir);
    }
};

class Path
{
public:
    Path();
    Path(int);
    Path(PathPoint points[], int i);
    PathPoint getPoint(int i);
    void addPoint(int i, const PathPoint &point);
    void addPoint(const PathPoint &point);
    bool isEmpty();
    int pointsCount();
    PathPoint getNext();

    void regularize();   // 如果三点共线，移除中间那个点

private:
    QList<PathPoint> path;
};

#endif // PATH_H
