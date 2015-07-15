////////////////////////////////////////////////////////////////////////////////
//
//  IDEUM
//  Copyright 2011-2013 Ideum
//  All Rights Reserved.
//
//  Gestureworks Core
//
//  File:     PointEventArray.cs
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
    /// Represents a collection of <see cref="PointEvent"/> objects as obtained from <see cref="GestureWorks.ConsumePointEvents"/>.
    /// </summary>
    /// <example>
    /// <code lang="c#">
    /// PointEventArray pointEvents = initializedGestureWorks.ConsumePointEvents();
    /// foreach (PointEvent pEvent in pointEvents)
    /// {        
    ///     Console.WriteLine(pEvent.PointId.ToString());
    /// }
    /// </code>
    /// </example>
    public class PointEventArray : List<PointEvent> { }
}
