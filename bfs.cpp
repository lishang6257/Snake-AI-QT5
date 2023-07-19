#include "bfs.h"
#include "utils.h"

QVector<QPoint> BFS::findPath(const QPoint &start, const QPoint &end, const QVector<QPoint> &obstacles)
{
    QVector<QPoint> path; // 存储找到的路径
    QQueue<QPoint> queue; // 使用队列来进行广度优先搜索
    QSet<QPoint> visited; // 存储已经访问过的节点
    QHash<QPoint, QPoint> cameFrom; // 存储每个节点的父节点，用于重构路径

    queue.enqueue(start); // 将起点加入队列
    visited.insert(start); // 标记起点为已访问

    while (!queue.isEmpty()) {
        QPoint current = queue.dequeue(); // 取出队列头部的节点进行探索

        if (current == end) {
            // 当前节点是终点，说明路径已找到，重构路径
            while (current != start) {
                path.prepend(current);
                current = cameFrom.value(current);
            }
            break;
        }

        // 遍历当前节点的相邻节点
        for (const QPoint& direction : DirectionState) {
            QPoint neighbor(current + direction); // 计算相邻节点的坐标

            // 判断相邻节点是否合法和是否已经访问过
            if (!visited.contains(neighbor) && isPointValid(neighbor, obstacles)) {
                queue.enqueue(neighbor); // 将相邻节点加入队列
                visited.insert(neighbor); // 标记相邻节点为已访问
                cameFrom[neighbor] = current; // 更新相邻节点的父节点为当前节点
            }
        }
    }

    return path; // 返回找到的路径
}

// 判断一个点是否在地图范围内且没有障碍物
bool BFS::isPointValid(const QPoint& point, const QVector<QPoint>& obstacles) const
{
    if (point.x() < 0 || point.x() >= SnakeGameSetting::UNIT_COUNT_X || point.y() < 0 || point.y() >= SnakeGameSetting::UNIT_COUNT_Y)
        return false;

    if (obstacles.contains(point))
        return false;

    return true;
}
