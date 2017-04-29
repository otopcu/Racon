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

using System.Collections.Generic;
// RACoN
using Racon.RtiLayer;
using Racon.RtiLayer.Native;

namespace Racon.ObjectModel
{
  /// <summary>
  /// The HLA object model supports the construction of the simulation object model, which includes the HLA objects, interactions, and routing spaces (plus regions).
  /// </summary>
  /// 
  /// <remarks>The routing space list is for backward compatibility and only used for HLA 1.3 federations. The data structure used for lists is the binding list (Bind-ingList) class, which is a generic collection class from .NET API. 
  /// </remarks>
  /// 
  internal class NamespaceDoc
  {
    // This class is only for documentation of Namespace: RACoN.ObjectModel in Sandcastle
  }

  /// <summary>
  /// CObjectModel
  /// </summary>
  public class CObjectModel
  {
    #region Fields
    /// <summary>
    /// Interaction Class List
    /// </summary>
    public List<HlaInteractionClass> ICList;

    /// <summary>
    /// Object Class List
    /// </summary>
    public List<HlaObjectClass> OCList;

    /// <summary>
    ///Routing Space List for HLA13
    /// </summary>
    public List<CRoutingSpace> RSList;
    #endregion

    #region Properties
    /// <summary>
    /// Dimensions
    /// </summary>
    public List<HlaDimension> Dimensions { get; set; }

    /// <summary>
    /// RtiAmb reference.
    /// </summary>
    public RtiAmb RtiAmb { get; set; }
    #endregion

    #region Constructors
    /// <summary>
    /// CObjectModel constructor.
    /// </summary>
    public CObjectModel()
      //: base()
    {
      RtiAmb = null;
      ICList = new List<HlaInteractionClass>();
      OCList = new List<HlaObjectClass>();
      RSList = new List<CRoutingSpace>();
      Dimensions = new List<HlaDimension>();
    }
    #endregion

    #region Methods
    /// <summary>
    /// Adds an object class to object model.
    /// </summary>
    public void AddToObjectModel(HlaObjectClass oc)
    {
      OCList.Add(oc);
    }

    /// <summary>
    /// Adds an interaction class to object model.
    /// </summary>
    public void AddToObjectModel(HlaInteractionClass ic)
    {
      ICList.Add(ic);
    }

    /// <summary>
    /// Adds a dimension to object model.
    /// </summary>
    /// <param name="dimension"></param>
    public void AddToObjectModel(HlaDimension dimension)
    {
      Dimensions.Add(dimension);
    }

    /// <summary>
    /// HLA13: Adds a routing space to object model.
    /// </summary>
    public void AddToObjectModel(CRoutingSpace rs)
    {
      rs.RtiAmb = RtiAmb;
      RSList.Add(rs);
    }
    #endregion

  }


}
