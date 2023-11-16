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

namespace Racon.RtiLayer
{
  /// <summary>
  /// </summary>
  public class AttributeHandleSetRegionHandleSetPair
  {
    #region Properties
    /// <summary>
    /// std:pair for C++ => KeyValuePair for .NET 3.5
    /// </summary>
    public KeyValuePair<List<HlaAttribute>, List<HlaRegion>> Pair { get; set; } // Use Tuple for .NET 4
    #endregion

    #region Constructors
    /// <summary>
    /// Constructor
    /// </summary>
    public AttributeHandleSetRegionHandleSetPair
()
    {
      Pair = new KeyValuePair<List<HlaAttribute>, List<HlaRegion>>();
    }
    #endregion
  }

  /// <summary>
  /// </summary>
  public class AttributeHandleSetRegionHandleSetPairVector
  {
    #region Properties
    /// <summary>
    /// std:vector for C++ => List for .NET
    /// </summary>
    public List<KeyValuePair<List<HlaAttribute>, List<HlaRegion>>> Pairs { get; set; }
    #endregion

    #region Constructors
    /// <summary>
    /// Constructor
    /// </summary>
    public AttributeHandleSetRegionHandleSetPairVector()
    {
      Pairs = new List<KeyValuePair<List<HlaAttribute>, List<HlaRegion>>>();
    }
    #endregion

    #region Methods
    /// <summary>
    /// Returns the attribute-region pairs as a string
    /// </summary>
    /// <returns></returns>
    public override string ToString()
    {
      string result = "[ ";
      foreach (var pair in Pairs)
      {
        string attributes = "";
        foreach (var item in pair.Key)
        {
          attributes += item.Name + ",";
        }
        attributes.Remove(attributes.Length - 1); // remove last comma
        string regions = "";
        foreach (var item in pair.Value)
        {
          regions += item.Name + ",";
        }
        regions.Remove(attributes.Length - 1); // remove last comma
        regions = ")";
        // add pair to result string
        result += "<(" + attributes + "), (" + regions + ")>,";
      }
      result.Remove(result.Length - 1); // remove last comma
      return result + " ]";
    }
    #endregion

  }
}
