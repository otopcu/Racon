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

#include "StdAfx.h"
#include "FdAmb_Hla13.h"
#include "RtiAmb.h"
#include "CallbackManager.h"

using namespace Racon::RtiLayer;

#pragma region Constructors
FdAmb_Hla13::FdAmb_Hla13(CallbackManager ^_parent){
	wrapper = _parent;
};
#pragma endregion	// Constructors		

#pragma region Federation Management Services

void FdAmb_Hla13::initiateFederateSave (const char *label)
	throw (RTI::UnableToPerformSave, RTI::FederateInternalError) {
		try{
			// Create Event Arguments
			HlaFederationManagementEventArgs ^args = gcnew HlaFederationManagementEventArgs();
			args->Label = gcnew String(label);
			args->TraceMessage = "RTI initiates a federate save (save label: " + args->Label + ").";
			args->EventType = RaconEventTypes::InitiateFederateSave;
			// Add to the Event Queue
			wrapper->FdAmbEventQueue->Enqueue(args);
		}
	catch (RTI::UnableToPerformSave& e){
		MessageBox::Show("MSG-(UnableToPerformSave - initiateFederateSave):" + Environment::NewLine + " Reason: " + Environment::NewLine + "The federate is unable to perform a save at the current time.", "FdAmb_HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
		catch (Exception^ e){
			MessageBox::Show("MSG-(initiateFederateSave):" + Environment::NewLine +  Environment::NewLine, "FdAmb_HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
		}
}
void FdAmb_Hla13::federationSaved ()
	throw (RTI::FederateInternalError) {
		try{
			// Create Event Arguments
			HlaFederationManagementEventArgs ^args = gcnew HlaFederationManagementEventArgs();
			args->Success = true;
			args->TraceMessage = "RTI informs that federation-wide save has completed successfully.";
			args->EventType = RaconEventTypes::FederationSaved;
			// Add to the Event Queue
			wrapper->FdAmbEventQueue->Enqueue(args);
		}
		catch (Exception^ e){
			MessageBox::Show("MSG-(federationSaved):" + Environment::NewLine +  Environment::NewLine, "FdAmb_HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
		}
}

void FdAmb_Hla13::federationNotSaved ()
	throw (
	RTI::FederateInternalError) {
		try{
			// Create Event Arguments
			HlaFederationManagementEventArgs ^args = gcnew HlaFederationManagementEventArgs();
			args->Success = false;
			args->TraceMessage = "RTI informs that federation-wide save has completed, but one or more federates have failed to correctly save their state.";
			args->EventType = RaconEventTypes::FederationSaved;
			// Add to the Event Queue
			wrapper->FdAmbEventQueue->Enqueue(args);
		}
		catch (Exception^ e){
			MessageBox::Show("MSG-(federationNotSaved):" + Environment::NewLine +  Environment::NewLine, "FdAmb_HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
		}
}

void FdAmb_Hla13::requestFederationRestoreSucceeded (const char *label)
	throw (RTI::FederateInternalError){
		try{
			// Create Event Arguments
			HlaFederationManagementEventArgs ^args = gcnew HlaFederationManagementEventArgs();
			args->Label = gcnew String(label);
			args->Success = true;
			args->TraceMessage = "RTI informs that a request to attempt to restore (save label: " + args->Label + ") has been accepted.";
			args->EventType = RaconEventTypes::FederationRestorationRequestConfirmed;
			// Add to the Event Queue
			wrapper->FdAmbEventQueue->Enqueue(args);
		}
		catch (Exception^ e){
			MessageBox::Show("MSG-(requestFederationRestoreSucceeded):" + Environment::NewLine +  Environment::NewLine, "FdAmb_HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
		}
}

void FdAmb_Hla13::requestFederationRestoreFailed (const char *label,	const char *reason)
	throw (RTI::FederateInternalError) {
		try{
			// Create Event Arguments
			HlaFederationManagementEventArgs ^args = gcnew HlaFederationManagementEventArgs();
			args->Label = gcnew String(label);
			args->Reason = gcnew String(reason);
			args->Success = false;
			args->TraceMessage = "RTI informs that a request to attempt to restore (save label: " + args->Label + ") has been denied.";
			args->EventType = RaconEventTypes::FederationRestorationRequestConfirmed;
			// Add to the Event Queue
			wrapper->FdAmbEventQueue->Enqueue(args);
		}
		catch (Exception^ e){
			MessageBox::Show("MSG-(requestFederationRestoreFailed):" + Environment::NewLine +  Environment::NewLine, "FdAmb_HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
		}
}

void FdAmb_Hla13::federationRestoreBegun ()
	throw (RTI::FederateInternalError) {
		try{
			// Create Event Arguments
			HlaFederationManagementEventArgs ^args = gcnew HlaFederationManagementEventArgs();
			args->TraceMessage = "RTI advises the federate that a federation-wide restoration has begun";
			args->EventType = RaconEventTypes::FederationRestoreBegun;
			// Add to the Event Queue
			wrapper->FdAmbEventQueue->Enqueue(args);
		}
		catch (Exception^ e){
			MessageBox::Show("MSG-(federationRestoreBegun):" + Environment::NewLine +  Environment::NewLine, "FdAmb_HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
		}
}

void FdAmb_Hla13::initiateFederateRestore (const char *label, RTI::FederateHandle handle)
	throw (RTI::SpecifiedSaveLabelDoesNotExist,	RTI::CouldNotRestore,	RTI::FederateInternalError){
		try{
			// Create Event Arguments
			HlaFederationManagementEventArgs ^args = gcnew HlaFederationManagementEventArgs();
			args->Label = gcnew String(label);
			args->FederateHandle = handle;
			args->TraceMessage = "RTI initiates a federate restore (save label: " + args->Label + ")(federate handle: " + args->FederateHandle + ").";
			args->EventType = RaconEventTypes::InitiateFederateRestore;
			// Add to the Event Queue
			wrapper->FdAmbEventQueue->Enqueue(args);
		}
	catch (RTI::SpecifiedSaveLabelDoesNotExist& e){
		MessageBox::Show("MSG-(SpecifiedSaveLabelDoesNotExist - initiateFederateRestore):" + Environment::NewLine + " Reason: " + Environment::NewLine + "The specified save label does not correspond to an existing labeled saved state.", "FdAmb_HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
	catch (RTI::CouldNotRestore& e){
		MessageBox::Show("MSG-(CouldNotRestore - initiateFederateRestore):" + Environment::NewLine + " Reason: " + Environment::NewLine + "TThe federate recognizes the save label but was unable to restore its state for some other reason.", "FdAmb_HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
		catch (Exception^ e){
			MessageBox::Show("MSG-(initiateFederateSave):" + Environment::NewLine +  Environment::NewLine, "FdAmb_HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
		}
}

void FdAmb_Hla13::federationRestored ()
	throw (RTI::FederateInternalError) {
		try{
			// Create Event Arguments
			HlaFederationManagementEventArgs ^args = gcnew HlaFederationManagementEventArgs();
			args->Success = true;
			args->TraceMessage = "RTI informs that federation-wide restore has completed successfully.";
			args->EventType = RaconEventTypes::FederationRestored;
			// Add to the Event Queue
			wrapper->FdAmbEventQueue->Enqueue(args);
		}
		catch (Exception^ e){
			MessageBox::Show("MSG-(federationRestored):" + Environment::NewLine +  Environment::NewLine, "FdAmb_HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
		}
}

void FdAmb_Hla13::federationNotRestored ()
	throw (RTI::FederateInternalError) {
		try{
			// Create Event Arguments
			HlaFederationManagementEventArgs ^args = gcnew HlaFederationManagementEventArgs();
			args->Success = false;
			args->TraceMessage = "RTI informs that federation-wide restore has completed, but one or more federates have failed to correctly restore their state.";
			args->EventType = RaconEventTypes::FederationRestored;
			// Add to the Event Queue
			wrapper->FdAmbEventQueue->Enqueue(args);
		}
		catch (Exception^ e){
			MessageBox::Show("MSG-(federationNotRestored):" + Environment::NewLine +  Environment::NewLine, "FdAmb_HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
		}
}

#pragma endregion // Federation Management Services

#pragma region Declaration Management Services
// startRegistrationForObjectClass
void FdAmb_Hla13::startRegistrationForObjectClass (RTI::ObjectClassHandle theClass)
	throw (RTI::ObjectClassNotPublished, RTI::FederateInternalError){
	// Response to EnableClassRelevanceAdvisory
		try{
			// Create Event Arguments
			HlaDeclarationManagementEventArgs ^args = gcnew HlaDeclarationManagementEventArgs();
			args->ObjectClassHandle = theClass;
			args->TraceMessage = "RTI informs that there is an active subscriber for the object class (handle: " + args->ObjectClassHandle + "), published by this federate.";
			args->EventType = RaconEventTypes::StartRegistrationForObjectClassAdvised;
			// Add to the Event Queue
			wrapper->FdAmbEventQueue->Enqueue(args);
		}
		catch (Exception^ e){
			MessageBox::Show("MSG-(startRegistrationForObjectClass):" + Environment::NewLine +  Environment::NewLine, "FdAmb-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
		}
};
// stopRegistrationForObjectClass
void FdAmb_Hla13::stopRegistrationForObjectClass (RTI::ObjectClassHandle theClass)
	throw (RTI::ObjectClassNotPublished, RTI::FederateInternalError){
	// Response to DisableClassRelevanceAdvisory
	try{
			// Create Event Arguments
			HlaDeclarationManagementEventArgs ^args = gcnew HlaDeclarationManagementEventArgs();
			args->ObjectClassHandle = theClass;
			args->TraceMessage = "RTI informs that there are no active subscribers for the object class (handle: " + args->ObjectClassHandle + ") that is published by this federate.";
			args->EventType = RaconEventTypes::StopRegistrationForObjectClassAdvised;
			// Add to the Event Queue
			wrapper->FdAmbEventQueue->Enqueue(args);
		}
		catch (Exception^ e){
			MessageBox::Show("MSG-(stopRegistrationForObjectClass):" + Environment::NewLine +  Environment::NewLine, "FdAmb-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
		}
};
// turnInteractionsOn
void FdAmb_Hla13::turnInteractionsOn (RTI::InteractionClassHandle theHandle)
			throw (RTI::InteractionClassNotPublished,	RTI::FederateInternalError){
		try{
			// Create Event Arguments
			HlaDeclarationManagementEventArgs ^args = gcnew HlaDeclarationManagementEventArgs();
			args->InteractionClassHandle = theHandle;
			args->TraceMessage = "RTI informs that there is an active subscriber for the interaction class (handle: " + args->InteractionClassHandle + ") that is published by this federate.";
			args->EventType = RaconEventTypes::TurnInteractionsOnAdvised;
			// Add to the Event Queue
			wrapper->FdAmbEventQueue->Enqueue(args);
		}
		catch (Exception^ e){
			MessageBox::Show("MSG-(turnInteractionsOn):" + Environment::NewLine +  Environment::NewLine, "FdAmb-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
		}
};
// turnInteractionsOff
void FdAmb_Hla13::turnInteractionsOff (RTI::InteractionClassHandle theHandle)
			throw (RTI::InteractionClassNotPublished,	RTI::FederateInternalError){
		try{
			// Create Event Arguments
			HlaDeclarationManagementEventArgs ^args = gcnew HlaDeclarationManagementEventArgs();
			args->InteractionClassHandle = theHandle;
			args->TraceMessage = "RTI informs that there are no active subscribers for the interaction class (handle: " + args->InteractionClassHandle + ") that is published by this federate.";
			args->EventType = RaconEventTypes::TurnInteractionsOffAdvised;
			// Add to the Event Queue
			wrapper->FdAmbEventQueue->Enqueue(args);
		}
		catch (Exception^ e){
			MessageBox::Show("MSG-(turnInteractionsOff):" + Environment::NewLine +  Environment::NewLine, "FdAmb-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
		}
};
#pragma endregion // Declaration Management Services

#pragma region Object Management Services
// Discover Object Instance
void FdAmb_Hla13::discoverObjectInstance(RTI::ObjectHandle theObject, RTI::ObjectClassHandle theObjectClass, const char *theObjectName) 
	throw (RTI::CouldNotDiscover, RTI::ObjectClassNotKnown, RTI::FederateInternalError){
		try{
			// Create Event Arguments
			HlaObjectEventArgs ^HlaObject = gcnew HlaObjectEventArgs();
			HlaObject->ClassHandle = theObjectClass;
			HlaObject->ObjectInstance->Handle = theObject;
			HlaObject->ObjectInstance->Name = gcnew String(theObjectName);

			HlaObject->TraceMessage = "A new object (handle: " + theObject + ") is discovered. Handle of its type is "  + theObjectClass;
			HlaObject->EventType = RaconEventTypes::ObjectDiscovered;

			// Add to the Event Queue
			wrapper->FdAmbEventQueue->Enqueue(HlaObject);
		}
		catch (Exception^ e){
			MessageBox::Show("MSG-(discoverObjectInstance):" + Environment::NewLine +  Environment::NewLine, "FdAmb-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
		}
};

// Reflect Attribute Values
void FdAmb_Hla13::reflectAttributeValues (RTI::ObjectHandle theObject, const RTI::AttributeHandleValuePairSet& theAttributes, const RTI::FedTime&  theTime, const char *theTag, RTI::EventRetractionHandle theHandle)
	throw (RTI::ObjectNotKnown, RTI::AttributeNotKnown, RTI::FederateOwnsAttributes, RTI::InvalidFederationTime, RTI::FederateInternalError){
	try{
		// Create Event Arguments
		HlaObjectEventArgs ^args = gcnew HlaObjectEventArgs();
		args->ObjectInstance->Handle = theObject;
		args->ObjectInstance->Tag = gcnew String(theTag);
		// Decode Time
		RTIfedTime time(theTime);
		args->Time = time.getTime();
		// DeCode RetractionHandle
		//args->RetractionHandle->SetRtiRetractionHandle(&theHandle);
		args->RetractionHandle->SendingFederate = theHandle.sendingFederate;
		args->RetractionHandle->SerialNumber = theHandle.theSerialNumber;
		RTI::ULong valueLength;
		for (unsigned int i = 0; i < theAttributes.size(); i++){
			// Create a buffer that has the capacity 4096 bytes for each param. Buffer size = 4096 bytes x number of parameters
			char *buffer = (char*)malloc(theAttributes.size()*sizeof(char[4096]));
			theAttributes.getValue(i, (char*)&buffer[i], valueLength);
			// Get the buffer pointer for the value of parameter[i]
			IntPtr p = (IntPtr)&buffer[i];
			// Pass the value 
      HlaAttribute^ attr = gcnew HlaAttribute();
      attr->AddValue(p);
      attr->Handle = theAttributes.getHandle(i);// Convert to Rom handle
      args->ObjectInstance->Attributes->Add(attr);

      //args->AttributeValues->Add(theAttributes.getHandle(i), p);
		}
		args->TraceMessage = "An update for attribute values is received for the object (" + theObject + "). Event Timestamp = " + args->Time + ". Event Retraction Handle (Serial No) = " + args->RetractionHandle->SerialNumber;
		args->EventType = RaconEventTypes::ObjectAttributesReflected;
		// Add to the Event Queue
		wrapper->FdAmbEventQueue->Enqueue(args);
	}
	catch (Exception^ e){
		MessageBox::Show("MSG-(GeneralException - reflectAttributeValues):" + Environment::NewLine + e->ToString() + Environment::NewLine, "FdAmb_HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
};
void FdAmb_Hla13::reflectAttributeValues (RTI::ObjectHandle theObject, const RTI::AttributeHandleValuePairSet& theAttributes, const char *theTag)
	throw (RTI::ObjectNotKnown,RTI::AttributeNotKnown,RTI::FederateOwnsAttributes,RTI::FederateInternalError){
		try{
			// Create Event Arguments
			HlaObjectEventArgs ^args = gcnew HlaObjectEventArgs();
      args->ObjectInstance->Handle = theObject;
      args->ObjectInstance->Tag = gcnew String(theTag);
			RTI::ULong valueLength;
			for (unsigned int i=0; i < theAttributes.size(); i++){
				// Create a buffer that has the capacity 4096 bytes for each param. Buffer size = 4096 bytes x number of parameters
				char *buffer = (char*)malloc(theAttributes.size()*sizeof(char[4096]));
				theAttributes.getValue(i, (char*) &buffer[i], valueLength);
				// Get the buffer pointer for the value of parameter[i]
				IntPtr p = (IntPtr)&buffer[i];
        // Pass the value 
        HlaAttribute^ attr = gcnew HlaAttribute();
        attr->AddValue(p);
        attr->Handle = theAttributes.getHandle(i);// Convert to Rom handle
        args->ObjectInstance->Attributes->Add(attr);
			}
      args->TraceMessage = "An update for attribute values is received for the object (" + theObject + ").";
      args->EventType = RaconEventTypes::ObjectAttributesReflected;
			// Add to the Event Queue
			wrapper->FdAmbEventQueue->Enqueue(args);
		}
		catch (Exception^ e){
			MessageBox::Show("MSG-(GeneralException - reflectAttributeValues):" + Environment::NewLine + e->ToString() + Environment::NewLine, "FdAmb_HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
};

// Remove Object Instance
void FdAmb_Hla13::removeObjectInstance (RTI::ObjectHandle theObject, const RTI::FedTime& theTime, const char *theTag, RTI::EventRetractionHandle theHandle)
	throw (RTI::ObjectNotKnown,RTI::InvalidFederationTime,RTI::FederateInternalError){
	// Create Event Arguments
	HlaObjectEventArgs ^args = gcnew HlaObjectEventArgs();
	args->ObjectInstance->Handle = theObject;
	args->ObjectInstance->Tag = gcnew String(theTag);
	// Decode Time
	RTIfedTime time(theTime);
	args->Time = time.getTime();
	// DeCode RetractionHandle
	args->RetractionHandle->SendingFederate = theHandle.sendingFederate;
	args->RetractionHandle->SerialNumber = theHandle.theSerialNumber;

	args->TraceMessage = "The object (handle: " + theObject + ") is removed." + "Event Timestamp = " + args->Time + ". Event Retraction Handle (Serial No) = " + args->RetractionHandle ->SerialNumber;
	args->EventType = RaconEventTypes::ObjectRemoved;
	// Add to the Event Queue
	wrapper->FdAmbEventQueue->Enqueue(args);
};
void FdAmb_Hla13::removeObjectInstance (RTI::ObjectHandle theObject, const char *theTag)
	throw (RTI::ObjectNotKnown, RTI::FederateInternalError){
		// Create Event Arguments
		HlaObjectEventArgs ^HlaObject = gcnew HlaObjectEventArgs();
		HlaObject->ObjectInstance->Handle = theObject;
		HlaObject->ObjectInstance->Tag = gcnew String(theTag);

		HlaObject->TraceMessage = "The object (handle: " + theObject + ") is removed.";
		HlaObject->EventType = RaconEventTypes::ObjectRemoved;
		// Add to the Event Queue
		wrapper->FdAmbEventQueue->Enqueue(HlaObject);
};

// Receive Interaction
// !!! Memory Leakage - see emails.
void FdAmb_Hla13::receiveInteraction (RTI::InteractionClassHandle theInteraction, const RTI::ParameterHandleValuePairSet& theParameters, const RTI::FedTime& theTime, const char  *theTag, RTI::EventRetractionHandle theHandle)      
	throw (RTI::InteractionClassNotKnown,RTI::InteractionParameterNotKnown,RTI::InvalidFederationTime,RTI::FederateInternalError){
	try{
		// Create Event Arguments
		HlaInteractionEventArgs ^args = gcnew HlaInteractionEventArgs();
		args->Interaction->ClassHandle = theInteraction;
		// Decode Time
		RTIfedTime time(theTime);
		args->Time = time.getTime();
		// DeCode RetractionHandle
		args->RetractionHandle->SendingFederate = theHandle.sendingFederate;
		args->RetractionHandle->SerialNumber = theHandle.theSerialNumber;

		RTI::ULong valueLength;
		for (unsigned int i = 0; i < theParameters.size(); i++){
			// Create a buffer that has the capacity 4096 bytes for each param. Buffer size = 4096 bytes x number of parameters
			char *buffer = (char*)malloc(theParameters.size()*sizeof(char[4096]));
			theParameters.getValue(i, (char*)&buffer[i], valueLength);
			// Get the buffer pointer for the value of parameter[i]
			IntPtr p = (IntPtr)&buffer[i];
			// Pass the value
			HlaParameter^ para = gcnew HlaParameter();
			para->AddValue(p);
			para->Handle = theParameters.getHandle(i);// Convert to Rom handle
			args->Interaction->Parameters->Add(para);
		}
		args->TraceMessage = "A new interaction is received. Interaction handle: " + theInteraction + ". Timestamp = " + args->Time + ". Event Retraction Handle (Serial No) = " + args->RetractionHandle->SerialNumber + " byy " + theHandle.sendingFederate;
		args->EventType = RaconEventTypes::InteractionReceived;
		// Add to the Event Queue
		wrapper->FdAmbEventQueue->Enqueue(args);
	}
	catch (Exception^ e){
		MessageBox::Show("MSG-(GeneralException - receiveInteraction(time)):" + Environment::NewLine + e->ToString() + Environment::NewLine, "FdAmb_HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
};
void FdAmb_Hla13::receiveInteraction (RTI::InteractionClassHandle theInteraction, const RTI::ParameterHandleValuePairSet& theParameters, const char *theTag)         
	throw (RTI::InteractionClassNotKnown,RTI::InteractionParameterNotKnown,RTI::FederateInternalError){
		try{
			// Create Event Arguments
			HlaInteractionEventArgs ^args = gcnew HlaInteractionEventArgs();
			args->Interaction->ClassHandle = theInteraction;

			RTI::ULong valueLength;
			for (unsigned int i=0; i < theParameters.size(); i++){
				// Create a buffer that has the capacity 4096 bytes for each param. Buffer size = 4096 bytes x number of parameters
				char *buffer = (char*)malloc(theParameters.size()*sizeof(char[4096]));
				theParameters.getValue(i, (char*) &buffer[i], valueLength);
				// Get the buffer pointer for the value of parameter[i]
				IntPtr p = (IntPtr)&buffer[i];
				// Pass the value
				HlaParameter^ para = gcnew HlaParameter();
				para->AddValue(p);
				para->Handle = theParameters.getHandle(i);// Convert to Rom handle
				args->Interaction->Parameters->Add(para);
			}		
			args->TraceMessage = "A new interaction is received. Interaction handle: " + theInteraction;
			args->EventType = RaconEventTypes::InteractionReceived;
			// Add to the Event Queue
			wrapper->FdAmbEventQueue->Enqueue(args);
		}
		catch (Exception^ e){
			MessageBox::Show("MSG-(GeneralException - receiveInteraction):" + Environment::NewLine + e->ToString() + Environment::NewLine, "FdAmb_HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
};

// Provide Attribute Value Update
void FdAmb_Hla13::provideAttributeValueUpdate(RTI::ObjectHandle theObject, const RTI::AttributeHandleSet& theAttributes)
	throw (RTI::ObjectNotKnown,RTI::AttributeNotKnown,RTI::AttributeNotOwned,RTI::FederateInternalError){
		// Create Event Arguments
		HlaObjectEventArgs^ args = gcnew HlaObjectEventArgs();
		String^ attributeString;
		for (unsigned int i=0; i < theAttributes.size(); i++){
      HlaAttribute^ attr = gcnew HlaAttribute();
      attr->Handle = theAttributes.getHandle(i);// Convert to Rom handle
      args->ObjectInstance->Attributes->Add(attr);
			attributeString += theAttributes.getHandle(i);
			if (i != theAttributes.size()-1) attributeString += ", ";
		}
    args->ObjectInstance->Handle = theObject;

    args->TraceMessage = "An update request is received for the attributes (Handles: {" + attributeString + "}) of the object (" + theObject + ").";
    args->EventType = RaconEventTypes::AttributeValueUpdateRequested;
		// Add to the Event Queue
		wrapper->FdAmbEventQueue->Enqueue(args);
};
#pragma endregion // Object Management Services

#pragma region Ownership Management Services
// requestAttributeOwnershipRelease
void FdAmb_Hla13::requestAttributeOwnershipRelease (RTI::ObjectHandle theObject, const RTI::AttributeHandleSet& candidateAttributes, const char  *theTag)
	throw (RTI::ObjectNotKnown, RTI::AttributeNotKnown,	RTI::AttributeNotOwned,	RTI::FederateInternalError){
		try{
			// Create Event Arguments
			HlaOwnershipManagementEventArgs ^args = gcnew HlaOwnershipManagementEventArgs();
			args->ObjectHandle = theObject;
			args->Tag = gcnew String(theTag);
			args->AttributeSet = gcnew RaconAttributeSet();
			String^ attributeString;
			for (unsigned int i=0; i < candidateAttributes.size(); i++){
				args->AttributeSet->Attributes->Add(candidateAttributes.getHandle(i));
				attributeString += candidateAttributes.getHandle(i);
				if (i != candidateAttributes.size()-1) attributeString += ", ";
			}
			args->TraceMessage = "A request for attribute ownership release for attributes (handles: {" + attributeString + "}) of the object (" + theObject + ") is received.";
			args->EventType = RaconEventTypes::AttributeOwnershipReleaseRequested;
			// Add to the Event Queue
			wrapper->FdAmbEventQueue->Enqueue(args);
		}
		catch (Exception^ e){
			MessageBox::Show("MSG-(GeneralException - requestAttributeOwnershipRelease):" + Environment::NewLine + e->ToString() + Environment::NewLine, "FdAmb_HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
};
// attributeOwnershipAcquisitionNotification
void FdAmb_Hla13::attributeOwnershipAcquisitionNotification(RTI::ObjectHandle theObject, const RTI::AttributeHandleSet& securedAttributes)
				throw (
				RTI::ObjectNotKnown,
				RTI::AttributeNotKnown,
				RTI::AttributeAcquisitionWasNotRequested,
				RTI::AttributeAlreadyOwned,
				RTI::AttributeNotPublished,
				RTI::FederateInternalError)
{
		try{
			// Create Event Arguments
			HlaOwnershipManagementEventArgs ^args = gcnew HlaOwnershipManagementEventArgs();
			args->ObjectHandle = theObject;
			args->AttributeSet = gcnew RaconAttributeSet();
			String^ attributeString;
			for (unsigned int i=0; i < securedAttributes.size(); i++){
				args->AttributeSet->Attributes->Add(securedAttributes.getHandle(i));
				attributeString += securedAttributes.getHandle(i);
				if (i != securedAttributes.size()-1) attributeString += ", ";
			}
			args->TraceMessage = "The ownership of a set of attributes (handles: {" + attributeString + "}) of the object (" + theObject + ") has been acquired.";
			args->EventType = RaconEventTypes::AttributeOwnershipAcquisitionNotified;
			// Add to the Event Queue
			wrapper->FdAmbEventQueue->Enqueue(args);
		}
		catch (Exception^ e){
			MessageBox::Show("MSG-(GeneralException - attributeOwnershipAcquisitionNotification):" + Environment::NewLine + e->ToString() + Environment::NewLine, "FdAmb_HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
};
// attributeOwnershipDivestitureNotification
void FdAmb_Hla13::attributeOwnershipDivestitureNotification (RTI::ObjectHandle  theObject, const RTI::AttributeHandleSet& releasedAttributes)
				throw (
				RTI::ObjectNotKnown,
				RTI::AttributeNotKnown,
				RTI::AttributeNotOwned,
				RTI::AttributeDivestitureWasNotRequested,
				RTI::FederateInternalError)
{
		try{
			// Create Event Arguments
			HlaOwnershipManagementEventArgs ^args = gcnew HlaOwnershipManagementEventArgs();
			args->ObjectHandle = theObject;
			args->AttributeSet = gcnew RaconAttributeSet();
			String^ attributeString;
			for (unsigned int i=0; i < releasedAttributes.size(); i++){
				args->AttributeSet->Attributes->Add(releasedAttributes.getHandle(i));
				attributeString += releasedAttributes.getHandle(i);
				if (i != releasedAttributes.size()-1) attributeString += ", ";
			}
			args->TraceMessage = "The federate has been relieved of ownership responsibilities for a set of attributes (handles: {" + attributeString + "}) of the object (" + theObject + ").";
			args->EventType = RaconEventTypes::AttributeOwnershipDivestitureNotified;
			// Add to the Event Queue
			wrapper->FdAmbEventQueue->Enqueue(args);
		}
		catch (Exception^ e){
			MessageBox::Show("MSG-(GeneralException - attributeOwnershipDivestitureNotification):" + Environment::NewLine + e->ToString() + Environment::NewLine, "FdAmb_HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
};
// attributeOwnershipUnavailable
void FdAmb_Hla13::attributeOwnershipUnavailable (RTI::ObjectHandle theObject,const RTI::AttributeHandleSet& theAttributes)
				throw (
				RTI::ObjectNotKnown,
				RTI::AttributeNotKnown,
				RTI::AttributeAlreadyOwned,
				RTI::AttributeAcquisitionWasNotRequested,
				RTI::FederateInternalError)
{
		try{
			// Create Event Arguments
			HlaOwnershipManagementEventArgs ^args = gcnew HlaOwnershipManagementEventArgs();
			args->ObjectHandle = theObject;
			args->AttributeSet = gcnew RaconAttributeSet();
			String^ attributeString;
			for (unsigned int i=0; i < theAttributes.size(); i++){
				args->AttributeSet->Attributes->Add(theAttributes.getHandle(i));
				attributeString += theAttributes.getHandle(i);
				if (i != theAttributes.size()-1) attributeString += ", ";
			}
			args->TraceMessage = "The attributes (handles: {" + attributeString + "}) of the object (" + theObject + ") that the federate attempted to acquire are currently owned by remote federate or by the RTI itself.";
			args->EventType = RaconEventTypes::AttributeOwnershipUnavailable;
			// Add to the Event Queue
			wrapper->FdAmbEventQueue->Enqueue(args);
		}
		catch (Exception^ e){
			MessageBox::Show("MSG-(GeneralException - attributeOwnershipUnavailable):" + Environment::NewLine + e->ToString() + Environment::NewLine, "FdAmb_HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
};
// confirmAttributeOwnershipAcquisitionCancellation
void FdAmb_Hla13::confirmAttributeOwnershipAcquisitionCancellation(RTI::ObjectHandle theObject, const RTI::AttributeHandleSet& theAttributes)
				throw (
				RTI::ObjectNotKnown,
				RTI::AttributeNotKnown,
				RTI::AttributeAlreadyOwned,
				RTI::AttributeAcquisitionWasNotCanceled,
				RTI::FederateInternalError)
{
		try{
			// Create Event Arguments
			HlaOwnershipManagementEventArgs ^args = gcnew HlaOwnershipManagementEventArgs();
			args->ObjectHandle = theObject;
			args->AttributeSet = gcnew RaconAttributeSet();
			String^ attributeString;
			for (unsigned int i=0; i < theAttributes.size(); i++){
				args->AttributeSet->Attributes->Add(theAttributes.getHandle(i));
				attributeString += theAttributes.getHandle(i);
				if (i != theAttributes.size()-1) attributeString += ", ";
			}
			args->TraceMessage = "The request to cancel acquisition of a specified set of attributes (handles: {" + attributeString + "}) of the object (" + theObject + ") has been achieved.";
			args->EventType = RaconEventTypes::AttributeOwnershipAcquisitionCancellationConfirmed;
			// Add to the Event Queue
			wrapper->FdAmbEventQueue->Enqueue(args);
		}
		catch (Exception^ e){
			MessageBox::Show("MSG-(GeneralException - confirmAttributeOwnershipAcquisitionCancellation):" + Environment::NewLine + e->ToString() + Environment::NewLine, "FdAmb_HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
};
// requestAttributeOwnershipAssumption
void FdAmb_Hla13::requestAttributeOwnershipAssumption(RTI::ObjectHandle theObject, const RTI::AttributeHandleSet& offeredAttributes, const char *theTag)
				throw (
				RTI::ObjectNotKnown,
				RTI::AttributeNotKnown,
				RTI::AttributeAlreadyOwned,
				RTI::AttributeNotPublished,
				RTI::FederateInternalError)
{
		try{
			// Create Event Arguments
			HlaOwnershipManagementEventArgs ^args = gcnew HlaOwnershipManagementEventArgs();
			args->ObjectHandle = theObject;
			args->Tag = gcnew String(theTag);
			args->AttributeSet = gcnew RaconAttributeSet();
			String^ attributeString;
			for (unsigned int i=0; i < offeredAttributes.size(); i++){
				args->AttributeSet->Attributes->Add(offeredAttributes.getHandle(i));
				attributeString += offeredAttributes.getHandle(i);
				if (i != offeredAttributes.size()-1) attributeString += ", ";
			}
			args->TraceMessage = "A specified set of attributes (handles: {" + attributeString + "}) of the object (" + theObject + ") has become available for acquisition.";
			args->EventType = RaconEventTypes::AttributeOwnershipAssumptionRequested;
			// Add to the Event Queue
			wrapper->FdAmbEventQueue->Enqueue(args);
		}
		catch (Exception^ e){
			MessageBox::Show("MSG-(GeneralException - requestAttributeOwnershipAssumption):" + Environment::NewLine + e->ToString() + Environment::NewLine, "FdAmb_HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
};
// informAttributeOwnership
void FdAmb_Hla13::informAttributeOwnership(RTI::ObjectHandle theObject, RTI::Handle theAttribute, RTI::FederateHandle  theOwner)
				throw (
				RTI::ObjectNotKnown,
				RTI::AttributeNotKnown,
				RTI::FederateInternalError)
{
		try{
			// Create Event Arguments
			HlaOwnershipManagementEventArgs ^args = gcnew HlaOwnershipManagementEventArgs();
			args->ObjectHandle = theObject;
			args->AttributeSet = gcnew RaconAttributeSet();
			args->AttributeSet->Attributes->Add(theAttribute);
			args->FederateHandle = theOwner;
			args->TraceMessage = "Information about the ownership of attribute (handle: " + theAttribute + ") of the object (" + theObject + ") is received. The owner federate is " + theOwner;
			args->EventType = RaconEventTypes::AttributeOwnershipInformed;
			// Add to the Event Queue
			wrapper->FdAmbEventQueue->Enqueue(args);
		}
		catch (Exception^ e){
			MessageBox::Show("MSG-(GeneralException - informAttributeOwnership):" + Environment::NewLine + e->ToString() + Environment::NewLine, "FdAmb_HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
};
// attributeIsNotOwned
void FdAmb_Hla13:: attributeIsNotOwned (RTI::ObjectHandle theObject, RTI::Handle theAttribute)
				throw (
				RTI::ObjectNotKnown,
				RTI::AttributeNotKnown,
				RTI::FederateInternalError)
{
		try{
			// Create Event Arguments
			HlaOwnershipManagementEventArgs ^args = gcnew HlaOwnershipManagementEventArgs();
			args->ObjectHandle = theObject;
			args->AttributeSet = gcnew RaconAttributeSet();
			args->AttributeSet->Attributes->Add(theAttribute);
			args->FederateHandle = 0; // No Owner
			args->TraceMessage = "Information about the ownership of attribute (handle: " + theAttribute + ") of the object (" + theObject + ") is received. The attribute exists in the federation but is not currently owned by any federate.";
			args->EventType = RaconEventTypes::AttributeOwnershipInformed;
			// Add to the Event Queue
			wrapper->FdAmbEventQueue->Enqueue(args);
		}
		catch (Exception^ e){
			MessageBox::Show("MSG-(GeneralException - attributeIsNotOwned):" + Environment::NewLine + e->ToString() + Environment::NewLine, "FdAmb_HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
};
// attributeOwnedByRTI
// Typically a MOM object or internal RTI object
void FdAmb_Hla13:: attributeOwnedByRTI (RTI::ObjectHandle theObject, RTI::Handle theAttribute)
				throw (
				RTI::ObjectNotKnown,
				RTI::AttributeNotKnown,
				RTI::FederateInternalError)
{
		try{
			// Create Event Arguments
			HlaOwnershipManagementEventArgs ^args = gcnew HlaOwnershipManagementEventArgs();
			args->ObjectHandle = theObject;
			args->AttributeSet = gcnew RaconAttributeSet();
			args->AttributeSet->Attributes->Add(theAttribute);
			args->FederateHandle = -1; // Owner is RTI
			args->TraceMessage = "Information about the ownership of attribute (handle: " + theAttribute + ") of the object (" + theObject + ") is received. The attribute is currently owned internally by the RTI.";
			args->EventType = RaconEventTypes::AttributeOwnershipInformed;
			// Add to the Event Queue
			wrapper->FdAmbEventQueue->Enqueue(args);
		}
		catch (Exception^ e){
			MessageBox::Show("MSG-(GeneralException - attributeOwnedByRTI):" + Environment::NewLine + e->ToString() + Environment::NewLine, "FdAmb_HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
};


#pragma endregion // Ownership Management Services

#pragma region Time Management Services

void FdAmb_Hla13::timeConstrainedEnabled(const RTI::FedTime& theFederateTime)
throw (RTI::InvalidFederationTime, RTI::EnableTimeConstrainedWasNotPending, RTI::FederateInternalError){
	// theFederateTime: the logical time of the federate at which time constraint takes effect
	// Create Event Arguments
	HlaTimeManagementEventArgs ^args = gcnew HlaTimeManagementEventArgs();
	RTIfedTime time(theFederateTime);
	args->Time = time.getTime();

	args->TraceMessage = "Time constrained enabled. The logical time of the federate at which time constraint takes effect is " + args->Time + " secs.";
	args->EventType = RaconEventTypes::TimeConstrainedEnabled;
	// Add to the Event Queue
	wrapper->FdAmbEventQueue->Enqueue(args);
};
void FdAmb_Hla13::timeRegulationEnabled(const RTI::FedTime& theFederateTime)
throw (RTI::InvalidFederationTime, RTI::EnableTimeConstrainedWasNotPending, RTI::FederateInternalError){
	// Create Event Arguments
	HlaTimeManagementEventArgs ^args = gcnew HlaTimeManagementEventArgs();
	RTIfedTime time(theFederateTime);
	args->Time = time.getTime();

	args->TraceMessage = "Time regulation enabled. The logical time at which regulation has been enabled is " + args->Time + " secs.";
	args->EventType = RaconEventTypes::TimeRegulationEnabled;
	// Add to the Event Queue
	wrapper->FdAmbEventQueue->Enqueue(args);
};
void FdAmb_Hla13::timeAdvanceGrant(const RTI::FedTime& theTime)
throw (RTI::InvalidFederationTime, RTI::TimeAdvanceWasNotInProgress, RTI::FederationTimeAlreadyPassed, RTI::FederateInternalError){
	// Create Event Arguments
	HlaTimeManagementEventArgs ^args = gcnew HlaTimeManagementEventArgs();
	RTIfedTime time(theTime);
	args->Time = time.getTime();

	args->TraceMessage = "Time regulation enabled. the logical time to which the federate has advanced as a result of the currently  time advancement service is " + args->Time + " secs.";
	args->EventType = RaconEventTypes::TimeAdvanceGrant;
	// Add to the Event Queue
	wrapper->FdAmbEventQueue->Enqueue(args);
};
void FdAmb_Hla13::requestRetraction(RTI::EventRetractionHandle theHandle)
throw (RTI::EventNotKnown, RTI::FederateInternalError){
	// Create Event Arguments
	HlaTimeManagementEventArgs ^args = gcnew HlaTimeManagementEventArgs();
	args->RetractionHandle->SendingFederate = theHandle.sendingFederate;
	args->RetractionHandle->SerialNumber = theHandle.theSerialNumber;
	args->TraceMessage = "TSO event is being retracted. The event retraction serial no: " + args->RetractionHandle->SerialNumber + ". The sending federate handle: " + args->RetractionHandle->SerialNumber + ".";
	args->EventType = RaconEventTypes::TimeAdvanceGrant;
	// Add to the Event Queue
	wrapper->FdAmbEventQueue->Enqueue(args);
};

#pragma endregion // time Management Services
