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
//using System.ComponentModel; // BindingList
using System.Collections.Generic; // List

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

    #region Finalizer
    ///// <summary>
    ///// Finalizer
    ///// </summary>
    //~RtiAmb()
    //{
    //}
    #endregion

    #region Events
    // Events related to the federate-initiated services
    /// <summary>
    /// 
    /// </summary>
    public event EventHandler<RaconEventArgs> NoConnection;
    /// <summary>
    /// 
    /// </summary>
    public event EventHandler<RaconEventArgs> FederationExecutionCreated;
    /// <summary>
    /// 
    /// </summary>
    public event EventHandler<RaconEventArgs> FederationExecutionDestroyed;
    /// <summary>
    /// 
    /// </summary>
    public event EventHandler<RaconEventArgs> FederateConnected;
    /// <summary>
    /// 
    /// </summary>
    public event EventHandler<RaconEventArgs> FederateDisconnected;
    /// <summary>
    /// 
    /// </summary>
    public event EventHandler<RaconEventArgs> FederateJoined;
    /// <summary>
    /// 
    /// </summary>
    public event EventHandler<RaconEventArgs> FederateResigned;
    /// <summary>
    /// 
    /// </summary>
    public event EventHandler<RaconEventArgs> ObjectRegistered;
    /// <summary>
    /// 
    /// </summary>
    public event EventHandler<RaconEventArgs> HLAClassPublished;
    /// <summary>
    /// 
    /// </summary>
    public event EventHandler<RaconEventArgs> HLAClassSubscribed;
    /// <summary>
    /// General RTI Event
    /// </summary>
    public event EventHandler<RaconEventArgs> RTIEventOccured;
    #endregion

    #region Triggers
    /// <summary>
    /// OnNotConnected
    /// </summary>
    /// <param name="_val"></param>
    protected void OnNotConnected(RaconEventArgs _val)
    {
      NoConnection(this, _val);
    }
    /// <summary>
    /// OnFederationExecutionCreated
    /// </summary>
    /// <param name="_val"></param>
    protected void OnFederationExecutionCreated(RaconEventArgs _val)
    {
      FederationExecutionCreated(this, _val);
    }
    /// <summary>
    /// OnFederationExecutionDestroyed
    /// </summary>
    /// <param name="_val"></param>
    protected void OnFederationExecutionDestroyed(RaconEventArgs _val)
    {
      FederationExecutionDestroyed(this, _val);
    }
    /// <summary>
    /// OnFederateConnected
    /// </summary>
    /// <param name="_val"></param>
    protected void OnFederateConnected(RaconEventArgs _val)
    {
      FederateConnected(this, _val);
    }
    /// <summary>
    /// OnFederateDisconnected
    /// </summary>
    /// <param name="_val"></param>
    protected void OnFederateDisconnected(RaconEventArgs _val)
    {
      FederateDisconnected(this, _val);
    }
    /// <summary>
    /// OnFederateJoined
    /// </summary>
    /// <param name="_val"></param>
    protected void OnFederateJoined(RaconEventArgs _val)
    {
      FederateJoined(this, _val);
    }
    /// <summary>
    /// OnFederateResigned
    /// </summary>
    /// <param name="_val"></param>
    protected void OnFederateResigned(RaconEventArgs _val)
    {
      FederateResigned(this, _val);
    }
    /// <summary>
    /// OnObjectRegistered
    /// </summary>
    /// <param name="_val"></param>
    protected void OnObjectRegistered(RaconEventArgs _val)
    {
      ObjectRegistered(this, _val);
    }
    /// <summary>
    /// OnHLAClassSubscribed
    /// </summary>
    /// <param name="_val"></param>
    protected void OnHLAClassSubscribed(RaconEventArgs _val)
    {
      HLAClassSubscribed(this, _val);
    }
    /// <summary>
    /// OnHLAClassPublished
    /// </summary>
    /// <param name="_val"></param>
    protected void OnHLAClassPublished(RaconEventArgs _val)
    {
      HLAClassPublished(this, _val);
    }
    /// <summary>
    /// OnRTIEventOccured
    /// </summary>
    /// <param name="_val"></param>
    protected void OnRTIEventOccured(RaconEventArgs _val)
    {
      RTIEventOccured?.Invoke(this, _val);
    }
    #endregion

    #region Methods

    #region Fedaration Management
    /// <summary>
    /// IEEE1516.1-2010 4.2
    /// </summary>
    /// <param name="callbackModel">callbackModel</param>
    /// <param name="localSettingsDesignator">localSettingsDesignator</param>
    public abstract void connect(CallbackModel callbackModel, string localSettingsDesignator);
    /// <summary>
    /// IEEE1516.1-2010 4.3: disconnect
    /// </summary>
    public abstract void disconnect();
    /// <summary>
    /// IEEE1516.1-2010 4.5: createFederation
    /// </summary>
    /// <param name="fedexName">fedex Name</param>
    /// <param name="fom">FDD</param>
    /// <param name="logicalTimeImplementationName">logicalTimeImplementationName</param>
    public abstract void createFederation(string fedexName, string fom, string logicalTimeImplementationName = "");
    /// <summary>
    /// IEEE1516.1-2010 4.5: createFederation
    /// </summary>
    /// <param name="fedexName">fedex Name</param>
    /// <param name="fomModules">fom Modules</param>
    /// <param name="logicalTimeImplementationName">logicalTimeImplementationName</param>
    public virtual void createFederation(string fedexName, List<string> fomModules, string logicalTimeImplementationName = "")
    {
      // Default implementation - so, native classes do not need to override
      OnRTIEventOccured(new RaconEventArgs("createFederation(string fedexName, List<string> fomModules) is IEEE1516.1-2010 specific.", LogLevel.ERROR));
    }
    /// <summary>
    /// IEEE1516.1-2010 4.5: createFederation
    /// </summary>
    /// <param name="fedexName">fedex Name</param>
    /// <param name="fomModules">fom Modules</param>
    /// <param name="mimModule">mimModule</param>
    /// <param name="logicalTimeImplementationName">logicalTimeImplementationName</param>
    public virtual void createFederation(string fedexName, List<string> fomModules, string mimModule, string logicalTimeImplementationName)
    {
      // Default implementation - so, native classes do not need to override
      OnRTIEventOccured(new RaconEventArgs("createFederation(string fedexName, List<string> fomModules, string mimModule, string logicalTimeImplementationName) is IEEE1516.1-2010 specific.", LogLevel.ERROR));
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="str"></param>
    public abstract void destroyFederation(string str); // 4.6
    /// <summary>
    /// IEEE1516.1-2010 4.7: listFederationExecutions
    /// </summary>
    public abstract void listFederationExecutions();
    /// <summary>
    /// IEEE1516.1-2010 4.9: joinFederation
    /// </summary>
    /// <param name="federateType"></param>
    /// <param name="federationExecutionName"></param>
    /// <param name="fomModules"></param>
    /// <returns></returns>
    public virtual uint joinFederationExecution(string federateType, string federationExecutionName, List<string> fomModules = null)
    {
      // Default implementation - so, native classes do not need to override
      OnRTIEventOccured(new RaconEventArgs("joinFederation(string federateType, string federationExecutionName, List<string> fomModules = null) is IEEE1516.1-2010 specific.", LogLevel.ERROR));
      return 0;
    }
    /// <summary>
    /// IEEE1516.1-2010 4.9: joinFederation with federate name
    /// </summary>
    /// <param name="federateName"></param>
    /// <param name="federateType"></param>
    /// <param name="federationExecutionName"></param>
    /// <param name="fomModules"></param>
    /// <returns></returns>
    public virtual uint joinFederationExecution(string federateName, string federateType, string federationExecutionName, List<string> fomModules = null)
    {
      // Default implementation - so, native classes do not need to override
      OnRTIEventOccured(new RaconEventArgs("joinFederation(string federateName, string federateType, string federationExecutionName, List<string> fomModules = null) is IEEE1516.1-2010 specific.", LogLevel.ERROR));
      return 0;
    }
    /// <summary>
    /// HLA13 - joinFederationExecution
    /// </summary>
    /// <param name="federateName"></param>
    /// <param name="fedexName"></param>
    /// <returns></returns>
    public virtual uint joinFederationExecution(string federateName, string fedexName)
    {
      // Default implementation - so, native classes do not need to override
      OnRTIEventOccured(new RaconEventArgs("joinFederationExecution(string federateName, string fedexName) is HLA13 specific. Use other overload methods for IEEE1516."));
      return 0;//sentinel value
    }
    /// <summary>
    ///  // 4.10
    /// </summary>
    /// <param name="p1"></param>
    public abstract void resignFederation(int p1);
    /// <summary>
    /// // 4.11
    /// </summary>
    /// <param name="synchronizationPointLabel"></param>
    /// <param name="userSuppliedTag"></param>
    public abstract void registerFederationSynchronizationPoint(string synchronizationPointLabel, string userSuppliedTag);
    /// <summary>
    /// // 4.11
    /// </summary>
    /// <param name="synchronizationPointLabel"></param>
    /// <param name="userSuppliedTag"></param>
    /// <param name="setOfJoinedFederateDesignators"></param>
    public abstract void registerFederationSynchronizationPoint(string synchronizationPointLabel, string userSuppliedTag, List<uint> setOfJoinedFederateDesignators);
    /// <summary>
    /// // 4.14
    /// </summary>
    /// <param name="synchronizationPointLabel"></param>
    /// <param name="synchronizationSuccess"></param>
    public abstract void synchronizationPointAchieved(string synchronizationPointLabel, bool synchronizationSuccess = true);
    /// <summary>
    /// // 4.16: requestFederationSave
    /// </summary>
    /// <param name="label"></param>
    public abstract void requestFederationSave(string label);
    /// <summary>
    /// // 4.16: requestFederationSave
    /// </summary>
    /// <param name="label"></param>
    /// <param name="p1"></param>
    public abstract void requestFederationSave(string label, double p1);
    /// <summary>
    /// // 4.18: federateSaveBegun
    /// </summary>
    public abstract void federateSaveBegun();
    /// <summary>
    /// // 4.19: federateSaveComplete
    /// </summary>
    /// <param name="p1"></param>
    public abstract void federateSaveComplete(bool p1);
    /// <summary>
    /// IEEE1516.1-2010 4.21: abortFederationSave
    /// </summary>
    public virtual void abortFederationSave()
    {
      // Default implementation - so, native classes do not need to override
      OnRTIEventOccured(new RaconEventArgs("abortFederationSave() is IEEE1516.1-2010 specific. !!! Not implemented", LogLevel.WARN));
    }
    /// <summary>
    /// IEEE1516.1-2010 4.22: queryFederationSaveStatus
    /// </summary>
    public virtual void queryFederationSaveStatus()
    {
      // Default implementation - so, native classes do not need to override
      OnRTIEventOccured(new RaconEventArgs("queryFederationSaveStatus() is IEEE1516.1-2010 specific. !!! Not implemented", LogLevel.WARN));
    }
    /// <summary>
    /// IEEE1516.1-2010 4.24: requestFederationRestore
    /// </summary>
    /// <param name="str"></param>
    public abstract void requestFederationRestore(string str);
    /// <summary>
    /// IEEE1516.1-2010 4.28: federateRestoreComplete
    /// </summary>
    /// <param name="p1"></param>
    public abstract void federateRestoreComplete(bool p1);
    /// <summary>
    /// IEEE1516.1-2010 4.30: abortFederationRestore
    /// </summary>
    public virtual void abortFederationRestore()
    {
      // Default implementation - so, native classes do not need to override
      OnRTIEventOccured(new RaconEventArgs("abortFederationRestore() is IEEE1516.1-2010 specific. !!! Not implemented", LogLevel.WARN));
    }
    /// <summary>
    /// IEEE1516.1-2010 4.31: queryFederationRestoreStatus
    /// </summary>
    public virtual void queryFederationRestoreStatus()
    {
      // Default implementation - so, native classes do not need to override
      OnRTIEventOccured(new RaconEventArgs("queryFederationRestoreStatus() is IEEE1516.1-2010 specific. !!! Not implemented", LogLevel.WARN));
    }
    #endregion // Fedaration Management

    #region Declaration Management
    /// <summary>
    /// 
    /// </summary>
    /// <param name="ic"></param>
    public abstract void publishInteractionClass(HlaInteractionClass ic);
    /// <summary>
    /// 
    /// </summary>
    /// <param name="oc"></param>
    /// <param name="list"></param>
    public abstract void publishObjectClass(HlaObjectClass oc, List<HlaAttribute> list);
    /// <summary>
    /// 
    /// </summary>
    /// <param name="ic"></param>
    public abstract void subscribeInteractionClass(HlaInteractionClass ic);
    /// <summary>
    /// 
    /// </summary>
    /// <param name="oc"></param>
    /// <param name="list"></param>
    /// <param name="p"></param>
    /// <returns></returns>
    public abstract bool subscribeObjectClass(HlaObjectClass oc, List<HlaAttribute> list, bool p);
    /// <summary>
    /// 
    /// </summary>
    /// <param name="ic"></param>
    /// <returns></returns>
    public abstract bool unsubscribeInteractionClass(HlaInteractionClass ic);
    /// <summary>
    /// 
    /// </summary>
    /// <param name="oc"></param>
    /// <returns></returns>
    public abstract bool unsubscribeObjectClass(HlaObjectClass oc);
    #endregion // Declaration Management

    #region Object Management
    /// <summary>
    /// IEEE1516.1-2010 6.8
    /// </summary>
    /// <param name="o"></param>
    /// <returns></returns>
    public abstract bool registerObject(HlaObject o);
    /// <summary>
    /// IEEE1516.1-2010 6.8
    /// </summary>
    /// <param name="o"></param>
    /// <param name="s"></param>
    /// <returns></returns>
    public abstract bool registerObject(HlaObject o, string s);

    /// <summary>
    /// IEEE1516.1-2010 6.10
    /// </summary>
    /// <param name="obj"></param>
    /// <param name="tag"></param>
    /// <returns></returns>
    public virtual bool updateAttributeValues(HlaObject obj, VariableLengthDataWrapper tag)
    {
      OnRTIEventOccured(new RaconEventArgs("updateAttributeValues(HlaObject obj, VariableLengthDataWrapper tag) is IEEE1516.1-2010 specific.", LogLevel.WARN));
      return false;
    }
    /// <summary>
    /// IEEE1516.1-2010 6.10
    /// </summary>
    /// <param name="obj"></param>
    /// <param name="tag"></param>
    /// <param name="timestamp"></param>
    /// <returns></returns>
    public virtual MessageRetraction updateAttributeValues(HlaObject obj, VariableLengthDataWrapper tag, double timestamp)
    {
      OnRTIEventOccured(new RaconEventArgs("deleteObjectInstance(HlaObject o, VariableLengthDataWrapper tag) is IEEE1516.1-2010 specific.", LogLevel.WARN));
      return null;
    }
    /// <summary>
    /// HLA13 - updateAttributeValues
    /// </summary>
    /// <param name="obj"></param>
    /// <returns></returns>
    public virtual bool updateAttributeValues(HlaObject obj)
    {
      OnRTIEventOccured(new RaconEventArgs("updateAttributeValues(HlaObject obj is HLA 1.3 specific.", LogLevel.WARN));
      return false;
    }
    /// <summary>
    /// HLA13 - updateAttributeValues
    /// </summary>
    /// <param name="obj"></param>
    /// <param name="timestamp"></param>
    /// <returns></returns>
    public virtual MessageRetraction updateAttributeValues(HlaObject obj, double timestamp)
    {
      OnRTIEventOccured(new RaconEventArgs("updateAttributeValues(HlaObject obj, double timestamp) is HLA 1.3 specific.", LogLevel.WARN));
      return null;
    }

    /// <summary>
    /// IEEE1516.1-2010 6.12
    /// </summary>
    /// <param name="interaction"></param>
    /// <param name="tag"></param>
    /// <returns></returns>
    public virtual bool sendInteraction(HlaInteraction interaction, VariableLengthDataWrapper tag)
    {
      OnRTIEventOccured(new RaconEventArgs("sendInteraction(HlaInteraction interaction, VariableLengthDataWrapper tag) is IEEE1516.1-2010 specific.", LogLevel.WARN));
      return false;
    }
    /// <summary>
    /// IEEE1516.1-2010 6.12
    /// </summary>
    /// <param name="interaction"></param>
    /// <param name="tag"></param>
    /// <param name="timestamp"></param>
    /// <returns></returns>
    public virtual MessageRetraction sendInteraction(HlaInteraction interaction, VariableLengthDataWrapper tag, double timestamp)
    {
      OnRTIEventOccured(new RaconEventArgs("sendInteraction(HlaInteraction interaction, VariableLengthDataWrapper tag, double timestamp) is IEEE1516.1-2010 specific.", LogLevel.WARN));
      return null;
    }
    /// <summary>
    /// HLA13: sendInteraction
    /// </summary>
    /// <param name="interaction"></param>
    /// <returns></returns>
    public virtual bool sendInteraction(HlaInteraction interaction)
    {
      OnRTIEventOccured(new RaconEventArgs("sendInteraction(HlaInteraction interaction) is HLA 1.3 specific.", LogLevel.WARN));
      return false;
    }
    /// <summary>
    /// HLA13: sendInteraction
    /// </summary>
    /// <param name="interaction"></param>
    /// <param name="timestamp"></param>
    /// <returns></returns>
    public virtual MessageRetraction sendInteraction(HlaInteraction interaction, double timestamp)
    {
      OnRTIEventOccured(new RaconEventArgs("sendInteraction(HlaInteraction interaction, double timestamp) is HLA 1.3 specific.", LogLevel.WARN));
      return null;
    }

    /// <summary>
    /// IEEE1516.1-2010 6.14
    /// </summary>
    /// <param name="obj">HLA Object</param>
    /// <param name="tag">User-Supplied Tag</param>
    /// <returns></returns>
    public virtual bool deleteObjectInstance(HlaObject obj, VariableLengthDataWrapper tag)
    {
      OnRTIEventOccured(new RaconEventArgs("deleteObjectInstance(HlaObject o, VariableLengthDataWrapper tag) is IEEE1516.1-2010 specific.", LogLevel.WARN));
      return false;
    }
    /// <summary>
    /// IEEE1516.1-2010 6.14
    /// </summary>
    /// <param name="obj">HLA Object</param>
    /// <param name="tag">User-Supplied Tag</param>
    /// <param name="timestamp">Timestamp</param>
    /// <returns></returns>
    public virtual MessageRetraction deleteObjectInstance(HlaObject obj, VariableLengthDataWrapper tag, double timestamp)
    {
      OnRTIEventOccured(new RaconEventArgs("deleteObjectInstance(HlaObject obj, VariableLengthDataWrapper tag, double time) is IEEE1516.1-2010 specific.", LogLevel.WARN));
      return null;
    }
    /// <summary>
    /// HLA13 - deleteObjectInstance
    /// </summary>
    /// <param name="obj"></param>
    /// <param name="tag"></param>
    /// <returns></returns>
    public virtual bool deleteObjectInstance(HlaObject obj, string tag)
    {
      OnRTIEventOccured(new RaconEventArgs("deleteObjectInstance(HlaObject obj, string tag) is HLA 1.3 specific.", LogLevel.WARN));
      return false;
    }
    /// <summary>
    /// HLA13 - deleteObjectInstance
    /// </summary>
    /// <param name="obj"></param>
    /// <param name="time"></param>
    /// <param name="tag"></param>
    /// <returns></returns>
    public virtual MessageRetraction deleteObjectInstance(HlaObject obj, double time, string tag)
    {
      OnRTIEventOccured(new RaconEventArgs("deleteObjectInstance(HlaObject obj, double time, string tag) is HLA 1.3 specific.", LogLevel.WARN));
      return null;
    }

    /// <summary>
    /// IEEE1516.1-2010 6.19
    /// </summary>
    /// <param name="obj"></param>
    /// <param name="attributes"></param>
    /// <param name="tag"></param>
    /// <returns></returns>
    public virtual bool requestAttributeValueUpdate(HlaObjectClass obj, List<HlaAttribute> attributes, VariableLengthDataWrapper tag)
    {
      OnRTIEventOccured(new RaconEventArgs("requestAttributeValueUpdate(HlaObjectClass obj, List<HlaAttribute> attributes, VariableLengthDataWrapper tag) is IEEE1516.1-2010 specific.", LogLevel.WARN));
      return false;
    }
    /// <summary>
    /// IEEE1516.1-2010 6.19
    /// </summary>
    /// <param name="obj"></param>
    /// <param name="attributes"></param>
    /// <param name="tag"></param>
    /// <returns></returns>
    public virtual bool requestAttributeValueUpdate(HlaObject obj, List<HlaAttribute> attributes, VariableLengthDataWrapper tag)
    {
      OnRTIEventOccured(new RaconEventArgs("requestAttributeValueUpdate(HlaObject obj, List<HlaAttribute> attributes, VariableLengthDataWrapper tag) is IEEE1516.1-2010 specific.", LogLevel.WARN));
      return false;
    }
    /// <summary>
    /// HLA13: requestAttributeValueUpdate - OC
    /// </summary>
    /// <param name="obj"></param>
    /// <param name="attributes"></param>
    /// <returns></returns>
    public virtual bool requestAttributeValueUpdate(HlaObjectClass obj, List<HlaAttribute> attributes)
    {
      OnRTIEventOccured(new RaconEventArgs("bool requestAttributeValueUpdate(HlaObjectClass obj, List<HlaAttribute> attributes) is HLA 1.3 specific.", LogLevel.WARN));
      return false;
    }
    /// <summary>
    /// HLA13: requestAttributeValueUpdate - object
    /// </summary>
    /// <param name="obj"></param>
    /// <param name="attributes"></param>
    /// <returns></returns>
    public virtual bool requestAttributeValueUpdate(HlaObject obj, List<HlaAttribute> attributes)
    {
      OnRTIEventOccured(new RaconEventArgs("requestAttributeValueUpdate(HlaObject obj, List<HlaAttribute> attributes) is HLA 1.3 specific.", LogLevel.WARN));
      return false;
    }
    #endregion

    #region Ownership Management

    /// <summary>
    /// IEEE1516.1-2010 7.2 and HLA13
    /// </summary>
    /// <param name="o"></param>
    /// <param name="attributeSet"></param>
    /// <returns></returns>
    public abstract bool unconditionalAttributeOwnershipDivestiture(HlaObject o, List<HlaAttribute> attributeSet);

    /// <summary>
    /// IEEE1516.1-2010 7.3 
    /// </summary>
    /// <param name="o"></param>
    /// <param name="attributeSet"></param>
    /// <param name="tag"></param>
    /// <returns></returns>
    public virtual bool negotiatedAttributeOwnershipDivestiture(HlaObject o, List<HlaAttribute> attributeSet, VariableLengthDataWrapper tag)
    {
      OnRTIEventOccured(new RaconEventArgs("negotiatedAttributeOwnershipDivestiture(HlaObject o, List<HlaAttribute> attributeSet, VariableLengthDataWrapper tag) is IEEE1516.1-2010 specific.", LogLevel.WARN));
      return false;
    }
    /// <summary>
    /// HLA13: negotiatedAttributeOwnershipDivestiture
    /// </summary>
    /// <param name="o"></param>
    /// <param name="attributeSet"></param>
    /// <param name="tag"></param>
    /// <returns></returns>
    public virtual bool negotiatedAttributeOwnershipDivestiture(HlaObject o, List<HlaAttribute> attributeSet, string tag)
    {
      OnRTIEventOccured(new RaconEventArgs("negotiatedAttributeOwnershipDivestiture(HlaObject o, List<HlaAttribute> attributeSet, string tag) is HLA 1.3 specific.", LogLevel.WARN));
      return false;
    }

    /// <summary>
    /// IEEE1516-2010 7.6
    /// </summary>
    /// <param name="theObject"></param>
    /// <param name="attributeSet"></param>
    /// <param name="tag"></param>
    /// <returns></returns>
    public virtual bool confirmDivestiture(HlaObject theObject, List<HlaAttribute> attributeSet, VariableLengthDataWrapper tag)
    {
      // Default implementation - so, native classes do not need to override
      OnRTIEventOccured(new RaconEventArgs("ConfirmDivestiture() is IEEE1516.1-2010 specific.", LogLevel.WARN));
      return false;
    }

    /// <summary>
    /// IEEE1516.1-2010 7.8 
    /// </summary>
    /// <param name="theObject"></param>
    /// <param name="attributeSet"></param>
    /// <param name="tag"></param>
    /// <returns></returns>
    public virtual bool attributeOwnershipAcquisition(HlaObject theObject, List<HlaAttribute> attributeSet, VariableLengthDataWrapper tag)
    {
      OnRTIEventOccured(new RaconEventArgs("requestAttributeValueUpdate(HlaObject obj, List<HlaAttribute> attributes, VariableLengthDataWrapper tag) is IEEE1516.1-2010 specific.", LogLevel.WARN));
      return false;
    }

    /// <summary>
    /// HLA13: attributeOwnershipAcquisition
    /// </summary>
    /// <param name="theObject"></param>
    /// <param name="attributeSet"></param>
    /// <param name="tag"></param>
    /// <returns></returns>
    public virtual bool attributeOwnershipAcquisition(HlaObject theObject, List<HlaAttribute> attributeSet, string tag)
    {
      OnRTIEventOccured(new RaconEventArgs("attributeOwnershipAcquisition(HlaObject theObject, List<HlaAttribute> attributeSet, string tag)) is HLA 1.3 specific.", LogLevel.WARN));
      return false;
    }

    /// <summary>
    /// IEEE1516.1-2010 7.9 and HLA13 
    /// </summary>
    /// <param name="theObject"></param>
    /// <param name="attributeSet"></param>
    /// <returns></returns>
    public abstract bool attributeOwnershipAcquisitionIfAvailable(HlaObject theObject, List<HlaAttribute> attributeSet);

    /// <summary>
    /// IEEE1516-2010 7.12
    /// </summary>
    /// <param name="theObject"></param>
    /// <param name="attributeSet"></param>
    /// <returns></returns>
    public virtual bool attributeOwnershipReleaseDenied(HlaObject theObject, List<HlaAttribute> attributeSet)
    {
      // Default implementation - so, native classes do not need to override
      OnRTIEventOccured(new RaconEventArgs("attributeOwnershipReleaseDenied() is IEEE1516.1-2010 specific.", LogLevel.WARN));
      return false;
    }

    /// <summary>
    /// IEEE1516-2010 7.13
    /// </summary>
    /// <param name="theObject"></param>
    /// <param name="attributes"></param>
    /// <param name="attributesDivested"></param>
    /// <returns></returns>
    public virtual bool attributeOwnershipDivestitureIfWanted(HlaObject theObject, List<HlaAttribute> attributes, out List<HlaAttribute> attributesDivested)
    {
      // Default implementation - so, native classes do not need to override
      OnRTIEventOccured(new RaconEventArgs("attributeOwnershipDivestitureIfWanted() is IEEE1516.1-2010 specific.", LogLevel.WARN));
      attributesDivested = null;
      return false;
    }

    /// <summary>
    /// IEEE1516.1-2010 7.14 and HLA13
    /// </summary>
    /// <param name="theObject"></param>
    /// <param name="attributes"></param>
    /// <returns></returns>
    public abstract bool cancelNegotiatedAttributeOwnershipDivestiture(HlaObject theObject, List<HlaAttribute> attributes);

    /// <summary>
    /// IEEE1516.1-2010 7.15 and HLA13
    /// </summary>
    /// <param name="theObject"></param>
    /// <param name="attributes"></param>
    /// <returns></returns>
    public abstract bool cancelAttributeOwnershipAcquisition(HlaObject theObject, List<HlaAttribute> attributes);

    /// <summary>
    /// IEEE1516.1-2010 7.17 and HLA13 
    /// </summary>
    /// <param name="o"></param>
    /// <param name="a"></param>
    /// <returns></returns>
    public abstract bool queryAttributeOwnership(HlaObject o, HlaAttribute a);

    /// <summary>
    /// IEEE1516.1-2010 7.19 and HLA13 
    /// </summary>
    /// <param name="o"></param>
    /// <param name="a"></param>
    /// <returns></returns>
    public abstract bool isAttributeOwnedByFederate(HlaObject o, HlaAttribute a);

    /// <summary>
    /// HLA13 
    /// </summary>
    /// <param name="theObject"></param>
    /// <param name="attributes"></param>
    /// <returns></returns>
    public virtual bool attributeOwnershipReleaseResponse(HlaObject theObject, List<HlaAttribute> attributes)
    {
      OnRTIEventOccured(new RaconEventArgs("attributeOwnershipReleaseResponse() is HLA13 specific. You can response to the request either with Attribute Ownership Divestiture If Wanted, Unconditional Attribute Divestiture, or Negotiated Attribute Owner-ship Divestiture calls for a positive response. For a negative response, you can use Attribute Ownership Release Denied service in case of HLA Evolved."));
      return false;
    }

    #endregion

    #region Data Distribution Management

    /// <summary>
    /// IEEE1516.1-2010 9.2: createRegion
    /// </summary>
    public virtual uint createRegion(HlaRegion region, List<HlaDimension> dimensions)
    {
      // Default implementation - so, native classes do not need to override
      OnRTIEventOccured(new RaconEventArgs("createRegion() is only used for IEEE1516-2010. !!! Not implemented", LogLevel.WARN));
      return 0;
    }
    /// <summary>
    /// HLA13: createRegion
    /// </summary>
    /// <param name="regionName"></param>
    /// <param name="noOfExtents"></param>
    /// <param name="spaceHandle"></param>
    /// <returns></returns>
    public virtual bool createRegion(string regionName, uint noOfExtents, uint spaceHandle)
    {
      // Default implementation - so, native classes do not need to override
      OnRTIEventOccured(new RaconEventArgs("createRegion(string regionName, uint noOfExtents, uint spaceHandle) is only used for HLA13. Use other overload methods for IEEE1516-2010.", LogLevel.WARN));
      return false;
    }
    /// <summary>
    /// IEEE1516.1-2010 9.3: commitRegionModifications
    /// </summary>
    /// <param name="regions"></param>
    public virtual void commitRegionModifications(List<HlaRegion> regions)
    {
      // Default implementation - so, native classes do not need to override
      OnRTIEventOccured(new RaconEventArgs("commitRegionModifications() is only used for IEEE1516-2010. !!! Not implemented", LogLevel.WARN));
    }
    /// <summary>
    /// IEEE1516.1-2010 9.4: deleteRegion
    /// </summary>
    /// <param name="region"></param>
    public abstract void deleteRegion(HlaRegion region);
    /// <summary>
    /// IEEE1516.1-2010 9.5: registerObjectInstanceWithRegions
    /// </summary>
    /// <param name="objectTobeRegistered"></param>
    /// <param name="pairs"></param>
    public virtual void registerObjectInstanceWithRegions(HlaObject objectTobeRegistered, AttributeHandleSetRegionHandleSetPairVector pairs)
    {
      // Default implementation - so, native classes do not need to override
      OnRTIEventOccured(new RaconEventArgs("registerObjectInstanceWithRegions(HlaObject objectTobeRegistered, AttributeHandleSetRegionHandleSetPairVector pairs) is only used for IEEE1516-2010. Use other overloaded methods for HLA13.", LogLevel.WARN));
    }
    /// <summary>
    /// HLA13: registerObjectInstanceWithRegions
    /// </summary>
    /// <param name="objectTobeRegistered"></param>
    /// <param name="attributes"></param>
    /// <param name="regions"></param>
    public virtual void registerObjectInstanceWithRegions(HlaObject objectTobeRegistered, List<HlaAttribute> attributes, List<HlaRegion> regions)
    {
      // Default implementation - so, native classes do not need to override
      OnRTIEventOccured(new RaconEventArgs("registerObjectInstanceWithRegions(HlaObject objectTobeRegistered, List<HlaAttribute> attributes, List<HlaRegion> regions) is only used for HLA13. Use other overload methods for IEEE1516-2010.", LogLevel.WARN));
    }
    /// <summary>
    /// IEEE1516.1-2010 9.6: associateRegionForUpdates
    /// </summary>
    /// <param name="obj"></param>
    /// <param name="pairs"></param>
    public virtual bool associateRegionsForUpdates(HlaObject obj, AttributeHandleSetRegionHandleSetPairVector pairs)
    {
      // Default implementation - so, native classes do not need to override
      OnRTIEventOccured(new RaconEventArgs("associateRegiosnForUpdates(HlaObject obj, AttributeHandleSetRegionHandleSetPairVector pairs) is only used for IEEE1516-2010. Use other overloadeded methods for HLA13.", LogLevel.WARN));
      return false;
    }
    /// <summary>
    /// HLA13: associateRegionForUpdates
    /// </summary>
    /// <param name="region"></param>
    /// <param name="obj"></param>
    /// <param name="attributes"></param>
    public virtual bool associateRegionForUpdates(HlaRegion region, HlaObject obj, List<HlaAttribute> attributes)
    {
      // Default implementation - so, native classes do not need to override
      OnRTIEventOccured(new RaconEventArgs("associateRegionForUpdates() is only used for HLA13. Use other overloaded methods for IEEE1516-2010.", LogLevel.WARN));
      return false;
    }

    /// <summary>
    /// IEEE1516.1-2010 9.7: unAssociateRegionsForUpdates
    /// </summary>
    /// <param name="obj"></param>
    /// <param name="pairs"></param>
    public virtual bool unassociateRegionsForUpdates(HlaObject obj, AttributeHandleSetRegionHandleSetPairVector pairs)
    {
      // Default implementation - so, native classes do not need to override
      OnRTIEventOccured(new RaconEventArgs("unAssociateRegionsForUpdates(HlaObject obj, AttributeHandleSetRegionHandleSetPairVector pairs) is only used for IEEE1516-2010. Use other overloadeded methods for HLA13.", LogLevel.WARN));
      return false;
    }
    /// <summary>
    /// HLA13
    /// </summary>
    /// <param name="region"></param>
    /// <param name="obj"></param>
    public virtual bool unassociateRegionForUpdates(HlaRegion region, HlaObject obj)
    {
      // Default implementation - so, native classes do not need to override
      OnRTIEventOccured(new RaconEventArgs("unAssociateRegionForUpdates() is only used for HLA13. Use other overloaded methods for IEEE1516-2010.", LogLevel.WARN));
      return false;
    }

    /// <summary>
    /// IEEE1516.1-2010 9.8: subscribeObjectClassAttributesWithRegions
    /// </summary>
    /// <param name="oc"></param>
    /// <param name="pairs"></param>
    /// <param name="indicator"></param>
    public virtual bool subscribeObjectClassAttributesWithRegions(HlaObjectClass oc, AttributeHandleSetRegionHandleSetPairVector pairs, bool indicator)
    {
      // Default implementation - so, native classes do not need to override
      OnRTIEventOccured(new RaconEventArgs("subscribeObjectClassAttributesWithRegions is only used for IEEE1516-2010. Use other overloaded methods for HLA13.", LogLevel.WARN));
      return false;
    }
    /// <summary>
    /// HLA13: subscribeObjectClassAttributesWithRegions
    /// </summary>
    /// <param name="oc"></param>
    /// <param name="list"></param>
    /// <param name="region"></param>
    /// <param name="indicator"></param>
    /// <returns></returns>
    public virtual bool subscribeObjectClassAttributesWithRegions(HlaObjectClass oc, List<HlaAttribute> list, HlaRegion region, bool indicator)
    {
      // Default implementation - so, native classes do not need to override
      OnRTIEventOccured(new RaconEventArgs("subscribeObjectClassAttributesWithRegions is only used for HLA13. Use other overload methods for IEEE1516-2010.", LogLevel.WARN));
      return false;
    }
    /// <summary>
    /// IEEE1516.1-2010 9.9: unsubscribeObjectClassWithRegions
    /// </summary>
    /// <param name="oc"></param>
    /// <param name="pairs"></param>
    /// <returns></returns>
    public virtual bool unsubscribeObjectClassWithRegions(HlaObjectClass oc, AttributeHandleSetRegionHandleSetPairVector pairs)
    {
      // Default implementation - so, native classes do not need to override
      OnRTIEventOccured(new RaconEventArgs("unsubscribeObjectClassWithRegions is only used for IEEE1516-2010. Use other overloaded methods for HLA13.", LogLevel.WARN));
      return false;
    }
    /// <summary>
    /// HLA13: unsubscribeObjectClassWithRegions
    /// </summary>
    /// <param name="oc"></param>
    /// <param name="region"></param>
    /// <returns></returns>
    public virtual bool unsubscribeObjectClassWithRegions(HlaObjectClass oc, HlaRegion region)
    {
      // Default implementation - so, native classes do not need to override
      OnRTIEventOccured(new RaconEventArgs("unsubscribeObjectClassWithRegions is only used for HLA13. Use other overloaded methods for IEEE1516-2010.", LogLevel.WARN));
      return false;
    }
    /// <summary>
    /// IEEE1516.1-2010 9.10: subscribeInteractionClassWithRegions
    /// </summary>
    /// <param name="ic"></param>
    /// <param name="regions"></param>
    /// <param name="indicator"></param>
    /// <returns></returns>
    public virtual bool subscribeInteractionClassWithRegions(HlaInteractionClass ic, List<HlaRegion> regions, bool indicator)
    {
      // Default implementation - so, native classes do not need to override
      OnRTIEventOccured(new RaconEventArgs("subscribeInteractionClassWithRegions is only used for IEEE1516-2010. Use other overloaded methods for HLA13.", LogLevel.WARN));
      return false;
    }
    /// <summary>
    /// HLA13: subscribeInteractionClassWithRegion
    /// </summary>
    /// <param name="ic"></param>
    /// <param name="region"></param>
    /// <returns></returns>
    public virtual bool subscribeInteractionClassWithRegion(HlaInteractionClass ic, HlaRegion region)
    {
      // Default implementation - so, native classes do not need to override
      OnRTIEventOccured(new RaconEventArgs("subscribeInteractionClassWithRegion is only used for HLA13. Use other overloaded methods for IEEE1516-2010.", LogLevel.WARN));
      return false;
    }
    /// <summary>
    /// IEEE1516.1-2010 9.11: unSubscribeInteractionClassWithRegions
    /// </summary>
    /// <param name="ic"></param>
    /// <param name="regions"></param>
    /// <returns></returns>
    public virtual bool unSubscribeInteractionClassWithRegions(HlaInteractionClass ic, List<HlaRegion> regions)
    {
      // Default implementation - so, native classes do not need to override
      OnRTIEventOccured(new RaconEventArgs("unSubscribeInteractionClassWithRegions is only used for IEEE1516-2010. Use other overloaded methods for HLA13.", LogLevel.WARN));
      return false;
    }
    /// <summary>
    /// HLA13: unSubscribeInteractionClassWithRegion
    /// </summary>
    /// <param name="ic"></param>
    /// <param name="region"></param>
    /// <returns></returns>
    public virtual bool unSubscribeInteractionClassWithRegion(HlaInteractionClass ic, HlaRegion region)
    {
      // Default implementation - so, native classes do not need to override
      OnRTIEventOccured(new RaconEventArgs("unSubscribeInteractionClassWithRegion is only used for HLA13. Use other overloaded methods for IEEE1516-2010.", LogLevel.WARN));
      return false;
    }
    /// <summary>
    /// IEEE1516.1-2010 9.12: sendInteractionWithRegions
    /// </summary>
    /// <param name="ic"></param>
    /// <param name="regions"></param>
    /// <param name="tag"></param>
    /// <returns></returns>
    public virtual bool sendInteractionWithRegions(HlaInteractionClass ic, List<HlaRegion> regions, VariableLengthDataWrapper tag)
    {
      // Default implementation - so, native classes do not need to override
      OnRTIEventOccured(new RaconEventArgs("sendInteractionWithRegions is only used for IEEE1516-2010. Use other overloaded methods for HLA13.", LogLevel.WARN));
      return true;
    }
    /// <summary>
    /// IEEE1516.1-2010 9.12: sendInteractionWithRegions with timestamp
    /// </summary>
    /// <param name="ic"></param>
    /// <param name="regions"></param>
    /// <param name="tag"></param>
    /// <param name="timestamp"></param>
    /// <returns></returns>
    public virtual MessageRetraction sendInteractionWithRegions(HlaInteractionClass ic, List<HlaRegion> regions, VariableLengthDataWrapper tag, double timestamp)
    {
      // Default implementation - so, native classes do not need to override
      OnRTIEventOccured(new RaconEventArgs("sendInteractionWithRegions is only used for IEEE1516-2010. Use other overloaded methods for HLA13.", LogLevel.WARN));
      return null;
    }
    /// <summary>
    /// HLA13: sendInteractionWithRegion
    /// </summary>
    /// <param name="ic"></param>
    /// <param name="region"></param>
    /// <returns></returns>
    public virtual bool sendInteractionWithRegion(HlaInteraction ic, HlaRegion region)
    {
      // Default implementation - so, native classes do not need to override
      OnRTIEventOccured(new RaconEventArgs("sendInteractionWithRegion is only used for HLA13. Use other overloaded methods for IEEE1516-2010.", LogLevel.WARN));
      return false;
    }

    /// <summary>
    /// IEEE1516.1-2010 9.13: requestClassAttributeValueUpdateWithRegions
    /// </summary>
    /// <param name="oc"></param>
    /// <param name="pairs"></param>
    /// <param name="tag"></param>
    /// <returns></returns>
    public virtual bool requestClassAttributeValueUpdateWithRegions(HlaObjectClass oc, AttributeHandleSetRegionHandleSetPairVector pairs, VariableLengthDataWrapper tag)
    {
      // Default implementation - so, native classes do not need to override
      OnRTIEventOccured(new RaconEventArgs("requestClassAttributeValueUpdateWithRegions is only used for IEEE1516-2010. Use other overloaded methods for HLA13.", LogLevel.WARN));
      return false;
    }
    /// <summary>
    /// HLA13: requestClassAttributeValueUpdateWithRegion
    /// </summary>
    /// <param name="oc"></param>
    /// <param name="attributes"></param>
    /// <param name="region"></param>
    /// <returns></returns>
    public virtual bool requestClassAttributeValueUpdateWithRegion(HlaObjectClass oc, List<HlaAttribute> attributes, HlaRegion region)
    {
      // Default implementation - so, native classes do not need to override
      OnRTIEventOccured(new RaconEventArgs("requestClassAttributeValueUpdateWithRegion is only used for HLA13. Use other overloaded methods for IEEE1516-2010.", LogLevel.WARN));
      return false;
    }

    #endregion

    #region Time Management
    /// <summary>
    /// IEEE1516.1-2010: 8.2 Enable Time Regulation
    /// </summary>
    /// <param name="theLookahead"></param>
    /// <returns></returns>
    public virtual bool enableTimeRegulation(double theLookahead)
    {
      // Default implementation - so, non-specification native classes do not need to override
      OnRTIEventOccured(new RaconEventArgs("enableTimeRegulation(double theLookahead) is IEEE1516.1-2010 specific.", LogLevel.WARN));
      return false;
    }
    /// <summary>
    /// HLA13 - enableTimeRegulation
    /// </summary>
    /// <param name="theFederateTime"></param>
    /// <param name="theLookahead"></param>
    /// <returns></returns>
    public virtual bool enableTimeRegulation(double theFederateTime, double theLookahead)
    {
      // Default implementation - so, non-specification native classes do not need to override
      OnRTIEventOccured(new RaconEventArgs("enableTimeRegulation(double theFederateTime, double theLookahead) is HLA13 specific. Use enableTimeRegulation(double theLookahead) for HLA1516e.", LogLevel.WARN));
      return false;
    }
    /// <summary>
    /// IEEE1516.1-2010: 8.4 Disable Time Regulation
    /// </summary>
    /// <returns></returns>
    public abstract bool disableTimeRegulation();
    /// <summary>
    /// IEEE1516.1-2010: 8.5 Enable Time Constrained
    /// </summary>
    /// <returns></returns>
    public abstract bool enableTimeConstrained();
    /// <summary>
    /// IEEE1516.1-2010: 8.7 Disable Time Constrained
    /// </summary>
    /// <returns></returns>
    public abstract bool disableTimeConstrained();

    /// <summary>
    /// IEEE1516.1-2010: 8.8 Time Advance Request
    /// </summary>
    /// <param name="theTime"></param>
    /// <returns></returns>
    public abstract bool timeAdvanceRequest(double theTime);

    /// <summary>
    /// IEEE1516.1-2010: 8.9 Time Advance Request Available
    /// </summary>
    /// <param name="theTime"></param>
    /// <returns></returns>
    public abstract bool timeAdvanceRequestAvailable(double theTime);

    /// <summary>
    /// IEEE1516.1-2010: 8.10 Next Message Request
    /// </summary>
    /// <param name="logicalTime"></param>
    /// <returns></returns>
    public virtual bool nextMessageRequest(double logicalTime)
    {
      // Default implementation - so, native classes do not need to override
      OnRTIEventOccured(new RaconEventArgs("nextMessageRequest() is IEEE1516.1-2010 specific. Use nextEventRequest() for HLA13."));
      return false;
    }

    /// <summary>
    /// HLA13: nextEventRequest
    /// </summary>
    /// <param name="logicalTime"></param>
    /// <returns></returns>
    public virtual bool nextEventRequest(double logicalTime)
    {
      // Default implementation - so, native classes do not need to override
      OnRTIEventOccured(new RaconEventArgs("nextEventRequest() is HLA13 specific. Use nextMessageRequest() for HLA1516e.", LogLevel.WARN));
      return false;
    }

    /// <summary>
    /// IEEE1516.1-2010: 8.11 Next Message Request Available
    /// </summary>
    /// <param name="logicalTime"></param>
    /// <returns></returns>
    public virtual bool nextMessageRequestAvailable(double logicalTime)
    {
      // Default implementation - so, native classes do not need to override
      OnRTIEventOccured(new RaconEventArgs("nextMessageRequestAvailable() is IEEE1516.1-2010 specific. Use nextEventRequestAvailable() for HLA13."));
      return false;
    }

    /// <summary>
    /// HLA13: nextEventRequestAvailable
    /// </summary>
    /// <param name="logicalTime"></param>
    /// <returns></returns>
    public virtual bool nextEventRequestAvailable(double logicalTime)
    {
      // Default implementation - so, native classes do not need to override
      OnRTIEventOccured(new RaconEventArgs("nextEventRequestAvailable() is HLA13 specific. Use nextMessageRequestAvailable() for HLA1516e.", LogLevel.WARN));
      return false;
    }

    /// <summary>
    /// IEEE1516.1-2010: 8.12 Flush Queue Request
    /// </summary>
    /// <param name="logicalTime"></param>
    /// <returns></returns>
    public abstract bool flushQueueRequest(double logicalTime);

    /// <summary>
    /// IEEE1516.1-2010: 8.14 Enable Asynchronous Delivery
    /// </summary>
    /// <returns></returns>
    public abstract bool enableAsynchronousDelivery();

    /// <summary>
    /// IEEE1516.1-2010: 8.15 Disable Asynchronous Delivery
    /// </summary>
    /// <returns></returns>
    public abstract bool disableAsynchronousDelivery();

    /// <summary>
    /// IEEE1516.1-2010: 8.16 Query Greatest Available Logical Time (GALT)
    /// </summary>
    /// <param name="Galt">Optional current value of invoking joined federate’s GALT</param>
    /// <returns>GALT definition indicator returns True if GALT is defined</returns>
    public virtual bool queryGALT(out double Galt)
    {
      // Default implementation - so, native classes do not need to override
      OnRTIEventOccured(new RaconEventArgs("queryGALTS() is IEEE1516.1-2010 specific. Use queryLBTS() for HLA13."));
      Galt = -1;//sentinel value
      return false;
    }

    /// <summary>
    /// HLA13 - queryLBTS
    /// </summary>
    /// <returns></returns>
    public virtual double queryLBTS()
    {
      // Default implementation - so, native classes do not need to override
      OnRTIEventOccured(new RaconEventArgs("queryLBTS() is HLA13 specific. Use queryGALTS() for HLA1516e.", LogLevel.WARN));
      return -1;
    }

    /// <summary>
    /// IEEE1516.1-2010: 8.17 Query Logical Time
    /// </summary>
    /// <returns></returns>
    public virtual double queryLogicalTime()
    {
      // Default implementation - so, native classes do not need to override
      OnRTIEventOccured(new RaconEventArgs("queryLogicalTime() is IEEE1516.1-2010 specific. Use queryFederateTime() for HLA13.", LogLevel.WARN));
      return -1;
    }

    /// <summary>
    /// HLA13 - queryFederateTime
    /// </summary>
    /// <returns></returns>
    public virtual double queryFederateTime()
    {
      // Default implementation - so, native classes do not need to override
      OnRTIEventOccured(new RaconEventArgs("queryFederateTime() is HLA13 specific. Use queryLogicalTime() for HLA1516e.", LogLevel.WARN));
      return -1;
    }

    /// <summary>
    /// IEEE1516.1-2010: 8.18 Query List Incoming Time Stamp (LIST)
    /// </summary>
    /// <param name="Lits">Optional current value of invoking joined federate’s LITS</param>
    /// <returns>LITS definition indicator returns True if LIST is defined</returns>
    public virtual bool queryLITS(out double Lits)
    {
      // Default implementation - so, native classes do not need to override
      OnRTIEventOccured(new RaconEventArgs("queryLITS() is IEEE1516.1-2010 specific. Use queryMinNextEventTime() for HLA13.", LogLevel.WARN));
      Lits = -1;//sentinel value
      return false;
    }

    /// <summary>
    /// HLA13 - queryMinNextEventTime
    /// </summary>
    /// <returns></returns>
    public virtual double queryMinNextEventTime()
    {
      // Default implementation - so, native classes do not need to override
      OnRTIEventOccured(new RaconEventArgs("queryMinNextEventTime() is HLA13 specific. Use queryLITS() for HLA1516e.", LogLevel.WARN));
      return -1;
    }

    /// <summary>
    /// IEEE1516.1-2010: 8.19 Modify Lookahead
    /// </summary>
    /// <param name="theLookahead"></param>
    /// <returns></returns>
    public abstract bool modifyLookahead(double theLookahead);

    /// <summary>
    /// IEEE1516.1-2010: 8.20 Query Lookahead
    /// </summary>
    /// <returns></returns>
    public abstract double queryLookahead();

    /// <summary>
    /// IEEE1516.1-2010: 8.21 Retracts the message with specified retraction handle.
    /// </summary>
    /// <param name="retraction"></param>
    /// <returns></returns>
    public abstract bool retract(MessageRetraction retraction);
    /// <summary>
    /// IEEE1516.1-2010: 8.23 Change Attribute Order Type
    /// </summary>
    /// <param name="theObject"></param>
    /// <param name="theAttributes"></param>
    /// <param name="theType"></param>
    /// <returns></returns>
    public abstract bool changeAttributeOrderType(HlaObject theObject, List<HlaAttribute> theAttributes, uint theType); // 6.23
    /// <summary>
    /// IEEE1516.1-2010: 8.24 Change Interaction Order Type
    /// </summary>
    /// <param name="theClass"></param>
    /// <param name="theType"></param>
    /// <returns></returns>
    public abstract bool changeInteractionOrderType(HlaInteractionClass theClass, uint theType);

    #endregion// Time Management

    #region Support Services
    /// <summary>
    /// IEEE1516.1-2010 10.4: getFederateHandle
    /// </summary>
    /// <param name="federateName"></param>
    /// <returns></returns>
    public abstract uint getFederateHandle(string federateName);
    /// <summary>
    /// IEEE1516.1-2010 10.5: getFederateName
    /// </summary>
    /// <param name="federateHandle"></param>
    /// <returns></returns>
    public abstract string getFederateName(uint federateHandle);
    /// <summary>
    /// IEEE1516.1-2010 10.25
    /// </summary>
    /// <param name="name">Dimension name</param>
    /// <returns></returns>
    public virtual uint getDimensionHandle(string name)
    {
      // Default implementation - so, native classes do not need to override
      OnRTIEventOccured(new RaconEventArgs("getDimensionHandle() is IEEE1516.1-2010 specific.Use other overload methods for HLA13.", LogLevel.ERROR));
      return 0;//sentinel value
    }

    /// <summary>
    /// HLA13: getDimensionHandle
    /// </summary>
    /// <param name="p1"></param>
    /// <param name="p2"></param>
    /// <returns></returns>
    public virtual uint getDimensionHandle(string p1, uint p2)
    {
      // Default implementation - so, native classes do not need to override
      OnRTIEventOccured(new RaconEventArgs("getDimensionHandle(string p1, uint p2) is HLA13 specific. Use other overload methods for IEEE1516.", LogLevel.WARN));
      return 0;//sentinel value
    }

    /// <summary>
    /// IEEE1516.1-2010 10.26
    /// </summary>
    /// <param name="handle">Dimension handle</param>
    /// <returns></returns>
    public virtual string getDimensionName(uint handle)
    {
      // Default implementation - so, native classes do not need to override
      OnRTIEventOccured(new RaconEventArgs("getDimensionName() is IEEE1516.1-2010 specific. Use other overload methods for HLA13.", LogLevel.ERROR));
      return "";//sentinel value
    }

    /// <summary>
    /// IEEE1516.1-2010 10.30
    /// </summary>
    /// <param name="regionHandle"></param>
    /// <param name="dimensionHandle"></param>
    /// <param name="lowerBound"></param>
    /// <param name="upperBound"></param>
    public virtual void setRangeBounds(uint regionHandle, uint dimensionHandle, uint lowerBound, uint upperBound)
    {
      // Default implementation - so, native classes do not need to override
      OnRTIEventOccured(new RaconEventArgs("setRangeBounds() is IEEE1516.1-2010 specific. Use other overload methods for HLA13.", LogLevel.ERROR));
    }

    /// <summary>
    /// IEEE1516.1-2010: 10.33 enableObjectClassRelevanceAdvisorySwitch
    /// </summary>
    /// <returns></returns>
    public abstract bool enableObjectClassRelevanceAdvisorySwitch();
    /// <summary>
    /// IEEE1516.1-2010: 10.34 disableObjectClassRelevanceAdvisorySwitch
    /// </summary>
    /// <returns></returns>
    public abstract bool disableObjectClassRelevanceAdvisorySwitch();
    /// <summary>
    /// IEEE1516.1-2010: 10.35 enableAttributeRelevanceAdvisorySwitch
    /// </summary>
    /// <returns></returns>
    public abstract bool enableAttributeRelevanceAdvisorySwitch();
    /// <summary>
    /// IEEE1516.1-2010: 10.36 disableAttributeRelevanceAdvisorySwitch
    /// </summary>
    /// <returns></returns>
    public abstract bool disableAttributeRelevanceAdvisorySwitch();
    /// <summary>
    /// IEEE1516.1-2010: 10.37 disableAttributeRelevanceAdvisorySwitch
    /// </summary>
    /// <returns></returns>
    public abstract bool enableAttributeScopeAdvisorySwitch();
    /// <summary>
    /// IEEE1516.1-2010: 10.38 disableAttributeRelevanceAdvisorySwitch
    /// </summary>
    /// <returns></returns>
    public abstract bool disableAttributeScopeAdvisorySwitch();
    /// <summary>
    /// IEEE1516.1-2010: 10.39 enableInteractionRelevanceAdvisorySwitch
    /// </summary>
    /// <returns></returns>
    public abstract bool enableInteractionRelevanceAdvisorySwitch();
    /// <summary>
    /// IEEE1516.1-2010: 10.40 disableInteractionRelevanceAdvisorySwitch
    /// </summary>
    /// <returns></returns>
    public abstract bool disableInteractionRelevanceAdvisorySwitch();
    /// <summary>
    /// IEEE1516.1-2010: 10.41 evokeCallback
    /// </summary>
    /// <param name="approximateMinimumTimeInSeconds"></param>
    public abstract void evokeCallback(double approximateMinimumTimeInSeconds);
    /// <summary>
    /// IEEE1516.1-2010: 10.42 evokeMultipleCallbacks
    /// </summary>
    /// <param name="approximateMinimumTimeInSeconds"></param>
    /// <param name="approximateMaximumTimeInSeconds"></param>
    public abstract void evokeMultipleCallbacks(double approximateMinimumTimeInSeconds, double approximateMaximumTimeInSeconds);//10.42
    /// <summary>
    /// HLA13 - getSpaceHandle
    /// </summary>
    /// <param name="p"></param>
    /// <returns></returns>
    public virtual uint getSpaceHandle(string p) { return 0; }
    /// <summary>
    /// getClassHandleFromRti
    /// </summary>
    /// <param name="ic"></param>
    public abstract void getClassHandleFromRti(HlaInteractionClass ic);
    /// <summary>
    /// getClassHandleFromRti
    /// </summary>
    /// <param name="oc"></param>
    public abstract void getClassHandleFromRti(HlaObjectClass oc);
    /// <summary>
    /// getParameterHandlesFromRti
    /// </summary>
    /// <param name="ic"></param>
    public abstract void getParameterHandlesFromRti(HlaInteractionClass ic);
    /// <summary>
    /// getAttributeHandlesFromRti
    /// </summary>
    /// <param name="oc"></param>
    public abstract void getAttributeHandlesFromRti(HlaObjectClass oc);
    #endregion// Support Services

    ///// <summary>
    ///// calls destructors of native RTI pointers.
    ///// </summary>
    //public abstract void Dispose();

    #endregion

  }
}
