////////////////////////////////////////////////////////////////////////////////
//
//  IDEUM
//  Copyright 2011-2013 Ideum
//  All Rights Reserved.
//
//  Gestureworks Core
//
//  File:     GestureEventArray.cs
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

namespace GestureWorksCoreNET
{
    /// <summary>
    /// Represents a collection of <see cref="GestureEvent"/> objects as obtained from <see cref="GestureWorks.ConsumeGestureEvents"/>.
    /// </summary>
    public class GestureEventArray : List<GestureEvent> { }
}
