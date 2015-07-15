////////////////////////////////////////////////////////////////////////////////
//
//  IDEUM
//  Copyright 2011-2013 Ideum
//  All Rights Reserved.
//
//  Gestureworks Core
//
//  File:    TouchMinuteHand.cs
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

public class TouchMinuteHand : TouchObject {
	
	public Transform HourHand;

	void Start () {
		// initalize anything else you need
	}
	
	void Update () {
		// You could add other frame-by-frame processing here. 
	}
		
	public void NRotate(GestureEvent gEvent){
		float dTheta = gEvent.Values["rotate_dtheta"];
		transform.Rotate(0, 0, dTheta);
		HourHand.transform.Rotate(0, 0, dTheta/12);
	}

}