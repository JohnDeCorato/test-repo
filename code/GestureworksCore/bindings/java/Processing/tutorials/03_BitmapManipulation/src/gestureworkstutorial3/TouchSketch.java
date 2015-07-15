package gestureworkstutorial3;

import gwc.Core;
import gwc.GWCUtils.GestureEvent;
import gwc.GWCUtils.PointEvent;
import gwc.GWCUtils.Status;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import processing.core.PApplet;
import sun.awt.windows.WComponentPeer;


public class TouchSketch extends PApplet{

    private Core gwCore;
    private LinkedHashMap<String, TouchObject> touchObjects = new LinkedHashMap<>();
    
    @Override
    public void setup() {
        super.setup();
        int width = 1920;
        int height = 1080;
        size(width, height, P2D);        
                
        gwCore = new Core("\\..\\..\\..\\..\\..\\..\\GestureworksCore32");
        println("gml loaded: "+gwCore.loadGML("resources/basic_manipulation.gml"));        
        gwCore.initializeGestureWorks(width, height);   
        
        long hwnd = ((WComponentPeer)getPeer()).getHWnd();        
        println("window registered: "+gwCore.registerWindowForTouch(hwnd));
        
        addTouchObject("resources/Processing.png", "img1");
        addTouchObject("resources/Processing.png", "img2");
    }

    @Override
    public void draw() {
        background(169, 169, 169);
        gwCore.processFrame();
        processPointEvents(gwCore.consumePointEvents());
        processGestureEvents(gwCore.consumeGesutureEvents());
        smooth();
        
        for(TouchObject to: touchObjects.values())
            to.draw();
    }
    
    public void addTouchObject(String path, String name)
    {
        touchObjects.put(name, new TouchObject(this, path));
        gwCore.registerTouchObject(name);
        gwCore.addGesture(name, "n-drag");
        gwCore.addGesture(name, "n-rotate");
        gwCore.addGesture(name, "n-scale");
    }
    
    private void processPointEvents(ArrayList<PointEvent> events)
    {
        for(PointEvent event: events)
        {
            switch(event.status)
            {
                case Status.TOUCHADDED:
                    Object[] names = touchObjects.keySet().toArray();
                    for(int i=names.length-1; i >= 0; i--)
                    {
                        String name = (String)names[i];
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
    
    private void processGestureEvents(ArrayList<GestureEvent> events)
    {
        for(GestureEvent event: events)
        {
            if(event.gesture_id.equals("n-drag")){
                handleDrag(event);
            }
            else if(event.gesture_id.equals("n-rotate")){
                handleRotate(event);
            }
            else if(event.gesture_id.equals("n-scale")){
                handleScale(event);
            }
        }
    }
    
    private void handleDrag(GestureEvent event)
    {
        TouchObject to = touchObjects.get(event.target);
        to.x += event.values.castValues()[0];
        to.y += event.values.castValues()[1];
    }
    
    private void handleRotate(GestureEvent event)
    {
        TouchObject to = touchObjects.get(event.target);
        to.rotation += event.values.castValues()[0];
        to.rotateX = event.x;
        to.rotateY = event.y;
    }
    
    private void handleScale(GestureEvent event)
    {
        TouchObject to = touchObjects.get(event.target);
        to.scale += event.values.castValues()[0];
    }
        
    public static void main(String[] args) {
            PApplet.main(new String[] { "--present", "gestureworkstutorial3.TouchSketch" } );
    } 
    
}
