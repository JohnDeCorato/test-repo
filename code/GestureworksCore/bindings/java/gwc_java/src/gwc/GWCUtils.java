////////////////////////////////////////////////////////////////////////////////
//
//  IDEUM
//  Copyright 2011-2013 Ideum
//  All Rights Reserved.
//
//  Gestureworks Core
//
//  File:    GWCUtils.java
//  Authors:  Ideum
//
//  NOTICE: Ideum permits you to use, modify, and distribute this file only
//  in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////

package gwc;

import java.util.Arrays;
import java.util.List;

import com.sun.jna.Pointer;
import com.sun.jna.Structure;


public class GWCUtils {
	
	
    public static interface Status 
    {
        public static final int TOUCHUPDATE = 0;
        public static final int TOUCHADDED = 1;
        public static final int TOUCHREMOVED = 2;
    }
	
	
   public static class Point extends Structure
   {
        protected List getFieldOrder() {
            return Arrays.asList(new String[] {"x", "y", "z", "w", "h", "r"});
        }

        public float x;
        public float y;
        public float z;
        public float w;
        public float h;
        public float r;

        public Point(){
            x = 100;
            y = 100;
        }

        public Point(Pointer p)
        {
            super(p);
            read();
        }

        @Override
        public String toString() {
            return "x: "+x+" y: "+y;
        }
   }	
   
   
   public static class PointEvent extends Structure
   {	   
        protected List getFieldOrder() {
            return Arrays.asList(new String[] {"point_id", "status", "position","timestamp"});
        }

        //public static class ByReference extends PointEvent implements Structure.ByReference{}
        public int point_id;
        public int status;
        public Point position;
        public int timestamp;

        public PointEvent(){
            point_id = 0;
            status = 0;
            position = new Point();			
        }

        public PointEvent(Pointer p)
        {
            super(p);
            read();
        }

        @Override
        public String toString() {
            return "point_id: "+point_id+"\nstatus: "+status+"\nposition: "+position+"\ntimestamp: "+timestamp;
        }
		
   } 
   
   public static class PointEventArray extends Structure
   {
        protected List getFieldOrder() {
            return Arrays.asList(new String[] {"events", "size"});
        }

        public Pointer events;
        public int size;

        public PointEventArray(){}
        public PointEventArray(Pointer p)
        {
            super(p);
            read();
        }
   }
   
   
   public static class MapStruct extends Structure
   {
        protected List getFieldOrder() {
            return Arrays.asList(new String[] {"names", "values", "size"});
        }

        public Pointer names;  //String[]
        public Pointer values; //float
        public int size;	

        public String[] castNames()
        {
            return names.getStringArray(0, size);
        }

        public float[] castValues()
        {
            return values.getFloatArray(0, size);
        }		
   } 


   public static class GestureEvent extends Structure
   {
        protected List getFieldOrder() {
            return Arrays.asList(new String[] {"ID", "gesture_type", "gesture_id", "target", "source", "n", "hold_n", "x", "y", "timestamp", "phase", "locked_points", "values"});
        }

        public int ID;
        public String gesture_type;
        public String gesture_id;
        public String target;
        public int source;
        public int n;
        public int hold_n;
        public float x;
        public float y;
        public int timestamp;
        public int phase;
        public int locked_points;
        public MapStruct values;

        public GestureEvent(){}
        public GestureEvent(Pointer p)
        {
            super(p);
            read();
        }

        @Override
        public String toString() {
            return "ID: "+ID+"\ngesture_type: "+"\ngesture_id: "+gesture_id+"\ntarget: "+target+"\nsource: "+source
                        +"\nn: "+n+"\nhold_n: "+hold_n+"\nx: "+x+"\ny: "+y+"\ntimestamp: "+timestamp+"\nphase: "+phase+"\nlocked_points: "+locked_points
                        +"\nvalues: "+values;
        }		
   }  

   public static class GestureEventArray extends Structure
   {
        protected List getFieldOrder() {
            return Arrays.asList(new String[] { "size", "events"});
        }

        public int size;
        public Pointer events;

        public GestureEventArray(){}
        public GestureEventArray(Pointer p)
        {
            super(p);
            read();
        }
   } 
   
   public static class touchpoint extends Structure
   {
        protected List getFieldOrder() {
            return Arrays.asList(new String[] {"timestamp", "status", "id", "x", "y", "z", "w", "h", "r", "p"});
        }

        public int timestamp;
        public int status;
        public int id;
        public float x;
        public float y;
        public float z;
        public float w;
        public float h;
        public float r;        
        public float p;        

        public void init(int id, float x, float y, float z, float w, float h){
            this.id = id;
            this.x = x;
            this.y = y;
            this.z = z;
            this.w = w;
            this.h = h;
        }

        @Override
        public String toString() {
            return "timestamp:"+timestamp+" status:"+status+" id:"+id+" x:"+x+" y:"+y+" z:"+z+" w:"+w+" h:"+h+" r:"+r+" p:"+p;
        }
   }   
   
    public static float rotateAboutCenterX(float point_x, float point_y, float center_x, float center_y, float ref_angle) 
    {
        float local_x = point_x - center_x;
        float local_y = point_y - center_y;

        float length = (float)java.lang.Math.sqrt(local_x*local_x + local_y*local_y);

        return (length * (float)java.lang.Math.cos(ref_angle + java.lang.Math.atan2(local_y, local_x)) + center_x);
    }

    public static float rotateAboutCenterY(float point_x, float point_y, float center_x, float center_y, float ref_angle) 
    {
        float local_x = point_x - center_x;
        float local_y = point_y - center_y;

        float length = (float)java.lang.Math.sqrt(local_x*local_x + local_y*local_y);

        return (length * (float)java.lang.Math.sin(ref_angle + java.lang.Math.atan2(local_y, local_x)) + center_y);
    }
    
    public static float getDrawingCoordX(float width, float height, float box_x, float box_y, float box_angle, float box_scale) {
            float length = (float)java.lang.Math.sqrt(box_scale*box_scale*(width*width + height*height)/4);
            return (float)(length * java.lang.Math.cos(box_angle + java.lang.Math.atan2(-height, -width)) + box_x);
    }

    public static float getDrawingCoordY(float width, float height, float box_x, float box_y, float box_angle, float box_scale) {
            float length = (float)java.lang.Math.sqrt(box_scale*box_scale*(width*width + height*height)/4);
            return (float)(length * java.lang.Math.sin(box_angle + java.lang.Math.atan2(-height, -width)) + box_y);
    }    

}
