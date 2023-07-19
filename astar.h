// -*- coding: utf-8 -*-

#ifndef ASTAR_H
#define ASTAR_H

#include <QVector>
#include <QPoint>

class AStar
{
public:
    AStar();

    QVector<QPoint> findPath(const QPoint& start, const QPoint& goal, const QVector<QPoint>& obstacles, bool whetherLonger = false);
    QVector<QPoint> findLongestPath(const QPoint& start, const QPoint& goal, const QVector<QPoint>& obstacles);

private:
    QVector<QPoint> path; // 路径数组，存储找到的最短路径

    int heuristic(const QPoint& point, const QPoint& goal) const; // 启发式函数，估算点到目标点的代价
    int getCost(const QPoint& point) const; // 获取点的代价
    bool isPointValid(const QPoint& point, const QVector<QPoint>& obstacles) const; // 判断点是否有效，即是否在障碍物数组中
};

#endif // ASTAR_H
