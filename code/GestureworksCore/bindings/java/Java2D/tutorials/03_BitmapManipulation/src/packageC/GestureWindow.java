////////////////////////////////////////////////////////////////////////////////
//
//  IDEUM
//  Copyright 2011-2013 Ideum
//  All Rights Reserved.
//
//  Gestureworks Core
//
//  File:    GestureWindow.java
//  Authors:  Ideum
//
//  NOTICE: Ideum permits you to use, modify, and distribute this file only
//  in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////

package packageC;

import gwc.GWCUtils.GestureEvent;
import gwc.GWCUtils.PointEvent;
import gwc.GWCUtils.Status;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import packageA.TouchWindow;

/**
 *
 * @author Shaun
 */
public class GestureWindow extends TouchWindow {
    
    private LinkedHashMap<String, Method> gestureEventHandlers;
    private LinkedHashMap<String, TouchObject> touchObjects;
    
    public GestureWindow(String name)
    {
        super(name);
        
        //map gesture events to hanlder methods
        try{
            gestureEventHandlers = new LinkedHashMap<>();
            gestureEventHandlers.put("n-drag", getClass().getMethod("handleDrag", GestureEvent.class));
            gestureEventHandlers.put("n-rotate", getClass().getMethod("handleRotate", GestureEvent.class));
            gestureEventHandlers.put("n-scale", getClass().getMethod("handleScale", GestureEvent.class));
        }catch(NoSuchMethodException e)
        {
            e.printStackTrace();
        }
        
        touchObjects = new LinkedHashMap<>();
    }
    
    public void addTouchObject(String path, String name)
    {
        TouchObject to = new TouchObject(path, name);
        touchObjects.put(name, to);
        add(to);
        validate();
        
        gwCore.registerTouchObject(name);
        for(String type: gestureEventHandlers.keySet()) {
            gwCore.addGesture(name, type);
        }
    }

    @Override
    public void processPointEvents(ArrayList<PointEvent> events) 
    {
        for(PointEvent event: events)
        {
            switch(event.status)
            {
                case Status.TOUCHADDED:
                    for(String name: touchObjects.keySet())
                    {
                        if(touchObjects.get(name).hitTest(event.position.x, event.position.y))
                            gwCore.addTouchPoint(name, event.point_id);
                    }
                    break;
                case Status.TOUCHUPDATE:
                    break;
                case Status.TOUCHREMOVED:
                    break;
                default:
                    break;
            }
        }
    }

    @Override
    public void processGestureEvents(ArrayList<GestureEvent> events) 
    {
        for(GestureEvent event: events)
        {
            try{
                gestureEventHandlers.get(event.gesture_id).invoke(this, event);
            }catch(IllegalAccessException | IllegalArgumentException | InvocationTargetException e){
                e.printStackTrace();
            }            
        }
    }
    
    public void handleDrag(GestureEvent event)
    {
        TouchObject to = touchObjects.get(event.target);
        to.addX(event.values.castValues()[0]);
        to.addY(event.values.castValues()[1]);
    }
    
    public void handleRotate(GestureEvent event)
    {
        TouchObject to = touchObjects.get(event.target);
        to.addRotation(event.values.castValues()[0]);
        to.setRotateX(event.x);
        to.setRotateY(event.y);
    }
    
    public void handleScale(GestureEvent event)
    {
       TouchObject to = touchObjects.get(event.target);
       to.addScale(event.values.castValues()[0]);
    }    
}
