// ******************************************************************************
//                          RtiAmb_OpenRti_1516e.h
//		begin                : Feb 03, 2016
// ******************************************************************************

/*
Racon - RTI abstraction component for MS.NET (Racon)
https://sites.google.com/site/okantopcu/racon

Copyright © Okan Topçu, 2009-2022
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
#include "RtiAmb_Hla1516e.h"

using namespace rti1516e;


namespace Racon
{
  namespace RtiLayer {
    namespace Native {

      public ref class RtiAmb_OpenRti_1516e : public RtiAmb_Hla1516e {

#pragma region Fields
#pragma endregion			

#pragma region Constructors
      public:
        RtiAmb_OpenRti_1516e(CallbackManager^ eventManager) : RtiAmb_Hla1516e(eventManager) {
          try {
            RtiVersion = gcnew String(rti1516e::rtiName().c_str()) + " v" + gcnew String(rti1516e::rtiVersion().c_str()) + " (OpenRTI-0.10.0 win64)";// rtiVersion() returns 1.0
          }
          catch (System::Exception^ e) {
            String^ msg = "RtiAmb_OpenRti_1516e-(Exception - Ctor). The native rti ambassador pointer creation  is failed. This may indicate a corrupted/a wrong RTI.rid file. Please ensure that RTI.rid is provided by Portico distribution. Reason: " + gcnew String(e->ToString());
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
          }
        };
#pragma endregion			

#pragma region Destructors
        ~RtiAmb_OpenRti_1516e()
        {
          this->!RtiAmb_OpenRti_1516e();
        };
        !RtiAmb_OpenRti_1516e()
        {
          delete rti;
          rti = 0;
          delete _nativeFdAmb;
        };
#pragma endregion						

#pragma region Methods
#pragma region Fedaration Management
      public:
        // 4.2 - Connect
        void connect(Racon::CallbackModel callbackModel, String ^localSetting) override {
          try
          {
            pin_ptr<const wchar_t> addr = PtrToStringChars(localSetting);
            // Only HLA_EVOKED is supported
            rti->connect(*this->_nativeFdAmb, rti1516e::HLA_EVOKED, addr);
            String^ msg = "Federate application is connected. The callback model is EVOKED (the only model supported by OpenRTI.)";
            if (localSetting != "")
              msg += " The connection protocol and the network address of the RTI server is " + gcnew String(addr) + ".";
            else
            {
              msg += "the connection protocol is THREAD.";
            }
            this->OnFederateConnected(gcnew RaconEventArgs(msg, LogLevel::INFO));
          }
#pragma region exceptions
          catch (ConnectionFailed& e) {
            String^ msg = "RtiAmb_OpenRti_1516e-(ConnectionFailed - connect). Check that rti (e.g. rtinode for OpenRTI) is running. Reason: " + gcnew String(e.what().c_str());
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
          }
          catch (InvalidLocalSettingsDesignator& e) {
            String^ msg = "RtiAmb_OpenRti_1516e-(InvalidLocalSettingsDesignator - connect). Reason: " + gcnew String(e.what().c_str());
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
          }
          catch (UnsupportedCallbackModel& e) {
            String^ msg = "RtiAmb_OpenRti_1516e-(UnsupportedCallbackModel - connect). Reason: " + gcnew String(e.what().c_str());
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
          }
          catch (AlreadyConnected& e) {
            String^ msg = "RtiAmb_OpenRti_1516e-(AlreadyConnected - connect). Reason: " + gcnew String(e.what().c_str());
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
          }
          catch (CallNotAllowedFromWithinCallback& e) {
            String^ msg = "RtiAmb_OpenRti_1516e-(CallNotAllowedFromWithinCallback - connect). Reason: " + gcnew String(e.what().c_str());
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
          }
          catch (RTIinternalError& e) {
            String^ msg = "RtiAmb_OpenRti_1516e-(RTIinternalError - connect). Reason: " + gcnew String(e.what().c_str());
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
          }
          catch (System::Exception^ e) {
            String^ msg = "RtiAmb_OpenRti_1516e-(Exception - connect). Reason: " + gcnew String(e->ToString());
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
          }
#pragma endregion
        };
#pragma endregion			

#pragma region Declaration Management
      public:
#pragma endregion			

#pragma region Object Management
      public:
        // 6.8 - Register Object
        bool registerObject(HlaObject ^obj) override {
          try {
            rti1516e::ObjectClassHandle chandle = nom->ObjectClasses[obj->Type->Handle];// get handle pointer
                                                                                       // register
            rti1516e::ObjectInstanceHandle ohandle = rti->registerObjectInstance(chandle); // register

            obj->Handle = toULong(gcnew String(ohandle.toString().c_str()));
            nom->Objects[obj->Handle] = ohandle; // update Nom
                                                // report
            String^ msg = "Object (class name: " + obj->Type->Name + ", class handle: " + obj->Type->Handle + ") is registered. Object handle: " + Handle2Long(ohandle).ToString();
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
            return true;
          }
#pragma region exceptions
          catch (ObjectClassNotDefined& e) {
            String^ msg = "MSG-(ObjectClassNotDefined - registerObject):" + " Reason: " + gcnew String(e.what().c_str());
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
            return false;
          }
          catch (ObjectClassNotPublished& e) {
            String^ msg = "MSG-(ObjectClassNotPublished - registerObject):" + " Reason: " + gcnew String(e.what().c_str());
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
            return false;
          }
          catch (FederateNotExecutionMember& e) {
            String^ msg = "MSG-(FederateNotExecutionMember - registerObject):" + " Reason: " + gcnew String(e.what().c_str());
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
            return false;
          }
          catch (SaveInProgress& e) {
            String^ msg = "MSG-(SaveInProgress - registerObject):" + " Reason: " + gcnew String(e.what().c_str());
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
            return false;
          }
          catch (RestoreInProgress& e) {
            String^ msg = "MSG-(RestoreInProgress - registerObject):" + " Reason: " + gcnew String(e.what().c_str());
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
            return false;
          }
          catch (NotConnected& e) {
            String^ msg = "MSG-(NotConnected - registerObject):" + " Reason: " + gcnew String(e.what().c_str());
						this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
						return false;
          }
          catch (RTIinternalError& e) {
            String^ msg = "RtiAmb_Hla13-(RTIinternalError - registerObject). Reason: " + gcnew String(e.what().c_str());
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
            return false;
          }
          catch (System::Exception^ e) {
            String^ msg = "RtiAmb_OpenRti_1516e-(Exception - registerObject). Reason: " + gcnew String(e->ToString());
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
            return false;
          }
#pragma endregion
        };
        // 6.8 - Register Object with a name
        bool registerObject(HlaObject ^obj, String^ name) override {
          try {
            rti1516e::ObjectClassHandle handle = nom->ObjectClasses[obj->Type->Handle];// get handle pointer
            pin_ptr<const wchar_t> oname = PtrToStringChars(name);
            rti1516e::ObjectInstanceHandle ohandle = rti->registerObjectInstance(handle, oname); // register
            obj->Handle = toULong(gcnew String(ohandle.toString().c_str()));
            nom->Objects[obj->Handle] = ohandle; // update Nom
            String^ msg = "Object (class name: " + obj->Type->Name + ", object name: " + name + ") is registered. Object handle: " + Handle2Long(ohandle).ToString();
            this->OnObjectRegistered(gcnew RaconEventArgs(msg));
            return true;
          }
#pragma region exceptions
          catch (ObjectClassNotDefined& e) {
            String^ msg = "MSG-(ObjectClassNotDefined - registerObject2):" + " Reason: " + gcnew String(e.what().c_str());
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
            return false;
          }
          catch (ObjectClassNotPublished& e) {
            String^ msg = "MSG-(ObjectClassNotPublished - registerObject2):" + " Reason: " + gcnew String(e.what().c_str());
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
            return false;
          }
          catch (ObjectInstanceNameNotReserved& e) {
            String^ msg = "MSG-(ObjectInstanceNameNotReserved - registerObject2):" + " Reason: " + gcnew String(e.what().c_str());
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
            return false;
          }
          catch (ObjectInstanceNameInUse& e) {
            String^ msg = "MSG-(ObjectInstanceNameInUse - registerObject2):" + " Reason: " + gcnew String(e.what().c_str());
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
            return false;
          }
          catch (FederateNotExecutionMember& e) {
            String^ msg = "MSG-(FederateNotExecutionMember - registerObject2):" + " Reason: " + gcnew String(e.what().c_str());
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
            return false;
          }
          catch (SaveInProgress& e) {
            String^ msg = "MSG-(SaveInProgress - registerObject2):" + " Reason: " + gcnew String(e.what().c_str());
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
            return false;
          }
          catch (RestoreInProgress& e) {
            String^ msg = "MSG-(RestoreInProgress - registerObject2):" + " Reason: " + gcnew String(e.what().c_str());
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
            return false;
          }
          catch (NotConnected& e) {
            String^ msg = "MSG-(NotConnected - registerObject2):" + " Reason: " + gcnew String(e.what().c_str());
						this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
						return false;
          }
          catch (RTIinternalError& e) {
            String^ msg = "RtiAmb_Hla13-(RTIinternalError - registerObject2). Reason: " + gcnew String(e.what().c_str());
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
            return false;
          }
          catch (System::Exception^ e) {
            String^ msg = "RtiAmb_OpenRti_1516e-(Exception - registerObject2). Reason: " + gcnew String(e->ToString());
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
            return false;
          }
#pragma endregion
        };
#pragma endregion

#pragma region Ownership Management
      public:
#pragma endregion 

#pragma region Data Distribution Management
      public:
#pragma endregion	

#pragma region Time Management
      public:
#pragma endregion

#pragma region Support Services
      public:
        // 10.4 - getFederateHandle
        unsigned int getFederateHandle(String ^federateName) override {
          try {
            pin_ptr<const wchar_t> name = PtrToStringChars(federateName);
            rti1516e::FederateHandle handle = rti->getFederateHandle(name);
            unsigned int fdHandle = Handle2Long(handle);
            String^ msg = "Federate handle for federate (" + federateName + ") is queried. Federate Handle: " + fdHandle;
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
            return fdHandle;
          }
#pragma region exceptions
          catch (NameNotFound& e) {
            String^ msg = "RtiAmb_OpenRti_1516e-(NameNotFound - getFederateHandle): Reason: " + gcnew String(e.what().c_str());
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
          }
          catch (FederateNotExecutionMember& e) {
            String^ msg = "RtiAmb_OpenRti_1516e-(FederateNotExecutionMember - getFederateHandle): Reason: " + gcnew String(e.what().c_str());
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
          }
          catch (NotConnected& e) {
            String^ msg = "RtiAmb_OpenRti_1516e-(NotConnected - getFederateHandle): Reason: " + gcnew String(e.what().c_str());
						this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
					}
          catch (RTIinternalError& e) {
            String^ msg = "RtiAmb_OpenRti_1516e-(RTIinternalError - getFederateHandle). Reason: " + gcnew String(e.what().c_str());
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
          }
          catch (System::Exception^ e) {
            String^ msg = "RtiAmb_OpenRti_1516e-(Exception - getFederateHandle). Reason: " + gcnew String(e->ToString());
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
          }
#pragma endregion
        };

        // 10.5 - getFederateName
        String^ getFederateName(unsigned int federateHandle) override {
          try {
            // Convert Federate Handle
            std::wstring name = rti->getFederateName(nom->FederateHandle);
            String ^fdName = gcnew String(name.c_str());
            String^ msg = "Federate name is requested for the federate id: " + federateHandle + ". Federate Name: " + fdName;
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
            return fdName;
          }
#pragma region exceptions
          catch (InvalidFederateHandle & e) {
            String^ msg = "RtiAmb_OpenRti_1516e-(InvalidFederateHandle - getFederateName): Reason: " + gcnew String(e.what().c_str());
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
          }
          catch (FederateHandleNotKnown & e) {
            String^ msg = "RtiAmb_OpenRti_1516e-(FederateHandleNotKnown - getFederateName): Reason: " + gcnew String(e.what().c_str());
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
          }
          catch (NotConnected& e) {
            String^ msg = "RtiAmb_OpenRti_1516e-(NotConnected - getFederateName): Reason: " + gcnew String(e.what().c_str());
						this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
					}
          catch (RTIinternalError& e) {
            String^ msg = "RtiAmb_OpenRti_1516e-(RTIinternalError - getFederateName). Reason: " + gcnew String(e.what().c_str());
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
          }
          catch (System::Exception^ e) {
            String^ msg = "RtiAmb_OpenRti_1516e-(Exception - getFederateName). Reason: " + gcnew String(e->ToString());
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
          }
#pragma endregion
        };

				// 10.31 - normalizeFederateHandle
				unsigned int normalizeFederateHandle(unsigned int federateHandle) override {
					try {
						// Convert Federate Handle
						unsigned int nHandle = rti->normalizeFederateHandle(nom->FederateHandle);
						String^ msg = "Normalized federate handle for federate (" + federateHandle + ") is: " + nHandle;
						this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
						return nHandle;
					}
#pragma region exceptions
					catch (NameNotFound & e) {
						String^ msg = "RtiAmb_OpenRti_1516e-(NameNotFound - getFederateName): Reason: " + gcnew String(e.what().c_str());
						this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
					}
					catch (FederateNotExecutionMember & e) {
						String^ msg = "RtiAmb_OpenRti_1516e-(FederateNotExecutionMember - getFederateName): Reason: " + gcnew String(e.what().c_str());
						this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
					}
					catch (NotConnected & e) {
						String^ msg = "RtiAmb_OpenRti_1516e-(NotConnected - getFederateName): Reason: " + gcnew String(e.what().c_str());
						this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
					}
					catch (RTIinternalError & e) {
						String^ msg = "RtiAmb_OpenRti_1516e-(RTIinternalError - getFederateName). Reason: " + gcnew String(e.what().c_str());
						this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
					}
					catch (System::Exception ^ e) {
						String^ msg = "RtiAmb_OpenRti_1516e-(Exception - getFederateName). Reason: " + gcnew String(e->ToString());
						this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
					}
#pragma endregion
				};
				
				// getObjectClassHandle
        rti1516e::ObjectClassHandle getObjectClassHandle(HlaObjectClass ^oc) override {
          try {
            pin_ptr<const wchar_t> name = PtrToStringChars(oc->Name);
            rti1516e::ObjectClassHandle handle = rti->getObjectClassHandle(name);// Get Object ClassHandle
            oc->Handle = toULong(gcnew String(handle.toString().c_str()));// Update Rom
                                                                          // Prevent duplicate inserts
            if (nom->ObjectClasses.find(oc->Handle) == nom->ObjectClasses.end()) // if not found
              nom->ObjectClasses[oc->Handle] = handle;// Update Nom
            return handle;
          }
#pragma region exceptions
          catch (NameNotFound& e) {
            String^ msg = "MSG-(NameNotFound - getObjectClassHandle): Reason: " + gcnew String(e.what().c_str());
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
          }
          catch (FederateNotExecutionMember& e) {
            String^ msg = "MSG-(FederateNotExecutionMember - getObjectClassHandle): Reason: " + gcnew String(e.what().c_str());
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
          }
          catch (NotConnected& e) {
            String^ msg = "MSG-(NotConnected - getObjectClassHandle): Reason: " + gcnew String(e.what().c_str());
						this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
					}
          catch (RTIinternalError& e) {
            String^ msg = "RtiAmb_Hla13-(RTIinternalError - getObjectClassHandle). Reason: " + gcnew String(e.what().c_str());
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
          }
          catch (System::Exception^ e) {
            String^ msg = "RtiAmb_OpenRti_1516e-(Exception - getObjectClassHandle). Reason: " + gcnew String(e->ToString());
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
          }
#pragma endregion
        };

        // getInteractionClassHandle
        rti1516e::InteractionClassHandle getInteractionClassHandle(HlaInteractionClass ^ic) override {
          try {
            pin_ptr<const wchar_t> name = PtrToStringChars(ic->Name);
            rti1516e::InteractionClassHandle handle = rti->getInteractionClassHandle(name);
            ic->Handle = toULong(gcnew String(handle.toString().c_str()));// Update Rom
                                                                          // Prevent duplicate inserts
            if (nom->InteractionClasses.find(ic->Handle) == nom->InteractionClasses.end()) // if not found
              nom->InteractionClasses[ic->Handle] = handle;// Update Nom
            return handle;
          }
#pragma region exceptions
          catch (NameNotFound& e) {
            String^ msg = "MSG-(NameNotFound - getObjectClassHandle): Reason: " + gcnew String(e.what().c_str());
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
          }
          catch (FederateNotExecutionMember& e) {
            String^ msg = "MSG-(FederateNotExecutionMember - getObjectClassHandle): Reason: " + gcnew String(e.what().c_str());
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
          }
          catch (NotConnected& e) {
            String^ msg = "MSG-(NotConnected - getObjectClassHandle): Reason: " + gcnew String(e.what().c_str());
						this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
					}
          catch (RTIinternalError& e) {
            String^ msg = "RtiAmb_Hla13-(RTIinternalError - getObjectClassHandle). Reason: " + gcnew String(e.what().c_str());
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
          }
          catch (System::Exception^ e) {
            String^ msg = "RtiAmb_OpenRti_1516e-(Exception - getObjectClassHandle). Reason: " + gcnew String(e->ToString());
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
          }
#pragma endregion
        };

        // getAttributeHandle
        rti1516e::AttributeHandle getAttributeHandle(HlaAttribute^ attr, rti1516e::ObjectClassHandle ohandle) override {
          try {
            pin_ptr<const wchar_t> aname = PtrToStringChars(attr->Name);
            rti1516e::AttributeHandle handle = rti->getAttributeHandle(ohandle, aname);// Get Handle
            attr->Handle = toULong(gcnew String(handle.toString().c_str()));// Update Rom
                                                                            // Prevent duplicate inserts
            if (nom->Attributes.find(attr->Handle) == nom->Attributes.end()) // if not found
              nom->Attributes[attr->Handle] = handle;// Update Nom
            return handle;
          }
#pragma region exceptions
          catch (NameNotFound& e) {
            String^ msg = "MSG-(NameNotFound - getAttributeHandle): Reason: " + gcnew String(e.what().c_str());
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
          }
          catch (InvalidObjectClassHandle& e) {
            String^ msg = "MSG-(InvalidObjectClassHandle - getAttributeHandle):" + " Reason: " + gcnew String(e.what().c_str());
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
          }
          catch (FederateNotExecutionMember& e) {
            String^ msg = "MSG-(FederateNotExecutionMember - getAttributeHandle):" + " Reason: " + gcnew String(e.what().c_str());
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
          }
          catch (NotConnected& e) {
            String^ msg = "MSG-(NotConnected - getAttributeHandle):" + " Reason: " + gcnew String(e.what().c_str());
						this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
					}
          catch (RTIinternalError& e) {
            String^ msg = "RtiAmb_Hla13-(RTIinternalError - getAttributeHandle). Reason: " + gcnew String(e.what().c_str());
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
          }
          catch (System::Exception^ e) {
            String^ msg = "RtiAmb_OpenRti_1516e-(Exception - getAttributeHandle). Reason: " + gcnew String(e->ToString());
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
          }
#pragma endregion
        };

        // getParameterHandle
        rti1516e::ParameterHandle getParameterHandle(HlaParameter^ prm, rti1516e::InteractionClassHandle ihandle) override {
          try {
            pin_ptr<const wchar_t> aname = PtrToStringChars(prm->Name);
            rti1516e::ParameterHandle handle = rti->getParameterHandle(ihandle, aname);// Get Handle
            prm->Handle = toULong(gcnew String(handle.toString().c_str())); // Update Rom
            // Prevent duplicate inserts
            if (nom->Parameters.find(prm->Handle) == nom->Parameters.end()) // if not found
              nom->Parameters[prm->Handle] = handle;// Update Nom
            return handle;
          }
#pragma region exceptions
          catch (NameNotFound& e) {
            String^ msg = "MSG-(NameNotFound - getParameterHandle):" + " Reason: " + gcnew String(e.what().c_str());
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
          }
          catch (InvalidInteractionClassHandle& e) {
            String^ msg = "MSG-(InvalidInteractionClassHandle - getParameterHandle):" + " Reason: " + gcnew String(e.what().c_str());
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
          }
          catch (FederateNotExecutionMember& e) {
            String^ msg = "MSG-(FederateNotExecutionMember - getParameterHandle):" + " Reason: " + gcnew String(e.what().c_str());
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
          }
          catch (NotConnected& e) {
            String^ msg = "MSG-(NotConnected - getParameterHandle):" + " Reason: " + gcnew String(e.what().c_str());
						this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
					}
          catch (RTIinternalError& e) {
            String^ msg = "RtiAmb_Hla13-(RTIinternalError - getParameterHandle). Reason: " + gcnew String(e.what().c_str());
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
          }
          catch (System::Exception^ e) {
            String^ msg = "RtiAmb_OpenRti_1516e-(Exception - getParameterHandle). Reason: " + gcnew String(e->ToString());
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
          }
#pragma endregion
        };

        // 10.25 - getDimensionHandle
        unsigned int getDimensionHandle(String ^name) override {
          try {
            pin_ptr<const wchar_t> _name = PtrToStringChars(name);
            rti1516e::DimensionHandle _handle = rti->getDimensionHandle(_name);
            unsigned int handle = Handle2Long(_handle);
            // Prevent duplicate inserts
            if (nom->Dimensions.find(handle) == nom->Dimensions.end()) // if not found
              nom->Dimensions[handle] = _handle;// Update Nom
            String^ msg = "Dimension handle for (name: " + name + "): " + handle;
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
            return handle;
          }
#pragma region exceptions
          catch (NameNotFound& e) {
            String^ msg = "RtiAmb_OpenRti_1516e-(NameNotFound - getDimensionHandle): Reason: " + gcnew String(e.what().c_str());
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
          }
          catch (FederateNotExecutionMember& e) {
            String^ msg = "RtiAmb_OpenRti_1516e-(FederateNotExecutionMember - getDimensionHandle): Reason: " + gcnew String(e.what().c_str());
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
          }
          catch (NotConnected& e) {
            String^ msg = "RtiAmb_OpenRti_1516e-(NotConnected - getDimensionHandle): Reason: " + gcnew String(e.what().c_str());
						this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
					}
          catch (RTIinternalError& e) {
            String^ msg = "RtiAmb_OpenRti_1516e-(RTIinternalError - getDimensionHandle). Reason: " + gcnew String(e.what().c_str());
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
          }
          catch (System::Exception^ e) {
            String^ msg = "RtiAmb_OpenRti_1516e-(Exception - getDimensionHandle). Reason: " + gcnew String(e->ToString());
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
          }
#pragma endregion
        };

        // 10.26 - getDimensionName
        String^  getDimensionName(unsigned int handle) override {
          try {
            // Convert handle
            std::wstring _name = rti->getDimensionName(nom->Dimensions[handle]);
            String ^name = gcnew String(_name.c_str());
            String^ msg = "Dimension name for (handle: " + handle + "): " + name;
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
            return name;
          }
#pragma region exceptions
          catch (NameNotFound& e) {
            String^ msg = "RtiAmb_OpenRti_1516e-(InvalidDimensionHandle - getDimensionName): Reason: " + gcnew String(e.what().c_str());
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
          }
          catch (FederateNotExecutionMember& e) {
            String^ msg = "RtiAmb_OpenRti_1516e-(FederateNotExecutionMember - getDimensionName): Reason: " + gcnew String(e.what().c_str());
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
          }
          catch (NotConnected& e) {
            String^ msg = "RtiAmb_OpenRti_1516e-(NotConnected - getDimensionName): Reason: " + gcnew String(e.what().c_str());
						this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
					}
          catch (RTIinternalError& e) {
            String^ msg = "RtiAmb_OpenRti_1516e-(RTIinternalError - getDimensionName). Reason: " + gcnew String(e.what().c_str());
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
          }
          catch (System::Exception^ e) {
            String^ msg = "RtiAmb_OpenRti_1516e-(Exception - getDimensionName). Reason: " + gcnew String(e->ToString());
            this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
          }
#pragma endregion
        };

        // getParameterHandlesFromRti
        void getParameterHandlesFromRti(HlaInteractionClass ^ic)override {
          for each (HlaParameter^ var in ic->Parameters)
            getParameterHandle(var, nom->InteractionClasses[ic->Handle]);
        };

        // getClassHandleFromRti
        void getClassHandleFromRti(HlaObjectClass ^oc) override {
          getObjectClassHandle(oc);
        };
        void getClassHandleFromRti(HlaInteractionClass ^ic)override {
          getInteractionClassHandle(ic);
        };

        // getAttributeHandlesFromRti
        void getAttributeHandlesFromRti(HlaObjectClass ^oc) override {
          for each (HlaAttribute^ var in oc->Attributes)
            getAttributeHandle(var, nom->ObjectClasses[oc->Handle]);
        };
#pragma endregion

#pragma region Helpers
      protected:
#pragma endregion

#pragma endregion

      };
    }
  }
}

