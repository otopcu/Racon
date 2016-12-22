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
  /// Event types 
  /// </summary>
  public enum RaconEventTypes
  {
    NA, // The event type is not important - used for RTIAmb and Racon events. The others are used for FdAmb events
    // FM
    ConnectionLost,
    FederationExecutionsReported,
    SynchronizationPointRegistrationSucceeded,
    synchronizationPointRegistrationFailed,
    SynchronizationPointAnnounced,
    FederationSynchronized,
    InitiateFederateSave,
    InitiateFederateRestore,
    FederationRestorationRequestConfirmed,
    FederationSaved,
    FederationRestored,
    FederationRestoreBegun,
    // DM
    StartRegistrationForObjectClassAdvised,
    StopRegistrationForObjectClassAdvised,
    TurnInteractionsOnAdvised,
    TurnInteractionsOffAdvised,
    // OM
    ObjectDiscovered,
    ObjectRemoved,
    ObjectAttributesReflected,
    AttributeValueUpdateRequested,
    InteractionReceived,
    TurnUpdatesOnForObjectInstanceAdvised,
    TurnUpdatesOffForObjectInstanceAdvised,
    // OwM
    AttributeOwnershipReleaseRequested,
    AttributeOwnershipInformed,
    AttributeOwnershipAcquisitionNotified,
    AttributeOwnershipDivestitureNotified,
    AttributeOwnershipUnavailable,
    AttributeOwnershipAcquisitionCancellationConfirmed,
    AttributeOwnershipAssumptionRequested,
    // TM
    TimeConstrainedEnabled,
    TimeRegulationEnabled,
    TimeAdvanceGrant,
    RequestRetraction
  };

  /// <summary>
  /// RaconEventArgs class extends the EventArgs class to convey a status message that reflects the information about the Racon and information level 
  /// </summary>
  public class RaconEventArgs : EventArgs
  {
    #region Properties
    /// <summary>
    /// Event message
    /// </summary>
    public string TraceMessage { get; set; }
    /// <summary>
    /// Event type
    /// </summary>
    public RaconEventTypes EventType { get; set; }
    /// <summary>
    /// Time
    /// </summary>
    public double Time;
    #endregion

    #region Constructors
    /// <summary>
    /// Constructor
    /// </summary>
    public RaconEventArgs()
    {
      TraceMessage = "A Racon event is occured.";
    }
    public RaconEventArgs(string val)
    {
      TraceMessage = val;
    }
    #endregion
  }

  /// <summary>
  /// HlaFederationManagementEventArgs 
  /// </summary>
  public class HlaFederationManagementEventArgs : RaconEventArgs
  {
    #region Properties
    /// <summary>
    /// Federation synchronization point label
    /// </summary>
    public string Label { get; set; }
    /// <summary>
    /// Reason. A string value for services such as ConnectionLost
    /// </summary>
    public string Reason { get; set; }
    /// <summary>
    /// Success/Failure. Return value for callbacks such as: requestFederationRestoreSuccedded, synchronizationPointRegistrationFailed
    /// </summary>
    public bool Success { get; set; }
    /// <summary>
    /// Tag for announceSynchronizationPoint
    /// </summary>
    public string Tag { get; set; }
    /// <summary>
    /// Handle of the Federate
    /// </summary>
    public uint FederateHandle { get; set; }
    #endregion

    #region Constructors
    /// <summary>
    /// Constructor
    /// </summary>
    public HlaFederationManagementEventArgs()
    {
      Success = false;
      FederateHandle = 0;
      Tag = "";
    }
    #endregion
  }

  /// <summary>
  /// HlaTimeManagementEventArgs 
  /// </summary>
  public class HlaTimeManagementEventArgs : RaconEventArgs
  {
    #region Properties
    /// <summary>
    /// EventRetractionHandle
    /// </summary>
    public EventRetractionHandle RetractionHandle { get; set; }
    #endregion

    #region Constructors
    /// <summary>
    /// Constructor
    /// </summary>
    public HlaTimeManagementEventArgs()
    {
      RetractionHandle = new EventRetractionHandle();
    }
    #endregion
  }


  /// <summary>
  /// CHlaDeclarationManagementEventArgs 
  /// </summary>
  public class HlaDeclarationManagementEventArgs : RaconEventArgs
  {
    #region Properties
    /// <summary>
    /// ObjectClassHandle
    /// </summary>
    public uint ObjectClassHandle { get; set; }
    /// <summary>
    /// InteractionClassHandle
    /// </summary>
    public uint InteractionClassHandle { get; set; }
    #endregion

    #region Constructors
    /// <summary>
    /// Constructor
    /// </summary>
    public HlaDeclarationManagementEventArgs()
    {
      InteractionClassHandle = 0;
      ObjectClassHandle = 0;
    }
    #endregion
  }

  /// <summary>
  /// CHlaOwnershipManagementEventArgs 
  /// </summary>
  public class HlaOwnershipManagementEventArgs : RaconEventArgs
  {
    #region Properties
    /// <summary>
    /// ObjectHandle
    /// </summary>
    public uint ObjectHandle { get; set; }
    /// <summary>
    /// FederateHandle
    /// </summary>
    public uint FederateHandle { get; set; }
    /// <summary>
    /// Tag
    /// </summary>
    public string Tag { get; set; }
    /// <summary>
    /// RaconAttributeSet
    /// </summary>
    public RaconAttributeSet AttributeSet { get; set; }
    #endregion

    #region Constructors
    /// <summary>
    /// Constructor
    /// </summary>
    public HlaOwnershipManagementEventArgs()
    {
      ObjectHandle = 0;
      FederateHandle = 0;
      Tag = "";
      AttributeSet = new RaconAttributeSet();
    }
    #endregion
  }

  /// <summary>
  /// RaconAttributeSet Implements an attribute handle set empoyed in callbacks. This class is not required at RtiAmb calls. See how to call attributes w/o this class in " ChangeAttributeOrderType" - RtiAmb_Dmso.cpp
  /// </summary>
  public class RaconAttributeSet
  {
    #region Properties
    /// <summary>
    /// Tag
    /// </summary>
    public BindingList<uint> Attributes { get; set; }
    #endregion

    #region Constructors
    /// <summary>
    /// Constructor
    /// </summary>
    public RaconAttributeSet()
    {
      Attributes = new BindingList<uint>();
    }
    #endregion
    public void AddAttribute(HlaAttribute _attr)
    {
      Attributes.Add(_attr.Handle);
    }

  }

}
