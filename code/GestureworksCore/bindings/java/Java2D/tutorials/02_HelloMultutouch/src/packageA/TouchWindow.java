////////////////////////////////////////////////////////////////////////////////
//
//  IDEUM
//  Copyright 2011-2013 Ideum
//  All Rights Reserved.
//
//  Gestureworks Core
//
//  File:    TouchWindow.java
//  Authors:  Ideum
//
//  NOTICE: Ideum permits you to use, modify, and distribute this file only
//  in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////

package packageA;

import gwc.Core;
import gwc.GWCUtils.GestureEvent;
import gwc.GWCUtils.PointEvent;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Frame;
import java.awt.Toolkit;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.util.ArrayList;
import java.util.Timer;
import java.util.TimerTask;
import javax.swing.JFrame;

/**
 *
 * @author Shaun
 */
public abstract class TouchWindow extends JFrame{
    
    protected Core gwCore;
    private boolean registered = false;
    private boolean gmlLoaded = false;
    
    
    public TouchWindow(String name)
    {
        super(name);
        getContentPane().setBackground(new Color(169, 169, 169));
        
        //initialize java binding
         gwCore = new Core("\\..\\..\\..\\..\\..\\..\\GestureworksCore32");  
        
        //kill process on window close
        addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent e) {
                System.exit(0);
            }            
        });
        
        //default to full screen
        int defaultWidth = (int) Toolkit.getDefaultToolkit().getScreenSize().getWidth();
        int defaultHeight = (int) Toolkit.getDefaultToolkit().getScreenSize().getHeight();
        setSize(defaultWidth, defaultHeight);    
        setExtendedState(Frame.MAXIMIZED_BOTH);
        setVisible(true);
        
        //register window with core and load gml file
        registered = gwCore.registerWindowForTouchByName(name);        
        gmlLoaded = gwCore.loadGML("resources/basic_manipulation.gml");
        System.out.println("Window: "+name+"\n\tregistered: "+registered+"\n\tgmlLoaded: "+gmlLoaded);
        
        //frame processing timer
        initializeTimer();
    }

    @Override
    public void setSize(int width, int height) {
        super.setSize(width, height);
        gwCore.initializeGestureWorks(width, height);
    }

    @Override
    public void setSize(Dimension d) {
        super.setSize(d);
        gwCore.initializeGestureWorks(d.width, d.height);
    }
    
    private void initializeTimer()
    {
        Timer timer = new Timer();
        timer.schedule(new UpdateGW(), 0, 16);
    }
    
    private class UpdateGW extends TimerTask
    {
        @Override
        public void run() {
            gwCore.processFrame();
            processPointEvents(gwCore.consumePointEvents());
            processGestureEvents(gwCore.consumeGesutureEvents());
        }        
    }
    
    public abstract void processPointEvents(ArrayList<PointEvent> events);    
    public abstract void processGestureEvents(ArrayList<GestureEvent> events);
}
