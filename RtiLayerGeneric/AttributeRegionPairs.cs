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
using System.Collections.Generic;

namespace Racon.RtiLayer
{
  /// <summary>
  /// HLA13: Keeps a dictionary structure for (attribute, region) pairs.
  /// </summary>
  public class AttributeRegionPairs
  {
    #region Fields
    #endregion

    #region Properties
    /// <summary>
    /// Dictionary
    /// </summary>
    private Dictionary<HlaAttribute, HlaRegion> Pairs { get; set; }
    #endregion

    #region Constructors
    /// <summary>
    /// AttributeRegionPairs constructor.
    /// </summary>
    public AttributeRegionPairs()
    {
      Pairs = new Dictionary<HlaAttribute, HlaRegion>();
    }
    #endregion

    #region Methods
    /// <summary>
    /// Adds an object class to object model.
    /// </summary>
    /// <exception cref="ArgumentException">An element with the same key already exists in the Attribute Region Pairs.</exception>
    /// 
    public void Add(HlaAttribute attribute, HlaRegion region)
    {
      try
      {
        Pairs.Add(attribute, region);
      }
      catch (ArgumentException e)
      {
        string msg = "EXC-(Add - AttributeRegionPairs): An element with the same key already exists in the Attribute Region Pairs. " + e.ToString();
        // !!! add this to Racon logger
      }
    }

    /// <summary>
    /// Adds an interaction class to object model.
    /// </summary>
    public Dictionary<HlaAttribute, HlaRegion> GetPairs()
    {
      return Pairs;
    }
    #endregion

  }
}
