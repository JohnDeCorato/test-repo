#include "gestureengine.h"
#include <QDebug>

GestureEngine* GestureEngine::mInstance = NULL;

GestureEngine::GestureEngine()
{
    mInstance = this;


}


void GestureEngine::addTouchPoint(QTouchEvent::TouchPoint e)
{
    gwc::touchpoint new_point;
    new_point.init(e.id(), e.pos().x() / 1920, e.pos().y() / 1080, 0, 1920, 1080);
    new_point.status = gwc::TOUCHADDED;
    addEvent(new_point);

    qDebug() << "Added to Gestureworks";
}

void GestureEngine::updateTouchPoint(QTouchEvent::TouchPoint e)
{
    gwc::touchpoint updated_point;
    updated_point.init(e.id(), e.pos().x() / 1920, e.pos().y()/ 1080, 0, 1, 1);
    updated_point.status = gwc::TOUCHUPDATE;
    addEvent(updated_point);
}

void GestureEngine::removeTouchPoint(QTouchEvent::TouchPoint e)
{
    gwc::touchpoint removed_point;
    removed_point.init(e.id(), e.pos().x() / 1920, e.pos().y()/ 1080, 0, 1, 1);
    removed_point.status = gwc::TOUCHREMOVED;
    addEvent(removed_point);
}

std::vector<gwc::GestureEvent> GestureEngine::getGestures()
{
    processFrame();
    std::vector<gwc::PointEvent> point_events = consumePointEvents();

    for(std::vector<gwc::PointEvent>::iterator event_it = point_events.begin(); event_it != point_events.end(); event_it++)
    {
            if(event_it->status == gwc::TOUCHADDED)
            {
                qDebug() << event_it->position.getX();
                assignTouchPoint("sketch", event_it->point_id);
            }
    }
    std::vector<gwc::GestureEvent> gesture_events = consumeGestureEvents();
    return gesture_events;
}
