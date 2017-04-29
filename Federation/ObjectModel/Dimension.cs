/*
RACoN - RTI abstraction component for MS.NET (RACoN)
https://sites.google.com/site/okantopcu/racon

Copyright © Okan Topçu, 2009-2017
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
// RACoN
using Racon.RtiLayer;
using Racon.RtiLayer.Native;

namespace Racon.ObjectModel
{
  /// <summary>
  /// Dimension
  /// </summary>
  public class CDimension
  {
    #region Fields
    private RtiAmb _rtiAmb;
    #endregion

    #region Properties
    /// <summary>
    /// Region name.
    /// </summary>
    public string Name { get; set; }

    /// <summary>
    /// Routing space reference. HLA13 specific.
    /// </summary>
    public CRoutingSpace Space { get; set; }

    /// <summary>
    /// Handle.
    /// </summary>
    public uint Handle
    {
      get;
      private set;
      //get
      //{
      //  if (this.RtiAmb != null)
      //    return this.RtiAmb.getDimensionHandle(this.Name, (int)Space.Handle);
      //  else return 666;
      //}
    }

    /// <summary>
    /// RtiAmb reference. If it is null, then Handle is -1.
    /// </summary>
    public RtiAmb RtiAmb
    {
      get { return _rtiAmb; }
      set 
      { 
        _rtiAmb = value;
        Handle = (_rtiAmb != null) ? _rtiAmb.getDimensionHandle(Name, Space.Handle) : 0;
        //MessageBox.Show("MSG-(DEBUG):" + Environment.NewLine + Handle, "CDimension", MessageBoxButtons.OK, MessageBoxIcon.Warning);
      }
    }
    #endregion

    #region Constructors
    /// <summary>
    /// CDimension constructor. Dimension name must be the same with the one specified in FED/FDD.
    /// </summary>
    public CDimension(String name, CRoutingSpace space)
      : base()
    {
      Name = name;
      Space = space;
      _rtiAmb = null;
    }
    #endregion

    #region Methods
    #endregion

  }
}
