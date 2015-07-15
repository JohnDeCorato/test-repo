////////////////////////////////////////////////////////////////////////////////
//
//  IDEUM
//  Copyright 2011-2013 Ideum
//  All Rights Reserved.
//
//  Gestureworks Core
//
//  File:     GwNative.cs
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
    /// Provides the static methods for all of the PInvoked calls into native functions exported by <c>GestureWorksCore.dll</c>.
    /// </summary>
    internal static class GwNative
    {
        /// <summary>
        /// Specifies the name of the GestureWorksCore DLL that should be used by the bindings. This is only the filename and extension, NOT the full path.
        /// Update this to reflect the name of the DLL you are passing to <see cref="GestureWorks.LoadGestureWorksDll"/>.
        /// </summary>
        private const string _dllName = "GestureworksCore32.dll";

        [DllImport(_dllName, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void initializeGestureWorks(int width, int height);

        [DllImport(_dllName, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void resizeScreen(int width, int height);

        [DllImport(_dllName, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAsAttribute(UnmanagedType.I1)]
        internal static extern bool loadGML(string filename);

        [DllImport(_dllName, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAsAttribute(UnmanagedType.I1)]
        internal static extern bool registerWindowForTouch(IntPtr hWnd);

        [DllImport(_dllName, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAsAttribute(UnmanagedType.I1)]
        internal static extern bool registerWindowForTouchByName(string window_name);

        [DllImport(_dllName, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAsAttribute(UnmanagedType.I1)]
        internal static extern bool registerTouchObject(string object_name);

        [DllImport(_dllName, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAsAttribute(UnmanagedType.I1)]
        internal static extern bool deregisterTouchObject(string object_name);

        [DllImport(_dllName, CallingConvention = CallingConvention.Cdecl)]
        [return: System.Runtime.InteropServices.MarshalAsAttribute(System.Runtime.InteropServices.UnmanagedType.I1)]
        internal static extern bool addGesture(string touch_object_name, string gesture_name);

        [DllImport(_dllName, CallingConvention = CallingConvention.Cdecl)]
        [return: System.Runtime.InteropServices.MarshalAsAttribute(System.Runtime.InteropServices.UnmanagedType.I1)]
        internal static extern bool addGestureSet(string touch_object_name, string set_name);

        [DllImport(_dllName, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAsAttribute(UnmanagedType.I1)]
        internal static extern bool removeGesture(string touch_object_name, string gesture_name);

        [DllImport(_dllName, CallingConvention = CallingConvention.Cdecl)]
        [return: System.Runtime.InteropServices.MarshalAsAttribute(System.Runtime.InteropServices.UnmanagedType.I1)]
        internal static extern bool removeGestureSet(string touch_object_name, string set_name);

        [DllImport(_dllName, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void processFrame();

        [DllImport(_dllName, CallingConvention = CallingConvention.Cdecl)]
        [return: System.Runtime.InteropServices.MarshalAsAttribute(System.Runtime.InteropServices.UnmanagedType.I1)]
        internal static extern bool enableGesture(string touch_object_name, string gesture_name);

        [DllImport(_dllName, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAsAttribute(UnmanagedType.I1)]
        internal static extern bool disableGesture(string touch_object_name, string gesture_name);

        [DllImport(_dllName, CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr consumePointEvents();

        [DllImport(_dllName, CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr consumeGestureEvents();

        [DllImport(_dllName, CallingConvention = CallingConvention.Cdecl)]
        [return: System.Runtime.InteropServices.MarshalAsAttribute(System.Runtime.InteropServices.UnmanagedType.I1)]
        internal static extern bool addTouchPoint(string touch_object_name, int point_id);

        [DllImport(_dllName, CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr addEvent(TouchEvent touchEvent);
    }
}
