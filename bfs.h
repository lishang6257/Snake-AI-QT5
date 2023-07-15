﻿#ifndef BFS_H
#define BFS_H

#include <QPoint>
#include <QQueue>
#include <QSet>


extern const int UNIT_COUNT_X;
extern const int UNIT_COUNT_Y;

class BFS
{
public:
    QVector<QPoint> findPath(const QPoint &start, const QPoint &end, const QVector<QPoint> &obstacles);

private:
    QVector<QPoint> reconstructPath(const QPoint &start, const QPoint &end, const QHash<QPoint, QPoint> &cameFrom);
    bool isPointValid(const QPoint& point, const QVector<QPoint>& obstacles) const;
};

#endif // BFS_H