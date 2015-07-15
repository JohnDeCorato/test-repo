////////////////////////////////////////////////////////////////////////////////
//
//  IDEUM
//  Copyright 2011-2013 Ideum
//  All Rights Reserved.
//
//  Gestureworks Core
//
//  File:    TouchImage.cs
//  Authors:  Ideum
//
//  NOTICE: Ideum permits you to use, modify, and distribute this file only
//  in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////

using UnityEngine;
using System.Collections;
using GestureWorksCoreNET;
using GestureWorksCoreNET.Unity;

public class TouchImage : TouchObject {

	void Start () {
		// initalize anything else you need
	}
	
	void Update () {
		// You could add other frame-by-frame processing here. 
	}
	
	
	public void NDrag(GestureEvent gEvent){
		
		Debug.Log("NDrag called");
		
		float multiplier = 1.0f;
				
		Camera cam = Camera.main;
	
		float dX = gEvent.Values["drag_dx"]*multiplier;
		float dY = gEvent.Values["drag_dy"]*multiplier*Flipped;
			
		float screenX = gEvent.X;
		float screenY = cam.GetScreenHeight() - gEvent.Y;
		float screenZ = cam.WorldToScreenPoint(this.transform.position).z;
	
		Vector3 touchLocation = cam.ScreenToWorldPoint(new Vector3(screenX, screenY, screenZ));
			
		StartAffineTransform(touchLocation);
			
			Vector3 previousPosition = Camera.main.WorldToScreenPoint(AffineTransform.transform.position);
			Vector3 nextPosition = new Vector3(dX, dY, 0.0f);
			Vector3 newPosition = previousPosition + nextPosition;
			AffineTransform.transform.position = Camera.main.ScreenToWorldPoint(newPosition);
	
		EndAffineTransform();	
	}
		
	public void NRotate(GestureEvent gEvent){
			
		float multiplier = 1.0f;//0.5f;
		Camera cam = Camera.main;
			
		float dTheta = gEvent.Values["rotate_dtheta"]*multiplier;
		
		float screenX = gEvent.X;
		float screenY = cam.GetScreenHeight() - gEvent.Y;
		float screenZ = cam.WorldToScreenPoint(this.transform.position).z;
		
		Vector3 touchLocation = cam.ScreenToWorldPoint(new Vector3(screenX, screenY, screenZ));
			
		//StartAffineTransform(touchLocation);
			
			transform.Rotate(0, dTheta, 0);
			
		//EndAffineTransform();
			
	}
		
	public void NScale(GestureEvent gEvent){
			
		float multiplier = 1.0f;
				
		Camera cam = Camera.main;
		
		float scaleDX = gEvent.Values["scale_dsx"]*multiplier;
		float scaleDY = gEvent.Values["scale_dsy"]*multiplier;
			
		float screenX = gEvent.X;
		float screenY = cam.GetScreenHeight() - gEvent.Y;
		float screenZ = cam.WorldToScreenPoint(this.transform.position).z;
	
		Vector3 touchLocation = cam.ScreenToWorldPoint(new Vector3(screenX, screenY, screenZ));
			
		StartAffineTransform(touchLocation);
			
			AffineTransform.transform.localScale += new Vector3(scaleDX, scaleDY, scaleDY);
			
		EndAffineTransform();
			
	}
		
	public void ThreeFingerTilt(GestureEvent gEvent){
		Camera cam = Camera.main;
				
		float tiltDX = gEvent.Values["tilt_dx"];
		float tiltDY = gEvent.Values["tilt_dy"]*-1;
			
		float screenX = gEvent.X;
		float screenY = cam.GetScreenHeight() - gEvent.Y;
		float screenZ = cam.WorldToScreenPoint(this.transform.position).z;
		
		Vector3 touchLocation = cam.ScreenToWorldPoint(new Vector3(screenX, screenY, screenZ));
			
		//StartAffineTransform(touchLocation);
				
			transform.Rotate(0,0 , tiltDX*Flipped);
			transform.Rotate(tiltDY,0 , 0);			
			
		//EndAffineTransform();
	}

}
