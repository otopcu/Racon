// ******************************************************************************
//                          RtiAmb_OpenRti_1516e.h
//		begin                : Feb 03, 2016
// ******************************************************************************

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

#pragma once
//#include"Helpers.h"
#include "RTI/RTI1516.h"
//#include "Rtiamb.h"
#include "FdAmb_Hla1516e.h"
//#include "OmHla1516e.h"

//using namespace System;
//using namespace System::Windows::Forms;
//using namespace System::ComponentModel;
//using namespace rti1516e;

namespace Racon
{
	namespace RtiLayer {
		ref class CHlaRegion;
		ref class HlaInteraction;

		public ref class RtiAmb_Hla1516e : public CRtiAmb {

#pragma region Fields
		protected:
			rti1516e::RTIambassador* rti; // native rti ambassador
			FdAmb_Hla1516e* _nativeFdAmb; // native federate ambassador
			OmHla1516e* om; // Native object model tracker
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
			void connect(String^) override;
			virtual void disconnect() override;
			virtual void createFederation(String^, String^) override;
			virtual void joinFederation(String^, String^) override;
			virtual void resignFederation(int) override;
			virtual void destroyFederation(String^) override;
			virtual void requestFederationSave(String^) override;
			virtual void requestFederationSave(String^, Double) override;
			virtual void federateSaveBegun() override;
			virtual void federateSaveComplete(bool) override;
			virtual void requestFederationRestore(String^) override;
			virtual void federateRestoreComplete(bool) override;
#pragma endregion			

#pragma region Declaration Management
		public:
			virtual void publishObjectClass(HlaObjectClass ^, BindingList<HlaAttribute^>^) override;
			virtual void publishInteractionClass(HlaInteractionClass ^) override;
			virtual void subscribeInteractionClass(HlaInteractionClass ^) override;
			virtual bool subscribeObjectClass(HlaObjectClass ^, BindingList<HlaAttribute^>^, Boolean) override;
			virtual bool unsubscribeInteractionClass(HlaInteractionClass ^) override;
			virtual bool unsubscribeObjectClass(HlaObjectClass ^) override;
			//virtual void publishInteractionClass(HlaInteractionClass ^) override;
#pragma endregion	// Declaration Management		

#pragma region Object Management
		public:
			virtual bool deleteObjectInstance(HlaObject ^) override;
			virtual EventRetractionHandle^ deleteObjectInstance(HlaObject ^, Double) override;
			//virtual bool registerObject(HlaObject ^) override;
			//virtual bool registerObject(HlaObject ^, String^) override;
			virtual bool updateAttributeValues(HlaObject ^) override;
			virtual EventRetractionHandle^ updateAttributeValues(HlaObject ^, Double) override;
			virtual bool sendInteraction(HlaInteraction ^) override;
			virtual EventRetractionHandle^ sendInteraction(HlaInteraction ^, Double) override;
			virtual bool requestAttributeValueUpdate(HlaObjectClass ^, List<HlaAttribute^>^) override;
			virtual bool requestAttributeValueUpdate(HlaObject ^, List<HlaAttribute^>^) override;
#pragma endregion	// Object Management

#pragma region Ownership Management
		public:
			virtual bool attributeOwnershipAcquisitionIfAvailable(HlaObject ^, RaconAttributeSet^) override;
			virtual bool attributeOwnershipAcquisition(HlaObject ^, RaconAttributeSet^) override;
			virtual bool cancelAttributeOwnershipAcquisition(HlaObject ^, RaconAttributeSet^) override;
			virtual bool queryAttributeOwnership(HlaObject ^, HlaAttribute^) override;
			virtual bool attributeOwnershipReleaseResponse(HlaObject ^, RaconAttributeSet^) override;
			virtual bool cancelNegotiatedAttributeOwnershipDivestiture(HlaObject ^, RaconAttributeSet^) override;
			virtual bool isAttributeOwnedByFederate(HlaObject ^, HlaAttribute^) override;
			virtual bool negotiatedAttributeOwnershipDivestiture(HlaObject ^, RaconAttributeSet^) override;
			virtual bool unconditionalAttributeOwnershipDivestiture(HlaObject ^, RaconAttributeSet^) override;
#pragma endregion // Ownership Management

#pragma region Data Distribution Management
		public:
			virtual bool associateRegionForUpdates(CHlaRegion ^, HlaObject ^, List<HlaAttribute^>^) override;
			virtual bool changeAttributeOrderType(HlaObject ^, List<HlaAttribute^>^, OrderingHandle) override;
			virtual bool changeInteractionOrderType(HlaInteractionClass ^, OrderingHandle) override;
			virtual bool createRegion(CHlaRegion^, unsigned long, long) override;
			virtual bool deleteRegion(CHlaRegion^) override;
			virtual bool registerObjectInstanceWithRegion(HlaObject ^, List<HlaAttribute^>^, List<CHlaRegion^>^) override;
			//virtual bool registerObjectInstanceWithRegion(HlaObject ^, List<HlaAttribute^>^, List<CHlaRegion^>^, String ^) override;
			//virtual bool registerObjectInstanceWithRegion(HlaObject ^, List<HlaAttribute^>^, CHlaRegion ^) override;// Registering object with a specific region with all its attributes - WITH OBJECT NAME
			//virtual bool registerObjectInstanceWithRegion(HlaObject ^, List<HlaAttribute^>^, CHlaRegion ^, String ^) override;// Registering object with a specific region with all its attributes
			virtual bool requestClassAttributeValueUpdateWithRegion(HlaObjectClass ^, List<HlaAttribute^>^, CHlaRegion ^) override;
			virtual bool sendInteraction(HlaInteraction ^, CHlaRegion ^) override;// Send Interaction with Region
			virtual bool subscribeObjectClassAttributesWithRegion(HlaObjectClass ^, List<HlaAttribute^>^, CHlaRegion ^, Boolean) override;// subscribeOC with Region
			// subscribe IC with Region
			virtual bool subscribeInteractionClass(HlaInteractionClass ^, CHlaRegion ^) override;
			// unassociateRegionForUpdates
			virtual bool unassociateRegionForUpdates(CHlaRegion ^, HlaObject ^) override;
			// unsubscribe IC with Region
			virtual void unsubscribeInteractionClass(HlaInteractionClass ^, CHlaRegion ^) override;
			// unsubscribe OC with Region
			virtual void unsubscribeObjectClassWithRegion(HlaObjectClass ^, CHlaRegion ^) override;

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
			virtual bool retract(EventRetractionHandle^) override;
			virtual bool timeAdvanceRequest(Double) override;
			virtual bool timeAdvanceRequestAvailable(Double) override;
#pragma endregion

#pragma region Support Services
		public:
			virtual bool enableObjectClassRelevanceAdvisorySwitch(void) override;// !!! Not implemented in Portico
			virtual bool disableObjectClassRelevanceAdvisorySwitch(void) override; // !!! Not implemented in Portico
			virtual bool enableAttributeRelevanceAdvisorySwitch(void) override; // !!! Not implemented in Portico
			virtual bool disableAttributeRelevanceAdvisorySwitch(void) override; // !!! Not implemented in Portico
			virtual bool enableAttributeScopeAdvisorySwitch(void) override;// !!! Not implemented in Portico
			virtual bool disableAttributeScopeAdvisorySwitch(void) override; // !!! Not implemented in Portico
			virtual bool enableInteractionRelevanceAdvisorySwitch(void) override;// !!! Not implemented in Portico
			virtual bool disableInteractionRelevanceAdvisorySwitch(void) override;// !!! Not implemented in Portico
			virtual unsigned int getDimensionHandle(String ^, unsigned int) override;
			virtual void evokeCallback(double) override;
			virtual void evokeMultipleCallbacks(double , double ) override;
#pragma endregion

#pragma region Helpers
		protected: // IEEE1516 Specific
			virtual rti1516e::ObjectClassHandle getObjectClassHandle(HlaObjectClass ^) = 0;
			virtual rti1516e::InteractionClassHandle getInteractionClassHandle(HlaInteractionClass ^) = 0;
			virtual rti1516e::AttributeHandle getAttributeHandle(HlaAttribute^, rti1516e::ObjectClassHandle) = 0;
			virtual rti1516e::ParameterHandle getParameterHandle(HlaParameter^, rti1516e::InteractionClassHandle) = 0;

			// Conversions for Handles - this may differentiate between portico and openrti
			virtual ULong Handle2Long(rti1516e::ObjectClassHandle handle) = 0;
			virtual ULong Handle2Long(rti1516e::InteractionClassHandle handle) = 0;
			virtual ULong Handle2Long(rti1516e::AttributeHandle handle) = 0;
			virtual ULong Handle2Long(rti1516e::ParameterHandle handle) = 0;
			virtual ULong Handle2Long(rti1516e::ObjectInstanceHandle handle)  = 0;
			virtual ULong Handle2Long(rti1516e::FederateHandle handle)  = 0;
			virtual ULong Handle2Long(rti1516e::RegionHandle handle) = 0;

#pragma endregion

#pragma endregion

		};
	}
}

