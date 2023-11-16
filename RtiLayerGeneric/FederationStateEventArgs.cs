﻿/*
RACoN - RTI abstraction component for MS.NET (RACoN)
https://sites.google.com/site/okantopcu/racon

Copyright © Okan Topçu, 2009-2019
otot.support@outlook.com

This program is free software : you can redistribute it and / or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.If not, see <http://www.gnu.org/licenses/>.
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Racon.RtiLayer
{
  /// <summary>
  /// Encapsulates event arguments for Federation State changed event.
  /// </summary>
  /// <remarks>This class inherits the EventArgs class. It is used in delegates and event handlers.
  /// </remarks>
  /// 
  public class CFederationStateEventArgs : EventArgs
	{
    /// <summary>
    /// Federation Execution State
    /// </summary>
    public FederationExecutionStates FedExecState;
    /// <summary>
    /// Constructor
    /// </summary>
    /// <param name="state">Federation State</param>
    /// 
    public CFederationStateEventArgs(FederationExecutionStates state)
		{
			FedExecState = state;
		}
	};
}
