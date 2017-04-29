/*
Racon - RTI abstraction component for MS.NET (Racon)
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
using System.ComponentModel;

namespace Racon.RtiLayer
{
  /// <summary>
  /// Wraps an interaction received. 
  /// </summary>
  public class HlaInteractionEventArgs : RaconEventArgs
  {

    #region Properties
    /// <summary>
    /// interaction instance
    /// </summary>
    public HlaInteraction Interaction { get; set; }
    /// <summary>
    ///  Retraction Handle
    /// </summary>
    public MessageRetraction RetractionHandle { get; set; }
    /// <summary>
    ///  Supplemental info.
    /// </summary>
    public SupplementalReceiveInfo SupplementalReceiveInfo { get; set; }
    #endregion

    #region Constructors
    /// <summary>
    /// Constructor
    /// </summary>
    public HlaInteractionEventArgs()
    {
      Interaction = new HlaInteraction();
      RetractionHandle = new MessageRetraction();
      SupplementalReceiveInfo = new SupplementalReceiveInfo();
    }
    #endregion

    #region Methods
    /// <summary>
    /// Is value updated?
    /// </summary>
    /// <param name="parameter">parameter</param>
    public bool IsValueUpdated(HlaParameter parameter)
    {
      foreach (var item in Interaction.Parameters)
        if (item.Handle == parameter.Handle)
          return true;
      return false;
    }

    /// <summary>
    /// Gets parameter value specified by parameter reference. This function simplifies parameter decoding, so that the user does not need to iterate all parameters of an interaction 
    /// </summary>
    /// <param name="parameter">Parameter</param>
    public T GetParameterValue<T>(HlaParameter parameter)
    {
      T result = default(T);
      foreach (var item in Interaction.Parameters)
      {
        if (item.Handle == parameter.Handle)
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
