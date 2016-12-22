/*
RACoN - RTI abstraction component for MS.NET (RACoN)
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
using System.ComponentModel;
// RACoN
using Racon.RtiLayer;
using Racon.RtiLayer.Native;

namespace Racon.ObjectModel
{
  /// <summary>
  /// Represents a routing space. HLA3 specific.
  /// </summary>
  public class CRoutingSpace 
  {
    #region Fields
    private CRtiAmb _rtiAmb;
    /// <summary>
    /// Dimension list.
    /// </summary>
    public BindingList<CDimension> Dimensions;
   
    /// <summary>
    /// Region list.
    /// </summary>
    public BindingList<CRegion> Regions;
    #endregion

    #region Properties
    /// <summary>
    /// Routing space name.
    /// </summary>
    public string Name { get; set; }
    /// <summary>
    /// Routing space handle.
    /// </summary>
    public uint Handle
    {
      get;
      set;
      //get { return (this.RtiAmb != null)?this.RtiAmb.getSpaceHandle(this.Name):-1; }
    }
    /// <summary>
    /// RtiAmb reference.
    /// </summary>
    public CRtiAmb RtiAmb
    {
      get { return _rtiAmb; }
      set 
      { 
        _rtiAmb = value;
        Handle = (_rtiAmb != null) ? _rtiAmb.getSpaceHandle(Name) : 0;
      }
    }
    #endregion

    #region Constructors
    /// <summary>
    /// CRoutingSpace. Name must be the same with specified in FED.
    /// </summary>
    public CRoutingSpace(string name)
    {
      Name = name;// FED space isimleri ile ayni olmali
      Dimensions = new BindingList<CDimension>();
      Regions = new BindingList<CRegion>();
      _rtiAmb = null;
    }
    #endregion

    #region Methods
    #endregion

  }
}
