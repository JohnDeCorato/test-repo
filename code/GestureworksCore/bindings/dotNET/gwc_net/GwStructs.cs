////////////////////////////////////////////////////////////////////////////////
//
//  IDEUM
//  Copyright 2011-2013 Ideum
//  All Rights Reserved.
//
//  Gestureworks Core
//
//  File:     GwStructs.cs
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

    #region Touch Event Structures
    /// <summary>
    /// Provides the type of touch event within a <see cref="PointEvent"/>.
    /// </summary>
    public enum TouchStatus
    {
        /// <summary>
        /// The position of the <see cref="TouchPoint"/> associated with the <see cref="PointEvent"/> has been updated.
        /// </summary>
        TOUCHUPDATE,
        /// <summary>
        /// This <see cref="TouchPoint"/> associated with a <see cref="PointEvent"/> has been added to the collection of points tracked by GestureWorksCore.
        /// </summary>
        TOUCHADDED,
        /// <summary>
        /// This <see cref="TouchPoint"/> associated with a <see cref="PointEvent"/> has been removed from the collection of points tracked by GestureWorksCore.
        /// </summary>
        TOUCHREMOVED
    }

    /// <summary>
    /// Defines the position of a touch point as tracked by GestureWorksCore.
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct TouchPoint
    {
        /// <summary>
        /// Position of the <see cref="T:TouchPoint"/> on the X axis.
        /// </summary>
        [MarshalAs(UnmanagedType.R4)]
        public float X;
        
        /// <summary>
        /// Position of the <see cref="T:TouchPoint"/> on the Y axis.
        /// </summary>
        [MarshalAs(UnmanagedType.R4)]
        public float Y;

        /// <summary>
        /// Position of the <see cref="T:TouchPoint"/> on the Z axis.
        /// </summary>
        [MarshalAs(UnmanagedType.R4)]
        public float Z;
        
        /// <summary>
        /// Width of the <see cref="T:TouchPoint"/>.
        /// </summary>
        [MarshalAs(UnmanagedType.R4)]
        public float W;
        
        /// <summary>
        /// The height of the <see cref="T:TouchPoint"/>.
        /// </summary>
        [MarshalAs(UnmanagedType.R4)]
        public float H;

        /// <summary>
        /// The radius of the <see cref="T:TouchPoint"/>.
        /// </summary>
        [MarshalAs(UnmanagedType.R4)]
        public float R;
    }

    /// <summary>
    /// The <see cref="T:PointEvent"/> provides touch point event information; this includes event type and status information via its <see cref="TouchStatus"/> and
    /// <see cref="TouchPoint"/> fields, among others. <see cref="T:PointEvent"/>s are retrieved from GestureWorks by calling <see cref="GestureWorks.ConsumePointEvents"/>,
    /// typically from within an application's draw loop (or similar).
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public class PointEvent
    {
        /// <summary>
        /// The numeric ID of the <see cref="T:PointEvent"/>.
        /// </summary>
        public int PointId;
        /// <summary>
        /// The type of touch event associated with the <see cref="T:PointEvent"/>.
        /// </summary>
        public TouchStatus Status;
        /// <summary>
        /// The <see cref="T:PointEvent"/>'s position information as represented by a <see cref="TouchPoint"/>.
        /// </summary>
        public TouchPoint Position;
        /// <summary>
        /// The timestamp of the <see cref="T:PointEvent"/>. This value is represented in clock ticks measuring elapsed processor time; the base time is arbitrary, but does not change within a single process.
        /// </summary>
        public Int32 Timestamp;
    }

    /// <summary>
    /// The <c>PointEventArrayIntermediate</c> struct is used during the construction of the publicly consumable <see cref="PointEventArray"/> and is not intended for use
    /// by <see cref="GestureWorksCoreNET"/> library users.
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    internal struct PointEventArrayIntermediate
    {
        public IntPtr Events;
        public int Size;
    }

    /// <summary>
    /// Defines the position of a point suitable for passing to Gestureworks Core for processing via the <see cref="GestureWorks.AddEvent"/> method; typically used when passing touch position information to
    /// Gestureworks Core from an external source (e.g. TUIO events).
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct TouchEvent
    {
        /// <summary>
        /// The timestamp of the <see cref="T:TouchEvent"/>.
        /// </summary>
        public Int32 Timestamp;

        /// <summary>
        /// The type of event associated with the <see cref="T:TouchEvent"/>.
        /// </summary>
        public TouchStatus Status;

        /// <summary>
        /// Unique identifier for the <see cref="T:TouchEvent"/>.
        /// </summary>
        public Int32 TouchEventID;

        /// <summary>
        /// Position of the <see cref="T:TouchEvent"/> on the X axis.
        /// </summary>
        [MarshalAs(UnmanagedType.R4)]
        public float X;

        /// <summary>
        /// Position of the <see cref="T:TouchEvent"/> on the Y axis.
        /// </summary>
        [MarshalAs(UnmanagedType.R4)]
        public float Y;

        /// <summary>
        /// Position of the <see cref="T:TouchEvent"/> on the Z axis.
        /// </summary>
        [MarshalAs(UnmanagedType.R4)]
        public float Z;

        /// <summary>
        /// Width of the <see cref="T:TouchEvent"/>.
        /// </summary>
        [MarshalAs(UnmanagedType.R4)]
        public float W;

        /// <summary>
        /// The height of the <see cref="T:TouchEvent"/>.
        /// </summary>
        [MarshalAs(UnmanagedType.R4)]
        public float H;

        /// <summary>
        /// The radius of the <see cref="T:TouchEvent"/>.
        /// </summary>
        [MarshalAs(UnmanagedType.R4)]
        public float R;

        /// <summary>
        /// The pressure of the <see cref="T:TouchEvent"/>.
        /// </summary>
        [MarshalAs(UnmanagedType.R4)]
        public float P;
    }
    #endregion

    #region Gesture Event Structures
    /// The <c>GestureEventArrayIntermediate</c> struct is used during the construction of publicly consumable <see cref="GestureEventArray"/> objects and is not intended for use
    /// by <see cref="GestureWorksCoreNET"/> library users.
    [StructLayout(LayoutKind.Sequential)]
    internal struct GestureEventArrayIntermediate
    {
        public int Size;
        public IntPtr Events;
    }

    /// The <c>GestureEventIntermediate</c> struct is used during the construction of publicly consumable <see cref="GestureEvent"/> objects and is not intended for use
    /// by <see cref="GestureWorksCoreNET"/> library users. Internally, the struct provides access to a number of GestureEvent properties not passed on to <see cref="GestureWorksCoreNET"/> users.
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    internal struct GestureEventIntermediate
    {
        /// <summary>
        /// Unique identifier for the gesture event.
        /// </summary>
        public Int32 GestureEventID;

        [MarshalAs(UnmanagedType.LPStr)]
        public string GestureType;

        /// <summary>
        /// The name a user has given for the gesture in GML
        /// </summary>
        [MarshalAs(UnmanagedType.LPStr)]
        public string GestureID;

        /// <summary>
        /// The name of the touch object which owns the gesture (as defined by registerTouchObject).
        /// </summary>
        [MarshalAs(UnmanagedType.LPStr)]
        public string Target;

        public Int32  Source;

        /// <summary>
        /// The number of touch points involved in the gesture.
        /// </summary>
        public Int32  N;

        public Int32  HoldN;

        /// <summary>
        /// X coordinate of where the gesture occurred.
        /// </summary>
        public float  X;
        /// <summary>
        /// Y coordinate of where the gesture occurred.
        /// </summary>
        public float  Y;

        public Int32  Timestamp;
        public Int32  Phase;
        public IntPtr LockedPoints;

        /// <summary>
        /// The <c>values</c> map is where relevant gesture values will be returned by name as defined in the GML.
        /// </summary>
        public MapStruct values;
    }

    /// The <c>MapStruct</c> struct is used during the construction of publicly consumable <see cref="GestureEvent"/> objects and is not intended for use
    /// by <see cref="GestureWorksCoreNET"/> library users.
    [StructLayout(LayoutKind.Sequential)]
    internal struct MapStruct
    {
        public IntPtr Names;  // ptr to array of strings
        public IntPtr Values; // ptr to array of floats
        public int Size;      // # array elements
    }
    #endregion

}
