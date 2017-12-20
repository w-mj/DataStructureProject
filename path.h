#ifndef PATH_H
#define PATH_H

#include<QList>
#include<QPoint>

//路径上的点和车在该点的角度
struct PathPoint
{
    QPointF point;
    qreal   dir;
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
