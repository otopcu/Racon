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

// Managed Wrapper Class for HLA13 - DMSO
#pragma once
#include<RTI.hh>
#include "Helpers_Hla13.h"
#include "Rtiamb.h"
#include "FdAmb_Hla13.h"

using namespace System;
using namespace System::ComponentModel;
// Racon
using namespace Racon;
using namespace Racon::RtiLayer;

namespace Racon {
	namespace RtiLayer {
		namespace Native {
			public ref class RtiAmb_Hla13 : public RtiAmb {

#pragma region Fields
			protected:
				RTI::RTIambassador* rti;// native rti ambassador
				FdAmb_Hla13* _nativeFdAmb;// native federate ambassador
        NomHla13* nom; // Native object model tracker
#pragma endregion			

#pragma region Ctor/Dtor
			public:
				RtiAmb_Hla13(CallbackManager^);
				~RtiAmb_Hla13() { rti->~RTIambassador(); _nativeFdAmb->~FdAmb_Hla13(); };
#pragma endregion						

#pragma region Methods
#pragma region Fedaration Management
			public:
				void connect(CallbackModel, String^) override;
				virtual void disconnect() override;
				virtual void createFederation(String^, String^, String^) override;
				virtual void destroyFederation(String^) override;
				virtual void listFederationExecutions(void) override; // Not supported by HLA13
				virtual unsigned int joinFederationExecution(String^, String^) override;
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
				virtual void subscribeInteractionClass(HlaInteractionClass ^) override;
				virtual bool subscribeObjectClass(HlaObjectClass ^, List<HlaAttribute^>^, Boolean) override;
				virtual bool unsubscribeInteractionClass(HlaInteractionClass ^) override;
				virtual bool unsubscribeObjectClass(HlaObjectClass ^) override;
				virtual void publishInteractionClass(HlaInteractionClass ^) override;
#pragma endregion	// Declaration Management		

#pragma region Object Management
			public:
				virtual bool deleteObjectInstance(HlaObject ^) override;
				virtual MessageRetraction^ deleteObjectInstance(HlaObject ^, Double) override;
				virtual bool registerObject(HlaObject ^) override;
				virtual bool registerObject(HlaObject ^, String^) override;
				virtual bool updateAttributeValues(HlaObject ^) override;
				virtual MessageRetraction^ updateAttributeValues(HlaObject ^, Double) override;
				virtual bool sendInteraction(HlaInteraction ^) override;
				virtual MessageRetraction^ sendInteraction(HlaInteraction ^, Double) override;
				virtual bool requestAttributeValueUpdate(HlaObjectClass ^, List<HlaAttribute^>^) override;
				virtual bool requestAttributeValueUpdate(HlaObject ^, List<HlaAttribute^>^) override;
#pragma endregion	// Object Management

#pragma region Ownership Management
			public:
				virtual bool attributeOwnershipAcquisitionIfAvailable(HlaObject ^, List<HlaAttribute^>^) override;
				virtual bool attributeOwnershipAcquisition(HlaObject ^, List<HlaAttribute^>^, String^) override;
				virtual bool cancelAttributeOwnershipAcquisition(HlaObject ^, List<HlaAttribute^>^) override;
				virtual bool queryAttributeOwnership(HlaObject ^, HlaAttribute^) override;
				virtual bool attributeOwnershipReleaseResponse(HlaObject ^, List<HlaAttribute^>^) override;
				virtual bool cancelNegotiatedAttributeOwnershipDivestiture(HlaObject ^, List<HlaAttribute^>^) override;
				virtual bool isAttributeOwnedByFederate(HlaObject ^, HlaAttribute^) override;
				virtual bool negotiatedAttributeOwnershipDivestiture(HlaObject ^, List<HlaAttribute^>^, String^) override;
				virtual bool unconditionalAttributeOwnershipDivestiture(HlaObject ^, List<HlaAttribute^>^) override;
#pragma endregion // Ownership Management

#pragma region Data Distribution Management
			public:
				virtual bool createRegion(String^, unsigned long, long) override;
				virtual void deleteRegion(HlaRegion^) override;
				virtual void registerObjectInstanceWithRegions(HlaObject ^, List<HlaAttribute^>^, List<HlaRegion^>^) override;
				virtual bool subscribeObjectClassAttributesWithRegions(HlaObjectClass ^, List<HlaAttribute^>^, HlaRegion ^, Boolean) override;
				virtual bool unsubscribeObjectClassWithRegions(HlaObjectClass ^, HlaRegion ^) override;
				virtual bool requestClassAttributeValueUpdateWithRegion(HlaObjectClass ^, List<HlaAttribute^>^, HlaRegion ^) override;
				virtual bool subscribeInteractionClass(HlaInteractionClass ^, HlaRegion ^) override;
				virtual void unsubscribeInteractionClass(HlaInteractionClass ^, HlaRegion ^) override;
				virtual bool sendInteractionWithRegion(HlaInteraction ^, HlaRegion ^) override;
				virtual bool associateRegionForUpdates(HlaRegion ^, HlaObject ^, List<HlaAttribute^>^) override;
				virtual bool unassociateRegionForUpdates(HlaRegion ^, HlaObject ^) override;

#pragma endregion	// Data Distribution Management

#pragma region Time Management
			public:
				virtual bool enableAsynchronousDelivery(void) override;
				virtual bool disableAsynchronousDelivery(void) override;
				virtual bool disableTimeConstrained(void) override;
				virtual bool disableTimeRegulation(void) override;
				virtual bool enableTimeConstrained(void) override;
				virtual bool enableTimeRegulation(Double, Double) override;
				virtual bool flushQueueRequest(Double) override;
				virtual bool modifyLookahead(Double) override;
				virtual bool nextEventRequest(Double) override;
				virtual bool nextEventRequestAvailable(Double) override;
				virtual Double queryFederateTime(void) override;
				virtual Double queryLBTS(void) override;
				virtual Double queryLookahead(void) override;
				virtual Double queryMinNextEventTime(void) override;
				virtual bool retract(MessageRetraction^) override;
				virtual bool timeAdvanceRequest(Double) override;
				virtual bool timeAdvanceRequestAvailable(Double) override;
				virtual bool changeAttributeOrderType(HlaObject ^, List<HlaAttribute^>^, unsigned int) override;
				virtual bool changeInteractionOrderType(HlaInteractionClass ^, unsigned int) override;
#pragma endregion

#pragma region Support Services
			public:
				unsigned int getFederateHandle(String ^federateName) override {
					// Not implemented
					return 0;
				};
				String^  getFederateName(unsigned int federateHandle) override {
					// !!! Not implemented
					return "";
				};

				virtual bool disableAttributeRelevanceAdvisorySwitch(void) override;
				virtual bool disableAttributeScopeAdvisorySwitch(void) override;
				virtual bool disableObjectClassRelevanceAdvisorySwitch(void) override;
				virtual bool disableInteractionRelevanceAdvisorySwitch(void) override;
				virtual bool enableAttributeRelevanceAdvisorySwitch(void) override;
				virtual bool enableAttributeScopeAdvisorySwitch(void) override;
				virtual bool enableObjectClassRelevanceAdvisorySwitch(void) override;
				virtual bool enableInteractionRelevanceAdvisorySwitch(void) override;
				virtual void getClassHandleFromRti(HlaInteractionClass ^) override;
				virtual void getClassHandleFromRti(HlaObjectClass ^) override;
				virtual void getAttributeHandlesFromRti(HlaObjectClass ^) override;
				virtual void getParameterHandlesFromRti(HlaInteractionClass ^) override;
				virtual unsigned int getSpaceHandle(String ^) override;
				virtual unsigned int getDimensionHandle(String ^, unsigned int) override;
				virtual void evokeCallback(double) override;
				virtual void evokeMultipleCallbacks(double, double) override;
#pragma endregion

#pragma region Helpers
			protected:
				virtual Handle getAttributeHandle(HlaAttribute^, HlaObjectClass ^) override;
#pragma endregion

#pragma endregion

			};
		}
	}
}

