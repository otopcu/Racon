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
but WITHOUT ANY WARRANTY{} without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once
#include <iostream> // for using std
#include <RTI\NullFederateAmbassador.h>
#include <vcclr.h> // For gcroot<>
#include "Helpers_Hla1516e.h"

using namespace System;
// Racon
using namespace Racon::RtiLayer;

namespace Racon
{
	namespace RtiLayer {
		namespace Native {
			//ref class CallbackManager;

			public class FdAmb_Hla1516e : public rti1516e::NullFederateAmbassador
			{
				// Fields
			private:
				gcroot<CallbackManager^> wrapper;
				OmHla1516e* om; // Native object model tracker

				// Methods
#pragma region Methods
			public:
				// Constructor
				FdAmb_Hla1516e(CallbackManager^, OmHla1516e*);
				// Destructor
				virtual ~FdAmb_Hla1516e(void) throw() {}

#pragma region Federation Management Services
				// 4.4
				virtual void connectionLost(
					std::wstring const & faultDescription)
					throw (
						rti1516e::FederateInternalError);

				// 4.8
				virtual void reportFederationExecutions(
					rti1516e::FederationExecutionInformationVector const &
					theFederationExecutionInformationList)
					throw (
						rti1516e::FederateInternalError);

				// 4.12
				virtual void synchronizationPointRegistrationSucceeded(
					std::wstring const & label)
					throw (
						rti1516e::FederateInternalError);

				virtual void synchronizationPointRegistrationFailed(
					std::wstring const & label,
					rti1516e::SynchronizationPointFailureReason reason)
					throw (
						rti1516e::FederateInternalError);

				// 4.13
				virtual void announceSynchronizationPoint(
					std::wstring  const & label,
					rti1516e::VariableLengthData const & theUserSuppliedTag)
					throw (
						rti1516e::FederateInternalError);

				// 4.15
				virtual void federationSynchronized(
					std::wstring const & label,
					rti1516e::FederateHandleSet const& failedToSyncSet)
					throw (
						rti1516e::FederateInternalError);

				// 4.17
				virtual void initiateFederateSave(
					std::wstring const & label)
					throw (
						rti1516e::FederateInternalError);

				virtual void initiateFederateSave(
					std::wstring const & label,
					rti1516e::LogicalTime const & theTime)
					throw (
						rti1516e::FederateInternalError);

				// 4.20
				virtual void federationSaved()
					throw (
						rti1516e::FederateInternalError);

				virtual void federationNotSaved(
					rti1516e::SaveFailureReason theSaveFailureReason)
					throw (
						rti1516e::FederateInternalError);


				// 4.23
				virtual void federationSaveStatusResponse(
					rti1516e::FederateHandleSaveStatusPairVector const &
					theFederateStatusVector)
					throw (
						rti1516e::FederateInternalError) {}

				// 4.25
				virtual void requestFederationRestoreSucceeded(
					std::wstring const & label)
					throw (
						rti1516e::FederateInternalError) {}

				virtual void requestFederationRestoreFailed(
					std::wstring const & label)
					throw (
						rti1516e::FederateInternalError) {}

				// 4.26
				virtual void federationRestoreBegun()
					throw (
						rti1516e::FederateInternalError) {}

				// 4.27
				virtual void initiateFederateRestore(
					std::wstring const & label,
					std::wstring const & federateName,
					rti1516e::FederateHandle handle)
					throw (
						rti1516e::FederateInternalError) {}

				// 4.29
				virtual void federationRestored()
					throw (
						rti1516e::FederateInternalError) {}

				virtual void federationNotRestored(
					rti1516e::RestoreFailureReason theRestoreFailureReason)
					throw (
						rti1516e::FederateInternalError) {}

				// 4.32
				virtual void federationRestoreStatusResponse(
					rti1516e::FederateRestoreStatusVector const &
					theFederateRestoreStatusVector)
					throw (
						rti1516e::FederateInternalError) {}

#pragma endregion // Federation Management Services

#pragma region Declaration Management Services
				// 5.10
				virtual void startRegistrationForObjectClass(
					rti1516e::ObjectClassHandle theClass)
					throw (
						rti1516e::FederateInternalError);

				// 5.11
				virtual void stopRegistrationForObjectClass(
					rti1516e::ObjectClassHandle theClass)
					throw (
						rti1516e::FederateInternalError);

				// 5.12
				virtual void turnInteractionsOn(
					rti1516e::InteractionClassHandle theHandle)
					throw (
						rti1516e::FederateInternalError);

				// 5.13
				virtual void turnInteractionsOff(
					rti1516e::InteractionClassHandle theHandle)
					throw (
						rti1516e::FederateInternalError);
#pragma endregion // Declaration Management Services

#pragma region Object Management Services
				// 6.3
				virtual void objectInstanceNameReservationSucceeded(
					std::wstring const & theObjectInstanceName)
					throw (
						rti1516e::FederateInternalError) {}

				virtual void objectInstanceNameReservationFailed(
					std::wstring const & theObjectInstanceName)
					throw (
						rti1516e::FederateInternalError) {}

				// 6.6
				virtual void multipleObjectInstanceNameReservationSucceeded(
					std::set<std::wstring> const & theObjectInstanceNames)
					throw (
						rti1516e::FederateInternalError) {}

				virtual void multipleObjectInstanceNameReservationFailed(
					std::set<std::wstring> const & theObjectInstanceNames)
					throw (
						rti1516e::FederateInternalError) {}

				// 6.9
				virtual void discoverObjectInstance(
					rti1516e::ObjectInstanceHandle theObject,
					rti1516e::ObjectClassHandle theObjectClass,
					std::wstring const & theObjectInstanceName)
					throw (
						rti1516e::FederateInternalError);

				virtual void discoverObjectInstance(
					rti1516e::ObjectInstanceHandle theObject,
					rti1516e::ObjectClassHandle theObjectClass,
					std::wstring const & theObjectInstanceName,
					rti1516e::FederateHandle producingFederate)
					throw (
						rti1516e::FederateInternalError);

				// 6.11
				virtual void reflectAttributeValues(
					rti1516e::ObjectInstanceHandle theObject,
					rti1516e::AttributeHandleValueMap const & theAttributeValues,
					rti1516e::VariableLengthData const & theUserSuppliedTag,
					rti1516e::OrderType sentOrder,
					rti1516e::TransportationType theType,
					rti1516e::SupplementalReflectInfo theReflectInfo)
					throw (
						rti1516e::FederateInternalError);

				virtual void reflectAttributeValues(
					rti1516e::ObjectInstanceHandle theObject,
					rti1516e::AttributeHandleValueMap const & theAttributeValues,
					rti1516e::VariableLengthData const & theUserSuppliedTag,
					rti1516e::OrderType sentOrder,
					rti1516e::TransportationType theType,
					rti1516e::LogicalTime const & theTime,
					rti1516e::OrderType receivedOrder,
					rti1516e::SupplementalReflectInfo theReflectInfo)
					throw (
						rti1516e::FederateInternalError);

				virtual void reflectAttributeValues(
					rti1516e::ObjectInstanceHandle theObject,
					rti1516e::AttributeHandleValueMap const & theAttributeValues,
					rti1516e::VariableLengthData const & theUserSuppliedTag,
					rti1516e::OrderType sentOrder,
					rti1516e::TransportationType theType,
					rti1516e::LogicalTime const & theTime,
					rti1516e::OrderType receivedOrder,
					rti1516e::MessageRetractionHandle theHandle,
					rti1516e::SupplementalReflectInfo theReflectInfo)
					throw (
						rti1516e::FederateInternalError);

				// 6.13
				virtual void receiveInteraction(
					rti1516e::InteractionClassHandle theInteraction,
					rti1516e::ParameterHandleValueMap const & theParameterValues,
					rti1516e::VariableLengthData const & theUserSuppliedTag,
					rti1516e::OrderType sentOrder,
					rti1516e::TransportationType theType,
					rti1516e::SupplementalReceiveInfo theReceiveInfo)
					throw (
						rti1516e::FederateInternalError);

				virtual void receiveInteraction(
					rti1516e::InteractionClassHandle theInteraction,
					rti1516e::ParameterHandleValueMap const & theParameterValues,
					rti1516e::VariableLengthData const & theUserSuppliedTag,
					rti1516e::OrderType sentOrder,
					rti1516e::TransportationType theType,
					rti1516e::LogicalTime const & theTime,
					rti1516e::OrderType receivedOrder,
					rti1516e::SupplementalReceiveInfo theReceiveInfo)
					throw (
						rti1516e::FederateInternalError);
				// !!! doesnot trigger
				virtual void receiveInteraction(
					rti1516e::InteractionClassHandle theInteraction,
					rti1516e::ParameterHandleValueMap const & theParameterValues,
					rti1516e::VariableLengthData const & theUserSuppliedTag,
					rti1516e::OrderType sentOrder,
					rti1516e::TransportationType theType,
					rti1516e::LogicalTime const & theTime,
					rti1516e::OrderType receivedOrder,
					rti1516e::MessageRetractionHandle theHandle,
					rti1516e::SupplementalReceiveInfo theReceiveInfo)
					throw (
						rti1516e::FederateInternalError);

				// 6.15
				virtual void removeObjectInstance(
					rti1516e::ObjectInstanceHandle theObject,
					rti1516e::VariableLengthData const & theUserSuppliedTag,
					rti1516e::OrderType sentOrder,
					rti1516e::SupplementalRemoveInfo theRemoveInfo)
					throw (
						rti1516e::FederateInternalError);

				virtual void removeObjectInstance(
					rti1516e::ObjectInstanceHandle theObject,
					rti1516e::VariableLengthData const & theUserSuppliedTag,
					rti1516e::OrderType sentOrder,
					rti1516e::LogicalTime const & theTime,
					rti1516e::OrderType receivedOrder,
					rti1516e::SupplementalRemoveInfo theRemoveInfo)
					throw (
						rti1516e::FederateInternalError);

				virtual void removeObjectInstance(
					rti1516e::ObjectInstanceHandle theObject,
					rti1516e::VariableLengthData const & theUserSuppliedTag,
					rti1516e::OrderType sentOrder,
					rti1516e::LogicalTime const & theTime,
					rti1516e::OrderType receivedOrder,
					rti1516e::MessageRetractionHandle theHandle,
					rti1516e::SupplementalRemoveInfo theRemoveInfo)
					throw (
						rti1516e::FederateInternalError);

				// 6.17
				virtual void attributesInScope(
					rti1516e::ObjectInstanceHandle theObject,
					rti1516e::AttributeHandleSet const & theAttributes)
					throw (
						rti1516e::FederateInternalError) {}

				// 6.18
				virtual void attributesOutOfScope(
					rti1516e::ObjectInstanceHandle theObject,
					rti1516e::AttributeHandleSet const & theAttributes)
					throw (
						rti1516e::FederateInternalError) {}

				// 6.20
				virtual void provideAttributeValueUpdate(
					rti1516e::ObjectInstanceHandle theObject,
					rti1516e::AttributeHandleSet const & theAttributes,
					rti1516e::VariableLengthData const & theUserSuppliedTag)
					throw (
						rti1516e::FederateInternalError);

				// 6.21
				virtual void turnUpdatesOnForObjectInstance(
					rti1516e::ObjectInstanceHandle theObject,
					rti1516e::AttributeHandleSet const & theAttributes)
					throw (
						rti1516e::FederateInternalError);

				virtual void turnUpdatesOnForObjectInstance(
					rti1516e::ObjectInstanceHandle theObject,
					rti1516e::AttributeHandleSet const & theAttributes,
					std::wstring const & updateRateDesignator)
					throw (
						rti1516e::FederateInternalError);

				// 6.22
				virtual void turnUpdatesOffForObjectInstance(
					rti1516e::ObjectInstanceHandle theObject,
					rti1516e::AttributeHandleSet const & theAttributes)
					throw (
						rti1516e::FederateInternalError) {}

				// 6.24
				virtual void confirmAttributeTransportationTypeChange(
					rti1516e::ObjectInstanceHandle theObject,
					rti1516e::AttributeHandleSet theAttributes,
					rti1516e::TransportationType theTransportation)
					throw (
						rti1516e::FederateInternalError) {}

				// 6.26
				virtual void reportAttributeTransportationType(
					rti1516e::ObjectInstanceHandle theObject,
					rti1516e::AttributeHandle theAttribute,
					rti1516e::TransportationType theTransportation)
					throw (
						rti1516e::FederateInternalError) {}

				// 6.28
				virtual void confirmInteractionTransportationTypeChange(
					rti1516e::InteractionClassHandle theInteraction,
					rti1516e::TransportationType theTransportation)
					throw (
						rti1516e::FederateInternalError) {}

				// 6.30
				virtual void reportInteractionTransportationType(
					rti1516e::FederateHandle federateHandle,
					rti1516e::InteractionClassHandle theInteraction,
					rti1516e::TransportationType  theTransportation)
					throw (
						rti1516e::FederateInternalError) {}
#pragma endregion // Object Management Services

#pragma region Ownership Management Services
				// 7.4
				virtual void requestAttributeOwnershipAssumption(
					rti1516e::ObjectInstanceHandle theObject,
					rti1516e::AttributeHandleSet const & offeredAttributes,
					rti1516e::VariableLengthData const & theUserSuppliedTag)
					throw (
						rti1516e::FederateInternalError) {}

				// 7.5
				virtual void requestDivestitureConfirmation(
					rti1516e::ObjectInstanceHandle theObject,
					rti1516e::AttributeHandleSet const & releasedAttributes)
					throw (
						rti1516e::FederateInternalError) {}

				// 7.7
				virtual void attributeOwnershipAcquisitionNotification(
					rti1516e::ObjectInstanceHandle theObject,
					rti1516e::AttributeHandleSet const & securedAttributes,
					rti1516e::VariableLengthData const & theUserSuppliedTag)
					throw (
						rti1516e::FederateInternalError) {}

				// 7.10
				virtual void attributeOwnershipUnavailable(
					rti1516e::ObjectInstanceHandle theObject,
					rti1516e::AttributeHandleSet const & theAttributes)
					throw (
						rti1516e::FederateInternalError) {}

				// 7.11
				virtual void requestAttributeOwnershipRelease(
					rti1516e::ObjectInstanceHandle theObject,
					rti1516e::AttributeHandleSet const & candidateAttributes,
					rti1516e::VariableLengthData const & theUserSuppliedTag)
					throw (
						rti1516e::FederateInternalError) {}

				// 7.16
				virtual void confirmAttributeOwnershipAcquisitionCancellation(
					rti1516e::ObjectInstanceHandle theObject,
					rti1516e::AttributeHandleSet const & theAttributes)
					throw (
						rti1516e::FederateInternalError) {}

				// 7.18
				virtual void informAttributeOwnership(
					rti1516e::ObjectInstanceHandle theObject,
					rti1516e::AttributeHandle theAttribute,
					rti1516e::FederateHandle theOwner)
					throw (
						rti1516e::FederateInternalError) {}

				virtual void attributeIsNotOwned(
					rti1516e::ObjectInstanceHandle theObject,
					rti1516e::AttributeHandle theAttribute)
					throw (
						rti1516e::FederateInternalError) {}

				virtual void attributeIsOwnedByRTI(
					rti1516e::ObjectInstanceHandle theObject,
					rti1516e::AttributeHandle theAttribute)
					throw (
						rti1516e::FederateInternalError) {}

#pragma endregion // Ownership Management Services

#pragma region Time Management Services
				// 8.3
				virtual void timeRegulationEnabled(
					rti1516e::LogicalTime const & theFederateTime)
					throw (
						rti1516e::FederateInternalError) {}

				// 8.6
				virtual void timeConstrainedEnabled(
					rti1516e::LogicalTime const & theFederateTime)
					throw (
						rti1516e::FederateInternalError) {}

				// 8.13
				virtual void timeAdvanceGrant(
					rti1516e::LogicalTime const & theTime)
					throw (
						rti1516e::FederateInternalError) {}

				// 8.22
				virtual void requestRetraction(
					rti1516e::MessageRetractionHandle theHandle)
					throw (
						rti1516e::FederateInternalError) {}
#pragma endregion // Time Management Services

#pragma endregion
			};

		}
	}
}
