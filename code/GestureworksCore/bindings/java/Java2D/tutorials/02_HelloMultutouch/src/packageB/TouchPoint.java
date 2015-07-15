////////////////////////////////////////////////////////////////////////////////
//
//  IDEUM
//  Copyright 2011-2013 Ideum
//  All Rights Reserved.
//
//  Gestureworks Core
//
//  File:    TouchPoint.java
//  Authors:  Ideum
//
//  NOTICE: Ideum permits you to use, modify, and distribute this file only
//  in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////

package packageB;

import gwc.GWCUtils.PointEvent;
import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Component;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Insets;
import java.awt.RenderingHints;
import java.awt.geom.Ellipse2D;

public class TouchPoint extends Component{
	
    private Ellipse2D.Double outerCircle;
    private Ellipse2D.Double innerCircle;
    private float x;
    private float y;
    private int id;
    private boolean dispose = false;
    private Insets windowInsets;
	     
    public TouchPoint(PointEvent event, Insets insets)
    {
        this.setSize(80, 80);
        windowInsets = insets;
    	x = event.position.x - windowInsets.left;
    	y = event.position.y - windowInsets.top;
    	id = event.point_id;
    	outerCircle = new Ellipse2D.Double(0, 0, 60, 60); 
        innerCircle = new Ellipse2D.Double(10, 10, 40, 40);
    }
    
    @Override
    public void paint(Graphics g) {
    	
    	if(dispose)
            g.dispose();
        
        int touchX = (int)x - windowInsets.left;
        int touchY = (int)y - windowInsets.top;
                
    	Graphics2D g2d = (Graphics2D)g;
    	g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
    	g2d.setStroke(new BasicStroke(2));
    	g2d.setColor(new Color(0x008cbf));
    	g2d.translate(touchX-20, touchY-20);  //center the graphic
    	g2d.draw(outerCircle);
        g2d.fill(innerCircle);
    	g2d.drawString("ID: "+Integer.toString(id), 80, 0);
    	g2d.drawString("X:"+Integer.toString(touchX)+"  Y:"+Integer.toString(touchY), 80, 15);
    }
    
    public void update(PointEvent event)
    {
    	x = event.position.x;
    	y = event.position.y;
    	repaint();
    }
    
    public void remove()
    {
    	dispose = true;
    	repaint();
    }

    @Override
    public String toString() {
        return "x: "+x+"\ny: "+y+"\nid: "+id;
    }
    
}
