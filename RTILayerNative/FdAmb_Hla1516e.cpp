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
#include "CallbackManager.h"
#include "Helpers_Hla1516e.h"
#include "FdAmb_Hla1516e.h"

using namespace rti1516e;

#pragma region Constructors
FdAmb_Hla1516e::FdAmb_Hla1516e(CallbackManager ^_parent, OmHla1516e* _om)
{
	wrapper = _parent;
	om = _om;
};
#pragma endregion	// Constructors		

#pragma region Federation Management Services

void FdAmb_Hla1516e::initiateFederateSave(std::wstring const & label)
throw (rti1516e::FederateInternalError) {
	try {
		// Create Event Arguments
		HlaFederationManagementEventArgs ^args = gcnew HlaFederationManagementEventArgs();
		args->Label = gcnew String(label.c_str());
		args->TraceMessage = "RTI initiates a federate save (save label: " + args->Label + ").";
		args->EventType = RaconEventTypes::InitiateFederateSave;
		// Add to the Event Queue
		wrapper->FdAmbEventQueue->Enqueue(args);
	}
	catch (FederateInternalError& e) {
		MessageBox::Show("MSG-(FederateInternalError - initiateFederateSave):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "FdAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	catch (System::Exception^ e) {
		MessageBox::Show("MSG-(initiateFederateSave):" + Environment::NewLine + e->ToString() + Environment::NewLine, "FdAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
}

void FdAmb_Hla1516e::initiateFederateSave(std::wstring const & label, rti1516e::LogicalTime const & theTime)
throw (rti1516e::FederateInternalError) {
	try {
		// Create Event Arguments
		HlaFederationManagementEventArgs ^args = gcnew HlaFederationManagementEventArgs();
		args->Label = gcnew String(label.c_str());
		args->TraceMessage = "RTI initiates a federate save (save label: " + args->Label + ").";
		args->EventType = RaconEventTypes::InitiateFederateSave;
		// Add to the Event Queue
		wrapper->FdAmbEventQueue->Enqueue(args);
	}
	catch (FederateInternalError& e) {
		MessageBox::Show("MSG-(FederateInternalError - initiateFederateSave):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "FdAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	catch (System::Exception^ e) {
		MessageBox::Show("MSG-(initiateFederateSave):" + Environment::NewLine + e->ToString() + Environment::NewLine, "FdAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
}

void FdAmb_Hla1516e::federationSaved()
throw (rti1516e::FederateInternalError) {
	try {
		// Create Event Arguments
		HlaFederationManagementEventArgs ^args = gcnew HlaFederationManagementEventArgs();
		args->Success = true;
		args->TraceMessage = "RTI informs that federation-wide save has completed successfully.";
		args->EventType = RaconEventTypes::FederationSaved;
		// Add to the Event Queue
		wrapper->FdAmbEventQueue->Enqueue(args);
	}
	catch (System::Exception^ e) {
		MessageBox::Show("MSG-(federationSaved):" + Environment::NewLine + e->ToString() + Environment::NewLine, "FdAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
}

void FdAmb_Hla1516e::federationNotSaved(rti1516e::SaveFailureReason theSaveFailureReason)
throw (
	rti1516e::FederateInternalError) {
	try {
		// Create Event Arguments
		HlaFederationManagementEventArgs ^args = gcnew HlaFederationManagementEventArgs();
		args->Success = false;
		args->TraceMessage = "RTI informs that federation-wide save has completed, but one or more federates have failed to correctly save their state. Save Failure Reason: "; //+static_cast<int>(theSaveFailureReason);
		args->EventType = RaconEventTypes::FederationSaved;
		// Add to the Event Queue
		wrapper->FdAmbEventQueue->Enqueue(args);
	}
	catch (System::Exception^ e) {
		MessageBox::Show("MSG-(federationNotSaved):" + Environment::NewLine + e->ToString() + Environment::NewLine, "FdAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
}

/*
void FdAmb_Hla1516e::requestFederationRestoreSucceeded (const char *label)
	throw (rti1516e::FederateInternalError){
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

void FdAmb_Hla1516e::requestFederationRestoreFailed (const char *label,	const char *reason)
	throw (rti1516e::FederateInternalError) {
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

void FdAmb_Hla1516e::federationRestoreBegun ()
	throw (rti1516e::FederateInternalError) {
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

void FdAmb_Hla1516e::initiateFederateRestore (const char *label, rti1516e::FederateHandle handle)
	throw (rti1516e::SpecifiedSaveLabelDoesNotExist,	rti1516e::CouldNotRestore,	rti1516e::FederateInternalError){
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
	catch (rti1516e::SpecifiedSaveLabelDoesNotExist& e){
		MessageBox::Show("MSG-(SpecifiedSaveLabelDoesNotExist - initiateFederateRestore):" + Environment::NewLine + " Reason: " + Environment::NewLine + "The specified save label does not correspond to an existing labeled saved state.", "FdAmb_HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
	catch (rti1516e::CouldNotRestore& e){
		MessageBox::Show("MSG-(CouldNotRestore - initiateFederateRestore):" + Environment::NewLine + " Reason: " + Environment::NewLine + "TThe federate recognizes the save label but was unable to restore its state for some other reason.", "FdAmb_HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
		catch (Exception^ e){
			MessageBox::Show("MSG-(initiateFederateSave):" + Environment::NewLine +  Environment::NewLine, "FdAmb_HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
		}
}

void FdAmb_Hla1516e::federationRestored ()
	throw (rti1516e::FederateInternalError) {
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

void FdAmb_Hla1516e::federationNotRestored ()
	throw (rti1516e::FederateInternalError) {
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
*/
#pragma endregion // Federation Management Services

#pragma region Declaration Management Services
// startRegistrationForObjectClass
void FdAmb_Hla1516e::startRegistrationForObjectClass(rti1516e::ObjectClassHandle theClass)
throw (rti1516e::FederateInternalError) {
	try {
		// Create Event Arguments
		HlaDeclarationManagementEventArgs ^args = gcnew HlaDeclarationManagementEventArgs();
		args->ObjectClassHandle = toULong(theClass.toString());
		args->TraceMessage = "RTI informs that there is an active subscriber for the object class (handle: " + args->ObjectClassHandle + "), published by this federate.";
		args->EventType = RaconEventTypes::StartRegistrationForObjectClassAdvised;
		// Add to the Event Queue
		wrapper->FdAmbEventQueue->Enqueue(args);
	}
	catch (FederateInternalError& e) {
		MessageBox::Show("MSG-(FederateInternalError - startRegistrationForObjectClass):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "FdAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	catch (System::Exception^ e) {
		MessageBox::Show("MSG-(startRegistrationForObjectClass):" + Environment::NewLine + e->ToString() + Environment::NewLine, "FdAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
};

// stopRegistrationForObjectClass
void FdAmb_Hla1516e::stopRegistrationForObjectClass(rti1516e::ObjectClassHandle theClass)
throw (rti1516e::FederateInternalError) {
	try {
		// Create Event Arguments
		HlaDeclarationManagementEventArgs ^args = gcnew HlaDeclarationManagementEventArgs();
		args->ObjectClassHandle = toULong(theClass.toString());
		args->TraceMessage = "RTI informs that there are no active subscribers for the object class (handle: " + args->ObjectClassHandle + "), published by this federate.";
		args->EventType = RaconEventTypes::StopRegistrationForObjectClassAdvised;
		// Add to the Event Queue
		wrapper->FdAmbEventQueue->Enqueue(args);
	}
	catch (FederateInternalError& e) {
		MessageBox::Show("MSG-(FederateInternalError - stopRegistrationForObjectClass):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "FdAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	catch (System::Exception^ e) {
		MessageBox::Show("MSG-(stopRegistrationForObjectClass):" + Environment::NewLine + e->ToString() + Environment::NewLine, "FdAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
};

// turnInteractionsOn
void FdAmb_Hla1516e::turnInteractionsOn(rti1516e::InteractionClassHandle theHandle)
throw (rti1516e::FederateInternalError) {
	try {
		// Create Event Arguments
		HlaDeclarationManagementEventArgs ^args = gcnew HlaDeclarationManagementEventArgs();
		args->InteractionClassHandle = toULong(theHandle.toString());
		args->TraceMessage = "RTI informs that there is an active subscriber for the interaction class (handle: " + args->InteractionClassHandle + "), published by this federate.";
		args->EventType = RaconEventTypes::TurnInteractionsOnAdvised;
		// Add to the Event Queue
		wrapper->FdAmbEventQueue->Enqueue(args);
	}
	catch (FederateInternalError& e) {
		MessageBox::Show("MSG-(FederateInternalError - turnInteractionsOn):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "FdAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	catch (System::Exception^ e) {
		MessageBox::Show("MSG-(turnInteractionsOn):" + Environment::NewLine + e->ToString() + Environment::NewLine, "FdAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
};

// turnInteractionsOff
void FdAmb_Hla1516e::turnInteractionsOff(rti1516e::InteractionClassHandle theHandle)
throw (rti1516e::FederateInternalError) {
	try {
		// Create Event Arguments
		HlaDeclarationManagementEventArgs ^args = gcnew HlaDeclarationManagementEventArgs();
		args->InteractionClassHandle = toULong(theHandle.toString());
		args->TraceMessage = "RTI informs that there are no active subscribers for the interaction class (handle: " + args->InteractionClassHandle + "), published by this federate.";
		args->EventType = RaconEventTypes::TurnInteractionsOffAdvised;
		// Add to the Event Queue
		wrapper->FdAmbEventQueue->Enqueue(args);
	}
	catch (FederateInternalError& e) {
		MessageBox::Show("MSG-(FederateInternalError - turnInteractionsOff):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "FdAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	catch (System::Exception^ e) {
		MessageBox::Show("MSG-(turnInteractionsOff):" + Environment::NewLine + e->ToString() + Environment::NewLine, "FdAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
};

#pragma endregion // Declaration Management Services

#pragma region Object Management Services

// 6.9 - Discover Object Instance
void FdAmb_Hla1516e::discoverObjectInstance(rti1516e::ObjectInstanceHandle theObject, rti1516e::ObjectClassHandle theObjectClass, std::wstring const & theObjectInstanceName)
throw (rti1516e::FederateInternalError) {
	try {
		// Create Event Arguments
		HlaObjectEventArgs ^args = gcnew HlaObjectEventArgs();
		args->ClassHandle = toULong(theObjectClass.toString());
		args->ObjectInstance->Handle = toULong(theObject.toString());
		args->ObjectInstance->Name = gcnew String(theObjectInstanceName.c_str());
		om->Objects[args->ObjectInstance->Handle] = theObject; // Update Nom
		args->TraceMessage = "A new object (handle: " + args->ObjectInstance->Handle + ") is discovered. Handle of its type is " + args->ClassHandle;
		args->EventType = RaconEventTypes::ObjectDiscovered;

		// Add to the Event Queue
		wrapper->FdAmbEventQueue->Enqueue(args);
	}
	catch (FederateInternalError& e) {
		MessageBox::Show("MSG-(FederateInternalError - discoverObjectInstance):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "FdAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	catch (System::Exception^ e) {
		MessageBox::Show("MSG-(discoverObjectInstance):" + Environment::NewLine + e->ToString() + Environment::NewLine, "FdAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
};

// 6.9 - Discover Object Instance
void FdAmb_Hla1516e::discoverObjectInstance(rti1516e::ObjectInstanceHandle theObject, rti1516e::ObjectClassHandle theObjectClass, std::wstring const & theObjectInstanceName, rti1516e::FederateHandle producingFederate)
throw (rti1516e::FederateInternalError) {
	try {
		// Create Event Arguments
		HlaObjectEventArgs ^args = gcnew HlaObjectEventArgs();
		args->ClassHandle = toULong(theObjectClass.toString());
		args->ObjectInstance->Handle = toULong(theObject.toString());
		args->ObjectInstance->Name = gcnew String(theObjectInstanceName.c_str());
		om->Objects[args->ObjectInstance->Handle] = theObject; // Update Nom
		args->TraceMessage = "A new object instance (handle: " + args->ObjectInstance->Handle + ") is discovered. Handle of its type is " + args->ClassHandle + ". Producing Federate: " + gcnew String(producingFederate.toString().c_str());
		args->EventType = RaconEventTypes::ObjectDiscovered;

		// Add to the Event Queue
		wrapper->FdAmbEventQueue->Enqueue(args);
	}
	catch (FederateInternalError& e) {
		MessageBox::Show("MSG-(FederateInternalError - discoverObjectInstance):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "FdAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	catch (System::Exception^ e) {
		MessageBox::Show("MSG-(discoverObjectInstance):" + Environment::NewLine + e->ToString() + Environment::NewLine, "FdAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
};

// 6.11 - Reflect Attributes
void FdAmb_Hla1516e::reflectAttributeValues(
	rti1516e::ObjectInstanceHandle theObject,
	rti1516e::AttributeHandleValueMap const & theAttributeValues,
	rti1516e::VariableLengthData const & theUserSuppliedTag,
	rti1516e::OrderType sentOrder,
	rti1516e::TransportationType theType,
	rti1516e::SupplementalReflectInfo theReflectInfo)
	throw (rti1516e::FederateInternalError) {
	try {
		// Create Event Arguments
		HlaObjectEventArgs ^args = gcnew HlaObjectEventArgs();
		args->ObjectInstance->Handle = toULong(theObject.toString());
		args->ObjectInstance->Tag = gcnew String((char*)(theUserSuppliedTag.data()));

		AttributeHandleValueMap::const_iterator iterator;
		for (iterator = theAttributeValues.begin(); iterator != theAttributeValues.end(); iterator++)
		{
			IntPtr p = IntPtr((char *)((*iterator).second.data()));
			// Allocate new memory
			char *buffer = (char*)malloc((*iterator).second.size());
			// Copy data to buffer
			memcpy(buffer, (void *)((*iterator).second.data()), (*iterator).second.size());
			// Pass the value 
			HlaAttribute^ attr = gcnew HlaAttribute();
			attr->AddValue(IntPtr(buffer));
			attr->Handle = toULong(gcnew String((*iterator).first.toString().c_str()));// Convert to Rom handle
			args->ObjectInstance->Attributes->Add(attr);
		}
		args->TraceMessage = "An update for the attribute values of the object instance (handle: " + args->ObjectInstance->Handle + ") is received.";
		args->EventType = RaconEventTypes::ObjectAttributesReflected;
		// Add to the Event Queue
		wrapper->FdAmbEventQueue->Enqueue(args);
	}
	catch (FederateInternalError& e) {
		MessageBox::Show("MSG-(FederateInternalError - reflectAttributeValues):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "FdAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	catch (System::Exception^ e) {
		MessageBox::Show("MSG-(reflectAttributeValues):" + Environment::NewLine + e->ToString() + Environment::NewLine, "FdAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
};

// 6.11 - Reflect Attributes w/ time
void FdAmb_Hla1516e::reflectAttributeValues(
	rti1516e::ObjectInstanceHandle theObject,
	rti1516e::AttributeHandleValueMap const & theAttributeValues,
	rti1516e::VariableLengthData const & theUserSuppliedTag,
	rti1516e::OrderType sentOrder,
	rti1516e::TransportationType theType,
	rti1516e::LogicalTime const & theTime,
	rti1516e::OrderType receivedOrder,
	rti1516e::SupplementalReflectInfo theReflectInfo)
	throw (rti1516e::FederateInternalError) {
	try {
		// Create Event Arguments
		HlaObjectEventArgs ^args = gcnew HlaObjectEventArgs();
		args->ObjectInstance->Handle = toULong(theObject.toString());
		args->ObjectInstance->Tag = gcnew String((char*)(theUserSuppliedTag.data()));
		// Decode Time
		args->Time = double::Parse(gcnew String(theTime.toString().c_str()));

		AttributeHandleValueMap::const_iterator iterator;
		for (iterator = theAttributeValues.begin(); iterator != theAttributeValues.end(); iterator++)
		{
			IntPtr p = IntPtr((char *)((*iterator).second.data()));
			// Allocate new memory
			char *buffer = (char*)malloc((*iterator).second.size());
			// Copy data to buffer
			memcpy(buffer, (void *)((*iterator).second.data()), (*iterator).second.size());
			// Pass the value 
			HlaAttribute^ attr = gcnew HlaAttribute();
			attr->AddValue(IntPtr(buffer));
			attr->Handle = toULong(gcnew String((*iterator).first.toString().c_str()));// Convert to Rom handle
			args->ObjectInstance->Attributes->Add(attr);
		}
		args->TraceMessage = "An update for the attribute values of the object instance (handle: " + args->ObjectInstance->Handle + ") is received."+ " Event Timestamp = " + args->Time;
		args->EventType = RaconEventTypes::ObjectAttributesReflected;
		// Add to the Event Queue
		wrapper->FdAmbEventQueue->Enqueue(args);
	}
	catch (FederateInternalError& e) {
		MessageBox::Show("MSG-(FederateInternalError - reflectAttributeValues2):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "FdAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	catch (System::Exception^ e) {
		MessageBox::Show("MSG-(reflectAttributeValues2):" + Environment::NewLine + e->ToString() + Environment::NewLine, "FdAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
};

// 6.11 - Reflect Attributes w/ time + retraction
void FdAmb_Hla1516e::reflectAttributeValues(
	rti1516e::ObjectInstanceHandle theObject,
	rti1516e::AttributeHandleValueMap const & theAttributeValues,
	rti1516e::VariableLengthData const & theUserSuppliedTag,
	rti1516e::OrderType sentOrder,
	rti1516e::TransportationType theType,
	rti1516e::LogicalTime const & theTime,
	rti1516e::OrderType receivedOrder,
	rti1516e::MessageRetractionHandle theHandle,
	rti1516e::SupplementalReflectInfo theReflectInfo)
	throw (rti1516e::FederateInternalError) {
	try {
		// Create Event Arguments
		HlaObjectEventArgs ^args = gcnew HlaObjectEventArgs();
		args->ObjectInstance->Handle = toULong(theObject.toString());
		args->ObjectInstance->Tag = gcnew String((char*)(theUserSuppliedTag.data()));
		// Decode Time
		args->Time = double::Parse(gcnew String(theTime.toString().c_str()));
		// Decode retraction
		args->RetractionHandle = gcnew EventRetractionHandle(gcnew String(theHandle.toString().c_str()));

		AttributeHandleValueMap::const_iterator iterator;
		for (iterator = theAttributeValues.begin(); iterator != theAttributeValues.end(); iterator++)
		{
			IntPtr p = IntPtr((char *)((*iterator).second.data()));
			// Allocate new memory
			char *buffer = (char*)malloc((*iterator).second.size());
			// Copy data to buffer
			memcpy(buffer, (void *)((*iterator).second.data()), (*iterator).second.size());
			// Pass the value 
			HlaAttribute^ attr = gcnew HlaAttribute();
			attr->AddValue(IntPtr(buffer));
			attr->Handle = toULong(gcnew String((*iterator).first.toString().c_str()));// Convert to Rom handle
			args->ObjectInstance->Attributes->Add(attr);
		}
		args->TraceMessage = "An update for the attribute values of the object instance (handle: " + args->ObjectInstance->Handle + ") is received." + " Event Timestamp = " + args->Time+ ". Retraction handle = " + gcnew String(theHandle.toString().c_str());
		args->EventType = RaconEventTypes::ObjectAttributesReflected;
		// Add to the Event Queue
		wrapper->FdAmbEventQueue->Enqueue(args);
	}
	catch (FederateInternalError& e) {
		MessageBox::Show("MSG-(FederateInternalError - reflectAttributeValues3):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "FdAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	catch (System::Exception^ e) {
		MessageBox::Show("MSG-(reflectAttributeValues3):" + Environment::NewLine + e->ToString() + Environment::NewLine, "FdAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
};

/*
// 6.15 - Remove Object Instance
void FdAmb_Hla1516e::removeObjectInstance(rti1516e::ObjectInstanceHandle theObject, rti1516e::VariableLengthData const & theUserSuppliedTag, rti1516e::OrderType sentOrder, rti1516e::SupplementalRemoveInfo theRemoveInfo)
throw (rti1516e::FederateInternalError) {
	try {
		// Create Event Arguments
		HlaObjectEventArgs ^args = gcnew HlaObjectEventArgs();
		args->ObjectHandle = toULong(theObject.toString());
		//args->Tag = gcnew String(theTag);
		// Decode Time
		RTIfedTime time(theTime);
		args->Time = time.getTime();
		// DeCode RetractionHandle
		args->RetractionHandle->SetRtiRetractionHandle(&theHandle);

		args->TraceMessage = "The object (handle: " + theObject + ") is removed." + "Event Timestamp = " + args->Time + ". Event Retraction Handle (Serial No) = " + args->RetractionHandle->SerialNumber;
		args->EventType = RaconEventTypes::ObjectRemoved;
		// Add to the Event Queue
		wrapper->FdAmbEventQueue->Enqueue(args);
	}
	catch (FederateInternalError& e) {
		MessageBox::Show("MSG-(FederateInternalError - discoverObjectInstance):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "FdAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	catch (System::Exception^ e) {
		MessageBox::Show("MSG-(discoverObjectInstance):" + Environment::NewLine + e->ToString() + Environment::NewLine, "FdAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
};
*/

// 6.13 - receiveInteraction
void FdAmb_Hla1516e::receiveInteraction(rti1516e::InteractionClassHandle theInteraction, ParameterHandleValueMap const & theParameterValues, VariableLengthData const & theUserSuppliedTag, OrderType sentOrder, TransportationType theType, SupplementalReceiveInfo theReceiveInfo)
throw (rti1516e::FederateInternalError) {
	try {
		// Create Event Arguments
		HlaInteractionEventArgs ^args = gcnew HlaInteractionEventArgs();
		args->Interaction->ClassHandle = toULong(theInteraction.toString()); ;
		args->Interaction->Tag = gcnew String((char*)(theUserSuppliedTag.data()));

		// Parameter count = theParameterValues.size()
		//args->TraceMessage += "Parameter Count: " + theParameterValues.size();
		ParameterHandleValueMap::const_iterator iterator;
		for (iterator = theParameterValues.begin(); iterator != theParameterValues.end(); iterator++)
		{
			// Allocate new memory
			char *buffer = (char*)malloc((*iterator).second.size());
			// Copy data to buffer
			memcpy(buffer, (void *)((*iterator).second.data()), (*iterator).second.size());

			//// Debug
			////args->TraceMessage += " Value Size: " + (*iterator).second.size();
			//if (toULong(gcnew String((*iterator).first.toString().c_str())) == 0)
			//{
			//  args->TraceMessage += " Value buf: " + Marshal::ReadInt32(IntPtr(buffer));
			//  args->TraceMessage += " Value data: " + Marshal::ReadInt32(IntPtr((char *)((*iterator).second.data())));
			//  args->TraceMessage += " Value Ptr: " + (long)(Marshal::PtrToStructure(IntPtr(buffer), long::typeid));
			//}
			////else
			////  args->TraceMessage += " Value: " + Marshal::PtrToStringAnsi(IntPtr(buffer));

			// Pass the value
			HlaParameter^ para = gcnew HlaParameter();
			para->AddValue(IntPtr(buffer)); // !!! Needs a copy of iterator.second.data
			para->Handle = toULong(gcnew String((*iterator).first.toString().c_str()));// Convert to Rom handle
			args->Interaction->Parameters->Add(para);
		}
		args->TraceMessage = "A new interaction is received. Interaction handle: " + args->Interaction->ClassHandle;
		args->EventType = RaconEventTypes::InteractionReceived;
		// Add to the Event Queue
		wrapper->FdAmbEventQueue->Enqueue(args);
	}
	catch (FederateInternalError& e) {
		MessageBox::Show("MSG-(FederateInternalError - receiveInteraction):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "FdAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	catch (System::Exception^ e) {
		MessageBox::Show("MSG-(receiveInteraction):" + Environment::NewLine + e->ToString() + Environment::NewLine, "FdAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
};

// 6.13 - receiveInteraction w/ time
void FdAmb_Hla1516e::receiveInteraction(
	rti1516e::InteractionClassHandle theInteraction,
	rti1516e::ParameterHandleValueMap const & theParameterValues,
	rti1516e::VariableLengthData const & theUserSuppliedTag,
	rti1516e::OrderType sentOrder,
	rti1516e::TransportationType theType,
	rti1516e::LogicalTime const & theTime,
	rti1516e::OrderType receivedOrder,
	rti1516e::SupplementalReceiveInfo theReceiveInfo)
	throw (rti1516e::FederateInternalError) {
	try {
		// Create Event Arguments
		HlaInteractionEventArgs ^args = gcnew HlaInteractionEventArgs();
		args->Interaction->ClassHandle = toULong(theInteraction.toString()); ;
		args->Interaction->Tag = gcnew String((char*)(theUserSuppliedTag.data()));
		// Decode Time
		args->Time = double::Parse(gcnew String(theTime.toString().c_str()));

		ParameterHandleValueMap::const_iterator iterator;
		for (iterator = theParameterValues.begin(); iterator != theParameterValues.end(); iterator++)
		{
			// Allocate new memory
			char *buffer = (char*)malloc((*iterator).second.size());
			// Copy data to buffer
			memcpy(buffer, (void *)((*iterator).second.data()), (*iterator).second.size());
			// Pass the value
			HlaParameter^ para = gcnew HlaParameter();
			para->AddValue(IntPtr(buffer));
			para->Handle = toULong(gcnew String((*iterator).first.toString().c_str()));// Convert to Rom handle
			args->Interaction->Parameters->Add(para);
		}
		args->TraceMessage = "A new interaction is received. Interaction handle: " + args->Interaction->ClassHandle + ". Event Timestamp = " + args->Time;
		args->EventType = RaconEventTypes::InteractionReceived;
		// Add to the Event Queue
		wrapper->FdAmbEventQueue->Enqueue(args);
	}
	catch (FederateInternalError& e) {
		MessageBox::Show("MSG-(FederateInternalError - receiveInteraction2):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "FdAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	catch (System::Exception^ e) {
		MessageBox::Show("MSG-(receiveInteraction2):" + Environment::NewLine + e->ToString() + Environment::NewLine, "FdAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
};

// 6.13 - receiveInteraction w/ time + retraction
void FdAmb_Hla1516e::receiveInteraction(
	rti1516e::InteractionClassHandle theInteraction,
	rti1516e::ParameterHandleValueMap const & theParameterValues,
	rti1516e::VariableLengthData const & theUserSuppliedTag,
	rti1516e::OrderType sentOrder,
	rti1516e::TransportationType theType,
	rti1516e::LogicalTime const & theTime,
	rti1516e::OrderType receivedOrder,
	rti1516e::MessageRetractionHandle theHandle,
	rti1516e::SupplementalReceiveInfo theReceiveInfo)
	throw (rti1516e::FederateInternalError) {
	try {
		// Create Event Arguments
		HlaInteractionEventArgs ^args = gcnew HlaInteractionEventArgs();
		args->Interaction->ClassHandle = toULong(theInteraction.toString()); ;
		args->Interaction->Tag = gcnew String((char*)(theUserSuppliedTag.data()));
		// Decode Time
		args->Time = double::Parse(gcnew String(theTime.toString().c_str()));
		// Decode retraction
		args->RetractionHandle = gcnew EventRetractionHandle(gcnew String(theHandle.toString().c_str()));

		ParameterHandleValueMap::const_iterator iterator;
		for (iterator = theParameterValues.begin(); iterator != theParameterValues.end(); iterator++)
		{
			// Allocate new memory
			char *buffer = (char*)malloc((*iterator).second.size());
			// Copy data to buffer
			memcpy(buffer, (void *)((*iterator).second.data()), (*iterator).second.size());
			// Pass the value
			HlaParameter^ para = gcnew HlaParameter();
			para->AddValue(IntPtr(buffer));
			para->Handle = toULong(gcnew String((*iterator).first.toString().c_str()));// Convert to Rom handle
			args->Interaction->Parameters->Add(para);
		}
		args->TraceMessage = "A new interaction is received. Interaction handle: " + args->Interaction->ClassHandle + ". Event Timestamp = " + args->Time + ". Retraction handle = " + gcnew String(theHandle.toString().c_str());
		args->EventType = RaconEventTypes::InteractionReceived;
		// Add to the Event Queue
		wrapper->FdAmbEventQueue->Enqueue(args);
	}
	catch (FederateInternalError& e) {
		MessageBox::Show("MSG-(FederateInternalError - receiveInteraction3):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "FdAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	catch (System::Exception^ e) {
		MessageBox::Show("MSG-(receiveInteraction3):" + Environment::NewLine + e->ToString() + Environment::NewLine, "FdAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
};

// 6.15 - Remove Object Instance
void FdAmb_Hla1516e::removeObjectInstance(rti1516e::ObjectInstanceHandle theObject, rti1516e::VariableLengthData const & theUserSuppliedTag, rti1516e::OrderType sentOrder, rti1516e::SupplementalRemoveInfo theRemoveInfo)
throw (rti1516e::FederateInternalError) {
	/// !!! sentOrder, theRemoveInfo not used
	try {
		// Create Event Arguments
		HlaObjectEventArgs ^args = gcnew HlaObjectEventArgs();
		args->ObjectInstance->Handle = toULong(theObject.toString());
		args->ObjectInstance->Tag = gcnew String((char*)(theUserSuppliedTag.data()));

		// Update Nom
		om->Objects.erase(args->ObjectInstance->Handle);
		args->TraceMessage = "The object (handle: " + args->ObjectInstance->Handle + ") is removed. Tag: " + args->ObjectInstance->Tag;
		args->EventType = RaconEventTypes::ObjectRemoved;
		// Add to the Event Queue
		wrapper->FdAmbEventQueue->Enqueue(args);
	}
	catch (FederateInternalError& e) {
		MessageBox::Show("MSG-(FederateInternalError - discoverObjectInstance):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "FdAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	catch (System::Exception^ e) {
		MessageBox::Show("MSG-(discoverObjectInstance):" + Environment::NewLine + e->ToString() + Environment::NewLine, "FdAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
};

// 6.15 - Remove Object Instance w/ time
void FdAmb_Hla1516e::removeObjectInstance(rti1516e::ObjectInstanceHandle theObject, rti1516e::VariableLengthData const & theUserSuppliedTag, rti1516e::OrderType sentOrder, rti1516e::LogicalTime const & theTime, rti1516e::OrderType receivedOrder, rti1516e::SupplementalRemoveInfo theRemoveInfo)
throw (rti1516e::FederateInternalError) {
	/// !!! sentOrder, receivedOrder, theRemoveInfo not used
	try {
		// Create Event Arguments
		HlaObjectEventArgs ^args = gcnew HlaObjectEventArgs();
		args->ObjectInstance->Handle = toULong(theObject.toString());
		args->ObjectInstance->Tag = gcnew String((char*)(theUserSuppliedTag.data()));
		// Decode Time
		args->Time = double::Parse(gcnew String(theTime.toString().c_str()));
		//if (theRemoveInfo.hasProducingFederate)
		//  theRemoveInfo.producingFederate

		// Update Nom
		om->Objects.erase(args->ObjectInstance->Handle);
		args->TraceMessage = "The object (handle: " + args->ObjectInstance->Handle + ") is removed. Tag: " + args->ObjectInstance->Tag + ". Event Timestamp = " + args->Time + ". Producing Federate: " + gcnew String(theRemoveInfo.producingFederate.toString().c_str());
		args->EventType = RaconEventTypes::ObjectRemoved;
		// Add to the Event Queue
		wrapper->FdAmbEventQueue->Enqueue(args);
	}
	catch (FederateInternalError& e) {
		MessageBox::Show("MSG-(FederateInternalError - removeObjectInstance2):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "FdAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	catch (System::Exception^ e) {
		MessageBox::Show("MSG-(removeObjectInstance2):" + Environment::NewLine + e->ToString() + Environment::NewLine, "FdAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
};

// 6.15 - Remove Object Instance w/ time + retraction
void FdAmb_Hla1516e::removeObjectInstance(
	rti1516e::ObjectInstanceHandle theObject,
	rti1516e::VariableLengthData const & theUserSuppliedTag,
	rti1516e::OrderType sentOrder,
	rti1516e::LogicalTime const & theTime,
	rti1516e::OrderType receivedOrder,
	rti1516e::MessageRetractionHandle theHandle,
	rti1516e::SupplementalRemoveInfo theRemoveInfo)
	throw (rti1516e::FederateInternalError) {
	/// !!! sentOrder, receivedOrder, theRemoveInfo not used
	try {
		// Create Event Arguments
		HlaObjectEventArgs ^args = gcnew HlaObjectEventArgs();
		args->ObjectInstance->Handle = toULong(theObject.toString());
		args->ObjectInstance->Tag = gcnew String((char*)(theUserSuppliedTag.data()));
		// Decode Time
		args->Time = double::Parse(gcnew String(theTime.toString().c_str()));
		// Decode retraction
		args->RetractionHandle = gcnew EventRetractionHandle(gcnew String(theHandle.toString().c_str()));
		//if (theRemoveInfo.hasProducingFederate)
		//  theRemoveInfo.producingFederate

		// Update Nom
		om->Objects.erase(args->ObjectInstance->Handle);
		args->TraceMessage = "The object (handle: " + args->ObjectInstance->Handle + ") is removed. Tag: " + args->ObjectInstance->Tag + ". Event Timestamp = " + args->Time + ". Producing Federate: " + gcnew String(theRemoveInfo.producingFederate.toString().c_str()) + ". Retraction handle = " + gcnew String(theHandle.toString().c_str());
		args->EventType = RaconEventTypes::ObjectRemoved;
		// Add to the Event Queue
		wrapper->FdAmbEventQueue->Enqueue(args);
	}
	catch (FederateInternalError& e) {
		MessageBox::Show("MSG-(FederateInternalError - removeObjectInstance3):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "FdAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	catch (System::Exception^ e) {
		MessageBox::Show("MSG-(removeObjectInstance3):" + Environment::NewLine + e->ToString() + Environment::NewLine, "FdAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
};

/*
// Receive Interaction
void FdAmb_Hla1516e::receiveInteraction (rti1516e::InteractionClassHandle theInteraction, const rti1516e::ParameterHandleValuePairSet& theParameters, const rti1516e::FedTime& theTime, const char  *theTag, rti1516e::EventRetractionHandle theHandle)
	throw (rti1516e::InteractionClassNotKnown,rti1516e::InteractionParameterNotKnown,rti1516e::InvalidFederationTime,rti1516e::FederateInternalError){
	try{
		// Create Event Arguments
		CHlaInteractionEventArgs ^args = gcnew CHlaInteractionEventArgs();
		args->InteractionClassHandle = theInteraction;
		// Decode Time
		RTIfedTime time(theTime);
		args->Time = time.getTime();
		// DeCode RetractionHandle
		args->RetractionHandle->SetRtiRetractionHandle(&theHandle);
		rti1516e::ULong valueLength;
		for (unsigned int i = 0; i < theParameters.size(); i++){
			// Create a buffer that has the capacity 4096 bytes for each param. Buffer size = 4096 bytes x number of parameters
			char *buffer = (char*)malloc(theParameters.size()*sizeof(char[4096]));
			theParameters.getValue(i, (char*)&buffer[i], valueLength);
			// Get the buffer pointer for the value of parameter[i]
			IntPtr p = (IntPtr)&buffer[i];
			// Pass the value
			args->ParameterValues->Add(theParameters.getHandle(i), p);
		}
		args->TraceMessage = "A new interaction is received. Interaction handle: " + theInteraction + ". Timestamp = " + args->Time + ". Event Retraction Handle (Serial No) = " + args->RetractionHandle->SerialNumber;
		args->EventType = RaconEventTypes::InteractionReceived;
		// Add to the Event Queue
		wrapper->FdAmbEventQueue->Enqueue(args);
	}
	catch (Exception^ e){
		MessageBox::Show("MSG-(GeneralException - receiveInteraction(time)):" + Environment::NewLine + e->ToString() + Environment::NewLine, "FdAmb_HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
};
*/


// 6.20 - Provide Attribute Value Update
void FdAmb_Hla1516e::provideAttributeValueUpdate(
	rti1516e::ObjectInstanceHandle theObject,
	rti1516e::AttributeHandleSet const & theAttributes,
	rti1516e::VariableLengthData const & theUserSuppliedTag)
	throw (rti1516e::FederateInternalError) {
	try {
		// Create Event Arguments
		HlaObjectEventArgs^ args = gcnew HlaObjectEventArgs();
		args->ObjectInstance->Handle = toULong(theObject.toString());

		String^ attributeString;
		AttributeHandleSet::const_iterator iterator;
		for (iterator = theAttributes.begin(); iterator != theAttributes.end(); iterator++)
		{
			HlaAttribute^ attr = gcnew HlaAttribute();
			attr->Handle = toULong(gcnew String(iterator->toString().c_str()));// Convert to Rom handle
			args->ObjectInstance->Attributes->Add(attr);
			attributeString += attr->Handle + ",";
		}

		args->TraceMessage = "An update request is received for the attributes (Handles: {" + attributeString + "}) of the object (" + args->ObjectInstance->Handle + ").";
		args->EventType = RaconEventTypes::AttributeValueUpdateRequested;
		// Add to the Event Queue
		wrapper->FdAmbEventQueue->Enqueue(args);
	}
	catch (FederateInternalError& e) {
		MessageBox::Show("MSG-(FederateInternalError - provideAttributeValueUpdate):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "FdAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	catch (System::Exception^ e) {
		MessageBox::Show("MSG-(provideAttributeValueUpdate):" + Environment::NewLine + e->ToString() + Environment::NewLine, "FdAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
};

// 6.21 - turnUpdatesOnForObjectInstance
//  Turn Updates On/Off is missing. !!! Not supported by OpenRti 0.8.0
void FdAmb_Hla1516e::turnUpdatesOnForObjectInstance(
	rti1516e::ObjectInstanceHandle theObject,
	rti1516e::AttributeHandleSet const & theAttributes)
	throw (
		rti1516e::FederateInternalError) {
	try {
		// Create Event Arguments
		HlaObjectEventArgs^ args = gcnew HlaObjectEventArgs();
		args->ObjectInstance->Handle = toULong(theObject.toString());

		String^ attributeString;
		AttributeHandleSet::const_iterator iterator;
		for (iterator = theAttributes.begin(); iterator != theAttributes.end(); iterator++)
		{
			HlaAttribute^ attr = gcnew HlaAttribute();
			attr->Handle = toULong(gcnew String(iterator->toString().c_str()));// Convert to Rom handle
			args->ObjectInstance->Attributes->Add(attr);
			attributeString += attr->Handle + ",";
		}

		args->TraceMessage = "Turn updates on for the attributes (Handles: {" + attributeString + "}) of the object instance (" + args->ObjectInstance->Handle + ").";
		args->EventType = RaconEventTypes::TurnUpdatesOnForObjectInstanceAdvised;
		// Add to the Event Queue
		wrapper->FdAmbEventQueue->Enqueue(args);
	}
	catch (FederateInternalError& e) {
		MessageBox::Show("MSG-(FederateInternalError - turnUpdatesOnForObjectInstance1):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "FdAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	catch (System::Exception^ e) {
		MessageBox::Show("MSG-(turnUpdatesOnForObjectInstance1):" + Environment::NewLine + e->ToString() + Environment::NewLine, "FdAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
};
// updateRateDesignator - not implemented
void FdAmb_Hla1516e::turnUpdatesOnForObjectInstance(
	rti1516e::ObjectInstanceHandle theObject,
	rti1516e::AttributeHandleSet const & theAttributes,
	std::wstring const & updateRateDesignator)
	throw (
		rti1516e::FederateInternalError) {
	try {
		// Create Event Arguments
		HlaObjectEventArgs^ args = gcnew HlaObjectEventArgs();
		args->ObjectInstance->Handle = toULong(theObject.toString());

		String^ attributeString;
		AttributeHandleSet::const_iterator iterator;
		for (iterator = theAttributes.begin(); iterator != theAttributes.end(); iterator++)
		{
			HlaAttribute^ attr = gcnew HlaAttribute();
			attr->Handle = toULong(gcnew String(iterator->toString().c_str()));// Convert to Rom handle
			args->ObjectInstance->Attributes->Add(attr);
			attributeString += attr->Handle + ",";
		}

		args->TraceMessage = "Turn updates on for the attributes (Handles: {" + attributeString + "}) of the object instance (" + args->ObjectInstance->Handle + ").";
		args->EventType = RaconEventTypes::TurnUpdatesOnForObjectInstanceAdvised;
		// Add to the Event Queue
		wrapper->FdAmbEventQueue->Enqueue(args);
	}
	catch (FederateInternalError& e) {
		MessageBox::Show("MSG-(FederateInternalError - turnUpdatesOnForObjectInstance2):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "FdAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	catch (System::Exception^ e) {
		MessageBox::Show("MSG-(turnUpdatesOnForObjectInstance2):" + Environment::NewLine + e->ToString() + Environment::NewLine, "FdAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
};
#pragma endregion // Object Management Services

/*
#pragma region Ownership Management Services
// requestAttributeOwnershipRelease
void FdAmb_Hla1516e::requestAttributeOwnershipRelease (rti1516e::ObjectHandle theObject, const rti1516e::AttributeHandleSet& candidateAttributes, const char  *theTag)
	throw (rti1516e::ObjectNotKnown, rti1516e::AttributeNotKnown,	rti1516e::AttributeNotOwned,	rti1516e::FederateInternalError){
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
void FdAmb_Hla1516e::attributeOwnershipAcquisitionNotification(rti1516e::ObjectHandle theObject, const rti1516e::AttributeHandleSet& securedAttributes)
				throw (
				rti1516e::ObjectNotKnown,
				rti1516e::AttributeNotKnown,
				rti1516e::AttributeAcquisitionWasNotRequested,
				rti1516e::AttributeAlreadyOwned,
				rti1516e::AttributeNotPublished,
				rti1516e::FederateInternalError)
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
void FdAmb_Hla1516e::attributeOwnershipDivestitureNotification (rti1516e::ObjectHandle  theObject, const rti1516e::AttributeHandleSet& releasedAttributes)
				throw (
				rti1516e::ObjectNotKnown,
				rti1516e::AttributeNotKnown,
				rti1516e::AttributeNotOwned,
				rti1516e::AttributeDivestitureWasNotRequested,
				rti1516e::FederateInternalError)
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
void FdAmb_Hla1516e::attributeOwnershipUnavailable (rti1516e::ObjectHandle theObject,const rti1516e::AttributeHandleSet& theAttributes)
				throw (
				rti1516e::ObjectNotKnown,
				rti1516e::AttributeNotKnown,
				rti1516e::AttributeAlreadyOwned,
				rti1516e::AttributeAcquisitionWasNotRequested,
				rti1516e::FederateInternalError)
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
void FdAmb_Hla1516e::confirmAttributeOwnershipAcquisitionCancellation(rti1516e::ObjectHandle theObject, const rti1516e::AttributeHandleSet& theAttributes)
				throw (
				rti1516e::ObjectNotKnown,
				rti1516e::AttributeNotKnown,
				rti1516e::AttributeAlreadyOwned,
				rti1516e::AttributeAcquisitionWasNotCanceled,
				rti1516e::FederateInternalError)
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
void FdAmb_Hla1516e::requestAttributeOwnershipAssumption(rti1516e::ObjectHandle theObject, const rti1516e::AttributeHandleSet& offeredAttributes, const char *theTag)
				throw (
				rti1516e::ObjectNotKnown,
				rti1516e::AttributeNotKnown,
				rti1516e::AttributeAlreadyOwned,
				rti1516e::AttributeNotPublished,
				rti1516e::FederateInternalError)
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
void FdAmb_Hla1516e::informAttributeOwnership(rti1516e::ObjectHandle theObject, rti1516e::Handle theAttribute, rti1516e::FederateHandle  theOwner)
				throw (
				rti1516e::ObjectNotKnown,
				rti1516e::AttributeNotKnown,
				rti1516e::FederateInternalError)
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
void FdAmb_Hla1516e:: attributeIsNotOwned (rti1516e::ObjectHandle theObject, rti1516e::Handle theAttribute)
				throw (
				rti1516e::ObjectNotKnown,
				rti1516e::AttributeNotKnown,
				rti1516e::FederateInternalError)
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
void FdAmb_Hla1516e:: attributeOwnedByRTI (rti1516e::ObjectHandle theObject, rti1516e::Handle theAttribute)
				throw (
				rti1516e::ObjectNotKnown,
				rti1516e::AttributeNotKnown,
				rti1516e::FederateInternalError)
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

void FdAmb_Hla1516e::timeConstrainedEnabled(const rti1516e::FedTime& theFederateTime)
throw (rti1516e::InvalidFederationTime, rti1516e::EnableTimeConstrainedWasNotPending, rti1516e::FederateInternalError){
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
void FdAmb_Hla1516e::timeRegulationEnabled(const rti1516e::FedTime& theFederateTime)
throw (rti1516e::InvalidFederationTime, rti1516e::EnableTimeConstrainedWasNotPending, rti1516e::FederateInternalError){
	// Create Event Arguments
	HlaTimeManagementEventArgs ^args = gcnew HlaTimeManagementEventArgs();
	RTIfedTime time(theFederateTime);
	args->Time = time.getTime();

	args->TraceMessage = "Time regulation enabled. The logical time at which regulation has been enabled is " + args->Time + " secs.";
	args->EventType = RaconEventTypes::TimeRegulationEnabled;
	// Add to the Event Queue
	wrapper->FdAmbEventQueue->Enqueue(args);
};
void FdAmb_Hla1516e::timeAdvanceGrant(const rti1516e::FedTime& theTime)
throw (rti1516e::InvalidFederationTime, rti1516e::TimeAdvanceWasNotInProgress, rti1516e::FederationTimeAlreadyPassed, rti1516e::FederateInternalError){
	// Create Event Arguments
	HlaTimeManagementEventArgs ^args = gcnew HlaTimeManagementEventArgs();
	RTIfedTime time(theTime);
	args->Time = time.getTime();

	args->TraceMessage = "Time regulation enabled. the logical time to which the federate has advanced as a result of the currently  time advancement service is " + args->Time + " secs.";
	args->EventType = RaconEventTypes::TimeAdvanceGrant;
	// Add to the Event Queue
	wrapper->FdAmbEventQueue->Enqueue(args);
};
void FdAmb_Hla1516e::requestRetraction(rti1516e::EventRetractionHandle theHandle)
throw (rti1516e::EventNotKnown, rti1516e::FederateInternalError){
	// Create Event Arguments
	HlaTimeManagementEventArgs ^args = gcnew HlaTimeManagementEventArgs();
	args->RetractionHandle->SetRtiRetractionHandle(&theHandle);
	args->TraceMessage = "TSO event is being retracted. The event retraction serial no: " + args->RetractionHandle->SerialNumber + ". The sending federate handle: " + args->RetractionHandle->SerialNumber + ".";
	args->EventType = RaconEventTypes::TimeAdvanceGrant;
	// Add to the Event Queue
	wrapper->FdAmbEventQueue->Enqueue(args);
};

#pragma endregion // time Management Services

*/