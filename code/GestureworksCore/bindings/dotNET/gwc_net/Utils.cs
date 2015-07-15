////////////////////////////////////////////////////////////////////////////////
//
//  IDEUM
//  Copyright 2011-2013 Ideum
//  All Rights Reserved.
//
//  Gestureworks Core
//
//  File:     Utils.cs
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
using System.IO;
using System.ComponentModel;

namespace GestureWorksCoreNET
{
    /// <summary>
    /// Provides various static utility methods useful when working with the <c>GestureWorksCore.dll</c> and its functions.
    /// </summary>
    internal static class Utils
    {
        /// <summary>
        /// Loads the specified DLL into memory.
        /// </summary>
        /// <param name="path">The full path including the filename and extension to <c>GestureWorksCore.dll</c></param>
        /// <returns>Value indicating whether the setting of the search path was successful.</returns>
        internal static bool LoadDll(string path)
        {
            string directory = String.Empty;
            
            if (File.Exists(path))
            {
                if (LoadLibrary(path) != null)
                    return true;
                else
                {
                    int errorCode = Marshal.GetLastWin32Error();

                    Console.WriteLine("Unable to load library due to: " + new Win32Exception(errorCode).Message.ToUpper());

                    throw new InvalidOperationException("Unable to load library due to: " + new Win32Exception(errorCode).Message.ToUpper());
                }
            }
            else
            {
                Console.WriteLine("Library not found at the specified location: " + path);

                throw new FileNotFoundException("Library not found at the specified location.", path);
            }
        }

        /// <summary>
        /// Loads the specified module into the address space of the calling process. The specified module may cause other modules to be loaded.
        /// </summary>
        /// <param name="dllName">The full path including the filename and extension to <c>GestureWorksCore.dll</c></param>
        /// <returns>If the function succeeds, the return value is a handle to the module. If the function fails, the return value is <c>NULL</c>.</returns>
        [DllImport("kernel32", CharSet = CharSet.Unicode, SetLastError = true)]
        private extern static IntPtr LoadLibrary(string dllName);

        /// <summary>
        /// Returns a Dictionary of string keys and float values from the supplied <see cref="MapStruct"/>.
        /// </summary>
        /// <param name="mapStruct">The <see cref="MapStruct"/> from which to construct the Dictionary.</param>
        /// <returns>A Dictionary generated from the supplied <see cref="MapStruct"/> whose keys are strings and values are floats.</returns>
        internal static Dictionary<string, float> GetDictFromMapStruct(MapStruct mapStruct)
        {
            Dictionary<string, float> dict = new Dictionary<string, float>();

            if (mapStruct.Size > 0)
            {
                IntPtr namePtr = mapStruct.Names;
                IntPtr valPtr  = mapStruct.Values;

                float[] values = new float[mapStruct.Size];

                Marshal.Copy(valPtr, values, 0, mapStruct.Size);
                Marshal.FreeHGlobal(valPtr);

                for (int i = 0; i < mapStruct.Size; i++)
                {
                    IntPtr strPtr = (IntPtr)Marshal.PtrToStructure(namePtr, typeof(IntPtr));
                    Marshal.DestroyStructure(strPtr, typeof(IntPtr));

                    dict.Add(Marshal.PtrToStringAnsi(strPtr), values[i]);

                    namePtr = new IntPtr(namePtr.ToInt64() + IntPtr.Size);
                }
            }

            return dict;
        }

    }
}
