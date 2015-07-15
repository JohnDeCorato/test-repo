#include "gestureengine.h"

GestureEngine::GestureEngine()
{
    mInstance = this;
}


void GestureEngine::addTouchPoint(QTouchEvent::TouchPoint *e)
{
    gwc::touchpoint new_point;
    new_point.init(e->id(), e->pos().x(), e->pos().y(), 0, 1, 1);
    new_point.status = gwc::TOUCHADDED;
    addEvent(new_point);
}

void GestureEngine::updateTouchPoint(QTouchEvent::TouchPoint *e)
{
    gwc::touchpoint updated_point;
    updated_point.init(e->id(), e->pos().x(), e->pos().y(), 0, 1, 1);
    updated_point.status = gwc::TOUCHUPDATE;
    addEvent(updated_point);
}

void GestureEngine::removeTouchPoint(QTouchEvent::TouchPoint *e)
{
    gwc::touchpoint removed_point;
    removed_point.init(e->id(), e->pos().x(), e->pos().y(), 0, 1, 1);
    removed_point.status = gwc::TOUCHREMOVED;
    addEvent(removed_point);
}
