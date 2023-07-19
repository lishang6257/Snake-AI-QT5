// -*- coding: utf-8 -*-

#ifndef BFS_H
#define BFS_H

#include <QPoint>
#include <QQueue>
#include <QSet>

class BFS
{
public:
    // 在给定起点和终点以及障碍物数组的情况下，找到一条路径
    QVector<QPoint> findPath(const QPoint &start, const QPoint &end, const QVector<QPoint> &obstacles);

private:
    // 判断点是否有效，即是否在障碍物数组中
    bool isPointValid(const QPoint& point, const QVector<QPoint>& obstacles) const;
};

#endif // BFS_H
