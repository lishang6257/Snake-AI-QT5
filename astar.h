#ifndef ASTAR_H
#define ASTAR_H

#include <QVector>
#include <QPoint>

class AStar
{
public:
    AStar();

//    QVector<QPoint> getPath() const;
    QVector<QPoint> findPath(const QPoint& start, const QPoint& goal, const QVector<QPoint>& obstacles);

private:
    QVector<QPoint> path;
    QVector<QPoint> SearchedButNotFoundLongestPath;

    int heuristic(const QPoint& point, const QPoint& goal) const;
    int getCost(const QPoint& point) const;
    bool isPointValid(const QPoint& point, const QVector<QPoint>& obstacles) const;
};

#endif // ASTAR_H
