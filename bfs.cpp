#include "bfs.h"
#include "utils.h"

QVector<QPoint> BFS::findPath(const QPoint &start, const QPoint &end, const QVector<QPoint> &obstacles)
{
    QVector<QPoint> path;
    QQueue<QPoint> queue;
    QSet<QPoint> visited;
    QHash<QPoint, QPoint> cameFrom;


    queue.enqueue(start);
    visited.insert(start);

    while (!queue.isEmpty()) {
        QPoint current = queue.dequeue();

        if (current == end) {
            // 找到路径，重建路径并返回
            path = reconstructPath(start, end, cameFrom);
            break;
        }

        // 遍历相邻的点（水平和竖直方向）
        QPoint directions[] = {
            QPoint(1, 0),   // 右
            QPoint(-1, 0),  // 左
            QPoint(0, 1),   // 下
            QPoint(0, -1)   // 上
        };

        for (const QPoint& direction : directions) {
            QPoint neighbor(current + direction);

            if (!visited.contains(neighbor) && isPointValid(neighbor, obstacles)) {
                queue.enqueue(neighbor);
                visited.insert(neighbor);
                cameFrom[neighbor] = current;
            }
        }

    }

    return path;
}

QVector<QPoint> BFS::reconstructPath(const QPoint &start, const QPoint &end, const QHash<QPoint, QPoint> &cameFrom)
{
    QVector<QPoint> path;
    QPoint current = end;

    while (current != start) {
        path.prepend(current);
        current = cameFrom.value(current);
    }

    return path;
}

bool BFS::isPointValid(const QPoint& point, const QVector<QPoint>& obstacles) const
{
    if (point.x() < 0 || point.x() >= SnakeGameSetting::UNIT_COUNT_X || point.y() < 0 || point.y() >= SnakeGameSetting::UNIT_COUNT_Y)
        return false;

    if (obstacles.contains(point))
        return false;

    return true;
}
