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

using System.ComponentModel;

namespace Racon.RtiLayer
{
  /// <summary>
  /// Base class for for supplemental info used in various federate callbacks. 
  /// </summary>
  public abstract class SupplementalInfo
  {
    private uint? _ProducingFederateHandle;

    #region Properties
    /// <summary>
    ///  Has Producing Federate? It is used in receive interaction service.
    /// </summary>
    public bool HasProducingFederate { get; set; }
    /// <summary>
    ///  Producing Federate handle. It is used in receive interaction service.
    /// </summary>
    public uint? ProducingFederateHandle
    {
      get
      {
        if (HasProducingFederate)
          return _ProducingFederateHandle;
        else return null; // indicates a non-sense value
      }
      set
      {
        _ProducingFederateHandle = value;
      }
    }
    #endregion

    #region Constructors
    /// <summary>
    /// Constructor
    /// </summary>
    public SupplementalInfo()
    {
      HasProducingFederate = false;
    }
    #endregion
  }

  /// <summary>
  /// Wrapper for rti1516e::SupplementalReceiveInfo. Used in Receive Interaction service.
  /// </summary>
  public class SupplementalReceiveInfo : SupplementalInfo
  {
    #region Properties
    /// <summary>
    ///  Has Sent Regions? Not implemented !!!
    /// </summary>
    public bool HasSentRegions { get; set; }
    #endregion

    #region Constructors
    /// <summary>
    /// Constructor
    /// </summary>
    public SupplementalReceiveInfo() : base()
    {
      HasSentRegions = false;
    }
    #endregion
  }

  /// <summary>
  /// Wrapper for rti1516e::SupplementalRemoveInfo. Used in Remove Object Instance service.
  /// </summary>
  public class SupplementalRemoveInfo : SupplementalInfo
  {
    #region Properties
    #endregion

    #region Constructors
    /// <summary>
    /// Constructor
    /// </summary>
    public SupplementalRemoveInfo() : base()
    {
    }
    #endregion
  }

  /// <summary>
  /// Wrapper for rti1516e::SupplementalReceiveInfo. Used in Receive Interaction service.
  /// </summary>
  public class SupplementalReflectInfo : SupplementalInfo
  {
    #region Properties
    /// <summary>
    ///  Has Sent Regions? Not implemented !!!
    /// </summary>
    public bool HasSentRegions { get; set; }
    #endregion

    #region Constructors
    /// <summary>
    /// Constructor
    /// </summary>
    public SupplementalReflectInfo() : base()
    {
      HasSentRegions = false;
    }
    #endregion
  }


}
