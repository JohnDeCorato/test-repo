package gestureworkstutorial3;

import gwc.GWCUtils;
import processing.core.PApplet;
import processing.core.PImage;

public class TouchObject  {
    
    private PApplet sketch;
    private PImage image;
    private float drawX;
    private float drawY;
    private float width;
    private float height;
    
    public float x = 200;
    public float y = 200;    
    public float scale = 1;
    public float rotation = 0;
    public float rotateX = 0;
    public float rotateY = 0;
    
    public TouchObject(PApplet sketch, String imgSrc)
    {
        this.sketch = sketch;
        this.image = sketch.loadImage(imgSrc);
        width = image.width;
        height = image.height;
    }
    
    public void draw()
    {
        drawX = GWCUtils.getDrawingCoordX(width, height, x, y, (float) Math.toRadians(rotation), scale);
        drawY = GWCUtils.getDrawingCoordY(width, height, x, y, (float) Math.toRadians(rotation), scale);
        
        sketch.pushMatrix();
        sketch.translate(drawX, drawY);
        sketch.rotate((float)Math.toRadians(rotation));
        sketch.scale(scale);
        sketch.image(image, 0, 0);
        sketch.popMatrix();
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
