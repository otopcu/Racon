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

namespace Racon.RtiLayer
{
  /// <summary>
  /// This is an abstract generic class for all RTI ambassadors.  
  /// </summary>
  /// <remarks>
  /// <para> RtiAmb (in generic layer) and CRtiAmb (in native layer) classes involve all the generic interface for RTI ambassadors. Both share the interface in such a way that the methods that have .NET arguments and return types are kept here, others are kept in CRtiAmb class which is inherited from this class</para>
  /// </remarks>
  public abstract class RtiAmb
  {
    #region Properties
    /// <summary>
    /// Native Rti Library
    /// </summary>
    public string RtiVersion { get; set; }
    #endregion

    #region Constructors
    /// <summary>
    /// Constructor
    /// </summary>
    public RtiAmb()
    {
      RtiVersion = "";
    }
    #endregion

    #region Events
    // Events related to the federate-initiated services
    public event EventHandler<RaconEventArgs> FederationExecutionCreated;
    public event EventHandler<RaconEventArgs> FederationExecutionDestroyed;
    public event EventHandler<RaconEventArgs> FederateConnected;
    public event EventHandler<RaconEventArgs> FederateDisconnected;
    public event EventHandler<RaconEventArgs> FederateJoined;
    public event EventHandler<RaconEventArgs> FederateResigned;
    public event EventHandler<RaconEventArgs> ObjectRegistered;
    public event EventHandler<RaconEventArgs> HLAClassPublished;
    public event EventHandler<RaconEventArgs> HLAClassSubscribed;
    public event EventHandler<RaconEventArgs> RTIEventOccured; // General RTI Event
    #endregion

    #region Triggers
    protected void OnFederationExecutionCreated(RaconEventArgs _val)
    {
      FederationExecutionCreated(this, _val);
    }
    protected void OnFederationExecutionDestroyed(RaconEventArgs _val)
    {
      FederationExecutionDestroyed(this, _val);
    }
    protected void OnFederateConnected(RaconEventArgs _val)
    {
      FederateConnected(this, _val);
    }
    protected void OnFederateDisconnected(RaconEventArgs _val)
    {
      FederateDisconnected(this, _val);
    }
    protected void OnFederateJoined(RaconEventArgs _val)
    {
      FederateJoined(this, _val);
    }
    protected void OnFederateResigned(RaconEventArgs _val)
    {
      FederateResigned(this, _val);
    }
    protected void OnObjectRegistered(RaconEventArgs _val)
    {
      ObjectRegistered(this, _val);
    }
    protected void OnHLAClassSubscribed(RaconEventArgs _val)
    {
      HLAClassSubscribed(this, _val);
    }
    protected void OnHLAClassPublished(RaconEventArgs _val)
    {
      HLAClassPublished(this, _val);
    }
    protected void OnRTIEventOccured(RaconEventArgs _val)
    {
      RTIEventOccured(this, _val);
    }
    #endregion

    #region Methods

    #region Fedaration Management
    public abstract void connect(string str);
    public abstract void disconnect();
    public abstract void createFederation(string str, string str2);
    public abstract void joinFederation(string str, string str2);
    public abstract void resignFederation(int p1);
    public abstract void destroyFederation(string str);
    public abstract void requestFederationSave(string str);
    public abstract void requestFederationSave(string str, double p1);
    public abstract void federateSaveBegun();
    public abstract void federateSaveComplete(bool p1);
    public abstract void requestFederationRestore(string str);
    public abstract void federateRestoreComplete(bool p1);
    #endregion // Fedaration Management

    #region Declaration Management
    #endregion // Declaration Management

    #region Object Management
    #endregion

    #region Ownership Management
    #endregion

    #region Data Distribution Management

    #endregion

    #region Time Management
    public abstract bool enableAsynchronousDelivery();
    public abstract bool disableAsynchronousDelivery();
    public abstract bool disableTimeConstrained();
    public abstract bool disableTimeRegulation();
    public abstract bool enableTimeConstrained();
    public abstract bool enableTimeRegulation(double p1, double p2);
    public abstract bool flushQueueRequest(double p);
    public abstract bool nextEventRequest(double p);
    public abstract bool nextEventRequestAvailable(double p);
    public abstract bool modifyLookahead(double p);
    public abstract double queryFederateTime();
    public abstract double queryLBTS();
    public abstract double queryLookahead();
    public abstract double queryMinNextEventTime();
    public abstract bool timeAdvanceRequest(double p);
    public abstract bool timeAdvanceRequestAvailable(double p);
    #endregion// Time Management

    #region Support Services
    public abstract uint getDimensionHandle(string p1, uint p2);//10.25
    public abstract bool enableObjectClassRelevanceAdvisorySwitch();//10.33
    public abstract bool disableObjectClassRelevanceAdvisorySwitch();//10.34
    public abstract bool enableAttributeRelevanceAdvisorySwitch();//10.35
    public abstract bool disableAttributeRelevanceAdvisorySwitch();//10.36
    public abstract bool enableAttributeScopeAdvisorySwitch();//10.37
    public abstract bool disableAttributeScopeAdvisorySwitch();//10.38
    public abstract bool enableInteractionRelevanceAdvisorySwitch();//10.39
    public abstract bool disableInteractionRelevanceAdvisorySwitch();//10.40
    public abstract void evokeCallback(double approximateMinimumTimeInSeconds);//10.41 
    public abstract void evokeMultipleCallbacks (double approximateMinimumTimeInSeconds, double approximateMaximumTimeInSeconds);//10.42
    public virtual uint getSpaceHandle(string p) { return 0; } // HLA13
    #endregion// Support Services

    #endregion

  }
}
