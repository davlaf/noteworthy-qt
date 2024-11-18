#pragma once

#include <list>
#include <qdebug.h>
#include <qline.h>
#include <qpoint.h>
#include <map>

class TouchState {
public:
    void print() {
        qDebug() << "TouchState:";
        for (int id : touch_priority) {
            QPointF point = touch_points.at(id);
            qDebug() << "ID: " << id << ", Location: ("
               << point.x() << ", " << point.y() << ")";
        }
    }

    void updateTouch(QPointF point, int id) {
#ifdef __EMSCRIPTEN__ // if its webassembly
        if (id == 0) {
            return;
        }
#endif
        if (touch_points.count(id) == 0) {
            touch_priority.push_back(id);
        }
        touch_points[id] = point;
    }

    void removeTouch(int id) {
#ifdef __EMSCRIPTEN__ // if its webassembly
        if (id == 0) {
            return;
        }
#endif
        touch_priority.remove(id);
        touch_points.erase(id);
        if (touch_priority.size() < 2) {
            prev_pinch_distance = 0;
        }
    }

    bool isPinching() {
        return touch_priority.size() >= 2;
    }

    double getPinchDistance() {
        if (touch_priority.size() < 2) {
            throw "doesn't work";
        }

        auto it = touch_priority.begin();
        int first = *it;
        ++it;
        int second = *it;
        QPointF first_point = touch_points[first];
        QPointF second_point = touch_points[second];

        QLineF line {first_point, second_point};

        return line.length();
    }

    double getZoomRatio() {
        double pinch_distance = getPinchDistance();
        if (prev_pinch_distance == 0) {
            prev_pinch_distance = pinch_distance;
            return 1;
        }
        double ratio = pinch_distance / prev_pinch_distance;

        prev_pinch_distance = pinch_distance;
        if (ratio > 3) {
            return 3;
        }
        if (ratio < 0.3) {
            return 0.3;
        }
        return ratio;
    }

    QPointF getPinchCenterPoint() {
        if (touch_priority.size() < 2) {
            throw "doesn't work";
        }

        auto it = touch_priority.begin();
        int first = *it;
        ++it;
        int second = *it;
        QPointF first_point = touch_points[first];
        QPointF second_point = touch_points[second];

        return {(first_point.rx() + second_point.rx())/2, (first_point.ry() + second_point.ry())/2};
    }

    void setPrevCenterPoint(QPointF point) {
        prev_pinch_center_point = point;
    }

    QPointF getRelativeCenter() {
        QPointF current_center = getPinchCenterPoint();
        QPointF relative_center = current_center-prev_pinch_center_point;
        prev_pinch_center_point = current_center;
        return relative_center;
    }

private:
    QPointF prev_pinch_center_point;
    double prev_pinch_distance = 0;
    std::map<int, QPointF> touch_points;
    std::list<int> touch_priority;
};
