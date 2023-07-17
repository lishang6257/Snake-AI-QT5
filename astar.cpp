﻿// -*- coding: utf-8 -*-

#include "astar.h"
#include <QHash>
#include <cmath>

#include "utils.h"

AStar::AStar()
{

}

QVector<QPoint> AStar::findPath(const QPoint& start, const QPoint& goal, const QVector<QPoint>& obstacles)
{
    path.clear();

    QVector<QPoint> openList;
    QVector<QPoint> closedList;
    QHash<QPoint, QPoint> parentMap;
    QHash<QPoint, int> gScore;
    QHash<QPoint, int> fScore;

    openList.append(start);
    parentMap.insert(start, start);
    gScore.insert(start, 0);
    fScore.insert(start, 0);

    while (!openList.isEmpty()) {
        // Find the point with the lowest fScore
        int minFScore = INT_MAX;
        QPoint currentPoint;
        for (const auto& point : openList) {
            if (fScore[point] < minFScore) {
                minFScore = fScore[point];
                currentPoint = point;
            }
        }

        if (currentPoint == goal) {
            // Reconstruct the path
            while (currentPoint != start) {
                path.prepend(currentPoint);
                currentPoint = parentMap[currentPoint];
            }
            break;
        }

        openList.removeOne(currentPoint);
        closedList.append(currentPoint);

        for (int i = 0; i < 4; ++i) {
            int newX = currentPoint.x() + DirectionState[i].x();
            int newY = currentPoint.y() + DirectionState[i].y();
            QPoint neighbor(newX, newY);

            if (!isPointValid(neighbor, obstacles) || closedList.contains(neighbor))
                continue;

            int tentativeGScore = gScore[currentPoint] + 1;

            if (!openList.contains(neighbor) || tentativeGScore < gScore[neighbor]) {
                parentMap[neighbor] = currentPoint;
                gScore[neighbor] = tentativeGScore;
                fScore[neighbor] = tentativeGScore + heuristic(neighbor, goal);

                if (!openList.contains(neighbor))
                    openList.append(neighbor);
            }
        }
    }
    // Reconstruct the path
    if (path.empty()) {
        int maxFScore = INT_MIN;
        QPoint currentPoint;
        for (const auto& point : closedList) {
            if (fScore[point] > maxFScore) {
                maxFScore = fScore[point];
                currentPoint = point;
            }
        }

        while (currentPoint != start) {
            path.prepend(currentPoint);
            currentPoint = parentMap[currentPoint];
        }
    }

    return path;
}

int AStar::heuristic(const QPoint& point, const QPoint& goal) const
{
    int dx = std::abs(point.x() - goal.x());
    int dy = std::abs(point.y() - goal.y());
    return dx + dy;
}

bool AStar::isPointValid(const QPoint& point, const QVector<QPoint>& obstacles) const
{
    if (point.x() < 0 || point.x() >= SnakeGameSetting::UNIT_COUNT_X || point.y() < 0 || point.y() >= SnakeGameSetting::UNIT_COUNT_Y)
        return false;

    if (obstacles.contains(point))
        return false;

    return true;
}
