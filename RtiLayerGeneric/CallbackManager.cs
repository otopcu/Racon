
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
using System.Collections.Generic;

namespace Racon.RtiLayer
{
  /// <summary>
  /// Encapsulates the federate ambassador (RTI-initiated) events and its triggers.
  /// </summary>
  public class CallbackManager
  {
    #region Properties
    /// <summary>
    /// Event Queues
    /// </summary>
    public Queue<RaconEventArgs> FdAmbEventQueue;
    #endregion

    #region Events- related to the RTI-initiated services
    // FM Events
    public virtual event EventHandler<HlaFederationManagementEventArgs> ConnectionLost; // 4.4
    public virtual event EventHandler<HlaFederationManagementEventArgs> FederationExecutionsReported; // 4.8
    public virtual event EventHandler<HlaFederationManagementEventArgs> SynchronizationPointRegistrationSucceeded; // 4.12
    public virtual event EventHandler<HlaFederationManagementEventArgs> SynchronizationPointRegistrationFailed; // 4.12
    public virtual event EventHandler<HlaFederationManagementEventArgs> SynchronizationPointAnnounced; // 4.13
    public virtual event EventHandler<HlaFederationManagementEventArgs> FederationSynchronized; // 4.15
    public virtual event EventHandler<HlaFederationManagementEventArgs> InitiateFederateSave; // 4.17
    public virtual event EventHandler<HlaFederationManagementEventArgs> FederationSaved; // 4.20
    public virtual event EventHandler<HlaFederationManagementEventArgs> FederationRestorationRequestConfirmed; // 4.25
    public virtual event EventHandler<HlaFederationManagementEventArgs> FederationRestoreBegun; // 4.26
    public virtual event EventHandler<HlaFederationManagementEventArgs> InitiateFederateRestore; // 4.27
    public virtual event EventHandler<HlaFederationManagementEventArgs> FederationRestored; // 4.29

    // DM Events
    public virtual event EventHandler<HlaDeclarationManagementEventArgs> StartRegistrationForObjectClassAdvised;
    public virtual event EventHandler<HlaDeclarationManagementEventArgs> StopRegistrationForObjectClassAdvised;
    public virtual event EventHandler<HlaDeclarationManagementEventArgs> TurnInteractionsOffAdvised;
    public virtual event EventHandler<HlaDeclarationManagementEventArgs> TurnInteractionsOnAdvised;
    // OM Events
    public virtual event EventHandler<HlaObjectEventArgs> ObjectDiscovered;
    public virtual event EventHandler<HlaInteractionEventArgs> InteractionReceived;
    public virtual event EventHandler<HlaObjectEventArgs> AttributeValueUpdateRequested;
    public virtual event EventHandler<HlaObjectEventArgs> ObjectRemoved;
    public virtual event EventHandler<HlaObjectEventArgs> ObjectAttributesReflected;
    public virtual event EventHandler<HlaObjectEventArgs> TurnUpdatesOnForObjectInstanceAdvised;
    public virtual event EventHandler<HlaObjectEventArgs> TurnUpdatesOffForObjectInstanceAdvised;
    // OwM Events
    public virtual event EventHandler<HlaOwnershipManagementEventArgs> AttributeOwnershipReleaseRequested;
    public virtual event EventHandler<HlaOwnershipManagementEventArgs> AttributeOwnershipInformed;
    public virtual event EventHandler<HlaOwnershipManagementEventArgs> AttributeOwnershipAcquisitionNotified;
    public virtual event EventHandler<HlaOwnershipManagementEventArgs> AttributeOwnershipDivestitureNotified;
    public virtual event EventHandler<HlaOwnershipManagementEventArgs> AttributeOwnershipUnavailable;
    public virtual event EventHandler<HlaOwnershipManagementEventArgs> AttributeOwnershipAcquisitionCancellationConfirmed;
    public virtual event EventHandler<HlaOwnershipManagementEventArgs> AttributeOwnershipAssumptionRequested;

    // TM Events
    public virtual event EventHandler<HlaTimeManagementEventArgs> TimeConstrainedEnabled;
    public virtual event EventHandler<HlaTimeManagementEventArgs> TimeRegulationEnabled;
    public virtual event EventHandler<HlaTimeManagementEventArgs> TimeAdvanceGrant;
    public virtual event EventHandler<HlaTimeManagementEventArgs> RequestRetraction;

    #endregion

    #region Triggers
    // FM
    protected virtual void OnConnectionLost(HlaFederationManagementEventArgs e)
    {
      ConnectionLost?.Invoke(this, e);// Raise the event.
    }
    protected virtual void OnFederationExecutionsReported(HlaFederationManagementEventArgs e)
    {
      FederationExecutionsReported?.Invoke(this, e);// Raise the event.
    }
    protected virtual void OnSynchronizationPointRegistrationConfirmed(HlaFederationManagementEventArgs e)
    {
      SynchronizationPointRegistrationSucceeded?.Invoke(this, e);// Raise the event.
    }
    protected virtual void OnSynchronizationPointRegistrationFailed(HlaFederationManagementEventArgs e)
    {
      SynchronizationPointRegistrationFailed?.Invoke(this, e);// Raise the event.
    }
    protected virtual void OnSynchronizationPointAnnounced(HlaFederationManagementEventArgs e)
    {
      SynchronizationPointAnnounced?.Invoke(this, e);// Raise the event.
    }
    protected virtual void OnFederationSynchronized(HlaFederationManagementEventArgs e)
    {
      FederationSynchronized?.Invoke(this, e);// Raise the event.
    }
    protected virtual void OnInitiateFederateSave(HlaFederationManagementEventArgs e)
    {
      InitiateFederateSave?.Invoke(this, e);// Raise the event.
    }
    protected virtual void OnInitiateFederateRestore(HlaFederationManagementEventArgs e)
    {
      InitiateFederateRestore?.Invoke(this, e);// Raise the event.
    }
    protected virtual void OnFederationRestorationRequestConfirmed(HlaFederationManagementEventArgs e)
    {
      FederationRestorationRequestConfirmed?.Invoke(this, e);// Raise the event.
    }
    protected virtual void OnFederationSaved(HlaFederationManagementEventArgs e)
    {
      FederationSaved?.Invoke(this, e);// Raise the event.
    }
    protected virtual void OnFederationRestored(HlaFederationManagementEventArgs e)
    {
      FederationRestored?.Invoke(this, e);// Raise the event.
    }
    protected virtual void OnFederationRestoreBegun(HlaFederationManagementEventArgs e)
    {
      FederationRestoreBegun?.Invoke(this, e);// Raise the event.
    }

    // DM
    protected virtual void OnStartRegistrationForObjectClassAdvised(HlaDeclarationManagementEventArgs e)
    {
      StartRegistrationForObjectClassAdvised?.Invoke(this, e);// Raise the event.
    }
    protected virtual void OnStopRegistrationForObjectClassAdvised(HlaDeclarationManagementEventArgs e)
    {
      StopRegistrationForObjectClassAdvised?.Invoke(this, e);// Raise the event.
    }
    protected virtual void OnTurnInteractionsOnAdvised(HlaDeclarationManagementEventArgs e)
    {
      TurnInteractionsOnAdvised?.Invoke(this, e);// Raise the event.
    }
    protected virtual void OnTurnInteractionsOffAdvised(HlaDeclarationManagementEventArgs e)
    {
      TurnInteractionsOffAdvised?.Invoke(this, e);// Raise the event.
    }

    // OM
    protected virtual void OnObjectDiscovered(HlaObjectEventArgs e)
    {
      ObjectDiscovered?.Invoke(this, e);// Raise the event.
    }
    protected virtual void OnInteractionReceived(HlaInteractionEventArgs e)
    {
      InteractionReceived?.Invoke(this, e);// Raise the event.
    }
    protected virtual void OnAttributeValueUpdateRequested(HlaObjectEventArgs e)
    {
      AttributeValueUpdateRequested?.Invoke(this, e);// Raise the event.
    }
    protected virtual void OnObjectRemoved(HlaObjectEventArgs e)
    {
      ObjectRemoved?.Invoke(this, e);// Raise the event.
    }
    protected virtual void OnObjectAttributesReflected(HlaObjectEventArgs e)
    {
      ObjectAttributesReflected?.Invoke(this, e);// Raise the event.
    }
    protected virtual void OnTurnUpdatesOnForObjectInstanceAdvised(HlaObjectEventArgs e)
    {
      TurnUpdatesOnForObjectInstanceAdvised?.Invoke(this, e);// Raise the event.
    }
    protected virtual void OnTurnUpdatesOffForObjectInstanceAdvised(HlaObjectEventArgs e)
    {
      TurnUpdatesOffForObjectInstanceAdvised?.Invoke(this, e);// Raise the event.
    }

    // OwM
    protected virtual void OnAttributeOwnershipReleaseRequested(HlaOwnershipManagementEventArgs e)
    {
      AttributeOwnershipReleaseRequested?.Invoke(this, e);// Raise the event.
    }
    protected virtual void OnAttributeOwnershipInformed(HlaOwnershipManagementEventArgs e)
    {
      AttributeOwnershipInformed?.Invoke(this, e);// Raise the event.
    }
    protected virtual void OnAttributeOwnershipAcquisitionNotified(HlaOwnershipManagementEventArgs e)
    {
      AttributeOwnershipAcquisitionNotified?.Invoke(this, e);// Raise the event.
    }
    protected virtual void OnAttributeOwnershipDivestitureNotified(HlaOwnershipManagementEventArgs e)
    {
      AttributeOwnershipDivestitureNotified?.Invoke(this, e);// Raise the event.
    }
    protected virtual void OnAttributeOwnershipUnavailable(HlaOwnershipManagementEventArgs e)
    {
      AttributeOwnershipUnavailable?.Invoke(this, e);// Raise the event.
    }
    protected virtual void OnAttributeOwnershipAcquisitionCancellationConfirmed(HlaOwnershipManagementEventArgs e)
    {
      AttributeOwnershipAcquisitionCancellationConfirmed?.Invoke(this, e);// Raise the event.
    }
    protected virtual void OnAttributeOwnershipAssumptionRequested(HlaOwnershipManagementEventArgs e)
    {
      AttributeOwnershipAssumptionRequested?.Invoke(this, e);// Raise the event.
    }

    // TM
    protected virtual void OnTimeConstrainedEnabled(HlaTimeManagementEventArgs e)
    {
      TimeConstrainedEnabled?.Invoke(this, e);// Raise the event.
    }
    protected virtual void OnTimeRegulationEnabled(HlaTimeManagementEventArgs e)
    {
      TimeRegulationEnabled?.Invoke(this, e);// Raise the event.
    }
    protected virtual void OnTimeAdvanceGrant(HlaTimeManagementEventArgs e)
    {
      TimeAdvanceGrant?.Invoke(this, e);// Raise the event.
    }
    protected virtual void OnRequestRetraction(HlaTimeManagementEventArgs e)
    {
      RequestRetraction?.Invoke(this, e);// Raise the event.
    }


    #endregion

    #region Constructors
    /// <summary>
    /// Constructor
    /// </summary>
    public CallbackManager()
    {
      // Create FdAmb Event Queue
      FdAmbEventQueue = new Queue<RaconEventArgs>();
    }
    #endregion
  }
}
