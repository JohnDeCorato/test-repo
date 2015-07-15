////////////////////////////////////////////////////////////////////////////////
//
//  IDEUM
//  Copyright 2011-2013 Ideum
//  All Rights Reserved.
//
//  Gestureworks Core
//
//  File:     GestureWorks.cs
//  Authors:  Ideum
//
//  NOTICE: Ideum permits you to use, modify, and distribute this file only
//  in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace GestureWorksCoreNET
{
    /// <summary>
    /// The <see cref="GestureWorks"/> object is the primary facility for interacting with the <c>GestureWorksCore.dll</c> native library from within .NET applications.
    /// </summary>
    /// <example>
    /// Creating a fully initialized <see cref="GestureWorks"/> object is performed as follows (parameter values should be changed as necessary).
    /// <code lang="c#">
    /// GestureWorks gw = new GestureWorks();
    /// 
    /// gw.LoadGestureWorksDll("C:\\path\\to\\GestureWorksCore.dll");
    /// gw.InitializeGestureWorks(1920, 1080);
    /// gw.LoadGML("C:\\path\\to\\my_gestures.gml");
    /// gw.RegisterWindowForTouchByName("My Touch Application");
    /// </code>
    /// If each of the above methods have returned <c>true</c>, the application may now make use of the various <see cref="GestureWorks"/> methods for processing touch events.
    /// </example>
    public class GestureWorks
    {
        #region Init Methods
        
        /// <summary>
        /// Loads the <c>GestureWorksCore.dll</c> library, providing access to the exported functions found in <c>GestureWorksCore.dll</c> native library.
        /// This should be the first method called during the initialization phase of <c>GestureWorksCore</c> interaction, and should be followed by a call to <see cref="InitializeGestureWorks"/>.
        /// Note that the name of the DLL must also be updated in <c>GwNative.cs</c>.
        /// </summary>
        /// <param name="gestureworksDllPath">The full path (including filename and extension) to the <c>GestureWorksCore.dll</c> file.</param>
        /// <returns>Value indicating whether the load operation was successful.</returns>
        public bool LoadGestureWorksDll(string gestureworksDllPath)
        {
            if (!String.IsNullOrEmpty(gestureworksDllPath))
                IsLoaded = Utils.LoadDll(gestureworksDllPath);
            else
                IsLoaded = false;

            return IsLoaded;
        }

        /// <summary>
        /// Sets the screen dimensions and instructs <c>GestureWorksCore</c> to perform various initialization functions for point, cluster, and gesture data.
        /// This should be the second method called during the initialization phase of <c>GestureWorksCore</c> interaction, and should be followed by calls
        /// to <see cref="LoadGML"/> and <see cref="RegisterWindowForTouchByName"/>.
        /// </summary>
        /// <param name="screenWidth">The width of the screen resolution in pixels.</param>
        /// <param name="screenHeight">The height of the screen resolution in pixels.</param>
        public void InitializeGestureWorks(int screenWidth, int screenHeight)
        {
            if (IsLoaded)
            {
                if (screenWidth > 0 && screenHeight > 0)
                {
                    GwNative.initializeGestureWorks(screenWidth, screenHeight);

                    IsInitialized = true;
                }
                else
                    throw new ArgumentException("Screen width and height cannot be null and must be greater than zero.");
            }
            else
                throw new InvalidOperationException("GestureWorksCore.dll has not been loaded; LoadGestureWorksDll must be called before calling InitializeGestureWorks.");
        }

        /// <summary>
        /// Loads and parses the specified GestureML (GML) file containing the gesture information to be tracked and processed by <c>GestureWorksCore</c>.
        /// This should be called after <see cref="InitializeGestureWorks"/> during the initialization phase of <c>GestureWorksCore</c> interaction.
        /// </summary>
        /// <param name="pathToGML">The full path (including filename and extension) to the GML file for GestureWorks.</param>
        /// <returns>Value indicating whether the specified GML file was successfully loaded by GestureWorks.</returns>
        public bool LoadGML(string pathToGML)
        {
            if (!String.IsNullOrEmpty(pathToGML) && System.IO.File.Exists(pathToGML))
            {
                if (IsInitialized)
                    IsGmlLoaded = GwNative.loadGML(pathToGML);
                else
                    throw new InvalidOperationException("GestureWorks has not been properly initialized; InitializeGestureWorks must be called before calling LoadGML.");
            }
            else
            {
                if (String.IsNullOrEmpty(pathToGML))
                    pathToGML = "NO FILE SPECIFIED";

                throw new System.IO.FileNotFoundException("The specified GML was not found; please specify the full path (including filename and extension) to a valid GML file.", pathToGML);
            }

            return IsGmlLoaded;
        }

        /// <summary>
        /// Registers the window for which GestureWorks should process gesture events.
        /// This should be called after <see cref="InitializeGestureWorks"/> during the initialization phase of <c>GestureWorksCore</c> interaction.
        /// </summary>
        /// <param name="hWnd">The <c>HWND</c> of the window for which GestureWorks should process gesture events.</param>
        /// <returns>Value indicating whether the specified window was successfully registered.</returns>
        public bool RegisterWindowForTouch(IntPtr hWnd)
        {
            if (IsInitialized)
                IsWindowRegistered = GwNative.registerWindowForTouch(hWnd);
            else
                throw new InvalidOperationException("GestureWorks has not been properly initialized; InitializeGestureWorks must be called before calling RegisterWindowForTouch.");

            return IsWindowRegistered;
        }

        /// <summary>
        /// Registers the window for which GestureWorks should process gesture events.
        /// This should be called after <see cref="InitializeGestureWorks"/> during the initialization phase of <c>GestureWorksCore</c> interaction.
        /// </summary>
        /// <param name="windowName">The main title (caption) of the window for which GestureWorks should process gesture events.</param>
        /// <returns>Value indicating whether the specified window was successfully registered.</returns>
        public bool RegisterWindowForTouchByName(string windowName)
        {
            if (IsInitialized)
                IsWindowRegistered = GwNative.registerWindowForTouchByName(windowName);
            else
                throw new InvalidOperationException("GestureWorks has not been properly initialized; InitializeGestureWorks must be called before calling RegisterWindowForTouchByName.");

            return IsWindowRegistered;
        }

        #endregion

        /// <summary>
        /// Registers the specified object with GestureWorks for touch processing.
        /// </summary>
        /// <param name="touchObjectName">The name of the object that should be registered for GestureWorks touch processing.</param>
        /// <returns>Value indicating whether the specified touch object was successfully registered.</returns>
        public bool RegisterTouchObject(string touchObjectName)
        {
            return GwNative.registerTouchObject(touchObjectName);
        }
        
        /// <summary>
        /// Removes the specified object from the collection of objects managed by GestureWorks for touch processing.
        /// </summary>
        /// <param name="touchObjectName">The name of the object that should be deregistered from GestureWorks touch processing.</param>
        /// <returns>Value indicating whether the specified touch object was successfully deregistered.</returns>
        public bool DeregisterTouchObject(string touchObjectName)
        {
            return GwNative.deregisterTouchObject(touchObjectName);
        }
        
        /// <summary>
        /// Adds and enables the specified gesture to those associated with the specified touch object.
        /// </summary>
        /// <param name="touchObjectName">The name of the object registered with GestureWorks for touch processing. The object should previously have been registered with GestureWorks via a call to <see cref="RegisterTouchObject"/>.</param>
        /// <param name="gestureName">The name of the gesture (from GML) that should be associated with the touch object.</param>
        /// <returns>Value indicating whether the specified touch object was successfully associated with the specified gesture.</returns>
        public bool AddGesture(string touchObjectName, string gestureName)
        {
            return GwNative.addGesture(touchObjectName, gestureName);
        }

        /// <summary>
        /// Adds and enables the specified gesture set to those associated with the specified touch object.
        /// </summary>
        /// <param name="touchObjectName">The name of the object registered with GestureWorks for touch processing. The object should previously have been registered with GestureWorks via a call to <see cref="RegisterTouchObject"/>.</param>
        /// <param name="gestureSetName">The name of the gesture set that should be associated with the touch object.</param>
        /// <returns>Value indicating whether the specified touch object was successfully associated with the specified gesture set.</returns>
        public bool AddGestureSet(string touchObjectName, string gestureSetName)
        {
            return GwNative.addGestureSet(touchObjectName, gestureSetName);
        }

        /// <summary>
        /// Removes the specified gesture from those associated with the specified touch object.
        /// </summary>
        /// <param name="touchObjectName">The name of the object registered with GestureWorks for touch processing.</param>
        /// <param name="gestureName">The name of the gesture (from GML) that should be disassociated from the touch object.</param>
        /// <returns>Value indicating whether the specified touch object was successfully disassociated from the specified gesture.</returns>
        public bool RemoveGesture(string touchObjectName, string gestureName)
        {
            return GwNative.removeGesture(touchObjectName, gestureName);
        }

        /// <summary>
        /// Enables the specified gesture for the specified touch object.
        /// </summary>
        /// <param name="touchObjectName">The name of the object registered with GestureWorks for touch processing.</param>
        /// <param name="gestureName">The name of the gesture (from GML) that should be ensabled for the touch object.</param>
        /// <returns>Value indicating whether the specified gesture was successfully enabled for the specified touch object.</returns>
        public bool EnableGesture(string touchObjectName, string gestureName)
        {
            return GwNative.enableGesture(touchObjectName, gestureName);
        }

        /// <summary>
        /// Disables the specified gesture for the specified touch object.
        /// </summary>
        /// <param name="touchObjectName">The name of the object registered with GestureWorks for touch processing.</param>
        /// <param name="gestureName">The name of the gesture (from GML) that should be disabled for the touch object.</param>
        /// <returns>Value indicating whether the specified gesture was successfully disabled for the specified touch object.</returns>
        public bool DisableGesture(string touchObjectName, string gestureName)
        {
            return GwNative.disableGesture(touchObjectName, gestureName);
        }

        /// <summary>
        /// Updates GestureWorks with new screen dimensions.
        /// </summary>
        /// <param name="screenWidth">The width of the screen resolution in pixels.</param>
        /// <param name="screenHeight">The height of the screen resolution in pixels.</param>
        public void ResizeScreen(int screenWidth, int screenHeight)
        {
            GwNative.resizeScreen(screenWidth, screenHeight);
        }

        /// <summary>
        /// Instructs GestureWorks to perform all pipeline processing tasks such as point, cluster, and gesture updating as well as temporal analysis.
        /// Typically called from within in an application's draw loop.
        /// </summary>
        public void ProcessFrame()
        {
            GwNative.processFrame();
        }

        /// <summary>
        /// Adds the point represented by the specified point ID to the specified touch object.
        /// </summary>
        /// <param name="touchObjectName">The name of the object registered with GestureWorks for touch processing.</param>
        /// <param name="pointId">The name of the gesture (from GML) that should be associated with the touch object.</param>
        /// <returns>Value indicating whether the specified touch object was successfully associated with the specified touch point.</returns>
        public bool AddTouchPoint(string touchObjectName, int pointId)
        {
            return GwNative.addTouchPoint(touchObjectName, pointId);
        }

        /// <summary>
        /// Adds the specified <see cref="TouchEvent"/> to Gestureworks' processing pipeline; typically used for passing touch position information to
        /// Gestureworks as obtained from external sources (e.g. TUIO).
        /// </summary>
        /// <param name="touchEvent">The <see cref="TouchEvent"/> to pass to Gestureworks for touch processing.</param>
        public void AddEvent(TouchEvent touchEvent)
        {
            GwNative.addEvent(touchEvent);
        }

        /// <summary>
        /// Obtains all currently tracked touch point events from <c>GestureWorksCore</c>, represented by a collection of <see cref="PointEvent"/> objects.
        /// </summary>
        /// <returns>A <see cref="PointEventArray"/> containing any point events as held by GestureWorks.</returns>
        public PointEventArray ConsumePointEvents()
        {
            PointEventArray points = new PointEventArray();

            IntPtr eventArrayPtr = GwNative.consumePointEvents();
            PointEventArrayIntermediate eventArray = (PointEventArrayIntermediate)Marshal.PtrToStructure(eventArrayPtr, typeof(PointEventArrayIntermediate));
            Marshal.DestroyStructure(eventArrayPtr, typeof(PointEventArrayIntermediate));

            IntPtr eventPtr = eventArray.Events;

            for (int i = 0; i < eventArray.Size; i++)
            {
                points.Add((PointEvent)Marshal.PtrToStructure((IntPtr)eventPtr, typeof(PointEvent)));
                Marshal.DestroyStructure(eventPtr, typeof(PointEvent));
                eventPtr = new IntPtr(eventPtr.ToInt64() + Marshal.SizeOf(typeof(PointEvent)));
            }

            return points;
        }
          
        /// <summary>
        /// Obtains all currently tracked gesture events from <c>GestureWorksCore</c>, represented by a collection of <see cref="GestureEvent"/> objects.
        /// </summary>
        /// <returns>A <see cref="GestureEventArray"/> containing any gesture events currently tracked by <c>GestureWorksCore</c>.</returns>
        public GestureEventArray ConsumeGestureEvents()
        {
            GestureEventArray gestures = new GestureEventArray();

            IntPtr eventArrayPtr = GwNative.consumeGestureEvents();
            GestureEventArrayIntermediate eventArray = (GestureEventArrayIntermediate)Marshal.PtrToStructure(eventArrayPtr, typeof(GestureEventArrayIntermediate));

            IntPtr eventPtr = eventArray.Events;

            Marshal.DestroyStructure(eventArrayPtr, typeof(GestureEventArrayIntermediate));

            for (int i = 0; i < eventArray.Size; i++)
            {
                GestureEventIntermediate gEventIntermediate = (GestureEventIntermediate)Marshal.PtrToStructure((IntPtr)eventPtr, typeof(GestureEventIntermediate));
                gestures.Add(GestureEvent.CreateGestureEvent(gEventIntermediate));
                Marshal.DestroyStructure(eventPtr, typeof(GestureEventIntermediate));
                eventPtr = new IntPtr(eventPtr.ToInt64() + Marshal.SizeOf(typeof(GestureEventIntermediate)));
            }

            return gestures;
        }

        #region Public Properties
        /// <summary>
        /// Gets a value indicating whether the <c>GestureWorksCore.dll</c> has been successfully loaded by a call to <see cref="LoadGestureWorksDll"/>.
        /// </summary>
        public bool IsLoaded { get; private set; }

        /// <summary>
        /// Gets a value indicating whether this <see cref="GestureWorks"/> object has been successfully initialized by a call to <see cref="InitializeGestureWorks"/>. 
        /// </summary>
        public bool IsInitialized { get; private set; }

        /// <summary>
        /// Gets a value indicating whether gestures contained in a properly formatted GestureML (GML) file have been successfully loaded by a call to <see cref="LoadGML"/>. 
        /// </summary>
        public bool IsGmlLoaded { get; private set; }

        /// <summary>
        /// Gets a value indicating whether a window name has been successfully registered by a call to <see cref="RegisterWindowForTouchByName"/>. 
        /// </summary>
        public bool IsWindowRegistered { get; private set; }
        #endregion
    }
}
