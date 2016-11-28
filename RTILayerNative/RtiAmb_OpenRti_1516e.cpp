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
#include <vcclr.h>
#include "RTI/RTI1516.h"
#include "RTI/time/HLAfloat64Interval.h"
#include "RTI/time/HLAfloat64Time.h"
#include "RtiAmb_OpenRti_1516e.h"
#include "Helpers_Hla1516e.h"
//#include <assert.h>

using namespace Racon::RtiLayer;
using namespace rti1516e;
using namespace System;
using namespace System::Runtime::InteropServices;// For Marshalling
using namespace System::Threading; // for sleep

#pragma region Constructor
RtiAmb_OpenRti_1516e::RtiAmb_OpenRti_1516e(CallbackManager^ eventManager) : RtiAmb_Hla1516e(eventManager) {
  try {
    RtiVersion = gcnew String(rti1516e::rtiName().c_str()) + " v" + gcnew String(rti1516e::rtiVersion().c_str()) + " (OpenRTI-0.8.0 win32)";// rtiVersion() returns 1.0
  }
  catch (System::Exception^ e) {
    MessageBox::Show("MSG-(RtiAmb_OpenRti_1516e Ctor):" + Environment::NewLine + gcnew String(e->Message) + Environment::NewLine + "The native rti ambassador pointer creation  is failed. This may indicate a corrupted/a wrong RTI.rid file. Please ensure that RTI.rid is provided by Portico distribution" + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
  }
};
#pragma endregion			

#pragma region Federation Management
// Join Federation
void RtiAmb_OpenRti_1516e::joinFederation(String ^fedexec, String ^fdName) {
  bool joined = false;
  int numTries = 0;
  while ((numTries++ < 20) && !joined) {
    try {
      pin_ptr<const wchar_t> fedx = PtrToStringChars(fedexec);
      pin_ptr<const wchar_t> fd = PtrToStringChars(fdName);

      rti1516e::FederateHandle federateHandle = rti->joinFederationExecution(fd, L"Federate Type (Not Implemented)", fedx);
      joined = true;
      String^ handle = gcnew String(federateHandle.toString().c_str());
      String^ msg = "Federate joined to the federation execution (" + fedexec + "). Federate handle: " + Handle2Long(federateHandle).ToString();
      this->OnFederateJoined(gcnew RaconEventArgs(msg));
    }
#pragma region exceptions
    catch (CouldNotCreateLogicalTimeFactory& e) {
      String^ msg = "MSG-(CouldNotCreateLogicalTimeFactory - joinFederation):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
      this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    }
    catch (FederationExecutionDoesNotExist& e) {
      MessageBox::Show("MSG-(FederationExecutionDoesNotExist - joinFederation):" + Environment::NewLine + "Federation Execution does not exist. Please, restart the federate. Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_OpenRti_1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    }
    catch (ErrorReadingFDD& e) {
      MessageBox::Show("MSG-(ErrorReadingFED - joinFederation):" + Environment::NewLine + "Invalid FOM Module. Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_OpenRti_1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    }
    catch (InconsistentFDD& e) {
      MessageBox::Show("MSG-(InconsistentFDD - joinFederation):" + Environment::NewLine + "Inconsistent FOM Module. Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_OpenRti_1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    }
    catch (CouldNotOpenFDD& e) {
      MessageBox::Show("MSG-(CouldNotOpenFDD - joinFederation):" + Environment::NewLine + "Federate cannot open the FED file. Either the file does not exist or there exists an error in the file name or path provided. Please, check that FED file exists and its path is correct. Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_OpenRti_1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    }
    catch (SaveInProgress& e) {
      String^ msg = "MSG-(SaveInProgress - joinFederation):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
      this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    }
    catch (RestoreInProgress& e) {
      String^ msg = "MSG-(RestoreInProgress - joinFederation):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
      this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    }
    catch (FederateAlreadyExecutionMember& e) {
      MessageBox::Show("MSG-(FederateAlreadyExecutionMember - joinFederation):" + Environment::NewLine + "Federate already is an federation execution member." + Environment::NewLine, "RtiAmb_OpenRti_1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    }
    catch (NotConnected& e) {
      String^ msg = "MSG-(NotConnected - joinFederation):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
      this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    }
    catch (RTIinternalError& e) {
      MessageBox::Show("MSG-(RTIinternalError - joinFederation):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_OpenRti_1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
    }
    catch (System::Exception^ e) {
      MessageBox::Show("MSG-(GeneralException - joinFederation):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RtiAmb_OpenRti_1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    }
#pragma endregion
  } // end of while
};
#pragma endregion	// Federation Management

#pragma region Declaration Management
#pragma endregion	// Declaration Management


#pragma region Object Management

// 6.8 - Register Object
bool RtiAmb_OpenRti_1516e::registerObject(HlaObject ^obj) {
  try {
    rti1516e::ObjectClassHandle chandle = om->ObjectClasses[obj->Type->Handle];// get handle pointer
    // register
    rti1516e::ObjectInstanceHandle ohandle = rti->registerObjectInstance(chandle); // register

    obj->Handle = toULong(gcnew String(ohandle.toString().c_str()));
    om->Objects[obj->Handle] = ohandle; // update Nom
    // report
    String^ msg = "Object (class name: " + obj->Type->Name + ", class handle: "+ obj->Type->Handle + ") is registered. Object handle: " + Handle2Long(ohandle).ToString();
    this->OnObjectRegistered(gcnew RaconEventArgs(msg));
    return true;
  }
#pragma region exceptions
  catch (ObjectClassNotDefined& e) {
    String^ msg = "MSG-(ObjectClassNotDefined - registerObject):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
  }
  catch (ObjectClassNotPublished& e) {
    String^ msg = "MSG-(ObjectClassNotPublished - registerObject):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
  }
  catch (FederateNotExecutionMember& e) {
    String^ msg = "MSG-(FederateNotExecutionMember - registerObject):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
  }
  catch (SaveInProgress& e) {
    String^ msg = "MSG-(SaveInProgress - registerObject):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
  }
  catch (RestoreInProgress& e) {
    String^ msg = "MSG-(RestoreInProgress - registerObject):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
  }
  catch (NotConnected& e) {
    String^ msg = "MSG-(NotConnected - registerObject):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
  }
  catch (RTIinternalError& e) {
    MessageBox::Show("MSG-(RTIinternalError - registerObject):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (System::Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - registerObject):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
  }
#pragma endregion
};

// 6.8 - Register Object with a name
bool RtiAmb_OpenRti_1516e::registerObject(HlaObject ^obj, String^ name) {
  try {
    rti1516e::ObjectClassHandle handle = om->ObjectClasses[obj->Type->Handle];// get handle pointer
    pin_ptr<const wchar_t> oname = PtrToStringChars(name);
    rti1516e::ObjectInstanceHandle ohandle = rti->registerObjectInstance(handle, oname); // register
    obj->Handle = toULong(gcnew String(ohandle.toString().c_str()));
    om->Objects[obj->Handle] = ohandle; // update Nom
    String^ msg = "Object (class name: " + obj->Type->Name + ", object name: " + name + ") is registered. Object handle: " + Handle2Long(ohandle).ToString();
    this->OnObjectRegistered(gcnew RaconEventArgs(msg));
    return true;
  }
#pragma region exceptions
  catch (ObjectClassNotDefined& e) {
    String^ msg = "MSG-(ObjectClassNotDefined - registerObject2):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
  }
  catch (ObjectClassNotPublished& e) {
    String^ msg = "MSG-(ObjectClassNotPublished - registerObject2):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
  }
  catch (ObjectInstanceNameNotReserved& e) {
    String^ msg = "MSG-(ObjectInstanceNameNotReserved - registerObject2):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
  }
  catch (ObjectInstanceNameInUse& e) {
    String^ msg = "MSG-(ObjectInstanceNameInUse - registerObject2):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
  }
  catch (FederateNotExecutionMember& e) {
    String^ msg = "MSG-(FederateNotExecutionMember - registerObject2):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
  }
  catch (SaveInProgress& e) {
    String^ msg = "MSG-(SaveInProgress - registerObject2):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
  }
  catch (RestoreInProgress& e) {
    String^ msg = "MSG-(RestoreInProgress - registerObject2):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
  }
  catch (NotConnected& e) {
    String^ msg = "MSG-(NotConnected - registerObject2):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
  }
  catch (RTIinternalError& e) {
    MessageBox::Show("MSG-(RTIinternalError - registerObject2):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (System::Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - registerObject2):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
  }
#pragma endregion
};

#pragma endregion	// Object Management

#pragma region Data Distribution Management
#pragma endregion // Data Distribution Management

#pragma region Time Management
#pragma endregion	// Time Management		

#pragma region Support Services

// getObjectClassHandle
rti1516e::ObjectClassHandle RtiAmb_OpenRti_1516e::getObjectClassHandle(HlaObjectClass ^oc) {
  try {
    pin_ptr<const wchar_t> name = PtrToStringChars(oc->Name);
    rti1516e::ObjectClassHandle handle = rti->getObjectClassHandle(name);// Get Object ClassHandle
    oc->Handle = toULong(gcnew String(handle.toString().c_str()));// Update Rom
    // Prevent duplicate inserts
    if (om->ObjectClasses.find(oc->Handle) == om->ObjectClasses.end()) // if not found
      om->ObjectClasses[oc->Handle] = handle;// Update Nom
    return handle;
  }
#pragma region exceptions
  catch (NameNotFound& e) {
    String^ msg = "MSG-(NameNotFound - getObjectClassHandle):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
  }
  catch (FederateNotExecutionMember& e) {
    String^ msg = "MSG-(FederateNotExecutionMember - getObjectClassHandle):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
  }
  catch (NotConnected& e) {
    String^ msg = "MSG-(NotConnected - getObjectClassHandle):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
  }
  catch (RTIinternalError& e) {
    MessageBox::Show("MSG-(RTIinternalError - getObjectClassHandle):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (System::Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - getObjectClassHandle):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
  }
#pragma endregion
};

// getInteractionClassHandle
rti1516e::InteractionClassHandle RtiAmb_OpenRti_1516e::getInteractionClassHandle(HlaInteractionClass ^ic) {
  try {
    pin_ptr<const wchar_t> name = PtrToStringChars(ic->Name);
    rti1516e::InteractionClassHandle handle = rti->getInteractionClassHandle(name);
    ic->Handle = toULong(gcnew String(handle.toString().c_str()));// Update Rom
    // Prevent duplicate inserts
    if (om->InteractionClasses.find(ic->Handle) == om->InteractionClasses.end()) // if not found
      om->InteractionClasses[ic->Handle] = handle;// Update Nom
    return handle;
  }
#pragma region exceptions
  catch (NameNotFound& e) {
    String^ msg = "MSG-(NameNotFound - getObjectClassHandle):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
  }
  catch (FederateNotExecutionMember& e) {
    String^ msg = "MSG-(FederateNotExecutionMember - getObjectClassHandle):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
  }
  catch (NotConnected& e) {
    String^ msg = "MSG-(NotConnected - getObjectClassHandle):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
  }
  catch (RTIinternalError& e) {
    MessageBox::Show("MSG-(RTIinternalError - getObjectClassHandle):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (System::Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - getObjectClassHandle):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
  }
#pragma endregion
};

// getAttributeHandle
rti1516e::AttributeHandle RtiAmb_OpenRti_1516e::getAttributeHandle(HlaAttribute^ attr, rti1516e::ObjectClassHandle ohandle) {
  try {
    pin_ptr<const wchar_t> aname = PtrToStringChars(attr->Name);
    rti1516e::AttributeHandle handle = rti->getAttributeHandle(ohandle, aname);// Get Handle
    attr->Handle = toULong(gcnew String(handle.toString().c_str()));// Update Rom
    // Prevent duplicate inserts
    if (om->Attributes.find(attr->Handle) == om->Attributes.end()) // if not found
      om->Attributes[attr->Handle] = handle;// Update Nom
    return handle;
  }
#pragma region exceptions
  catch (NameNotFound& e) {
    String^ msg = "MSG-(NameNotFound - getAttributeHandle):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
  }
  catch (InvalidObjectClassHandle& e) {
    String^ msg = "MSG-(InvalidObjectClassHandle - getAttributeHandle):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
  }
  catch (FederateNotExecutionMember& e) {
    String^ msg = "MSG-(FederateNotExecutionMember - getAttributeHandle):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
  }
  catch (NotConnected& e) {
    String^ msg = "MSG-(NotConnected - getAttributeHandle):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
  }
  catch (RTIinternalError& e) {
    MessageBox::Show("MSG-(RTIinternalError - getAttributeHandle):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (System::Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - getAttributeHandle):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
  }
#pragma endregion
};

// getParameterHandle
rti1516e::ParameterHandle RtiAmb_OpenRti_1516e::getParameterHandle(HlaParameter^ prm, rti1516e::InteractionClassHandle ihandle) {
  try {
    pin_ptr<const wchar_t> aname = PtrToStringChars(prm->Name);
    rti1516e::ParameterHandle handle = rti->getParameterHandle(ihandle, aname);// Get Handle
    prm->Handle = toULong(gcnew String(handle.toString().c_str())); // Update Rom
    // Prevent duplicate inserts
    if (om->Parameters.find(prm->Handle) == om->Parameters.end()) // if not found
      om->Parameters[prm->Handle] = handle;// Update Nom
    return handle;
  }
#pragma region exceptions
  catch (NameNotFound& e) {
    String^ msg = "MSG-(NameNotFound - getParameterHandle):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
  }
  catch (InvalidInteractionClassHandle& e) {
    String^ msg = "MSG-(InvalidInteractionClassHandle - getParameterHandle):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
  }
  catch (FederateNotExecutionMember& e) {
    String^ msg = "MSG-(FederateNotExecutionMember - getParameterHandle):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
  }
  catch (NotConnected& e) {
    String^ msg = "MSG-(NotConnected - getParameterHandle):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
  }
  catch (RTIinternalError& e) {
    MessageBox::Show("MSG-(RTIinternalError - getParameterHandle):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (System::Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - getParameterHandle):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
  }
#pragma endregion
};

// getParameterHandlesFromRti
void RtiAmb_OpenRti_1516e::getParameterHandlesFromRti(HlaInteractionClass ^ic) {
  for each (HlaParameter^ var in ic->Parameters)
    getParameterHandle(var, om->InteractionClasses[ic->Handle]);
};

// getClassHandleFromRti
void RtiAmb_OpenRti_1516e::getClassHandleFromRti(HlaObjectClass ^oc) {
  getObjectClassHandle(oc);
};
void RtiAmb_OpenRti_1516e::getClassHandleFromRti(HlaInteractionClass ^ic) {
  getInteractionClassHandle(ic);
};

// getAttributeHandlesFromRti
void RtiAmb_OpenRti_1516e::getAttributeHandlesFromRti(HlaObjectClass ^oc) {
  for each (HlaAttribute^ var in oc->Attributes)
    getAttributeHandle(var, om->ObjectClasses[oc->Handle]);
};

#pragma endregion

#pragma region Helpers
#pragma endregion // Helpers			
