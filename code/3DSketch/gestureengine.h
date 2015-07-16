#ifndef GESTUREENGINE_H
#define GESTUREENGINE_H

#include "GestureWorksCore.h"
#include <QTouchEvent>

class GestureEngine
{
public:
    GestureEngine();

    static GestureEngine* instance() { return mInstance; }
    void addTouchPoint(QTouchEvent::TouchPoint e);
    void updateTouchPoint(QTouchEvent::TouchPoint e);
    void removeTouchPoint(QTouchEvent::TouchPoint e);
    std::vector<gwc::GestureEvent> getGestures();

protected:
    static GestureEngine* mInstance;
    std::map<int, QTouchEvent::TouchPoint> pointMap;

};

#endif // GESTUREENGINE_H
