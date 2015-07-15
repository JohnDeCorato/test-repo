////////////////////////////////////////////////////////////////////////////////
//
//  IDEUM
//  Copyright 2011-2013 Ideum
//  All Rights Reserved.
//
//  Gestureworks Core
//
//  File:    Core.java
//  Authors:  Ideum
//
//  NOTICE: Ideum permits you to use, modify, and distribute this file only
//  in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////

package gwc;

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Pointer;
import gwc.GWCUtils.GestureEvent;
import gwc.GWCUtils.GestureEventArray;
import gwc.GWCUtils.PointEvent;
import gwc.GWCUtils.PointEventArray;
import gwc.GWCUtils.touchpoint;
import java.util.ArrayList;


public class Core {
	
    private GWCore gwcDLL;

    public Core(String dllPath){
        gwcDLL = (GWCore)Native.loadLibrary(dllPath, GWCore.class);
    }
	
    public interface GWCore extends Library
    {		
        public void initializeGestureWorks(int screenWidth, int screenHeight);
        public void resizeScreen(int width, int height);
        public boolean loadGML(String path);
        public boolean registerWindowForTouch(long hwnd);
        public boolean registerWindowForTouchByName(String windowName);
        public boolean registerTouchObject(String objectName);
        public boolean deregisterTouchObject(String objectName);
        public boolean addGesture(String touchObjectName, String gestureName);
        public boolean addGestureSet(String touchObjectName, String setName);
        public boolean removeGesture(String touchObjectName, String gestureName);
        public boolean removeGestureSet(String touchObjectName, String setName);
        public void processFrame();	
        public PointEventArray consumePointEvents();
        public GestureEventArray consumeGestureEvents();
        public boolean addTouchPoint(String touchObjectName, int pointId);
        public void addEvent(touchpoint touchEvent);
    }

    /**
     * Initializes GestureWorks and calibrates stroke based gestures for your resolution.
     * @param screenWidth  width of the screen receiving touch
     * @param screenHeight height of the screen receiving touch
     */
    public void initializeGestureWorks(int screenWidth, int screenHeight)
    {
        gwcDLL.initializeGestureWorks(screenWidth, screenHeight);
    }

    /**
     * Updates the internal screen size.
     * @param width  width of the screen receiving touch
     * @param height height of the screen receiving touch
     */
    public void resizeScreen(int width, int height)
    {
        gwcDLL.resizeScreen(width, height);
    }

    /**
     * Loads all gesture definitions from the specified GML file. Multiple files may be loaded. Returns true if one of more gesture
     * object was created.
     * @param pathToGML
     * @return true upon success
     */
    public boolean loadGML(String pathToGML)
    {
        return gwcDLL.loadGML(pathToGML);
    }

    /**
     * Tells GestureWorks which window (application) you want to touch enable via a handle to the window. 
     * @param hwnd  handle to the window
     * @return  true upon success
     */
    public boolean registerWindowForTouch(long hwnd)
    {
        return gwcDLL.registerWindowForTouch(hwnd);
    }

    /**
     * Tells GestureWorks which window (application) you want to touch enable. Returns true upon success.
     * @param windowName
     * @return true upon success
     */
    public boolean registerWindowForTouchByName(String windowName)
    {
        return gwcDLL.registerWindowForTouchByName(windowName);
    }

    /**
     * Registers a touch object with GestureWorks by name. The object's name is arbitrary and can be anything the application developer
     * chooses.
     * @param objectName
     * @return true upon success
     */
    public boolean registerTouchObject(String objectName)
    {
        return gwcDLL.registerTouchObject(objectName);
    }

    /**
     * Enables processing of the specified gesture on the touch object. The gesture name is defined in the GML and the touch object name is
     * defined in the call to registerTouchObject. The GML file containing the gesture must be loaded first.  
     * @param touchObjectName
     * @param gestureName
     * @return true upon success
     */
    public boolean addGesture(String touchObjectName, String gestureName)
    {
        return gwcDLL.addGesture(touchObjectName, gestureName);
    }

    /**
     * Enables a set of gestures on a touch object where the set is defined in a loaded GML file. 
     * @param touchObjectName
     * @param setName
     * @return true upon success
     */
    public boolean addGestureSet(String touchObjectName, String setName)
    {
        return gwcDLL.addGestureSet(touchObjectName, setName);
    }

    /**
     * Removes gesture processing of the specified gesture from the specified touch object.
     * @param touchObjectName  
     * @param gestureName
     * @return  true upon successful removal
     */
    public boolean removeGesture(String touchObjectName, String gestureName)
    {
        return gwcDLL.removeGesture(touchObjectName, gestureName);
    }

    /**
     * Removes gesture processing for a set of gestures form the specified touch object where the set is defined in a loaded GML file.
     * @param touchObjectName  
     * @param setName
     * @return  true upon successful removal
     */	
    public boolean removeGestureSet(String touchObjectName, String setName)
    {
        return gwcDLL.removeGestureSet(touchObjectName, setName);
    }	

    /**
     * Tell GestureWorks to process the current frame of data.
     */
    public void processFrame()
    {
        gwcDLL.processFrame();
    }

    /**
     * Consume point events that were processed in the call to processFrame. 
     */
    public ArrayList<PointEvent> consumePointEvents()
    {		
        ArrayList<PointEvent> points = new ArrayList<PointEvent>();
        PointEventArray pea = gwcDLL.consumePointEvents();
        Pointer events = pea.events;
        int size = pea.size;

        if(size > 0)
        {		
            PointEvent pe = new PointEvent(events);
            PointEvent[] pevents = (PointEvent[])pe.toArray(size);

            for(int i=0; i<pevents.length; i++)
                points.add(pevents[i]);			
        }

        return points;
    }

    /**
     * Consume gesture events that were processed in the call to processFrame.
     */
    public ArrayList<GestureEvent> consumeGesutureEvents()
    {
        ArrayList<GestureEvent> gestures = new ArrayList<GestureEvent>();
        GestureEventArray gea = gwcDLL.consumeGestureEvents();
        Pointer events = gea.events;
        int size = gea.size;

        if(size > 0)
        {
            GestureEvent ge = new GestureEvent(events);
            GestureEvent[] gevents = (GestureEvent[])ge.toArray(size);

            for(int i=0; i<gevents.length; i++)				
                gestures.add(gevents[i]);
        }

        return gestures;
    }

    /**
     * Assigns a touch point to a touch object, usually after the client application has performed a hit test
     * or otherwise determined that the touch point has collided with the touch object.
     * @param touchObjectName
     * @param pointId
     * @return
     */
    public boolean addTouchPoint(String touchObjectName, int pointId)
    {
        return gwcDLL.addTouchPoint(touchObjectName, pointId);
    }
    
    /**
     * Adds a touch event to the GestureWorks pipeline for processing. This is used to feed GestureWorks touch events
     * obtained from sources other than natively through Windows. When using this method to process touch events, DO NOT
     * CALL registerWindowForTouch AND registerWindowForTouchByName - your application can only use one method or the other, 
     * NOT BOTH. 
     * @param touchEvent 
     */
    public void addEvent(touchpoint touchEvent)
    {
        gwcDLL.addEvent(touchEvent);
    }
}
