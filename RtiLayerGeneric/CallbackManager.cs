
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
    #region FM Events
    /// <summary>
    /// Federation state change event.
    /// </summary>
    public event EventHandler<CFederationStateEventArgs> FederationStateChanged;
    /// <summary>
    /// 4.4
    /// </summary>
    public virtual event EventHandler<HlaFederationManagementEventArgs> ConnectionLost;
    /// <summary>
    /// // 4.8
    /// </summary>
    public virtual event EventHandler<HlaFederationManagementEventArgs> FederationExecutionsReported;
    /// <summary>
    /// // 4.12
    /// </summary>
    public virtual event EventHandler<HlaFederationManagementEventArgs> SynchronizationPointRegistrationSucceeded;
    /// <summary>
    /// // 4.12
    /// </summary>
    public virtual event EventHandler<HlaFederationManagementEventArgs> SynchronizationPointRegistrationFailed;
    /// <summary>
    /// // 4.13
    /// </summary>
    public virtual event EventHandler<HlaFederationManagementEventArgs> SynchronizationPointAnnounced;
    /// <summary>
    /// // 4.15
    /// </summary>
    public virtual event EventHandler<HlaFederationManagementEventArgs> FederationSynchronized;
    /// <summary>
    /// // 4.17
    /// </summary>
    public virtual event EventHandler<HlaFederationManagementEventArgs> InitiateFederateSave;
    /// <summary>
    /// // 4.20
    /// </summary>
    public virtual event EventHandler<HlaFederationManagementEventArgs> FederationSaved;
    /// <summary>
    /// 4.23
    /// </summary>
    public virtual event EventHandler<HlaFederationManagementEventArgs> FederationSaveStatusResponse;
    /// <summary>
    /// // 4.25
    /// </summary>
    public virtual event EventHandler<HlaFederationManagementEventArgs> FederationRestorationRequestConfirmed;
    /// <summary>
    /// // 4.26
    /// </summary>
    public virtual event EventHandler<HlaFederationManagementEventArgs> FederationRestoreBegun;
    /// <summary>
    /// // 4.27
    /// </summary>
    public virtual event EventHandler<HlaFederationManagementEventArgs> InitiateFederateRestore;
    /// <summary>
    /// // 4.29
    /// </summary>
    public virtual event EventHandler<HlaFederationManagementEventArgs> FederationRestored;
    /// <summary>
    /// 4.32
    /// </summary>
    public virtual event EventHandler<HlaFederationManagementEventArgs> FederationRestoreStatusResponse;

    #endregion

    #region DM Events
    /// <summary>
    /// 
    /// </summary>
    public virtual event EventHandler<HlaDeclarationManagementEventArgs> StartRegistrationForObjectClassAdvised;
    /// <summary>
    /// 
    /// </summary>
    public virtual event EventHandler<HlaDeclarationManagementEventArgs> StopRegistrationForObjectClassAdvised;
    /// <summary>
    /// 
    /// </summary>
    public virtual event EventHandler<HlaDeclarationManagementEventArgs> TurnInteractionsOffAdvised;
    /// <summary>
    /// 
    /// </summary>
    public virtual event EventHandler<HlaDeclarationManagementEventArgs> TurnInteractionsOnAdvised;
    #endregion

    #region OM Events
    /// <summary>
    /// 
    /// </summary>
    public virtual event EventHandler<HlaObjectEventArgs> ObjectDiscovered;
    /// <summary>
    /// 
    /// </summary>
    public virtual event EventHandler<HlaInteractionEventArgs> InteractionReceived;
    /// <summary>
    /// 
    /// </summary>
    public virtual event EventHandler<HlaObjectEventArgs> AttributeValueUpdateRequested;
    /// <summary>
    /// 
    /// </summary>
    public virtual event EventHandler<HlaObjectEventArgs> ObjectRemoved;
    /// <summary>
    /// 
    /// </summary>
    public virtual event EventHandler<HlaObjectEventArgs> ObjectAttributesReflected;
    /// <summary>
    /// 
    /// </summary>
    public virtual event EventHandler<HlaObjectEventArgs> TurnUpdatesOnForObjectInstanceAdvised;
    /// <summary>
    /// 
    /// </summary>
    public virtual event EventHandler<HlaObjectEventArgs> TurnUpdatesOffForObjectInstanceAdvised;
    #endregion

    #region OwM Events
    /// <summary>
    /// 7.5
    /// </summary>
    public virtual event EventHandler<HlaOwnershipManagementEventArgs> RequestDivestitureConfirmation;
    /// <summary>
    /// 
    /// </summary>
    public virtual event EventHandler<HlaOwnershipManagementEventArgs> AttributeOwnershipReleaseRequested;
    /// <summary>
    /// 
    /// </summary>
    public virtual event EventHandler<HlaOwnershipManagementEventArgs> AttributeOwnershipInformed;
    /// <summary>
    /// 
    /// </summary>
    public virtual event EventHandler<HlaOwnershipManagementEventArgs> AttributeOwnershipAcquisitionNotified;
    /// <summary>
    /// 
    /// </summary>
    public virtual event EventHandler<HlaOwnershipManagementEventArgs> AttributeOwnershipDivestitureNotified;
    /// <summary>
    /// 
    /// </summary>
    public virtual event EventHandler<HlaOwnershipManagementEventArgs> AttributeOwnershipUnavailable;
    /// <summary>
    /// 
    /// </summary>
    public virtual event EventHandler<HlaOwnershipManagementEventArgs> AttributeOwnershipAcquisitionCancellationConfirmed;
    /// <summary>
    /// 
    /// </summary>
    public virtual event EventHandler<HlaOwnershipManagementEventArgs> AttributeOwnershipAssumptionRequested;

    #endregion

    #region TM Events
    /// <summary>
    /// 
    /// </summary>
    public virtual event EventHandler<HlaTimeManagementEventArgs> TimeConstrainedEnabled;
    /// <summary>
    /// 
    /// </summary>
    public virtual event EventHandler<HlaTimeManagementEventArgs> TimeRegulationEnabled;
    /// <summary>
    /// 
    /// </summary>
    public virtual event EventHandler<HlaTimeManagementEventArgs> TimeAdvanceGrant;
    /// <summary>
    /// 
    /// </summary>
    public virtual event EventHandler<HlaTimeManagementEventArgs> RequestRetraction;
    #endregion

    /// <summary>
    /// General RTI Event for tracing log
    /// </summary>
    // 
    public event EventHandler<RaconEventArgs> RTIEventOccured;

    #endregion

    #region Triggers
    /// <summary>
    /// General rti event occured
    /// </summary>
    /// <param name="_val"></param>
    protected void OnRTIEventOccured(RaconEventArgs _val)
    {
      RTIEventOccured?.Invoke(this, _val);
    }

    #region FM
    /// <summary>
    /// Wraps the event in a protected virtual method to enable derived classes to raise the event.
    /// </summary>
    protected virtual void OnFederationStateChanged(CFederationStateEventArgs e) => FederationStateChanged?.Invoke(this, e);// Raise the event.

    /// <summary>
    /// 
    /// </summary>
    protected virtual void OnConnectionLost(HlaFederationManagementEventArgs e)
    {
      ConnectionLost?.Invoke(this, e);// Raise the event.
    }
    /// <summary>
    /// 
    /// </summary>
    protected virtual void OnFederationExecutionsReported(HlaFederationManagementEventArgs e)
    {
      FederationExecutionsReported?.Invoke(this, e);// Raise the event.
    }
    /// <summary>
    /// 
    /// </summary>
    protected virtual void OnSynchronizationPointRegistrationConfirmed(HlaFederationManagementEventArgs e)
    {
      SynchronizationPointRegistrationSucceeded?.Invoke(this, e);// Raise the event.
    }
    /// <summary>
    /// 
    /// </summary>
    protected virtual void OnSynchronizationPointRegistrationFailed(HlaFederationManagementEventArgs e)
    {
      SynchronizationPointRegistrationFailed?.Invoke(this, e);// Raise the event.
    }
    /// <summary>
    /// 
    /// </summary>
    protected virtual void OnSynchronizationPointAnnounced(HlaFederationManagementEventArgs e)
    {
      SynchronizationPointAnnounced?.Invoke(this, e);// Raise the event.
    }
    /// <summary>
    /// 
    /// </summary>
    protected virtual void OnFederationSynchronized(HlaFederationManagementEventArgs e)
    {
      FederationSynchronized?.Invoke(this, e);// Raise the event.
    }
    /// <summary>
    /// 
    /// </summary>
    protected virtual void OnInitiateFederateSave(HlaFederationManagementEventArgs e)
    {
      InitiateFederateSave?.Invoke(this, e);// Raise the event.
    }
    /// <summary>
    /// 
    /// </summary>
    protected virtual void OnInitiateFederateRestore(HlaFederationManagementEventArgs e)
    {
      InitiateFederateRestore?.Invoke(this, e);// Raise the event.
    }
    /// <summary>
    /// 
    /// </summary>
    protected virtual void OnFederationRestorationRequestConfirmed(HlaFederationManagementEventArgs e)
    {
      FederationRestorationRequestConfirmed?.Invoke(this, e);// Raise the event.
    }
    /// <summary>
    /// 
    /// </summary>
    protected virtual void OnFederationSaved(HlaFederationManagementEventArgs e)
    {
      FederationSaved?.Invoke(this, e);// Raise the event.
    }
    /// <summary>
    /// </summary>
    protected virtual void OnFederationSaveStatusResponse(HlaFederationManagementEventArgs e)
    {
      FederationSaveStatusResponse?.Invoke(this, e);// Raise the event.
    }
    /// <summary>
    /// 
    /// </summary>
    protected virtual void OnFederationRestored(HlaFederationManagementEventArgs e)
    {
      FederationRestored?.Invoke(this, e);// Raise the event.
    }
    /// <summary>
    /// 
    /// </summary>
    protected virtual void OnFederationRestoreBegun(HlaFederationManagementEventArgs e)
    {
      FederationRestoreBegun?.Invoke(this, e);// Raise the event.
    }
    /// <summary>
    /// </summary>
    protected virtual void OnFederationRestoreStatusResponse(HlaFederationManagementEventArgs e)
    {
      FederationRestoreStatusResponse?.Invoke(this, e);// Raise the event.
    }
    #endregion

    #region DM
    /// <summary>
    /// 
    /// </summary>
    protected virtual void OnStartRegistrationForObjectClassAdvised(HlaDeclarationManagementEventArgs e)
    {
      StartRegistrationForObjectClassAdvised?.Invoke(this, e);// Raise the event.
    }
    /// <summary>
    /// 
    /// </summary>
    protected virtual void OnStopRegistrationForObjectClassAdvised(HlaDeclarationManagementEventArgs e)
    {
      StopRegistrationForObjectClassAdvised?.Invoke(this, e);// Raise the event.
    }
    /// <summary>
    /// 
    /// </summary>
    protected virtual void OnTurnInteractionsOnAdvised(HlaDeclarationManagementEventArgs e)
    {
      TurnInteractionsOnAdvised?.Invoke(this, e);// Raise the event.
    }
    /// <summary>
    /// 
    /// </summary>
    protected virtual void OnTurnInteractionsOffAdvised(HlaDeclarationManagementEventArgs e)
    {
      TurnInteractionsOffAdvised?.Invoke(this, e);// Raise the event.
    }
    #endregion

    #region OM
    /// <summary>
    /// 
    /// </summary>
    protected virtual void OnObjectDiscovered(HlaObjectEventArgs e)
    {
      ObjectDiscovered?.Invoke(this, e);// Raise the event.
    }
    /// <summary>
    /// 
    /// </summary>
    protected virtual void OnInteractionReceived(HlaInteractionEventArgs e)
    {
      InteractionReceived?.Invoke(this, e);// Raise the event.
    }
    /// <summary>
    /// 
    /// </summary>
    protected virtual void OnAttributeValueUpdateRequested(HlaObjectEventArgs e)
    {
      AttributeValueUpdateRequested?.Invoke(this, e);// Raise the event.
    }
    /// <summary>
    /// 
    /// </summary>
    protected virtual void OnObjectRemoved(HlaObjectEventArgs e)
    {
      ObjectRemoved?.Invoke(this, e);// Raise the event.
    }
    /// <summary>
    /// 
    /// </summary>
    protected virtual void OnObjectAttributesReflected(HlaObjectEventArgs e)
    {
      ObjectAttributesReflected?.Invoke(this, e);// Raise the event.
    }
    /// <summary>
    /// 
    /// </summary>
    protected virtual void OnTurnUpdatesOnForObjectInstanceAdvised(HlaObjectEventArgs e)
    {
      TurnUpdatesOnForObjectInstanceAdvised?.Invoke(this, e);// Raise the event.
    }
    /// <summary>
    /// 
    /// </summary>
    protected virtual void OnTurnUpdatesOffForObjectInstanceAdvised(HlaObjectEventArgs e)
    {
      TurnUpdatesOffForObjectInstanceAdvised?.Invoke(this, e);// Raise the event.
    }
    #endregion

    #region OwM
    /// <summary>
    /// OnRequestDivestitureConfirmation
    /// </summary>
    protected virtual void OnRequestDivestitureConfirmation(HlaOwnershipManagementEventArgs e)
    {
      RequestDivestitureConfirmation?.Invoke(this, e);// Raise the event.
    }
    /// <summary>
    /// OnAttributeOwnershipReleaseRequested
    /// </summary>
    protected virtual void OnAttributeOwnershipReleaseRequested(HlaOwnershipManagementEventArgs e)
    {
      AttributeOwnershipReleaseRequested?.Invoke(this, e);// Raise the event.
    }
    /// <summary>
    /// OnAttributeOwnershipInformed
    /// </summary>
    protected virtual void OnAttributeOwnershipInformed(HlaOwnershipManagementEventArgs e)
    {
      AttributeOwnershipInformed?.Invoke(this, e);// Raise the event.
    }
    /// <summary>
    /// 
    /// </summary>
    protected virtual void OnAttributeOwnershipAcquisitionNotified(HlaOwnershipManagementEventArgs e)
    {
      AttributeOwnershipAcquisitionNotified?.Invoke(this, e);// Raise the event.
    }
    /// <summary>
    /// 
    /// </summary>
    protected virtual void OnAttributeOwnershipDivestitureNotified(HlaOwnershipManagementEventArgs e)
    {
      AttributeOwnershipDivestitureNotified?.Invoke(this, e);// Raise the event.
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="e"></param>
    protected virtual void OnAttributeOwnershipUnavailable(HlaOwnershipManagementEventArgs e)
    {
      AttributeOwnershipUnavailable?.Invoke(this, e);// Raise the event.
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="e"></param>
    protected virtual void OnAttributeOwnershipAcquisitionCancellationConfirmed(HlaOwnershipManagementEventArgs e)
    {
      AttributeOwnershipAcquisitionCancellationConfirmed?.Invoke(this, e);// Raise the event.
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="e"></param>
    protected virtual void OnAttributeOwnershipAssumptionRequested(HlaOwnershipManagementEventArgs e)
    {
      AttributeOwnershipAssumptionRequested?.Invoke(this, e);// Raise the event.
    }
    #endregion

    #region TM
    /// <summary>
    /// 
    /// </summary>
    /// <param name="e"></param>
    protected virtual void OnTimeConstrainedEnabled(HlaTimeManagementEventArgs e)
    {
      TimeConstrainedEnabled?.Invoke(this, e);// Raise the event.
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="e"></param>
    protected virtual void OnTimeRegulationEnabled(HlaTimeManagementEventArgs e)
    {
      TimeRegulationEnabled?.Invoke(this, e);// Raise the event.
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="e"></param>
    protected virtual void OnTimeAdvanceGrant(HlaTimeManagementEventArgs e)
    {
      TimeAdvanceGrant?.Invoke(this, e);// Raise the event.
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="e"></param>
    protected virtual void OnRequestRetraction(HlaTimeManagementEventArgs e)
    {
      RequestRetraction?.Invoke(this, e);// Raise the event.
    }

    #endregion
    
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
