package gestureworkstutorial2;

import gwc.Core;
import gwc.GWCUtils.PointEvent;
import gwc.GWCUtils.Status;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import processing.core.PApplet;
import sun.awt.windows.WComponentPeer;


public class TouchSketch extends PApplet{
    
    private Core gwCore;
    private LinkedHashMap<Integer, PointEvent> activePoints = new LinkedHashMap<>();

    @Override
    public void setup() {
        super.setup();
        super.setup();
        int width = 1920;
        int height = 1080;
        size(width, height, P2D);        
                
        gwCore = new Core("\\..\\..\\..\\..\\..\\..\\GestureworksCore32");
        println("gml loaded: "+gwCore.loadGML("resources/basic_manipulation.gml"));        
        gwCore.initializeGestureWorks(width, height);   
        
        long hwnd = ((WComponentPeer)getPeer()).getHWnd();        
        println("window registered: "+gwCore.registerWindowForTouch(hwnd));
    }

    @Override
    public void draw() {
        background(169, 169, 169);
        gwCore.processFrame();
        processPointEvents(gwCore.consumePointEvents());
        drawTouchPoints();
    }  
    
    private void processPointEvents(ArrayList<PointEvent> events)
    {
        for(PointEvent event: events)
        {
            if(event.status == Status.TOUCHADDED || event.status == Status.TOUCHUPDATE) {
                activePoints.put(event.point_id, event);
            }
            else {
                activePoints.remove(event.point_id);
            }
        }
    }
    
    private void drawTouchPoints()
    {
        for(PointEvent event: activePoints.values())
        {
            int touchX = (int)event.position.x;            
            int touchY = (int)event.position.y;                        
            ellipseMode(RADIUS);
            smooth();
            
            //outer circle
            strokeWeight(2); 
            stroke(0, 140, 191);
            noFill();
            ellipse(touchX, touchY, 30, 30);
            
            //inner circle
            fill(0, 140, 191);
            ellipse(touchX, touchY, 20, 20);
            
            //info
            textSize(12);
            text("ID: "+event.point_id, touchX+40, touchY-20);
            text("X: "+touchX+"  Y: "+touchY, touchX+40, touchY-8);
        }
    }
    
    public static void main(String[] args) {
        PApplet.main(new String[] { "--present", "gestureworkstutorial2.TouchSketch" } );        
    }
    
}
