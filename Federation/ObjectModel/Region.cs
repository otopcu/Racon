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
using System.Windows.Forms;//MessageBox
using System.Diagnostics.Contracts;
// RACoN
using Racon.RtiLayer;
using Racon.RtiLayer.Native;

namespace Racon.ObjectModel
{
  /// <summary>
  /// CRegion
  /// </summary>
  public class CRegion : CHlaRegion
  {
    #region Fields
    private CRtiAmb _rtiAmb;
    /// <summary>
    ///  Extent List - It should be defined and added to list for each extent according to dimension count.
    /// </summary>
    public BindingList<CExtent> Extents;
    #endregion

    #region Properties
    /// <summary>
    ///  Routing space. HLA13 specific.
    /// </summary>
    public CRoutingSpace Space { get; set; }
   
    /// <summary>
    ///  RtiAmb reference.
    /// </summary>
    public CRtiAmb RtiAmb
    {
      get { return _rtiAmb; }
      set
      {
        _rtiAmb = value;
      }
    }
    #endregion

    #region Constructors
    /// <summary>
    ///  CRegion constructor.
    /// </summary>
    public CRegion(String name)
      : base()
    {
      Name = name;
      _rtiAmb = null;
      Extents = new BindingList<CExtent>();
    }
    #endregion

    #region Methods
    /// <summary>
    ///  CreateRegion.
    /// </summary>
    public void CreateRegion(CRoutingSpace space)
    {
      #region Contracts
      // Preconditions
      Contract.Requires(RtiAmb != null, "RtiAmb is null in CreateRegion()");
      #endregion

      try
      {
        Space = space;
        RtiAmb.createRegion(this, (uint)Extents.Count, (int)space.Handle);
        SetRangeBounds();
      }
      catch (Exception e)
      {
        MessageBox.Show("MSG-(CreateRegion):" + Environment.NewLine + e.ToString(), "CRegion", MessageBoxButtons.OK, MessageBoxIcon.Warning);
      }     
    }

    /// <summary>
    ///  SetRangeBounds.
    /// </summary>
    public void SetRangeBounds()
    {
      #region Contracts
      #endregion
      try
      {
        uint extentIndex = 0;
        //Set Extent Limits for each dimension
        foreach (var extent in Extents)
        {
          uint dimHandle = (uint)extent.Dimension.Handle;
          setRangeLowerBound((uint)extentIndex, dimHandle, (uint)extent.LowerBound);
          setRangeUpperBound((uint)extentIndex++, dimHandle, (uint)extent.UpperBound);
        }
      }
      catch (Exception e)
      {
        MessageBox.Show("MSG-(SetRangeBounds):" + Environment.NewLine + e.ToString(), "CRegion", MessageBoxButtons.OK, MessageBoxIcon.Warning);
      }
    }
    #endregion

  }
}
