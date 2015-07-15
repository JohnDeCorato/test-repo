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
	
	private string dllFileName;
	private string gmlFileName;

	private bool DllLoaded; 
	private bool GmlLoaded;
	private bool WindowLoaded;
	private PointEventArray pEvents;
	
	private Dictionary<int, TouchCircle> TouchPoints;
	private GestureEventArray gEvents;
	private List<TouchObject> GestureObjects;
	private HitManager HitManager;

	// Use this for initialization
	void Start () {

		dllFileName = "GestureworksCore32.dll";
		gmlFileName = "my_gestures.gml";
		
		if(Application.isEditor==true){
			windowName = "Unity - MyScene.unity - 2-Finished - PC, Mac & Linux Standalone*";
			dllFilePath = Application.dataPath.Replace("/", "\\")+"\\Plugins\\GestureWorks\\Core\\"+dllFileName;
			gmlFilePath = Application.dataPath.Replace("/", "\\")+"\\MyScripts\\"+gmlFileName;
		} else {
			// Running exe 
			windowName = "InteractiveClock";
			dllFilePath = Application.dataPath.Replace("/", "\\")+"\\"+dllFileName;
			gmlFilePath = Application.dataPath.Replace("/", "\\")+"\\"+gmlFileName;
		}
		
		Core = new GestureWorks();
		DllLoaded = Core.LoadGestureWorksDll(dllFilePath);
		Debug.Log("DllLoaded: "+DllLoaded);
		
		Core.InitializeGestureWorks(Screen.width, Screen.height);
		GmlLoaded = Core.LoadGML(gmlFilePath);
		WindowLoaded = Core.RegisterWindowForTouchByName(windowName);
		
		TouchPoints = new Dictionary<int, TouchCircle>();

		Debug.Log("Interactive Clock");
		Debug.Log("Is DLL Loaded: " + DllLoaded);
		Debug.Log("lIs GML Loaded: " + GmlLoaded.ToString());
		Debug.Log("Is Window Loaded: " + WindowLoaded.ToString());
		
		GestureObjects = new List<TouchObject>();
		HitManager = new HitManager(Camera.main);
		InitializeGestureObjects();

	}
	
	private void InitializeGestureObjects(){
		GameObject[] objects = FindObjectsOfType(typeof(GameObject)) as GameObject[];
        foreach (GameObject obj in objects) {
        	TouchObject script = obj.GetComponent<TouchObject>();
			if(script!=null){
				Debug.Log("I found "+script.GetObjectName()+" a TouchObject.");
				// register object with core
				Core.RegisterTouchObject(script.GetObjectName());
				// and register gestures
				foreach(string gesture in script.SupportedGestures){
					Core.AddGesture(script.GetObjectName(), gesture);
				}
				// keep references to TouchObjects in scene
				GestureObjects.Add(script);
			}
        }
	}

	
	// Update is called once per frame
	void Update () {

		Core.ProcessFrame();
        pEvents = Core.ConsumePointEvents();
		
		if (Input.GetKey ("escape")) {
        	Application.Quit();
		}

		
		//string output;
		
		/*		
		foreach (PointEvent pEvent in pEvents)
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
					bool touchPointHitSomething = false;
					foreach(TouchObject obj in GestureObjects){
						if(HitManager.DetectHit(pEvent.Position.X, Camera.main.GetScreenHeight()-pEvent.Position.Y, obj.gameObject)){
							Core.AddTouchPoint(obj.GetObjectName(), pEvent.PointId);
							
							touchPointHitSomething = true;
							//Debug.Log("I hit "+obj.GetObjectName()+" pEvent.PointId"+pEvent.PointId);
						} 
					}
					if(touchPointHitSomething==false){
						Core.AddTouchPoint(Camera.main.name, pEvent.PointId);
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
		
		gEvents = Core.ConsumeGestureEvents();
		string activeGestures = "";
		string activeGestureTargets = "";
		string o;
				
		if(gEvents!=null){
			
			foreach (GestureEvent gEvent in gEvents){
			
				#region Test Console Output
		        o = "-----------------------------\r\n";
		        o += gEvent.ToString();
		        o += "EventID: "   + gEvent.EventID;
		        o += "\r\n GestureID: " + gEvent.GestureID;
		        o += "\r\n Target: "    + gEvent.Target;
		        o += "\r\n N: "         + gEvent.N.ToString();
		        o += "\r\n X: "         + gEvent.X.ToString();
		        o += "\r\n Y: "         + gEvent.Y.ToString();
		        o += "\r\n Timestamp: " + gEvent.Timestamp.ToString();
		        o += "\r\n Locked Points: " + gEvent.LockedPoints.GetLength(0).ToString();
				o += "\r\n";
						
				//Debug.Log(o);
			    #endregion
			
				//activeGestures += gEvent.GestureID + ", "; // for debugging
				//activeGestureTargets += gEvent.Target + ", "; // for debugging
					
				// send gesture events to all subscribers
				foreach(TouchObject obj in GestureObjects){
					//send events only to corresponding registered touch objects
					//Debug.Log(gEvent.GestureID+" gesture and for "+gEvent.Target);
					if(obj.name==gEvent.Target){
						//Debug.Log(gEvent.GestureID+" gesture found for "+gEvent.Target);
						obj.SendMessage(gEvent.GestureID, gEvent);
					}
				}
			}
		} else {
			//Debug.Log("gEvents null");
		}
	}
}
