// ******************************************************************************
//                          FdAmb_Hla13.h
//		begin                : Jul 17, 2009
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
#include<NullFederateAmbassador.hh>
#include <vcclr.h> // For gcroot<>
#include <fedtime.hh>

using namespace System;

namespace Racon
{
	namespace RtiLayer {
		ref class CallbackManager;

		public class FdAmb_Hla13 : public NullFederateAmbassador
		{
			// Fields
		private:
			gcroot<CallbackManager^> wrapper;

			// Methods
#pragma region Methods
		public:
			// Constructor
			FdAmb_Hla13(CallbackManager^);
			// Destructor
			~FdAmb_Hla13(void){};

#pragma region Federation Management Services
			virtual void initiateFederateSave(
				const char *label) // supplied C4
				throw (
				RTI::UnableToPerformSave,
				RTI::FederateInternalError);

			virtual void federationSaved()
				throw (
				RTI::FederateInternalError);

			virtual void federationNotSaved()
				throw (
				RTI::FederateInternalError);

			virtual void requestFederationRestoreSucceeded(
				const char *label) // supplied C4
				throw (
				RTI::FederateInternalError);

			virtual void requestFederationRestoreFailed(
				const char *label,
				const char *reason) // supplied C4
				throw (
				RTI::FederateInternalError);

			virtual void federationRestoreBegun()
				throw (
				RTI::FederateInternalError);

			virtual void initiateFederateRestore(
				const char               *label,   // supplied C4
				RTI::FederateHandle handle)  // supplied C1
				throw (
				RTI::SpecifiedSaveLabelDoesNotExist,
				RTI::CouldNotRestore,
				RTI::FederateInternalError);

			virtual void federationRestored()
				throw (
				RTI::FederateInternalError);

			virtual void federationNotRestored()
				throw (
				RTI::FederateInternalError);

#pragma endregion // Federation Management Services

#pragma region Declaration Management Services
			virtual void startRegistrationForObjectClass(
				RTI::ObjectClassHandle   theClass)
				throw (
				RTI::ObjectClassNotPublished,
				RTI::FederateInternalError);

			virtual void stopRegistrationForObjectClass(
				RTI::ObjectClassHandle   theClass)
				throw (
				RTI::ObjectClassNotPublished,
				RTI::FederateInternalError);

			virtual void turnInteractionsOn(
				RTI::InteractionClassHandle theHandle)
				throw (
				RTI::InteractionClassNotPublished,
				RTI::FederateInternalError);

			virtual void turnInteractionsOff(
				RTI::InteractionClassHandle theHandle)
				throw (
				RTI::InteractionClassNotPublished,
				RTI::FederateInternalError);

#pragma endregion // Declaration Management Services

#pragma region Object Management Services
			// discoverObjectInstance
			virtual void discoverObjectInstance(RTI::ObjectHandle, RTI::ObjectClassHandle, const char*)
				throw (RTI::CouldNotDiscover, RTI::ObjectClassNotKnown, RTI::FederateInternalError);

			// reflectAttributeValues
			virtual void reflectAttributeValues(RTI::ObjectHandle theObject, const RTI::AttributeHandleValuePairSet& theAttributes, const RTI::FedTime&  theTime, const char *theTag, RTI::EventRetractionHandle theHandle)
				throw (RTI::ObjectNotKnown, RTI::AttributeNotKnown, RTI::FederateOwnsAttributes, RTI::InvalidFederationTime, RTI::FederateInternalError);
			virtual void reflectAttributeValues(RTI::ObjectHandle theObject, const RTI::AttributeHandleValuePairSet& theAttributes, const char *theTag)
				throw (RTI::ObjectNotKnown, RTI::AttributeNotKnown, RTI::FederateOwnsAttributes, RTI::FederateInternalError);

			// receiveInteraction
			virtual void receiveInteraction(RTI::InteractionClassHandle theInteraction, const RTI::ParameterHandleValuePairSet& theParameters, const RTI::FedTime& theTime, const char *theTag, RTI::EventRetractionHandle theHandle)
				throw (RTI::InteractionClassNotKnown, RTI::InteractionParameterNotKnown, RTI::InvalidFederationTime, RTI::FederateInternalError);
			virtual void receiveInteraction(RTI::InteractionClassHandle theInteraction, const RTI::ParameterHandleValuePairSet& theParameters, const char *theTag)
				throw (RTI::InteractionClassNotKnown, RTI::InteractionParameterNotKnown, RTI::FederateInternalError);

			// removeObjectInstance
			virtual void removeObjectInstance(RTI::ObjectHandle theObject, const RTI::FedTime& theTime, const char *theTag, RTI::EventRetractionHandle theHandle)
				throw (RTI::ObjectNotKnown, RTI::InvalidFederationTime, RTI::FederateInternalError);
			virtual void removeObjectInstance(RTI::ObjectHandle theObject, const char *theTag)
				throw (RTI::ObjectNotKnown, RTI::FederateInternalError);

			// provideAttributeValueUpdate
			virtual void provideAttributeValueUpdate(RTI::ObjectHandle theObject, const RTI::AttributeHandleSet& theAttributes)
				throw (RTI::ObjectNotKnown, RTI::AttributeNotKnown, RTI::AttributeNotOwned, RTI::FederateInternalError);
#pragma endregion // Object Management Services

#pragma region Ownership Management Services
			virtual void requestAttributeOwnershipAssumption(RTI::ObjectHandle theObject, const RTI::AttributeHandleSet& offeredAttributes, const char *theTag)
				throw (
				RTI::ObjectNotKnown,
				RTI::AttributeNotKnown,
				RTI::AttributeAlreadyOwned,
				RTI::AttributeNotPublished,
				RTI::FederateInternalError);

			virtual void attributeOwnershipDivestitureNotification(RTI::ObjectHandle  theObject, const RTI::AttributeHandleSet& releasedAttributes)
				throw (
				RTI::ObjectNotKnown,
				RTI::AttributeNotKnown,
				RTI::AttributeNotOwned,
				RTI::AttributeDivestitureWasNotRequested,
				RTI::FederateInternalError);

			virtual void attributeOwnershipAcquisitionNotification(RTI::ObjectHandle theObject, const RTI::AttributeHandleSet& securedAttributes)
				throw (
				RTI::ObjectNotKnown,
				RTI::AttributeNotKnown,
				RTI::AttributeAcquisitionWasNotRequested,
				RTI::AttributeAlreadyOwned,
				RTI::AttributeNotPublished,
				RTI::FederateInternalError);

			virtual void attributeOwnershipUnavailable(RTI::ObjectHandle theObject, const RTI::AttributeHandleSet& theAttributes)
				throw (
				RTI::ObjectNotKnown,
				RTI::AttributeNotKnown,
				RTI::AttributeAlreadyOwned,
				RTI::AttributeAcquisitionWasNotRequested,
				RTI::FederateInternalError);

			virtual void requestAttributeOwnershipRelease(RTI::ObjectHandle theObject, const RTI::AttributeHandleSet& candidateAttributes, const char  *theTag)
				throw (
				RTI::ObjectNotKnown,
				RTI::AttributeNotKnown,
				RTI::AttributeNotOwned,
				RTI::FederateInternalError);

			virtual void confirmAttributeOwnershipAcquisitionCancellation(RTI::ObjectHandle theObject, const RTI::AttributeHandleSet& theAttributes)
				throw (
				RTI::ObjectNotKnown,
				RTI::AttributeNotKnown,
				RTI::AttributeAlreadyOwned,
				RTI::AttributeAcquisitionWasNotCanceled,
				RTI::FederateInternalError);

			virtual void informAttributeOwnership(RTI::ObjectHandle    theObject, RTI::Handle theAttribute, RTI::FederateHandle  theOwner)
				throw (
				RTI::ObjectNotKnown,
				RTI::AttributeNotKnown,
				RTI::FederateInternalError);

			virtual void attributeIsNotOwned(RTI::ObjectHandle    theObject, RTI::Handle theAttribute)
				throw (
				RTI::ObjectNotKnown,
				RTI::AttributeNotKnown,
				RTI::FederateInternalError);

			virtual void attributeOwnedByRTI(RTI::ObjectHandle theObject, RTI::Handle theAttribute)
				throw (
				RTI::ObjectNotKnown,
				RTI::AttributeNotKnown,
				RTI::FederateInternalError);

#pragma endregion // Ownership Management Services

#pragma region Time Management Services
			virtual void timeConstrainedEnabled(const RTI::FedTime& theFederateTime)
				throw (
				RTI::InvalidFederationTime,
				RTI::EnableTimeConstrainedWasNotPending,
				RTI::FederateInternalError
				);
			virtual void timeRegulationEnabled(const RTI::FedTime& theFederateTime)
				throw (
				RTI::InvalidFederationTime,
				RTI::EnableTimeConstrainedWasNotPending,
				RTI::FederateInternalError
				);
			virtual void timeAdvanceGrant(const RTI::FedTime& theTime)
				throw (
				RTI::InvalidFederationTime,
				RTI::TimeAdvanceWasNotInProgress,
				RTI::FederationTimeAlreadyPassed,
				RTI::FederateInternalError
				);
			virtual void requestRetraction(RTI::EventRetractionHandle theHandle)
				throw (
				RTI::EventNotKnown,
				RTI::FederateInternalError
				);
#pragma endregion // Time Management Services

#pragma endregion
		};

	}
}
