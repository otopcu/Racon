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
using System.ComponentModel;

namespace Racon.RtiLayer
{
  /// <summary>
  /// Wraps an HLA object for receiving the values of its attributes. 
  /// </summary>
  public class HlaObjectEventArgs : RaconEventArgs
  {
    #region Properties
    /// <summary>
    /// Object instance
    /// </summary>
    public HlaObject ObjectInstance { get; set; }
    /// <summary>
    /// ObjectClassHandle
    /// </summary>
    public uint ClassHandle { get; set; }
    /// <summary>
    ///  Retraction Handle
    /// </summary>
    public EventRetractionHandle RetractionHandle { get; set; }
    #endregion

    #region Constructors
    /// <summary>
    /// Constructor
    /// </summary>
    public HlaObjectEventArgs()
    {
      ObjectInstance = new HlaObject();
      RetractionHandle = new EventRetractionHandle();
    }
    #endregion

    #region Methods
    /// <summary>
    /// Is value updated?
    /// </summary>
    /// <param name="attribute">attribute</param>
    public bool IsValueUpdated(HlaAttribute attribute)
    {
      foreach (var item in ObjectInstance.Attributes)
        if (item.Handle == attribute.Handle)
          return true;
      return false;
    }

    /// <summary>
    /// Gets attribute value specified by attribute reference. This function simplifies decoding value of an attribute, so that the user does not need to iterate all attributes of an object instance.
    /// </summary>
    /// <param name="attribute">attribute</param>
    public T GetAttributeValue<T>(HlaAttribute attribute)
    {
      T result = default(T);
      foreach (var item in ObjectInstance.Attributes)
      {
        if (item.Handle == attribute.Handle)
        {
          result = item.GetValue<T>();
          break;
        }
      }
      return result;
    }
    #endregion
  }

}
