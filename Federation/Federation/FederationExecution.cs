/*
Racon - RTI abstraction component for MS.NET (Racon)
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

namespace Racon
{
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

    private string _FDD;
    /// <summary>
    /// FDD (Federation Document Data) / FED (Federation Execution Data) file name with full path
    /// </summary>
    public string FDD
    {
      get { return _FDD; }
      set
      {
        _FDD = value;
        FomModules.Add(_FDD);
      }
    }

    /// <summary>
    /// Fom Modules
    /// </summary>
    public List<string> FomModules { get; set; }

    /// <summary>
    /// Name of the Joined Federate in the Federation Execution
    /// </summary>
    public string FederateName { get; set; }

    /// <summary>
    /// Type of the Joined Federate in the Federation Execution
    /// </summary>
    public string FederateType { get; set; }

    /// <summary>
    /// Connection settings - local Settings Designator in Connect() call
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
      _FDD = "";
      FederateName = "";
      FederateType = "";
      ConnectionSettings = "";
      FomModules = new List<string>();
    }
    #endregion

    #region Methods
    #endregion

  }
}
