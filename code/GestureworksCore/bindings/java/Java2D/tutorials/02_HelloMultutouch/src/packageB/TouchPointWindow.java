////////////////////////////////////////////////////////////////////////////////
//
//  IDEUM
//  Copyright 2011-2013 Ideum
//  All Rights Reserved.
//
//  Gestureworks Core
//
//  File:    TouchPointWindow
//  Authors:  Ideum
//
//  NOTICE: Ideum permits you to use, modify, and distribute this file only
//  in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////

package packageB;

import gwc.GWCUtils.GestureEvent;
import gwc.GWCUtils.PointEvent;
import gwc.GWCUtils.Status;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import packageA.TouchWindow;

/**
 *
 * @author Shaun
 */
public class TouchPointWindow extends TouchWindow{
    
    private LinkedHashMap<Integer, TouchPoint> activePoints = new LinkedHashMap<>();
    
    public TouchPointWindow(String name)
    {
        super(name);
    }

    @Override
    public void processPointEvents(ArrayList<PointEvent> events) {
        for(PointEvent event: events)
        {
            switch(event.status)
            {
                case Status.TOUCHADDED:
                    addTouchPoint(event);
                    break;
                case Status.TOUCHUPDATE:
                    updateTouchPoint(event);
                    break;
                case Status.TOUCHREMOVED:
                    removeTouchPoint(event);
                    break;
                default:
                    break;
            }
        }        
    }

    @Override
    public void processGestureEvents(ArrayList<GestureEvent> events) {
        
    }
    
    private void addTouchPoint(PointEvent event)
    {
        TouchPoint tp = new TouchPoint(event, this.getInsets());
        activePoints.put(event.point_id, tp);
        add(tp);
        this.validate();
    }
    
    private void updateTouchPoint(PointEvent event)
    {
        activePoints.get(event.point_id).update(event);
    }
    
    private void removeTouchPoint(PointEvent event)
    {
        if(!activePoints.containsKey(event.point_id))
            return;
        
        activePoints.get(event.point_id).remove();
        activePoints.remove(event.point_id);
    }
    
}
