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

using System.ComponentModel;
using Racon.RtiLayer;

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
    public BindingList<HlaInteractionClass> ICList;

    /// <summary>
    /// Object Class List
    /// </summary>
    public BindingList<HlaObjectClass> OCList;

    /// <summary>
    ///Routing Space List for HLA13
    /// </summary>
    public BindingList<CRoutingSpace> RSList;
    #endregion

    #region Properties
    /// <summary>
    /// RtiAmb reference.
    /// </summary>
    public CRtiAmb RtiAmb { get; set; }
    #endregion

    #region Constructors
    /// <summary>
    /// CObjectModel constructor.
    /// </summary>
    public CObjectModel()
      : base()
    {
      RtiAmb = null;
      ICList = new BindingList<HlaInteractionClass>();
      OCList = new BindingList<HlaObjectClass>();
      RSList = new BindingList<CRoutingSpace>();
    }
    #endregion

    #region Methods
    /// <summary>
    /// Adds an object class to object model.
    /// </summary>
    public void AddToObjectModel(HlaObjectClass oc)
    {
      this.OCList.Add(oc);
    }

    /// <summary>
    /// Adds an interaction class to object model.
    /// </summary>
    public void AddToObjectModel(HlaInteractionClass ic)
    {
      this.ICList.Add(ic);
    }

    /// <summary>
    /// Adds a routing space to object model.
    /// </summary>
    public void AddToObjectModel(CRoutingSpace rs)
    {
      rs.RtiAmb = this.RtiAmb;
      this.RSList.Add(rs);
    }
    #endregion

  }


}
