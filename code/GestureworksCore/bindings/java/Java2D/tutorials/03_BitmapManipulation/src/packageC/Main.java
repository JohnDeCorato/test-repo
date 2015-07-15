////////////////////////////////////////////////////////////////////////////////
//
//  IDEUM
//  Copyright 2011-2013 Ideum
//  All Rights Reserved.
//
//  Gestureworks Core
//
//  File:    Main.java
//  Authors:  Ideum
//
//  NOTICE: Ideum permits you to use, modify, and distribute this file only
//  in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////

package packageC;

/**
 *
 * @author Shaun
 */
public class Main {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        GestureWindow window = new GestureWindow("Interactive Bitmaps");  
        window.addTouchObject("resources/Processing.png", "java_img");        
        window.addTouchObject("resources/Processing.png", "java_img2"); 
    }
}
