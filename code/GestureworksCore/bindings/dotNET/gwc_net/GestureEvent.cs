////////////////////////////////////////////////////////////////////////////////
//
//  IDEUM
//  Copyright 2011-2013 Ideum
//  All Rights Reserved.
//
//  Gestureworks Core
//
//  File:     GestureEvent.cs
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
    /// The <see cref="T:GestureEvent"/> provides data associated with a particular gesture event as tracked by GestureWorksCore.
    /// <see cref="T:GestureEvent"/> objects are retrieved from GestureWorks by calling <see cref="GestureWorks.ConsumeGestureEvents"/>,
    /// typically from within an application's draw loop (or similar).
    /// </summary>
    public class GestureEvent
    {
        private GestureEvent() { } //Private constructor to enforce usage of CreateGestureEvent()

        /// <summary>
        /// Creates and returns a new, fully initialized instance of a <see cref="GestureEvent"/> based off of the supplied <see cref="GestureEventIntermediate"/>.
        /// </summary>
        /// <param name="intermediateGestureEvent">The raw GestureEvent as received from GestureWorksCore that contains the "non-consumer" fields.</param>
        /// <returns>Consumable <see cref="GestureEvent"/> with only consumer-accessible fields.</returns>
        internal static GestureEvent CreateGestureEvent(GestureEventIntermediate intermediateGestureEvent)
        {
            GestureEvent gestureEvent = new GestureEvent();

            gestureEvent.EventID   = intermediateGestureEvent.GestureEventID;
            gestureEvent.GestureID = intermediateGestureEvent.GestureID;
            gestureEvent.Target    = intermediateGestureEvent.Target;
            gestureEvent.N         = intermediateGestureEvent.N;
            gestureEvent.X         = intermediateGestureEvent.X;
            gestureEvent.Y         = intermediateGestureEvent.Y;
            gestureEvent.Phase     = (GesturePhase)intermediateGestureEvent.Phase;
            gestureEvent.Timestamp = intermediateGestureEvent.Timestamp;
            gestureEvent.Values    = Utils.GetDictFromMapStruct(intermediateGestureEvent.values);

            //Here we populate the LockedPoints field of the new GestureEvent if "hold_in" in exists in the
            //Values property (which indicates this is a hold event). We can then populate the LockedPoints field from the
            //pointer to the integer array by using Marshal.Copy since the number of elements in that unmanaged array will be
            //equal to this hold_in value.
            float numElements = 0;
            gestureEvent.Values.TryGetValue("hold_n", out numElements);

            if (numElements > 0)
            {
                gestureEvent.LockedPoints = new Int32[(int)numElements];

                Marshal.Copy(intermediateGestureEvent.LockedPoints, gestureEvent.LockedPoints, 0, (int)numElements);

            }
            else
            {
                gestureEvent.LockedPoints = new Int32[0];
            }
            
            return gestureEvent;
        }
        
        /// <summary>
        /// Gets the ID of the <see cref="T:GestureEvent"/>.
        /// </summary>
        public int EventID { get; private set; }
        
        /// <summary>
        /// Gets the  name of the gesture. This corresponds to gesture names as read from GML via <see cref="GestureWorks.LoadGML"/>.
        /// </summary>
        public string GestureID { get; private set; }
        
        /// <summary>
        /// Gets the  name of the touch object associated with this <see cref="T:GestureEvent"/>. This value corresponds to a touch object previously associated with a <see cref="T:GestureEvent"/> via <see cref="GestureWorks.AddGesture"/>.
        /// </summary>
        public string Target { get; private set; }
        
        /// <summary>
        /// Gets the current number of touch points that are actively associated with this <see cref="T:GestureEvent"/>.
        /// </summary>
        public int N { get; private set; }
        
        /// <summary>
        /// Gets the screen coordinate of the <see cref="T:GestureEvent"/> on the X-axis.
        /// </summary>
        public float X { get; private set; }
        
        /// <summary>
        /// Gets the screen coordinate of the <see cref="T:GestureEvent"/> on the Y-axis.
        /// </summary>
        public float Y { get; private set; }

        /// <summary>
        /// Gets the current state of the gesture.
        /// </summary>
        public GesturePhase Phase { get; private set; }
        
        /// <summary>
        /// Gets the  timestamp of the <see cref="T:GestureEvent"/>. This value is represented in clock ticks measuring elapsed processor time; the base time is arbitrary, but does not change within a single process.
        /// </summary>
        public Int32 Timestamp { get; private set; }

        /// <summary>
        /// Gets an array of TouchPoint IDs for any TouchPoints during a 'hold' <see cref="T:GestureEvent"/>. This value will be 0 if there are no locked points or if the event is not of type 'hold' or a subset thereof.
        /// </summary>
        public Int32[] LockedPoints { get; private set; }

        /// <summary>
        /// Gets the value of the property as defined by the gesture dimension for this <see cref="T:GestureEvent"/>. These properties are defined in GML and their values are computed and returned by GestureWorks.
        /// </summary>
        public Dictionary<string, float> Values { get; private set; }
    }
}
