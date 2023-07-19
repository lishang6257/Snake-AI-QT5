// -*- coding: utf-8 -*-

#include "astar.h"
#include <QHash>
#include <cmath>

#include "utils.h"

AStar::AStar()
{

}

// 寻找路径的核心函数，使用A*算法在地图上找到起点到终点的最短路径或最长路径
QVector<QPoint> AStar::findPath(const QPoint& start, const QPoint& goal, const QVector<QPoint>& obstacles, bool whetherLonger)
{
    path.clear(); // 清空上一次寻找路径的结果

    QVector<QPoint> openList; // 存储待探索的节点
    QVector<QPoint> closedList; // 存储已探索的节点
    QHash<QPoint, QPoint> parentMap; // 存储每个节点的父节点，用于重构路径
    QHash<QPoint, int> gScore; // 存储每个节点的实际代价，即起点到该节点的路径长度
    QHash<QPoint, int> fScore; // 存储每个节点的估计代价，即起点经过该节点到终点的估计路径长度

    openList.append(start); // 将起点加入待探索列表
    parentMap.insert(start, start); // 起点的父节点是它自己
    gScore.insert(start, 0); // 起点到起点的路径长度为0
    fScore.insert(start, 0); // 起点到终点的估计路径长度为0

    while (!openList.isEmpty()) {
        QPoint currentPoint; // 当前节点
        int minFScore; // 最小（或最大）估计代价

        // 根据是否寻找最长路径，选择使用最小或最大估计代价进行搜索
        if (!whetherLonger){
            minFScore = INT_MAX;
            for (const auto& point : openList) {
                if (fScore[point] < minFScore) {
                    minFScore = fScore[point];
                    currentPoint = point;
                }
            }
        } else {
            minFScore = INT_MIN;
            for (const auto& point : openList) {
                if (fScore[point] > minFScore) {
                    minFScore = fScore[point];
                    currentPoint = point;
                }
            }
        }

        if (currentPoint == goal) {
            // 当前节点是终点，说明路径已找到，重构路径
            while (currentPoint != start) {
                path.prepend(currentPoint);
                currentPoint = parentMap[currentPoint];
            }
            break;
        }

        openList.removeOne(currentPoint); // 将当前节点从待探索列表中移除
        closedList.append(currentPoint); // 将当前节点加入已探索列表

        // 遍历当前节点的相邻节点
        for (int i = 0; i < 4; ++i) {
            QPoint neighbor(currentPoint + DirectionState[i]); // 计算相邻节点的坐标

            // 判断相邻节点是否合法和是否已经探索过
            if (!isPointValid(neighbor, obstacles) || closedList.contains(neighbor))
                continue;

            int tentativeGScore = gScore[currentPoint] + 1; // 计算起点经过当前节点到相邻节点的路径长度

            // 如果相邻节点不在待探索列表中或者新路径长度更短（或更长）则更新其代价和父节点
            if (!openList.contains(neighbor) ||
                ((!whetherLonger && (tentativeGScore < gScore[neighbor])) ||
                 (whetherLonger && (tentativeGScore > gScore[neighbor])) )) {
                parentMap[neighbor] = currentPoint; // 更新相邻节点的父节点
                gScore[neighbor] = tentativeGScore; // 更新相邻节点的实际代价
                fScore[neighbor] = tentativeGScore + heuristic(neighbor, goal); // 更新相邻节点的估计代价

                if (!openList.contains(neighbor))
                    openList.append(neighbor); // 将更新后的相邻节点加入待探索列表
            }
        }
    }
    return path; // 返回找到的路径
}

// 启发式函数，用于估计两个点之间的距离，这里使用曼哈顿距离
int AStar::heuristic(const QPoint& point, const QPoint& goal) const
{
    return (goal - point).manhattanLength();
}

// 判断一个点是否在地图范围内且没有障碍物
bool AStar::isPointValid(const QPoint& point, const QVector<QPoint>& obstacles) const
{
    if (point.x() < 0 || point.x() >= SnakeGameSetting::UNIT_COUNT_X || point.y() < 0 || point.y() >= SnakeGameSetting::UNIT_COUNT_Y)
        return false;

    if (obstacles.contains(point))
        return false;

    return true;
}
