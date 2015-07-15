////////////////////////////////////////////////////////////////////////////////
//
//  IDEUM
//  Copyright 2011-2013 Ideum
//  All Rights Reserved.
//
//  Gestureworks Core
//
//  File:    TouchObject.java
//  Authors:  Ideum
//
//  NOTICE: Ideum permits you to use, modify, and distribute this file only
//  in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////

package packageC;

import gwc.GWCUtils;
import java.awt.Component;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.RenderingHints;
import java.awt.geom.AffineTransform;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.net.URL;
import javax.imageio.ImageIO;

/**
 *
 * @author Shaun
 */
public class TouchObject extends Component{
    
    private BufferedImage bi;
    private float width;
    private float height;
    private float x = 200;
    private float y = 200;
    private float drawX;
    private float drawY;
    private float scale = 1;
    private float rotation = 0;
    private float rotateX;
    private float rotateY;
    public String name;  
    
    
    public TouchObject(String imagePath, String name)
    {
        this.name = name;
        try{
            URL imageSrc = ((new File(imagePath))).toURI().toURL();
            bi = ImageIO.read(imageSrc);
            width = bi.getWidth();
            height = bi.getHeight();
            setSize((int)width, (int)height);
        }catch (IOException e){
            System.err.println("Image could not be read");
            System.exit(1);
        }
    }

    @Override
    public void paint(Graphics g) {
        super.paint(g);
        Graphics2D g2d = (Graphics2D)g;
        g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
        
        drawX = GWCUtils.getDrawingCoordX(width, height, x, y, (float) Math.toRadians(rotation), scale);
        drawY = GWCUtils.getDrawingCoordY(width, height, x, y, (float) Math.toRadians(rotation), scale);
        
        AffineTransform transform = new AffineTransform();  
        transform.translate(drawX, drawY); 
        transform.rotate(Math.toRadians(rotation));    
        transform.scale(scale, scale);
        g2d.drawImage(bi, transform, null);   
    }
    
    public void addX(float x)
    {
        this.x += x; 
        repaint();
    }
    
   public void addY(float y)
   {
       this.y += y;
       repaint();
   }
   
   public void addScale(float scale)
   {
       this.scale += scale;
       repaint();
   }
   
   public void setRotateX(float x)
   {
       rotateX = x;
   }
   
   public void setRotateY(float y)
   {
       rotateY = y;
   }
    
   public void addRotation(float rotation)
   {
       this.rotation +=rotation;
       repaint();
   }
   
   public boolean hitTest(float x, float y)
   {       
       float localX = GWCUtils.rotateAboutCenterX(x, y, this.x, this.y, -(float) Math.toRadians(rotation));
       float localY = GWCUtils.rotateAboutCenterY(x, y, this.x, this.y, -(float) Math.toRadians(rotation));      
       
       boolean inWidth = Math.abs(localX - this.x) <= (width*scale)/2;
       boolean inHeight = Math.abs(localY - this.y) <= (height*scale)/2;       
       return inWidth && inHeight;
   }
}
