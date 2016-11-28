/*
Racon - RTI abstraction component for MS.NET (Racon)
https://sites.google.com/site/okantopcu/racon

Copyright © Okan Topçu, 2009-2016
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

namespace Racon.Federation
{
  /// <summary>
  /// The federation package (Federation) is the extension library for the simulations.
  /// </summary>
  /// 
  /// <remarks>The library implements a federation execution and a generic federate, which is an extension point inherited by each federate (i.e. the simulation ap-plication-specific federate). The simulation layer classes directly include or extend the classes found in the federation package.
  /// </remarks>
  /// 
  internal class NamespaceDoc
  {
    // Only for documentation of Namespace: Racon.Federation in Sandcastle
  }
  /// <summary>
  /// Represents a federation execution.
  /// </summary>
  /// 
  /// <remarks>This class captures the all federation execution related data such as the name of the federation execution from the view of a joined federate.
  /// </remarks>
  public class CFederationExecution
	{
		#region Properties
    /// <summary>
    /// Federation execution name
    /// </summary>
    public string Name { get; set; }

    /// <summary>
    /// FDD (Federation Document Data) / FED (Federation Execution Data) file name with full path
    /// </summary>
    public string FDD { get; set; }

    /// <summary>
    /// Name of the Joined Federate in the Federation Execution
    /// </summary>
    public string FederateName { get; set; }

    /// <summary>
    /// Type of the Joined Federate in the Federation Execution
    /// </summary>
    public string FederateType { get; set; }

    /// <summary>
    /// Connection settings - localSettingsDesignator in Connect() call
    /// </summary>
    public string ConnectionSettings { get; set; }

    #endregion

    #region Constructors
    /// <summary>
    /// Constructor
    /// </summary>
    public CFederationExecution()
		{
      Name = "";
      FDD = "";
      FederateName = "";
      FederateType = "";
      ConnectionSettings = "";
		}
    #endregion
  }
}
