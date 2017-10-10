// ******************************************************************************
//                          RtiAmb_OpenRti_1516e.h
//		begin                : Feb 03, 2016
// ******************************************************************************

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

#pragma once
#include "RTI/RTI1516.h"
#include "FdAmb_Hla1516e.h"

namespace Racon
{
  namespace RtiLayer {
    namespace Native {
      ref class CHlaRegion;


      public ref class RtiAmb_Hla1516e : public RtiAmb {

#pragma region Fields
      protected:
        rti1516e::RTIambassador* rti; // native rti ambassador
        FdAmb_Hla1516e* _nativeFdAmb; // native federate ambassador
        NomHla1516e* nom; // Native object model tracker
#pragma endregion			

#pragma region Constructors
      public:
        RtiAmb_Hla1516e() {};
        RtiAmb_Hla1516e(CallbackManager^);

        ~RtiAmb_Hla1516e()
        {
          this->!RtiAmb_Hla1516e();
        };
        !RtiAmb_Hla1516e()
        {
          delete rti;
          rti = 0;
          delete _nativeFdAmb;
        };
#pragma endregion						

#pragma region Methods
#pragma region Fedaration Management
      public:
        void connect(CallbackModel, String^) override;
        virtual void disconnect(void) override;
        virtual void createFederation(String^, String^, String^) override;
        virtual void createFederation(String^, List<String^>^, String^) override;
        virtual void createFederation(String^, List<String^>^, String^, String^) override;
        virtual void destroyFederation(String^) override;
        virtual void listFederationExecutions(void) override; // 4.7
        virtual unsigned int joinFederationExecution(String^, String^, List<String^>^ modules) override;
        virtual unsigned int joinFederationExecution(String^, String^, String^, List<String^>^ modules) override;
        virtual void resignFederation(int) override;
        virtual void registerFederationSynchronizationPoint(String^ synchronizationPointLabel, String^ userSuppliedTag) override; // 4.11
        virtual void registerFederationSynchronizationPoint(String^ synchronizationPointLabel, String^ userSuppliedTag, List<unsigned int>^ setOfJoinedFederateDesignators) override; // 4.11
        virtual void synchronizationPointAchieved(String^ synchronizationPointLabel, bool synchronizationSuccess) override; // 4.14
        virtual void requestFederationSave(String^) override;
        virtual void requestFederationSave(String^, Double) override;
        virtual void federateSaveBegun() override;
        virtual void federateSaveComplete(bool) override;
        virtual void requestFederationRestore(String^) override;
        virtual void federateRestoreComplete(bool) override;
#pragma endregion			

#pragma region Declaration Management
      public:
        virtual void publishObjectClass(HlaObjectClass ^, List<HlaAttribute^>^) override;
        virtual void publishInteractionClass(HlaInteractionClass ^) override;
        virtual void subscribeInteractionClass(HlaInteractionClass ^) override;
        virtual bool subscribeObjectClass(HlaObjectClass ^, List<HlaAttribute^>^, Boolean) override;
        virtual bool unsubscribeInteractionClass(HlaInteractionClass ^) override;
        virtual bool unsubscribeObjectClass(HlaObjectClass ^) override;
        //virtual void publishInteractionClass(HlaInteractionClass ^) override;
#pragma endregion	// Declaration Management		

#pragma region Object Management
      public:
        bool updateAttributeValues(HlaObject^, VariableLengthDataWrapper^) override;
        MessageRetraction^ updateAttributeValues(HlaObject^, VariableLengthDataWrapper^, Double) override;
        bool sendInteraction(HlaInteraction^, VariableLengthDataWrapper^) override;
        MessageRetraction^ sendInteraction(HlaInteraction^, VariableLengthDataWrapper^, Double) override;
        bool deleteObjectInstance(HlaObject ^, VariableLengthDataWrapper^) override;
        MessageRetraction^ deleteObjectInstance(HlaObject ^, VariableLengthDataWrapper^, Double) override;
        virtual bool requestAttributeValueUpdate(HlaObjectClass^, List<HlaAttribute^>^, VariableLengthDataWrapper^) override;
        virtual bool requestAttributeValueUpdate(HlaObject^, List<HlaAttribute^>^, VariableLengthDataWrapper^) override;
#pragma endregion	// Object Management

#pragma region Ownership Management
      public:
        virtual bool unconditionalAttributeOwnershipDivestiture(HlaObject ^, List<HlaAttribute^>^) override;
        bool negotiatedAttributeOwnershipDivestiture(HlaObject ^, List<HlaAttribute^>^, VariableLengthDataWrapper^) override;
        bool confirmDivestiture(HlaObject ^, List<HlaAttribute^>^, VariableLengthDataWrapper^) override;
        bool attributeOwnershipAcquisition(HlaObject ^, List<HlaAttribute^>^, VariableLengthDataWrapper^) override;
        virtual bool attributeOwnershipAcquisitionIfAvailable(HlaObject ^, List<HlaAttribute^>^) override;
        virtual bool attributeOwnershipReleaseDenied(HlaObject ^, List<HlaAttribute^>^) override;
        virtual bool attributeOwnershipDivestitureIfWanted(HlaObject ^, List<HlaAttribute^>^, List<HlaAttribute^>^%) override;
        virtual bool cancelNegotiatedAttributeOwnershipDivestiture(HlaObject ^, List<HlaAttribute^>^) override;
        virtual bool cancelAttributeOwnershipAcquisition(HlaObject ^, List<HlaAttribute^>^) override;
        virtual bool queryAttributeOwnership(HlaObject ^, HlaAttribute^) override;
        virtual bool isAttributeOwnedByFederate(HlaObject ^, HlaAttribute^) override;
#pragma endregion // Ownership Management

#pragma region Data Distribution Management
      public:
        virtual unsigned int createRegion(HlaRegion^, List<HlaDimension^>^) override;
        virtual void deleteRegion(HlaRegion^) override;
        virtual void registerObjectInstanceWithRegions(HlaObject ^, AttributeHandleSetRegionHandleSetPairVector ^pairs) override;
        virtual bool subscribeObjectClassAttributesWithRegions(HlaObjectClass ^oc, AttributeHandleSetRegionHandleSetPairVector ^list, bool indicator) override;
        virtual bool unsubscribeObjectClassWithRegions(HlaObjectClass ^, AttributeHandleSetRegionHandleSetPairVector ^list) override;
        virtual bool subscribeInteractionClass(HlaInteractionClass ^, List<HlaRegion^>^, bool indicator) override;
        virtual bool unsubscribeInteractionClass(HlaInteractionClass ^, List<HlaRegion^>^) override;
        bool sendInteractionWithRegions(HlaInteraction ^, List<HlaRegion^>^ regions, VariableLengthDataWrapper^) override;
        MessageRetraction^ sendInteractionWithRegions(HlaInteraction ^, List<HlaRegion^>^ regions, VariableLengthDataWrapper^, Double) override;
        virtual bool associateRegionsForUpdates(HlaObject ^, AttributeHandleSetRegionHandleSetPairVector ^list) override;
        virtual bool unassociateRegionsForUpdates(HlaObject ^, AttributeHandleSetRegionHandleSetPairVector ^list) override;
        virtual bool requestClassAttributeValueUpdateWithRegions(HlaObjectClass ^, AttributeHandleSetRegionHandleSetPairVector ^list, VariableLengthDataWrapper ^) override;
#pragma endregion	// Data Distribution Management

#pragma region Time Management
      public:
        virtual bool enableTimeRegulation(Double) override;
        virtual bool disableTimeRegulation(void) override;
        virtual bool enableTimeConstrained(void) override;
        virtual bool disableTimeConstrained(void) override;
        virtual bool timeAdvanceRequest(Double) override;
        /// <summary>
        /// IEEE1516.1-2010: 8.9 Time Advance Request Available
        /// </summary>
        virtual bool timeAdvanceRequestAvailable(Double) override;
        /// <summary>
        /// IEEE1516.1-2010: 8.10 Next Message Request
        /// </summary>
        /// <param name="logicalTime"></param>
        /// <returns></returns>
        virtual bool nextMessageRequest(Double) override;
        /// <summary>
        /// IEEE1516.1-2010: 8.11 Next Message Request Available
        /// </summary>
        /// <param name="logicalTime"></param>
        /// <returns></returns>
        virtual bool nextMessageRequestAvailable(Double) override;
        virtual bool flushQueueRequest(Double) override;
        /// <summary>
        /// IEEE1516.1-2010: 8.14 Enable Asynchronous Delivery
        /// </summary>
        /// <returns></returns>
        virtual bool enableAsynchronousDelivery(void) override;
        /// <summary>
        /// IEEE1516.1-2010: 8.15 Disable Asynchronous Delivery
        /// </summary>
        /// <returns></returns>
        virtual bool disableAsynchronousDelivery(void) override;
        /// <summary>
        /// IEEE1516.1-2010: 8.16 Query Greatest Available Logical Time (GALT)
        /// </summary>
        /// <returns></returns>
        virtual bool queryGALT(Double% Galt) override;
        /// <summary>
        /// IEEE1516.1-2010: 8.17 Query Logical Time
        /// </summary>
        /// <returns></returns>
        virtual Double queryLogicalTime(void) override;
        /// <summary>
        /// IEEE1516.1-2010: 8.18 Query List Incoming Time Stamp (LIST)
        /// </summary>
        /// <param name="Lits">Optional current value of invoking joined federate’s LITS</param>
        /// <returns>LITS definition indicator returns True if LIST is defined</returns>
        virtual bool queryLITS(Double% Galt) override;
        /// <summary>
        /// IEEE1516.1-2010: 8.20 Query Lookahead
        /// </summary>
        /// <returns></returns>
        virtual Double queryLookahead(void) override;
        virtual bool modifyLookahead(Double) override;
        virtual bool retract(MessageRetraction^) override;
        virtual bool changeAttributeOrderType(HlaObject ^, List<HlaAttribute^>^, unsigned int) override;
        virtual bool changeInteractionOrderType(HlaInteractionClass ^, unsigned int) override;
#pragma endregion

#pragma region Support Services
      public:
        void  setRangeBounds(unsigned int regionHandle, unsigned int dimensionHandle, unsigned int lowerBound, unsigned int upperBound) override;// 10.30
        virtual bool enableObjectClassRelevanceAdvisorySwitch(void) override;// !!! Not implemented in Portico
        virtual bool disableObjectClassRelevanceAdvisorySwitch(void) override; // !!! Not implemented in Portico
        virtual bool enableAttributeRelevanceAdvisorySwitch(void) override; // !!! Not implemented in Portico
        virtual bool disableAttributeRelevanceAdvisorySwitch(void) override; // !!! Not implemented in Portico
        virtual bool enableAttributeScopeAdvisorySwitch(void) override;// !!! Not implemented in Portico
        virtual bool disableAttributeScopeAdvisorySwitch(void) override; // !!! Not implemented in Portico
        virtual bool enableInteractionRelevanceAdvisorySwitch(void) override;// !!! Not implemented in Portico
        virtual bool disableInteractionRelevanceAdvisorySwitch(void) override;// !!! Not implemented in Portico
        //virtual unsigned int getDimensionHandle(String ^, unsigned int) override;
        virtual void evokeCallback(double) override;
        virtual void evokeMultipleCallbacks(double, double) override;
#pragma endregion

#pragma region Helpers
      protected: // IEEE1516 Specific
        virtual rti1516e::ObjectClassHandle getObjectClassHandle(HlaObjectClass ^) = 0;
        virtual rti1516e::InteractionClassHandle getInteractionClassHandle(HlaInteractionClass ^) = 0;
        virtual rti1516e::AttributeHandle getAttributeHandle(HlaAttribute^, rti1516e::ObjectClassHandle) = 0;
        virtual rti1516e::ParameterHandle getParameterHandle(HlaParameter^, rti1516e::InteractionClassHandle) = 0;

#pragma endregion

#pragma endregion

      };
    }
  }
}

