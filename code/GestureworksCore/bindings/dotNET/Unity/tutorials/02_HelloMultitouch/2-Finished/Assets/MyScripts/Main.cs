////////////////////////////////////////////////////////////////////////////////
//
//  IDEUM
//  Copyright 2011-2013 Ideum
//  All Rights Reserved.
//
//  Gestureworks Core
//
//  File:    Main.cs
//  Authors:  Ideum
//
//  NOTICE: Ideum permits you to use, modify, and distribute this file only
//  in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////


using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using GestureWorksCoreNET;
using GestureWorksCoreNET.Unity;

public class Main : MonoBehaviour {
	
	private GestureWorks Core;

	private string windowName;
	
	private string dllFilePath;
	private string gmlFilePath;

	private bool DllLoaded; 
	private bool GmlLoaded;
	private bool WindowLoaded;
	private PointEventArray pEvents;
	
	private Dictionary<int, TouchCircle> TouchPoints;

	// Use this for initialization
	void Start () {

		if(Application.isEditor==true)
		{
			windowName = "Unity - MyScene.unity - 2-Finished - PC, Mac & Linux Standalone*";
		} else {
			// Running exe 
			windowName = "HelloMultitouch";
		}
		                          
		dllFilePath = "C:\\gwdistro\\release\\1.0\\GestureworksCore\\bindings\\dotNET\\Unity\\tutorials\\02_HelloMultitouch\\2-Finished\\Assets\\Plugins\\Gestureworks\\Core\\GestureworksCore32.dll";
		gmlFilePath = "C:\\gwdistro\\release\\1.0\\GestureworksCore\\bindings\\dotNET\\Unity\\tutorials\\02_HelloMultitouch\\2-Finished\\Assets\\MyScripts\\my_gestures.gml";
		Debug.Log(dllFilePath);
		Core = new GestureWorks();
		DllLoaded = Core.LoadGestureWorksDll(dllFilePath);
		Debug.Log("DllLoaded: "+DllLoaded);
		
		Core.InitializeGestureWorks(Screen.width, Screen.height);
		GmlLoaded = Core.LoadGML(gmlFilePath);
		WindowLoaded = Core.RegisterWindowForTouchByName(windowName);
		
		TouchPoints = new Dictionary<int, TouchCircle>();

		Debug.Log("Hello Multitouch");
		Debug.Log("Is DLL Loaded: " + DllLoaded);
		Debug.Log("lIs GML Loaded: " + GmlLoaded.ToString());
		Debug.Log("Is Window Loaded: " + WindowLoaded.ToString());

	}
	
	// Update is called once per frame
	void Update () {

		Core.ProcessFrame();
        pEvents = Core.ConsumePointEvents();
		
		//string output;
		
/*		foreach (PointEvent pEvent in pEvents)
		{
			if (pEvent.Status == TouchStatus.TOUCHADDED){
				
				output = "TOUCHADDED-----------------------------\r\n";
				output += "Point ID:  " +    pEvent.PointId.ToString();
				output += "\r\n X: " +           pEvent.Position.X.ToString();
				output += "\r\n Y: " +            pEvent.Position.Y.ToString();
				output += "\r\n W: " +            pEvent.Position.W.ToString();
				output += "\r\n H: " +            pEvent.Position.H.ToString();
				output += "\r\n Z: " +            pEvent.Position.Z.ToString();
				output += "\r\n Touch Status: " + pEvent.Status.ToString();
				output += "\r\n Timestamp: \r\n" +    pEvent.Timestamp.ToString();
				
				Debug.Log(output);
			}
			
		}
*/
		
		if(pEvents!=null){

			foreach(PointEvent pEvent in pEvents){
				if(pEvent.Status == TouchStatus.TOUCHADDED){
					if(!TouchPoints.ContainsKey(pEvent.PointId)){
						TouchPoints.Add(pEvent.PointId, new TouchCircle(pEvent.PointId, pEvent.Position.X, pEvent.Position.Y));
					}
				}
				if(pEvent.Status == TouchStatus.TOUCHREMOVED){
					if(TouchPoints.ContainsKey(pEvent.PointId)){
						TouchPoints[pEvent.PointId].RemoveRing();
						TouchPoints.Remove(pEvent.PointId);
					}
					
				}
				if(pEvent.Status == TouchStatus.TOUCHUPDATE){
					if(TouchPoints.ContainsKey(pEvent.PointId)){
						TouchPoints[pEvent.PointId].Update(pEvent.Position.X, pEvent.Position.Y);
					}
				}
			}
		}
		
	}
}
