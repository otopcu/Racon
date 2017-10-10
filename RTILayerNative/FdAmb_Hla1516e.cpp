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

#include "StdAfx.h"
#include <iostream> // for using std
#include "RTI/time/HLAfloat64Interval.h"
#include "RTI/time/HLAfloat64Time.h"
#include "Helpers_Hla1516e.h"
#include "FdAmb_Hla1516e.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace rti1516e;
// Racon
using namespace Racon::RtiLayer;
using namespace Racon::RtiLayer::Native;

#pragma region Constructors
FdAmb_Hla1516e::FdAmb_Hla1516e(CallbackManager ^_parent, NomHla1516e* _om)
{
  wrapper = _parent;
  om = _om;
};
#pragma endregion		

#pragma region Federation Management Services
// 4.4 - ConnectionLost
void FdAmb_Hla1516e::connectionLost(std::wstring const & faultDescription)
throw (rti1516e::FederateInternalError) {
  try {
    // Create Event Arguments
    HlaFederationManagementEventArgs ^args = gcnew HlaFederationManagementEventArgs();
    args->Reason = gcnew String(faultDescription.c_str());
    args->TraceMessage = "Connection lost (Fault description: " + args->Reason + ").";
    args->EventType = RaconEventTypes::ConnectionLost;
    // Add to the Event Queue
    wrapper->FdAmbEventQueue->Enqueue(args);
#pragma region exceptions
  }
  catch (FederateInternalError& e) {
    String^ msg = "FdAmb_Hla1516e-(FederateInternalError - connectionLost). Reason: " + gcnew String(e.what().c_str());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
  catch (System::Exception^ e) {
    String^ msg = "FdAmb_Hla1516e-(Exception - connectionLost). Reason: " + gcnew String(e->ToString());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
#pragma endregion
};

// 4.8 - reportFederationExecutions
void FdAmb_Hla1516e::reportFederationExecutions(rti1516e::FederationExecutionInformationVector const &theFederationExecutionInformationList)
throw (rti1516e::FederateInternalError) {
  try {
    // Create Event Arguments
    HlaFederationManagementEventArgs ^args = gcnew HlaFederationManagementEventArgs();
    // Iterate the vector - using range c++11 
    String^ fedex = "";
    for (auto const& value : theFederationExecutionInformationList)
      fedex += "<Federation Execution Name: " + gcnew String((value.federationExecutionName).c_str()) + ", logical Time Implementation Name: " + gcnew String((value.logicalTimeImplementationName).c_str()) + "> ";

    args->TraceMessage = "Federation executions that are reported: " + fedex;
    args->EventType = RaconEventTypes::FederationExecutionsReported;
    args->Level = LogLevel::INFO;
    // Add to the Event Queue
    wrapper->FdAmbEventQueue->Enqueue(args);
  }
#pragma region exceptions
  catch (FederateInternalError& e) {
    String^ msg = "FdAmb_Hla1516e-(FederateInternalError - reportFederationExecutions). Reason: " + gcnew String(e.what().c_str());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
  catch (System::Exception^ e) {
    String^ msg = "FdAmb_Hla1516e-(Exception - reportFederationExecutions). Reason: " + gcnew String(e->ToString());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
#pragma endregion
};

// 4.12 - synchronizationPointRegistrationSucceeded/synchronizationPointRegistrationFailed
void FdAmb_Hla1516e::synchronizationPointRegistrationSucceeded(std::wstring const & label)
throw (rti1516e::FederateInternalError) {
  try {
    // Create Event Arguments
    HlaFederationManagementEventArgs ^args = gcnew HlaFederationManagementEventArgs();
    args->Label = gcnew String(label.c_str());
    args->TraceMessage = "<< Synchronization point registration is succesfull. Label: " + args->Label;
    args->EventType = RaconEventTypes::SynchronizationPointRegistrationSucceeded;
    // Add to the Event Queue
    wrapper->FdAmbEventQueue->Enqueue(args);
  }
#pragma region exceptions
  catch (FederateInternalError& e) {
    String^ msg = "FdAmb_Hla1516e-(FederateInternalError - synchronizationPointRegistrationSucceeded). Reason: " + gcnew String(e.what().c_str());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
  catch (System::Exception^ e) {
    String^ msg = "FdAmb_Hla1516e-(Exception - synchronizationPointRegistrationSucceeded). Reason: " + gcnew String(e->ToString());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
#pragma endregion
};
void FdAmb_Hla1516e::synchronizationPointRegistrationFailed(std::wstring const & label,
  rti1516e::SynchronizationPointFailureReason reason)
  throw (rti1516e::FederateInternalError) {
  try {
    // Create Event Arguments
    HlaFederationManagementEventArgs ^args = gcnew HlaFederationManagementEventArgs();
    args->Label = gcnew String(label.c_str());
    args->Reason = getTextForSynchronizationPointFailureReason(reason);
    args->TraceMessage = "<< Synchronization point registration is failed. Label: " + args->Label + "" + args->Reason;
    args->EventType = RaconEventTypes::synchronizationPointRegistrationFailed;
    // Add to the Event Queue
    wrapper->FdAmbEventQueue->Enqueue(args);
  }
#pragma region exceptions
  catch (FederateInternalError& e) {
    String^ msg = "FdAmb_Hla1516e-(FederateInternalError - synchronizationPointRegistrationFailed). Reason: " + gcnew String(e.what().c_str());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
  catch (System::Exception^ e) {
    String^ msg = "FdAmb_Hla1516e-(Exception - synchronizationPointRegistrationFailed). Reason: " + gcnew String(e->ToString());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
#pragma endregion
};

// 4.13 - announceSynchronizationPoint
void FdAmb_Hla1516e::announceSynchronizationPoint(std::wstring  const & label, rti1516e::VariableLengthData const & theUserSuppliedTag)
throw (rti1516e::FederateInternalError) {
  try {
    HlaFederationManagementEventArgs ^args = gcnew HlaFederationManagementEventArgs();
    args->Label = gcnew String(label.c_str());
    args->Tag = gcnew String((char*)(theUserSuppliedTag.data()));
    args->TraceMessage = "<< Synchronization point announced. Label: " + args->Label + ". Tag: " + args->Tag;
    args->EventType = RaconEventTypes::SynchronizationPointAnnounced;
    // Add to the Event Queue
    wrapper->FdAmbEventQueue->Enqueue(args);
  }
#pragma region exceptions
  catch (FederateInternalError& e) {
    String^ msg = "FdAmb_Hla1516e-(FederateInternalError - announceSynchronizationPoint). Reason: " + gcnew String(e.what().c_str());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
  catch (System::Exception^ e) {
    String^ msg = "FdAmb_Hla1516e-(Exception - announceSynchronizationPoint). Reason: " + gcnew String(e->ToString());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
#pragma endregion
};

// 4.15 - federationSynchronized
void FdAmb_Hla1516e::federationSynchronized(std::wstring const & label, rti1516e::FederateHandleSet const& failedToSyncSet)
throw (rti1516e::FederateInternalError) {
  try {
    HlaFederationManagementEventArgs ^args = gcnew HlaFederationManagementEventArgs();
    args->Label = gcnew String(label.c_str());
    String^ federates = "";
    for (auto const& value : failedToSyncSet)
      federates = federates + gcnew String((value).toString().c_str()) + ", ";
    args->TraceMessage = "<< Federation is synchronized. Label: " + args->Label + ". Federates, which are unsuccessful at synchronization: " + federates;
    args->EventType = RaconEventTypes::FederationSynchronized;
    // Add to the Event Queue
    wrapper->FdAmbEventQueue->Enqueue(args);
  }
#pragma region exceptions
  catch (FederateInternalError& e) {
    String^ msg = "FdAmb_Hla1516e-(FederateInternalError - federationSynchronized). Reason: " + gcnew String(e.what().c_str());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
  catch (System::Exception^ e) {
    String^ msg = "FdAmb_Hla1516e-(Exception - federationSynchronized). Reason: " + gcnew String(e->ToString());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
#pragma endregion
};

// 4.17 - initiateFederateSave
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
#pragma region exceptions
  catch (FederateInternalError& e) {
    String^ msg = "FdAmb_Hla1516e-(FederateInternalError - initiateFederateSave). Reason: " + gcnew String(e.what().c_str());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
  catch (System::Exception^ e) {
    String^ msg = "FdAmb_Hla1516e-(Exception - initiateFederateSave). Reason: " + gcnew String(e->ToString());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
#pragma endregion
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
#pragma region exceptions
  catch (FederateInternalError& e) {
    String^ msg = "FdAmb_Hla1516e-(FederateInternalError - initiateFederateSave). Reason: " + gcnew String(e.what().c_str());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
  catch (System::Exception^ e) {
    String^ msg = "FdAmb_Hla1516e-(Exception - initiateFederateSave). Reason: " + gcnew String(e->ToString());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
#pragma endregion
}

// 4.20 - federationSaved/federationNotSaved
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
#pragma region exceptions
  catch (FederateInternalError& e) {
    String^ msg = "FdAmb_Hla1516e-(FederateInternalError - federationSaved). Reason: " + gcnew String(e.what().c_str());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
  catch (System::Exception^ e) {
    String^ msg = "FdAmb_Hla1516e-(Exception - federationSaved). Reason: " + gcnew String(e->ToString());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
#pragma endregion
}
void FdAmb_Hla1516e::federationNotSaved(rti1516e::SaveFailureReason theSaveFailureReason)
throw (
  rti1516e::FederateInternalError) {
  try {
    // Create Event Arguments
    HlaFederationManagementEventArgs ^args = gcnew HlaFederationManagementEventArgs();
    args->Success = false;
    args->Reason = ToString(theSaveFailureReason);
    args->TraceMessage = "RTI informs that federation-wide save has completed, but one or more federates have failed to correctly save their state. Save Failure Reason: " + args->Reason;
    args->EventType = RaconEventTypes::FederationSaved;
    // Add to the Event Queue
    wrapper->FdAmbEventQueue->Enqueue(args);
  }
#pragma region exceptions
  catch (FederateInternalError& e) {
    String^ msg = "FdAmb_Hla1516e-(FederateInternalError - federationNotSaved). Reason: " + gcnew String(e.what().c_str());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
  catch (System::Exception^ e) {
    String^ msg = "FdAmb_Hla1516e-(Exception - federationNotSaved). Reason: " + gcnew String(e->ToString());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
#pragma endregion
}

// 4.23 - federationSaveStatusResponse
void FdAmb_Hla1516e::federationSaveStatusResponse(rti1516e::FederateHandleSaveStatusPairVector const &  theFederateStatusVector)
throw (rti1516e::FederateInternalError) {
  try {
    // Create Event Arguments
    HlaFederationManagementEventArgs ^args = gcnew HlaFederationManagementEventArgs();
    args->TraceMessage = "federationSaveStatusResponse is called. !!! Not implemented";
    args->EventType = RaconEventTypes::FederationSaveStatusResponse;
    args->Level = LogLevel::INFO;
    // Add to the Event Queue
    wrapper->FdAmbEventQueue->Enqueue(args);
  }
#pragma region exceptions
  catch (FederateInternalError& e) {
    String^ msg = "FdAmb_Hla1516e-(FederateInternalError - federationSaveStatusResponse). Reason: " + gcnew String(e.what().c_str());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
  catch (System::Exception^ e) {
    String^ msg = "FdAmb_Hla1516e-(Exception - federationSaveStatusResponse). Reason: " + gcnew String(e->ToString());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
#pragma endregion
};

// 4.25 - requestFederationRestoreSucceeded/requestFederationRestoreFailed
void FdAmb_Hla1516e::requestFederationRestoreSucceeded(std::wstring const & label)
throw (rti1516e::FederateInternalError) {
  try {
    // Create Event Arguments
    HlaFederationManagementEventArgs ^args = gcnew HlaFederationManagementEventArgs();
    args->Label = gcnew String(label.c_str());
    args->Success = true;
    args->TraceMessage = "RTI informs that a request to attempt to restore (save label: " + args->Label + ") has been accepted.";
    args->EventType = RaconEventTypes::FederationRestorationRequestConfirmed;
    // Add to the Event Queue
    wrapper->FdAmbEventQueue->Enqueue(args);
  }
#pragma region exceptions
  catch (FederateInternalError& e) {
    String^ msg = "FdAmb_Hla1516e-(FederateInternalError - requestFederationRestoreSucceeded). Reason: " + gcnew String(e.what().c_str());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
  catch (System::Exception^ e) {
    String^ msg = "FdAmb_Hla1516e-(Exception - requestFederationRestoreSucceeded). Reason: " + gcnew String(e->ToString());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
#pragma endregion
}
void FdAmb_Hla1516e::requestFederationRestoreFailed(std::wstring const & label)
throw (rti1516e::FederateInternalError) {
  try {
    // Create Event Arguments
    HlaFederationManagementEventArgs ^args = gcnew HlaFederationManagementEventArgs();
    args->Label = gcnew String(label.c_str());
    args->Success = false;
    args->TraceMessage = "RTI informs that a request to attempt to restore (save label: " + args->Label + ") has been denied.";
    args->EventType = RaconEventTypes::FederationRestorationRequestConfirmed;
    // Add to the Event Queue
    wrapper->FdAmbEventQueue->Enqueue(args);
  }
#pragma region exceptions
  catch (FederateInternalError& e) {
    String^ msg = "FdAmb_Hla1516e-(FederateInternalError - requestFederationRestoreFailed). Reason: " + gcnew String(e.what().c_str());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
  catch (System::Exception^ e) {
    String^ msg = "FdAmb_Hla1516e-(Exception - requestFederationRestoreFailed). Reason: " + gcnew String(e->ToString());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
#pragma endregion
}

// 4.26 - federationRestoreBegun
void FdAmb_Hla1516e::federationRestoreBegun()
throw (rti1516e::FederateInternalError) {
  try {
    // Create Event Arguments
    HlaFederationManagementEventArgs ^args = gcnew HlaFederationManagementEventArgs();
    args->TraceMessage = "RTI advises the federate that a federation-wide restoration has begun";
    args->EventType = RaconEventTypes::FederationRestoreBegun;
    // Add to the Event Queue
    wrapper->FdAmbEventQueue->Enqueue(args);
  }
#pragma region exceptions
  catch (FederateInternalError& e) {
    String^ msg = "FdAmb_Hla1516e-(FederateInternalError - federationRestoreBegun). Reason: " + gcnew String(e.what().c_str());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
  catch (System::Exception^ e) {
    String^ msg = "FdAmb_Hla1516e-(Exception - federationRestoreBegun). Reason: " + gcnew String(e->ToString());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
#pragma endregion
}

// 4.27 - initiateFederateRestore
void FdAmb_Hla1516e::initiateFederateRestore(std::wstring const & label, std::wstring const & federateName, rti1516e::FederateHandle handle)
throw (rti1516e::FederateInternalError) {
  try {
    // Create Event Arguments
    HlaFederationManagementEventArgs ^args = gcnew HlaFederationManagementEventArgs();
    args->Label = gcnew String(label.c_str());
    args->FederateHandle = Handle2Long(handle);
    args->TraceMessage = "RTI initiates a federate restore (save label: " + args->Label + ")(federate handle: " + args->FederateHandle + ").";
    args->EventType = RaconEventTypes::InitiateFederateRestore;
    // Add to the Event Queue
    wrapper->FdAmbEventQueue->Enqueue(args);
  }
#pragma region exceptions
  catch (FederateInternalError& e) {
    String^ msg = "FdAmb_Hla1516e-(FederateInternalError - initiateFederateRestore). Reason: " + gcnew String(e.what().c_str());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
  catch (System::Exception^ e) {
    String^ msg = "FdAmb_Hla1516e-(Exception - initiateFederateRestore). Reason: " + gcnew String(e->ToString());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
#pragma endregion
}

// 4.29 - federationRestored/federationNotRestored
void FdAmb_Hla1516e::federationRestored()
throw (rti1516e::FederateInternalError) {
  try {
    // Create Event Arguments
    HlaFederationManagementEventArgs ^args = gcnew HlaFederationManagementEventArgs();
    args->Success = true;
    args->TraceMessage = "RTI informs that federation-wide restore has completed successfully.";
    args->EventType = RaconEventTypes::FederationRestored;
    // Add to the Event Queue
    wrapper->FdAmbEventQueue->Enqueue(args);
  }
#pragma region exceptions
  catch (FederateInternalError& e) {
    String^ msg = "FdAmb_Hla1516e-(FederateInternalError - federationRestored). Reason: " + gcnew String(e.what().c_str());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
  catch (System::Exception^ e) {
    String^ msg = "FdAmb_Hla1516e-(Exception - federationRestored). Reason: " + gcnew String(e->ToString());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
#pragma endregion
}
void FdAmb_Hla1516e::federationNotRestored(rti1516e::RestoreFailureReason theRestoreFailureReason)
throw (rti1516e::FederateInternalError) {
  try {
    // Create Event Arguments
    HlaFederationManagementEventArgs ^args = gcnew HlaFederationManagementEventArgs();
    args->Success = false;
    args->Reason = ToString(theRestoreFailureReason);
    args->TraceMessage = "RTI informs that federation-wide restore has completed, but one or more federates have failed to correctly restore their state. Restore failure reason: " + args->Reason;
    args->EventType = RaconEventTypes::FederationRestored;
    // Add to the Event Queue
    wrapper->FdAmbEventQueue->Enqueue(args);
  }
#pragma region exceptions
  catch (FederateInternalError& e) {
    String^ msg = "FdAmb_Hla1516e-(FederateInternalError - federationNotRestored). Reason: " + gcnew String(e.what().c_str());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
  catch (System::Exception^ e) {
    String^ msg = "FdAmb_Hla1516e-(Exception - federationNotRestored). Reason: " + gcnew String(e->ToString());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
#pragma endregion
}

// 4.32 - federationRestoreStatusResponse
void FdAmb_Hla1516e::federationRestoreStatusResponse(rti1516e::FederateRestoreStatusVector const & theFederateRestoreStatusVector)
throw (rti1516e::FederateInternalError) {
  try {
    // Create Event Arguments
    HlaFederationManagementEventArgs ^args = gcnew HlaFederationManagementEventArgs();
    args->TraceMessage = "federationRestoreStatusResponse is called. !!! Not implemented";
    args->EventType = RaconEventTypes::FederationRestoreStatusResponse;
    args->Level = LogLevel::INFO;
    // Add to the Event Queue
    wrapper->FdAmbEventQueue->Enqueue(args);
  }
#pragma region exceptions
  catch (FederateInternalError& e) {
    String^ msg = "FdAmb_Hla1516e-(FederateInternalError - federationRestoreStatusResponse). Reason: " + gcnew String(e.what().c_str());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
  catch (System::Exception^ e) {
    String^ msg = "FdAmb_Hla1516e-(Exception - federationRestoreStatusResponse). Reason: " + gcnew String(e->ToString());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
#pragma endregion
};

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
#pragma region exceptions
  catch (FederateInternalError& e) {
    String^ msg = "FdAmb_Hla1516e-(FederateInternalError - startRegistrationForObjectClass). Reason: " + gcnew String(e.what().c_str());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
  catch (System::Exception^ e) {
    String^ msg = "FdAmb_Hla1516e-(Exception - startRegistrationForObjectClass). Reason: " + gcnew String(e->ToString());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
#pragma endregion
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
#pragma region exceptions
  catch (FederateInternalError& e) {
    String^ msg = "FdAmb_Hla1516e-(FederateInternalError - stopRegistrationForObjectClass). Reason: " + gcnew String(e.what().c_str());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
  catch (System::Exception^ e) {
    String^ msg = "FdAmb_Hla1516e-(Exception - stopRegistrationForObjectClass). Reason: " + gcnew String(e->ToString());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
#pragma endregion
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
#pragma region exceptions
  catch (FederateInternalError& e) {
    String^ msg = "FdAmb_Hla1516e-(FederateInternalError - turnInteractionsOn). Reason: " + gcnew String(e.what().c_str());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
  catch (System::Exception^ e) {
    String^ msg = "FdAmb_Hla1516e-(Exception - turnInteractionsOn). Reason: " + gcnew String(e->ToString());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
#pragma endregion
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
#pragma region exceptions
  catch (FederateInternalError& e) {
    String^ msg = "FdAmb_Hla1516e-(FederateInternalError - turnInteractionsOff). Reason: " + gcnew String(e.what().c_str());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
  catch (System::Exception^ e) {
    String^ msg = "FdAmb_Hla1516e-(Exception - turnInteractionsOff). Reason: " + gcnew String(e->ToString());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
#pragma endregion
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
#pragma region exceptions
  catch (FederateInternalError& e) {
    String^ msg = "FdAmb_Hla1516e-(FederateInternalError - discoverObjectInstance). Reason: " + gcnew String(e.what().c_str());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
  catch (System::Exception^ e) {
    String^ msg = "FdAmb_Hla1516e-(Exception - discoverObjectInstance). Reason: " + gcnew String(e->ToString());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
#pragma endregion
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
#pragma region exceptions
  catch (FederateInternalError& e) {
    String^ msg = "FdAmb_Hla1516e-(FederateInternalError - discoverObjectInstance). Reason: " + gcnew String(e.what().c_str());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
  catch (System::Exception^ e) {
    String^ msg = "FdAmb_Hla1516e-(Exception - discoverObjectInstance). Reason: " + gcnew String(e->ToString());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
#pragma endregion
};

// 6.11 - Reflect Attributes
void FdAmb_Hla1516e::reflectAttributeValues( rti1516e::ObjectInstanceHandle theObject, rti1516e::AttributeHandleValueMap const & theAttributeValues, rti1516e::VariableLengthData const & theUserSuppliedTag, rti1516e::OrderType sentOrder, rti1516e::TransportationType theType, rti1516e::SupplementalReflectInfo theReflectInfo)
  throw (rti1516e::FederateInternalError) {
  try {
    // Create Event Arguments
    HlaObjectEventArgs ^args = gcnew HlaObjectEventArgs();
    args->ObjectInstance->Handle = toULong(theObject.toString());
    // Tag
    char *buffer = (char*)malloc(theUserSuppliedTag.size());
    memcpy(buffer, (void *)(theUserSuppliedTag.data()), theUserSuppliedTag.size()); // Copy data to buffer
    args->Tag->Data = IntPtr(buffer);

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
      attr->Value = IntPtr(buffer);
      attr->Handle = toULong(gcnew String((*iterator).first.toString().c_str()));// Convert to Rom handle
      args->ObjectInstance->Attributes->Add(attr);
    }
    args->TraceMessage = "An update for the attribute values of the object instance (handle: " + args->ObjectInstance->Handle + ") is received.";
    if (theReflectInfo.hasProducingFederate)
    {
      args->SupplementalReflectInfo->HasProducingFederate = true;
      args->SupplementalReflectInfo->ProducingFederateHandle = toULong(theReflectInfo.producingFederate.toString());
      args->TraceMessage += ". Producing Federate: " + args->SupplementalReflectInfo->ProducingFederateHandle;
    }
    args->EventType = RaconEventTypes::ObjectAttributesReflected;
    // Add to the Event Queue
    wrapper->FdAmbEventQueue->Enqueue(args);
  }
#pragma region exceptions
  catch (FederateInternalError& e) {
    String^ msg = "FdAmb_Hla1516e-(FederateInternalError - reflectAttributeValues). Reason: " + gcnew String(e.what().c_str());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
  catch (System::Exception^ e) {
    String^ msg = "FdAmb_Hla1516e-(Exception - reflectAttributeValues). Reason: " + gcnew String(e->ToString());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
#pragma endregion
};
// 6.11 - Reflect Attributes w/ time
void FdAmb_Hla1516e::reflectAttributeValues( rti1516e::ObjectInstanceHandle theObject, rti1516e::AttributeHandleValueMap const & theAttributeValues,
  rti1516e::VariableLengthData const & theUserSuppliedTag, rti1516e::OrderType sentOrder, rti1516e::TransportationType theType, rti1516e::LogicalTime const & theTime, rti1516e::OrderType receivedOrder, rti1516e::SupplementalReflectInfo theReflectInfo)
  throw (rti1516e::FederateInternalError) {
  try {
    // Create Event Arguments
    HlaObjectEventArgs ^args = gcnew HlaObjectEventArgs();
    args->ObjectInstance->Handle = toULong(theObject.toString());
    // Tag
    char *buffer = (char*)malloc(theUserSuppliedTag.size());
    memcpy(buffer, (void *)(theUserSuppliedTag.data()), theUserSuppliedTag.size()); // Copy data to buffer
    args->Tag->Data = IntPtr(buffer);
    // Time
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
      attr->Value = IntPtr(buffer);
      attr->Handle = toULong(gcnew String((*iterator).first.toString().c_str()));// Convert to Rom handle
      args->ObjectInstance->Attributes->Add(attr);
    }
    args->TraceMessage = "An update for the attribute values of the object instance (handle: " + args->ObjectInstance->Handle + ") is received. Message Timestamp = " + args->Time;
    if (theReflectInfo.hasProducingFederate)
    {
      args->SupplementalReflectInfo->HasProducingFederate = true;
      args->SupplementalReflectInfo->ProducingFederateHandle = toULong(theReflectInfo.producingFederate.toString());
      args->TraceMessage += ". Producing Federate: " + args->SupplementalReflectInfo->ProducingFederateHandle;
    }
    args->EventType = RaconEventTypes::ObjectAttributesReflected;
    // Add to the Event Queue
    wrapper->FdAmbEventQueue->Enqueue(args);
  }
#pragma region exceptions
  catch (FederateInternalError& e) {
    String^ msg = "FdAmb_Hla1516e-(FederateInternalError - reflectAttributeValues). Reason: " + gcnew String(e.what().c_str());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
  catch (System::Exception^ e) {
    String^ msg = "FdAmb_Hla1516e-(Exception - reflectAttributeValues). Reason: " + gcnew String(e->ToString());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
#pragma endregion
};
// 6.11 - Reflect Attributes w/ time + retraction
void FdAmb_Hla1516e::reflectAttributeValues(rti1516e::ObjectInstanceHandle theObject, rti1516e::AttributeHandleValueMap const & theAttributeValues, rti1516e::VariableLengthData const & theUserSuppliedTag, rti1516e::OrderType sentOrder, rti1516e::TransportationType theType, rti1516e::LogicalTime const & theTime, rti1516e::OrderType receivedOrder, rti1516e::MessageRetractionHandle theHandle, rti1516e::SupplementalReflectInfo theReflectInfo)
  throw (rti1516e::FederateInternalError) {
  try {
    // Create Event Arguments
    HlaObjectEventArgs ^args = gcnew HlaObjectEventArgs();
    args->ObjectInstance->Handle = toULong(theObject.toString());
    args->Time = double::Parse(gcnew String(theTime.toString().c_str()));// Decode Time
    args->RetractionHandle = gcnew MessageRetraction(toULong(theHandle.toString()));// DeCode RetractionHandle
    // Tag
    char *buffer = (char*)malloc(theUserSuppliedTag.size());
    memcpy(buffer, (void *)(theUserSuppliedTag.data()), theUserSuppliedTag.size()); // Copy data to buffer
    args->Tag->Data = IntPtr(buffer);

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
      attr->Value = IntPtr(buffer);
      attr->Handle = toULong(gcnew String((*iterator).first.toString().c_str()));// Convert to Rom handle
      args->ObjectInstance->Attributes->Add(attr);
    }
    args->TraceMessage = "An update for the attribute values of the object instance (handle: " + args->ObjectInstance->Handle + ") is received." + " Event Timestamp = " + args->Time + ". Retraction handle = " + gcnew String(theHandle.toString().c_str());
    if (theReflectInfo.hasProducingFederate)
    {
      args->SupplementalReflectInfo->HasProducingFederate = true;
      args->SupplementalReflectInfo->ProducingFederateHandle = toULong(theReflectInfo.producingFederate.toString());
      args->TraceMessage += ". Producing Federate: " + args->SupplementalReflectInfo->ProducingFederateHandle;
    }
    args->EventType = RaconEventTypes::ObjectAttributesReflected;
    // Add to the Event Queue
    wrapper->FdAmbEventQueue->Enqueue(args);
  }
#pragma region exceptions
  catch (FederateInternalError& e) {
    String^ msg = "FdAmb_Hla1516e-(FederateInternalError - reflectAttributeValues). Reason: " + gcnew String(e.what().c_str());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
  catch (System::Exception^ e) {
    String^ msg = "FdAmb_Hla1516e-(Exception - reflectAttributeValues). Reason: " + gcnew String(e->ToString());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
#pragma endregion
};

// 6.13 - receiveInteraction
void FdAmb_Hla1516e::receiveInteraction(rti1516e::InteractionClassHandle theInteraction, ParameterHandleValueMap const & theParameterValues, VariableLengthData const & theUserSuppliedTag, OrderType sentOrder, TransportationType theType, rti1516e::SupplementalReceiveInfo theReceiveInfo)
throw (rti1516e::FederateInternalError) {
  try {
    // Create Event Arguments
    HlaInteractionEventArgs ^args = gcnew HlaInteractionEventArgs();
    args->Interaction->ClassHandle = toULong(theInteraction.toString());
    // Tag
    char *buffer = (char*)malloc(theUserSuppliedTag.size());
    memcpy(buffer, (void *)(theUserSuppliedTag.data()), theUserSuppliedTag.size()); // Copy data to buffer
    args->Tag->Data = IntPtr(buffer);

    // Parameter count = theParameterValues.size()
    //args->TraceMessage += "Parameter Count: " + theParameterValues.size();
    for (auto iterator : theParameterValues)
    {
      // Allocate new memory
      char *buffer = (char*)malloc(iterator.second.size());
      // Copy data to buffer
      memcpy(buffer, (void *)(iterator.second.data()), iterator.second.size());
      //// Debug
      ////args->TraceMessage += " Value Size: " + (*iterator).second.size();
      //if (toULong(gcnew String(iterator.first.toString().c_str())) == 0)
      //{
      //  args->TraceMessage += " Value buf: " + Marshal::ReadInt32(IntPtr(buffer));
      //  args->TraceMessage += " Value data: " + Marshal::ReadInt32(IntPtr((char *)(iterator.second.data())));
      //  args->TraceMessage += " Value Ptr: " + (long)(Marshal::PtrToStructure(IntPtr(buffer), long::typeid));
      //}
      ////else
      ////  args->TraceMessage += " Value: " + Marshal::PtrToStringAnsi(IntPtr(buffer));
      // Pass the value
      HlaParameter^ para = gcnew HlaParameter();
      para->Value = IntPtr(buffer);
      para->Handle = toULong(gcnew String(iterator.first.toString().c_str()));// Convert to Rom handle
      args->Interaction->Parameters->Add(para);
    }
    args->TraceMessage = "A new interaction is received. Interaction handle: " + args->Interaction->ClassHandle + ". Sent Order Type: " + ToString(sentOrder) + ". Transportation Type: " + ToString(theType);
    if (theReceiveInfo.hasProducingFederate)
    {
      args->SupplementalReceiveInfo->HasProducingFederate = true;
      args->SupplementalReceiveInfo->ProducingFederateHandle = toULong(theReceiveInfo.producingFederate.toString());
      args->TraceMessage += ". Producing Federate: " + toULong(theReceiveInfo.producingFederate.toString());
    }
    if (theReceiveInfo.hasSentRegions)
    {
      args->TraceMessage += ". Sent Regions: ";
      for (auto rh : theReceiveInfo.sentRegions) // iterate through set<RegionHandle>
        args->TraceMessage += toULong(rh.toString()) + " ";
    }
    args->EventType = RaconEventTypes::InteractionReceived;
    // Add to the Event Queue
    wrapper->FdAmbEventQueue->Enqueue(args);
  }
#pragma region exceptions
  catch (FederateInternalError& e) {
    String^ msg = "FdAmb_Hla1516e-(FederateInternalError - receiveInteraction). Reason: " + gcnew String(e.what().c_str());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
  catch (System::Exception^ e) {
    String^ msg = "FdAmb_Hla1516e-(Exception - receiveInteraction). Reason: " + gcnew String(e->ToString());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
#pragma endregion
};
// 6.13 - receiveInteraction2 w/ time
void FdAmb_Hla1516e::receiveInteraction(rti1516e::InteractionClassHandle theInteraction, rti1516e::ParameterHandleValueMap const & theParameterValues, rti1516e::VariableLengthData const & theUserSuppliedTag, rti1516e::OrderType sentOrder, rti1516e::TransportationType theType, rti1516e::LogicalTime const & theTime, rti1516e::OrderType receivedOrder, rti1516e::SupplementalReceiveInfo theReceiveInfo)
  throw (rti1516e::FederateInternalError) {
  try {
    // Create Event Arguments
    HlaInteractionEventArgs ^args = gcnew HlaInteractionEventArgs();
    args->Interaction->ClassHandle = toULong(theInteraction.toString()); ;
    args->Time = double::Parse(gcnew String(theTime.toString().c_str()));// Decode Time
    // Tag
    char *buffer = (char*)malloc(theUserSuppliedTag.size());
    memcpy(buffer, (void *)(theUserSuppliedTag.data()), theUserSuppliedTag.size()); // Copy data to buffer
    args->Tag->Data = IntPtr(buffer);

    for (auto iterator : theParameterValues)
    {
      // Allocate new memory
      char *buffer = (char*)malloc(iterator.second.size());
      // Copy data to buffer
      memcpy(buffer, (void *)(iterator.second.data()), iterator.second.size());
      // Pass the value
      HlaParameter^ para = gcnew HlaParameter();
      para->Value = IntPtr(buffer);
      para->Handle = toULong(gcnew String(iterator.first.toString().c_str()));// Convert to Rom handle
      args->Interaction->Parameters->Add(para);
    }
    args->TraceMessage = "A new interaction is received. Interaction handle: " + args->Interaction->ClassHandle + ". Event Timestamp: " + args->Time + ". Sent Order Type: " + ToString(sentOrder) + ". Received Order Type: " + ToString(receivedOrder) + ". Transportation Type: " + ToString(theType);
    if (theReceiveInfo.hasProducingFederate)
    {
      args->SupplementalReceiveInfo->HasProducingFederate = true;
      args->SupplementalReceiveInfo->ProducingFederateHandle = toULong(theReceiveInfo.producingFederate.toString());
      args->TraceMessage += ". Producing Federate: " + toULong(theReceiveInfo.producingFederate.toString());
    }
    if (theReceiveInfo.hasSentRegions)
    {
      args->TraceMessage += ". Sent Regions: ";
      for (auto rh : theReceiveInfo.sentRegions) // iterate through set<RegionHandle>
        args->TraceMessage += toULong(rh.toString()) + " ";
    }
    args->EventType = RaconEventTypes::InteractionReceived;
    // Add to the Event Queue
    wrapper->FdAmbEventQueue->Enqueue(args);
  }
#pragma region exceptions
  catch (FederateInternalError& e) {
    String^ msg = "FdAmb_Hla1516e-(FederateInternalError - receiveInteraction). Reason: " + gcnew String(e.what().c_str());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
  catch (System::Exception^ e) {
    String^ msg = "FdAmb_Hla1516e-(Exception - receiveInteraction). Reason: " + gcnew String(e->ToString());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
#pragma endregion
};
// 6.13 - receiveInteraction3 w/ time + retraction
// !!! doesnot trigger - OpenRTI does not support
void FdAmb_Hla1516e::receiveInteraction(rti1516e::InteractionClassHandle theInteraction, rti1516e::ParameterHandleValueMap const & theParameterValues, rti1516e::VariableLengthData const & theUserSuppliedTag, rti1516e::OrderType sentOrder, rti1516e::TransportationType theType, rti1516e::LogicalTime const & theTime, rti1516e::OrderType receivedOrder, rti1516e::MessageRetractionHandle theHandle, rti1516e::SupplementalReceiveInfo theReceiveInfo)
  throw (rti1516e::FederateInternalError) {
  try {
    // Create Event Arguments
    HlaInteractionEventArgs ^args = gcnew HlaInteractionEventArgs();
    args->Interaction->ClassHandle = toULong(theInteraction.toString()); ;
    args->Time = double::Parse(gcnew String(theTime.toString().c_str()));// Decode Time
    args->RetractionHandle = gcnew MessageRetraction(toULong(theHandle.toString()));// DeCode RetractionHandle
    // Tag
    char *buffer = (char*)malloc(theUserSuppliedTag.size());
    memcpy(buffer, (void *)(theUserSuppliedTag.data()), theUserSuppliedTag.size()); // Copy data to buffer
    args->Tag->Data = IntPtr(buffer);

    for (auto iterator : theParameterValues)
    {
      // Allocate new memory
      char *buffer = (char*)malloc(iterator.second.size());
      // Copy data to buffer
      memcpy(buffer, (void *)(iterator.second.data()), iterator.second.size());
      // Pass the value
      HlaParameter^ para = gcnew HlaParameter();
      para->Value = IntPtr(buffer);
      para->Handle = toULong(gcnew String(iterator.first.toString().c_str()));// Convert to Rom handle
      args->Interaction->Parameters->Add(para);
    }
    args->TraceMessage = "A new interaction is received. Interaction handle: " + args->Interaction->ClassHandle + ". Event Timestamp: " + args->Time + ". Retraction handle = " + gcnew String(theHandle.toString().c_str()) + ". Sent Order Type: " + ToString(sentOrder) + ". Received Order Type: " + ToString(receivedOrder) + ". Transportation Type: " + ToString(theType);
    if (theReceiveInfo.hasProducingFederate)
    {
      args->SupplementalReceiveInfo->HasProducingFederate = true;
      args->SupplementalReceiveInfo->ProducingFederateHandle = toULong(theReceiveInfo.producingFederate.toString());
      args->TraceMessage += ". Producing Federate: " + toULong(theReceiveInfo.producingFederate.toString());
    }
    if (theReceiveInfo.hasSentRegions)
    {
      args->TraceMessage += ". Sent Regions: ";
      for (auto rh : theReceiveInfo.sentRegions) // iterate through set<RegionHandle>
        args->TraceMessage += toULong(rh.toString()) + " ";
    }
    args->EventType = RaconEventTypes::InteractionReceived;
    // Add to the Event Queue
    wrapper->FdAmbEventQueue->Enqueue(args);
  }
#pragma region exceptions
  catch (FederateInternalError& e) {
    String^ msg = "FdAmb_Hla1516e-(FederateInternalError - receiveInteraction). Reason: " + gcnew String(e.what().c_str());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
  catch (System::Exception^ e) {
    String^ msg = "FdAmb_Hla1516e-(Exception - receiveInteraction). Reason: " + gcnew String(e->ToString());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
#pragma endregion
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

// 6.15 - Remove Object Instance
void FdAmb_Hla1516e::removeObjectInstance(rti1516e::ObjectInstanceHandle theObject, rti1516e::VariableLengthData const & theUserSuppliedTag, rti1516e::OrderType sentOrder, rti1516e::SupplementalRemoveInfo theRemoveInfo)
throw (rti1516e::FederateInternalError) {
  try {
    // Create Event Arguments
    HlaObjectEventArgs ^args = gcnew HlaObjectEventArgs();
    args->ObjectInstance->Handle = toULong(theObject.toString());
    // Allocate new memory for tag
    char *buffer = (char*)malloc(theUserSuppliedTag.size());
    memcpy(buffer, (void *)(theUserSuppliedTag.data()), theUserSuppliedTag.size()); // Copy data to buffer
    args->Tag->Data = IntPtr(buffer);
    // Update Nom
    om->Objects.erase(args->ObjectInstance->Handle);
    args->TraceMessage = "The object (handle: " + args->ObjectInstance->Handle + ") is removed. ";
    if (theRemoveInfo.hasProducingFederate)
    {
      args->SupplementalRemoveInfo->HasProducingFederate = true;
      args->SupplementalRemoveInfo->ProducingFederateHandle = toULong(theRemoveInfo.producingFederate.toString());
      args->TraceMessage += "Producing Federate: " + toULong(theRemoveInfo.producingFederate.toString());
    }
    args->EventType = RaconEventTypes::ObjectRemoved;
    // Add to the Event Queue
    wrapper->FdAmbEventQueue->Enqueue(args);
  }
#pragma region exceptions
  catch (FederateInternalError& e) {
    String^ msg = "FdAmb_Hla1516e-(FederateInternalError - removeObjectInstance). Reason: " + gcnew String(e.what().c_str());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
  catch (System::Exception^ e) {
    String^ msg = "FdAmb_Hla1516e-(Exception - removeObjectInstance). Reason: " + gcnew String(e->ToString());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
#pragma endregion
};
// 6.15 - Remove Object Instance w/ time
void FdAmb_Hla1516e::removeObjectInstance(rti1516e::ObjectInstanceHandle theObject, rti1516e::VariableLengthData const & theUserSuppliedTag, rti1516e::OrderType sentOrder, rti1516e::LogicalTime const & theTime, rti1516e::OrderType receivedOrder, rti1516e::SupplementalRemoveInfo theRemoveInfo)
throw (rti1516e::FederateInternalError) {
  try {
    // Create Event Arguments
    HlaObjectEventArgs ^args = gcnew HlaObjectEventArgs();
    args->ObjectInstance->Handle = toULong(theObject.toString());
    // Allocate new memory for tag
    char *buffer = (char*)malloc(theUserSuppliedTag.size());
    memcpy(buffer, (void *)(theUserSuppliedTag.data()), theUserSuppliedTag.size()); // Copy data to buffer
    args->Tag->Data = IntPtr(buffer);
    // Decode Time
    args->Time = double::Parse(gcnew String(theTime.toString().c_str()));

    // Update Nom
    om->Objects.erase(args->ObjectInstance->Handle);
    args->TraceMessage = "The object (handle: " + args->ObjectInstance->Handle + ") is removed. Event Timestamp = " + args->Time;
    if (theRemoveInfo.hasProducingFederate)
    {
      args->SupplementalRemoveInfo->HasProducingFederate = true;
      args->SupplementalRemoveInfo->ProducingFederateHandle = toULong(theRemoveInfo.producingFederate.toString());
      args->TraceMessage += ". Producing Federate: " + toULong(theRemoveInfo.producingFederate.toString());
    }
    args->EventType = RaconEventTypes::ObjectRemoved;
    // Add to the Event Queue
    wrapper->FdAmbEventQueue->Enqueue(args);
  }
#pragma region exceptions
  catch (FederateInternalError& e) {
    String^ msg = "FdAmb_Hla1516e-(FederateInternalError - removeObjectInstance). Reason: " + gcnew String(e.what().c_str());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
  catch (System::Exception^ e) {
    String^ msg = "FdAmb_Hla1516e-(Exception - removeObjectInstance). Reason: " + gcnew String(e->ToString());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
#pragma endregion
};
// 6.15 - Remove Object Instance w/ time + retraction
void FdAmb_Hla1516e::removeObjectInstance(rti1516e::ObjectInstanceHandle theObject, rti1516e::VariableLengthData const & theUserSuppliedTag, rti1516e::OrderType sentOrder, rti1516e::LogicalTime const & theTime, rti1516e::OrderType receivedOrder, rti1516e::MessageRetractionHandle theHandle, rti1516e::SupplementalRemoveInfo theRemoveInfo)
  throw (rti1516e::FederateInternalError) {
  /// !!! sentOrder, receivedOrder not used
  try {
    // Create Event Arguments
    HlaObjectEventArgs ^args = gcnew HlaObjectEventArgs();
    args->ObjectInstance->Handle = toULong(theObject.toString());
    // Allocate new memory for tag
    char *buffer = (char*)malloc(theUserSuppliedTag.size());
    memcpy(buffer, (void *)(theUserSuppliedTag.data()), theUserSuppliedTag.size()); // Copy data to buffer
    args->Tag->Data = IntPtr(buffer);
    args->Time = double::Parse(gcnew String(theTime.toString().c_str()));// Decode Time
    args->RetractionHandle = gcnew MessageRetraction(toULong(theHandle.toString()));// DeCode RetractionHandle

    // Update Nom
    om->Objects.erase(args->ObjectInstance->Handle);
    args->TraceMessage = "The object (handle: " + args->ObjectInstance->Handle + ") is removed. Event Timestamp = " + args->Time + ". Retraction handle = " + gcnew String(theHandle.toString().c_str());
    if (theRemoveInfo.hasProducingFederate)
    {
      args->SupplementalRemoveInfo->HasProducingFederate = true;
      args->SupplementalRemoveInfo->ProducingFederateHandle = toULong(theRemoveInfo.producingFederate.toString());
      args->TraceMessage += ". Producing Federate: " + toULong(theRemoveInfo.producingFederate.toString());
    }
    args->EventType = RaconEventTypes::ObjectRemoved;
    // Add to the Event Queue
    wrapper->FdAmbEventQueue->Enqueue(args);
  }
#pragma region exceptions
  catch (FederateInternalError& e) {
    String^ msg = "FdAmb_Hla1516e-(FederateInternalError - removeObjectInstance). Reason: " + gcnew String(e.what().c_str());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
  catch (System::Exception^ e) {
    String^ msg = "FdAmb_Hla1516e-(Exception - removeObjectInstance). Reason: " + gcnew String(e->ToString());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
#pragma endregion
};

// 6.20 - Provide Attribute Value Update
void FdAmb_Hla1516e::provideAttributeValueUpdate(rti1516e::ObjectInstanceHandle theObject, rti1516e::AttributeHandleSet const & theAttributes, rti1516e::VariableLengthData const & theUserSuppliedTag)
  throw (rti1516e::FederateInternalError) {
  try {
    // Create Event Arguments
    HlaObjectEventArgs^ args = gcnew HlaObjectEventArgs();
    args->ObjectInstance->Handle = toULong(theObject.toString());
    // Tag
    char *buffer = (char*)malloc(theUserSuppliedTag.size());
    memcpy(buffer, (void *)(theUserSuppliedTag.data()), theUserSuppliedTag.size()); // Copy data to buffer
    args->Tag->Data = IntPtr(buffer);

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
#pragma region exceptions
  catch (FederateInternalError& e) {
    String^ msg = "FdAmb_Hla1516e-(FederateInternalError - provideAttributeValueUpdate). Reason: " + gcnew String(e.what().c_str());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
  catch (System::Exception^ e) {
    String^ msg = "FdAmb_Hla1516e-(Exception - provideAttributeValueUpdate). Reason: " + gcnew String(e->ToString());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
#pragma endregion
};

// 6.21 - turnUpdatesOnForObjectInstance
//  Turn Updates On/Off is missing. !!! Not supported by OpenRti 0.8.0
void FdAmb_Hla1516e::turnUpdatesOnForObjectInstance(rti1516e::ObjectInstanceHandle theObject, rti1516e::AttributeHandleSet const & theAttributes)
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
#pragma region exceptions
  catch (FederateInternalError& e) {
    String^ msg = "FdAmb_Hla1516e-(FederateInternalError - turnUpdatesOnForObjectInstance). Reason: " + gcnew String(e.what().c_str());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
  catch (System::Exception^ e) {
    String^ msg = "FdAmb_Hla1516e-(Exception - turnUpdatesOnForObjectInstance). Reason: " + gcnew String(e->ToString());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
#pragma endregion
};
// updateRateDesignator - not implemented
void FdAmb_Hla1516e::turnUpdatesOnForObjectInstance(rti1516e::ObjectInstanceHandle theObject, rti1516e::AttributeHandleSet const & theAttributes, std::wstring const & updateRateDesignator)
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
#pragma region exceptions
  catch (FederateInternalError& e) {
    String^ msg = "FdAmb_Hla1516e-(FederateInternalError - turnUpdatesOnForObjectInstance). Reason: " + gcnew String(e.what().c_str());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
  catch (System::Exception^ e) {
    String^ msg = "FdAmb_Hla1516e-(Exception - turnUpdatesOnForObjectInstance). Reason: " + gcnew String(e->ToString());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
#pragma endregion
};
#pragma endregion // Object Management Services

#pragma region Time Management Services

void FdAmb_Hla1516e::timeRegulationEnabled(rti1516e::LogicalTime const & theFederateTime)
throw (rti1516e::FederateInternalError) {
  try {
    // Create Event Arguments
    HlaTimeManagementEventArgs ^args = gcnew HlaTimeManagementEventArgs();
    rti1516e::HLAfloat64Time time(theFederateTime);
    args->Time = time.getTime();

    args->TraceMessage = "Time regulation enabled. Current logical time of the joined federate: " + args->Time;
    args->EventType = RaconEventTypes::TimeRegulationEnabled;
    // Add to the Event Queue
    wrapper->FdAmbEventQueue->Enqueue(args);
  }
#pragma region exceptions
  catch (FederateInternalError& e) {
    String^ msg = "FdAmb_Hla1516e-(FederateInternalError - timeRegulationEnabled). Reason: " + gcnew String(e.what().c_str());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
  catch (System::Exception^ e) {
    String^ msg = "FdAmb_Hla1516e-(Exception - timeRegulationEnabled). Reason: " + gcnew String(e->ToString());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
#pragma endregion
};

void FdAmb_Hla1516e::timeConstrainedEnabled(rti1516e::LogicalTime const & theFederateTime)
throw (rti1516e::FederateInternalError) {
  try {
    // Create Event Arguments
    HlaTimeManagementEventArgs ^args = gcnew HlaTimeManagementEventArgs();
    rti1516e::HLAfloat64Time time(theFederateTime);
    args->Time = time.getTime();

    args->TraceMessage = "Time constrained enabled. Current logical time of the joined federate: " + args->Time;
    args->EventType = RaconEventTypes::TimeConstrainedEnabled;
    // Add to the Event Queue
    wrapper->FdAmbEventQueue->Enqueue(args);
  }
#pragma region exceptions
  catch (FederateInternalError& e) {
    String^ msg = "FdAmb_Hla1516e-(FederateInternalError - connectionLost). Reason: " + gcnew String(e.what().c_str());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
  catch (System::Exception^ e) {
    String^ msg = "FdAmb_Hla1516e-(Exception - connectionLost). Reason: " + gcnew String(e->ToString());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
#pragma endregion
};
// 8.13 - timeAdvanceGrant
void FdAmb_Hla1516e::timeAdvanceGrant(rti1516e::LogicalTime const & theTime)
throw (rti1516e::FederateInternalError) {
  try {
    // Create Event Arguments
    HlaTimeManagementEventArgs ^args = gcnew HlaTimeManagementEventArgs();
    rti1516e::HLAfloat64Time time(theTime);
    args->Time = time.getTime();

    args->TraceMessage = "Time advance is granted. The logical time to which the federate has advanced: " + args->Time;
    args->EventType = RaconEventTypes::TimeAdvanceGrant;
    // Add to the Event Queue
    wrapper->FdAmbEventQueue->Enqueue(args);
  }
#pragma region exceptions
  catch (FederateInternalError& e) {
    String^ msg = "FdAmb_Hla1516e-(FederateInternalError - timeConstrainedEnabled). Reason: " + gcnew String(e.what().c_str());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
  catch (System::Exception^ e) {
    String^ msg = "FdAmb_Hla1516e-(Exception - timeConstrainedEnabled). Reason: " + gcnew String(e->ToString());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
#pragma endregion

};
void FdAmb_Hla1516e::requestRetraction(rti1516e::MessageRetractionHandle theHandle)
throw (rti1516e::FederateInternalError) {
  // Create Event Arguments
  HlaTimeManagementEventArgs ^args = gcnew HlaTimeManagementEventArgs();
  args->RetractionHandle = gcnew MessageRetraction(toULong(theHandle.toString()));// DeCode RetractionHandle

  args->TraceMessage = "TSO event is being retracted. The event retraction serial no: " + args->RetractionHandle->SerialNumber + ". The sending federate handle: " + args->RetractionHandle->SerialNumber + ".";
  args->EventType = RaconEventTypes::TimeAdvanceGrant;
  // Add to the Event Queue
  wrapper->FdAmbEventQueue->Enqueue(args);
};

#pragma endregion // time Management Services

#pragma region Ownership Management Services
// 7.4 - requestAttributeOwnershipAssumption
void FdAmb_Hla1516e::requestAttributeOwnershipAssumption(rti1516e::ObjectInstanceHandle theObject, rti1516e::AttributeHandleSet const & offeredAttributes,
  rti1516e::VariableLengthData const & theUserSuppliedTag)
  throw (rti1516e::FederateInternalError)
{
  try {
    // Create Event Arguments
    HlaOwnershipManagementEventArgs ^args = gcnew HlaOwnershipManagementEventArgs();
    args->ObjectHandle = toULong(theObject.toString());
    // Tag
    char *buffer = (char*)malloc(theUserSuppliedTag.size());
    memcpy(buffer, (void *)(theUserSuppliedTag.data()), theUserSuppliedTag.size()); // Copy data to buffer
    args->Tag->Data = IntPtr(buffer);
    String^ attributeString;
    AttributeHandleSet::const_iterator iterator;
    for (iterator = offeredAttributes.begin(); iterator != offeredAttributes.end(); iterator++)
    {
      HlaAttribute^ attr = gcnew HlaAttribute();
      attr->Handle = toULong(gcnew String(iterator->toString().c_str()));// Convert to Rom handle
      args->AttributeSet->Add(attr);
      attributeString += attr->Handle + ",";
    }

    args->TraceMessage = "A specified set of offered attributes (handles: {" + attributeString + "}) of the object (" + args->ObjectHandle + ") has become available for acquisition.";
    args->EventType = RaconEventTypes::AttributeOwnershipAssumptionRequested;
    // Add to the Event Queue
    wrapper->FdAmbEventQueue->Enqueue(args);
  }
#pragma region exceptions
  catch (FederateInternalError& e) {
    String^ msg = "FdAmb_Hla1516e-(FederateInternalError - requestAttributeOwnershipAssumption). Reason: " + gcnew String(e.what().c_str());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
  catch (System::Exception^ e) {
    String^ msg = "FdAmb_Hla1516e-(Exception - requestAttributeOwnershipAssumption). Reason: " + gcnew String(e->ToString());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
#pragma endregion
};

// 7.5 - requestDivestitureConfirmation
void FdAmb_Hla1516e::requestDivestitureConfirmation(rti1516e::ObjectInstanceHandle theObject, rti1516e::AttributeHandleSet const & releasedAttributes)
throw (rti1516e::FederateInternalError)
{
  try {
    // Create Event Arguments
    HlaOwnershipManagementEventArgs ^args = gcnew HlaOwnershipManagementEventArgs();
    args->ObjectHandle = toULong(theObject.toString());
    String^ attributeString;
    AttributeHandleSet::const_iterator iterator;
    for (iterator = releasedAttributes.begin(); iterator != releasedAttributes.end(); iterator++)
    {
      HlaAttribute^ attr = gcnew HlaAttribute();
      attr->Handle = toULong(gcnew String(iterator->toString().c_str()));// Convert to Rom handle
      args->AttributeSet->Add(attr);
      attributeString += attr->Handle + ",";
    }

    args->TraceMessage = "A new owner has been found for the released attributes (handles: {" + attributeString + "}) of the object (" + args->ObjectHandle + ").";
    args->EventType = RaconEventTypes::RequestDivestitureConfirmation;
    // Add to the Event Queue
    wrapper->FdAmbEventQueue->Enqueue(args);
  }
#pragma region exceptions
  catch (FederateInternalError& e) {
    String^ msg = "FdAmb_Hla1516e-(FederateInternalError - requestDivestitureConfirmation). Reason: " + gcnew String(e.what().c_str());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
  catch (System::Exception^ e) {
    String^ msg = "FdAmb_Hla1516e-(Exception - requestDivestitureConfirmation). Reason: " + gcnew String(e->ToString());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
#pragma endregion

};

// 7.7 - attributeOwnershipAcquisitionNotification
void FdAmb_Hla1516e::attributeOwnershipAcquisitionNotification(rti1516e::ObjectInstanceHandle theObject, rti1516e::AttributeHandleSet const & securedAttributes, rti1516e::VariableLengthData const & theUserSuppliedTag)
  throw (rti1516e::FederateInternalError)
{
  try {
    // Create Event Arguments
    HlaOwnershipManagementEventArgs ^args = gcnew HlaOwnershipManagementEventArgs();
    args->ObjectHandle = toULong(theObject.toString());
    String^ attributeString;
    AttributeHandleSet::const_iterator iterator;
    for (iterator = securedAttributes.begin(); iterator != securedAttributes.end(); iterator++)
    {
      HlaAttribute^ attr = gcnew HlaAttribute();
      attr->Handle = toULong(gcnew String(iterator->toString().c_str()));// Convert to Rom handle
      args->AttributeSet->Add(attr);
      attributeString += attr->Handle + ",";
    }
    // Tag
    char *buffer = (char*)malloc(theUserSuppliedTag.size());
    memcpy(buffer, (void *)(theUserSuppliedTag.data()), theUserSuppliedTag.size()); // Copy data to buffer
    args->Tag->Data = IntPtr(buffer);

    args->TraceMessage = "The ownership of a set of attributes (handles: {" + attributeString + "}) of the object (" + args->ObjectHandle + ") has been acquired.";
    args->EventType = RaconEventTypes::AttributeOwnershipAcquisitionNotified;
    // Add to the Event Queue
    wrapper->FdAmbEventQueue->Enqueue(args);
  }
#pragma region exceptions
  catch (FederateInternalError& e) {
    String^ msg = "FdAmb_Hla1516e-(FederateInternalError - attributeOwnershipAcquisitionNotification). Reason: " + gcnew String(e.what().c_str());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
  catch (System::Exception^ e) {
    String^ msg = "FdAmb_Hla1516e-(Exception - attributeOwnershipAcquisitionNotification). Reason: " + gcnew String(e->ToString());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
#pragma endregion
};

// 7.10 - attributeOwnershipUnavailable
void FdAmb_Hla1516e::attributeOwnershipUnavailable(rti1516e::ObjectInstanceHandle theObject, rti1516e::AttributeHandleSet const & theAttributes)
  throw (
    rti1516e::FederateInternalError)
{
  try {
    // Create Event Arguments
    HlaOwnershipManagementEventArgs ^args = gcnew HlaOwnershipManagementEventArgs();
    args->ObjectHandle = toULong(theObject.toString());
    String^ attributeString;
    AttributeHandleSet::const_iterator iterator;
    for (iterator = theAttributes.begin(); iterator != theAttributes.end(); iterator++)
    {
      HlaAttribute^ attr = gcnew HlaAttribute();
      attr->Handle = toULong(gcnew String(iterator->toString().c_str()));// Convert to Rom handle
      args->AttributeSet->Add(attr);
      attributeString += attr->Handle + ",";
    }

    args->TraceMessage = "The attributes (handles: {" + attributeString + "}) of the object (" + args->ObjectHandle + ") that the federate attempted to acquire are currently owned by remote federate or by the RTI itself.";
    args->EventType = RaconEventTypes::AttributeOwnershipUnavailable;
    // Add to the Event Queue
    wrapper->FdAmbEventQueue->Enqueue(args);
  }
#pragma region exceptions
  catch (FederateInternalError& e) {
    String^ msg = "FdAmb_Hla1516e-(FederateInternalError - attributeOwnershipUnavailable). Reason: " + gcnew String(e.what().c_str());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
  catch (System::Exception^ e) {
    String^ msg = "FdAmb_Hla1516e-(Exception - attributeOwnershipUnavailable). Reason: " + gcnew String(e->ToString());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
#pragma endregion
};

// 7.11 - requestAttributeOwnershipRelease
void FdAmb_Hla1516e::requestAttributeOwnershipRelease(rti1516e::ObjectInstanceHandle theObject, rti1516e::AttributeHandleSet const & candidateAttributes, rti1516e::VariableLengthData const & theUserSuppliedTag)
  throw (RTI::FederateInternalError) {
  try {
    // Create Event Arguments
    HlaOwnershipManagementEventArgs ^args = gcnew HlaOwnershipManagementEventArgs();
    args->ObjectHandle = toULong(theObject.toString());
    String^ attributeString;
    AttributeHandleSet::const_iterator iterator;
    for (iterator = candidateAttributes.begin(); iterator != candidateAttributes.end(); iterator++)
    {
      HlaAttribute^ attr = gcnew HlaAttribute();
      attr->Handle = toULong(gcnew String(iterator->toString().c_str()));// Convert to Rom handle
      args->AttributeSet->Add(attr);
      attributeString += attr->Handle + ",";
    }
    // Tag
    char *buffer = (char*)malloc(theUserSuppliedTag.size());
    memcpy(buffer, (void *)(theUserSuppliedTag.data()), theUserSuppliedTag.size()); // Copy data to buffer
    args->Tag->Data = IntPtr(buffer);

    args->TraceMessage = "A request for attribute ownership release for candidate attributes (handles: {" + attributeString + "}) of the object (" + args->ObjectHandle + ") is received.";
    args->EventType = RaconEventTypes::AttributeOwnershipReleaseRequested;
    // Add to the Event Queue
    wrapper->FdAmbEventQueue->Enqueue(args);
  }
#pragma region exceptions
  catch (FederateInternalError& e) {
    String^ msg = "FdAmb_Hla1516e-(FederateInternalError - requestAttributeOwnershipRelease). Reason: " + gcnew String(e.what().c_str());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
  catch (System::Exception^ e) {
    String^ msg = "FdAmb_Hla1516e-(Exception - requestAttributeOwnershipRelease). Reason: " + gcnew String(e->ToString());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
#pragma endregion
};

// 7.16 - confirmAttributeOwnershipAcquisitionCancellation
void FdAmb_Hla1516e::confirmAttributeOwnershipAcquisitionCancellation(rti1516e::ObjectInstanceHandle theObject,
  rti1516e::AttributeHandleSet const & theAttributes)
  throw (
    rti1516e::FederateInternalError)
{
  try {
    // Create Event Arguments
    HlaOwnershipManagementEventArgs ^args = gcnew HlaOwnershipManagementEventArgs();
    args->ObjectHandle = toULong(theObject.toString());
    String^ attributeString;
    AttributeHandleSet::const_iterator iterator;
    for (iterator = theAttributes.begin(); iterator != theAttributes.end(); iterator++)
    {
      HlaAttribute^ attr = gcnew HlaAttribute();
      attr->Handle = toULong(gcnew String(iterator->toString().c_str()));// Convert to Rom handle
      args->AttributeSet->Add(attr);
      attributeString += attr->Handle + ",";
    }
    args->TraceMessage = "The request to cancel acquisition of a specified set of attributes (handles: {" + attributeString + "}) of the object (" + args->ObjectHandle + ") has been achieved.";
    args->EventType = RaconEventTypes::AttributeOwnershipAcquisitionCancellationConfirmed;
    // Add to the Event Queue
    wrapper->FdAmbEventQueue->Enqueue(args);
  }
#pragma region exceptions
  catch (FederateInternalError& e) {
    String^ msg = "FdAmb_Hla1516e-(FederateInternalError - confirmAttributeOwnershipAcquisitionCancellation). Reason: " + gcnew String(e.what().c_str());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
  catch (System::Exception^ e) {
    String^ msg = "FdAmb_Hla1516e-(Exception - confirmAttributeOwnershipAcquisitionCancellation). Reason: " + gcnew String(e->ToString());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
#pragma endregion
};

// 7.18 - informAttributeOwnership
void FdAmb_Hla1516e::informAttributeOwnership(rti1516e::ObjectInstanceHandle theObject, rti1516e::AttributeHandle theAttribute, rti1516e::FederateHandle theOwner)
  throw (
    rti1516e::FederateInternalError)
{
  try {
    HlaOwnershipManagementEventArgs ^args = gcnew HlaOwnershipManagementEventArgs();
    // oHandle
    args->ObjectHandle = toULong(theObject.toString());
    // Attr
    HlaAttribute^ attr = gcnew HlaAttribute();
    attr->Handle = Handle2Long(theAttribute);// Convert to Rom handle
    args->AttributeSet->Add(attr);
    // federate
    args->FederateHandle = Handle2Long(theOwner);// Convert to Rom handle

    args->TraceMessage = "Information about the ownership of attribute (handle: " + attr->Handle + ") of the object (" + args->ObjectHandle + ") is received. The owner federate is " + args->FederateHandle;
    args->EventType = RaconEventTypes::AttributeOwnershipInformed;
    // Add to the Event Queue
    wrapper->FdAmbEventQueue->Enqueue(args);
  }
#pragma region exceptions
  catch (FederateInternalError& e) {
    String^ msg = "FdAmb_Hla1516e-(FederateInternalError - informAttributeOwnership). Reason: " + gcnew String(e.what().c_str());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
  catch (System::Exception^ e) {
    String^ msg = "FdAmb_Hla1516e-(Exception - informAttributeOwnership). Reason: " + gcnew String(e->ToString());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
#pragma endregion
};

// 7.18 - attributeIsNotOwned
void FdAmb_Hla1516e::attributeIsNotOwned(rti1516e::ObjectInstanceHandle theObject, rti1516e::AttributeHandle theAttribute)
  throw (
    rti1516e::FederateInternalError)
{
  try {
    // Create Event Arguments
    HlaOwnershipManagementEventArgs ^args = gcnew HlaOwnershipManagementEventArgs();
    args->ObjectHandle = toULong(theObject.toString());
    HlaAttribute^ attr = gcnew HlaAttribute();
    args->AttributeSet->Add(attr);

    args->FederateHandle = 0; // No Owner
    args->TraceMessage = "Information about the ownership of attribute (handle: " + attr->Handle + ") of the object (" + args->ObjectHandle + ") is received. The attribute exists in the federation but is  currently not owned by any federate.";
    args->EventType = RaconEventTypes::AttributeOwnershipInformed;
    // Add to the Event Queue
    wrapper->FdAmbEventQueue->Enqueue(args);
  }
#pragma region exceptions
  catch (FederateInternalError& e) {
    String^ msg = "FdAmb_Hla1516e-(FederateInternalError - attributeIsNotOwned). Reason: " + gcnew String(e.what().c_str());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
  catch (System::Exception^ e) {
    String^ msg = "FdAmb_Hla1516e-(Exception - attributeIsNotOwned). Reason: " + gcnew String(e->ToString());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
#pragma endregion
};

// 7.18 - attributeOwnedByRTI
// Typically a MOM object or internal RTI object
void FdAmb_Hla1516e::attributeIsOwnedByRTI(rti1516e::ObjectInstanceHandle theObject, rti1516e::AttributeHandle theAttribute)
  throw (
    rti1516e::FederateInternalError)
{
  try {
    HlaOwnershipManagementEventArgs ^args = gcnew HlaOwnershipManagementEventArgs();
    args->ObjectHandle = toULong(theObject.toString());
    HlaAttribute^ attr = gcnew HlaAttribute();
    args->AttributeSet->Add(attr);
    args->FederateHandle = -1; // Owner is RTI
    args->TraceMessage = "Information about the ownership of attribute (handle: " + attr->Handle + ") of the object (" + args->ObjectHandle + ") is received. The attribute is currently owned internally by the RTI.";
    args->EventType = RaconEventTypes::AttributeOwnershipInformed;
    // Add to the Event Queue
    wrapper->FdAmbEventQueue->Enqueue(args);
  }
#pragma region exceptions
  catch (FederateInternalError& e) {
    String^ msg = "FdAmb_Hla1516e-(FederateInternalError - attributeIsOwnedByRTI). Reason: " + gcnew String(e.what().c_str());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
  catch (System::Exception^ e) {
    String^ msg = "FdAmb_Hla1516e-(Exception - attributeIsOwnedByRTI). Reason: " + gcnew String(e->ToString());
    wrapper->FdAmbEventQueue->Enqueue(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
#pragma endregion
};

#pragma endregion // Ownership Management Services

