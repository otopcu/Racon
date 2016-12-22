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
#include "RtiAmb_Hla1516e.h"
#include <assert.h>

using namespace rti1516e;
using namespace System;
using namespace System::Runtime::InteropServices;// For Marshalling
//using namespace System::Windows::Forms;
using namespace System::Threading; // for sleep
// Racon
using namespace Racon::RtiLayer;
using namespace Racon::RtiLayer::Native;

#pragma region Constructor
RtiAmb_Hla1516e::RtiAmb_Hla1516e(CallbackManager^ eventManager) : CRtiAmb() {
	try {
		RTIambassadorFactory factory = RTIambassadorFactory();
		this->rti = factory.createRTIambassador().release();
		om = new OmHla1516e();
		_nativeFdAmb = new FdAmb_Hla1516e(eventManager, om);
	}
	catch (System::Exception^ e) {
		MessageBox::Show("MSG-(RtiAmb_Hla1516e Ctor):" + Environment::NewLine + gcnew String(e->Message) + Environment::NewLine + "The native rti ambassador pointer creation  is failed. This may indicate a corrupted/a wrong RTI.rid file. Please ensure that RTI.rid is provided by Portico distribution" + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
};
#pragma endregion			

#pragma region Federation Management
// 4.2 - Connect
void RtiAmb_Hla1516e::connect(String ^localSetting) {
	try
	{
		pin_ptr<const wchar_t> addr = PtrToStringChars(localSetting);

		rti->connect(*this->_nativeFdAmb, HLA_EVOKED, addr); // Only HLA_EVOKED is supported
		String^ msg = "Federate application is connected.";
		if (localSetting != "")
			msg += " The connection protocol and the network address of the RTI server is " + gcnew String(addr) + ".";
		else
		{
			msg += "the connection protocol is THREAD.";
		}
		this->OnFederateConnected(gcnew RaconEventArgs(msg));
	}
#pragma region exceptions
	catch (ConnectionFailed& e) {
		String^ msg = "MSG-(ConnectionFailed - connect):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (InvalidLocalSettingsDesignator& e) {
		String^ msg = "MSG-(InvalidLocalSettingsDesignator - connect):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (UnsupportedCallbackModel& e) {
		String^ msg = "MSG-(UnsupportedCallbackModel - connect):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (AlreadyConnected& e) {
		String^ msg = "MSG-(AlreadyConnected - connect):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (CallNotAllowedFromWithinCallback& e) {
		String^ msg = "MSG-(CallNotAllowedFromWithinCallback - connect):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RTIinternalError& e) {
		MessageBox::Show("MSG-(RTIinternalError - connect):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	catch (System::Exception^ e) {
		MessageBox::Show("MSG-(GeneralException - connect):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
#pragma endregion
};

// 4.3 - Disconnect
void RtiAmb_Hla1516e::disconnect() {
	try
	{
		rti->disconnect();
		String^ msg = "Federate application is disconnected.";
		this->OnFederateDisconnected(gcnew RaconEventArgs(msg));
	}
#pragma region exceptions
	catch (FederateIsExecutionMember& e) {
		String^ msg = "MSG-(FederateIsExecutionMember - disconnect):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (NotConnected& e) {
		String^ msg = "MSG-(NotConnected - disconnect):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnNotConnected(gcnew RaconEventArgs(msg));
	}
	catch (RTIinternalError& e) {
		MessageBox::Show("MSG-(RTIinternalError - disconnect):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	catch (System::Exception^ e) {
		MessageBox::Show("MSG-(GeneralException - disconnect):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
#pragma endregion
};

// 4.5 - Create Federation
void RtiAmb_Hla1516e::createFederation(String ^fedexec, String ^fdd) {
	// !!! No support for FOM modules
	try {
		pin_ptr<const wchar_t> fedx = PtrToStringChars(fedexec);
		pin_ptr<const wchar_t> fed = PtrToStringChars(fdd);

		rti->createFederationExecution(fedx, fed);
		String^ msg = "Federation execution (name: " + fedexec + ") is created.";
		this->OnFederationExecutionCreated(gcnew RaconEventArgs(msg));
	}
#pragma region exceptions
	catch (CouldNotCreateLogicalTimeFactory& e) {
		String^ msg = "MSG-(CouldNotCreateLogicalTimeFactory - createFederation):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (ErrorReadingFDD& e) {
		MessageBox::Show("MSG-(ErrorReadingFDD - createFederation):" + Environment::NewLine + "Invalid FOM Module. Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
	catch (CouldNotOpenFDD& e) {
		MessageBox::Show("MSG-(CouldNotOpenFDD - createFederation):\r\n" + "Federate cannot open the FDD file. Either the file does not exist or there exists an error in the file name or path provided. Please, check that FDD file exists and its path is correct. Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
	catch (DesignatorIsHLAstandardMIM& e) {
		String^ msg = "MSG-(DesignatorIsHLAstandardMIM - createFederation):" + Environment::NewLine + "MIM designator shall not be HLAstandardMIM. Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (ErrorReadingMIM& e) {
		String^ msg = "MSG-(ErrorReadingMIM - createFederation):" + Environment::NewLine + "Invalid MIM. Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (CouldNotOpenMIM& e) {
		String^ msg = "MSG-(CouldNotOpenMIM - createFederation):" + Environment::NewLine + "Could not locate MIM indicated by supplied designator.. Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederationExecutionAlreadyExists&)
	{
		String^ msg = "Federation execution (name: " + fedexec + ") already exists.";
		this->OnFederationExecutionCreated(gcnew RaconEventArgs(msg));
	}
	catch (NotConnected& e) {
		String^ msg = "MSG-(NotConnected - createFederation):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg));
	}
	catch (RTIinternalError& e) {
		MessageBox::Show("MSG-(RTIinternalError - createFederation):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	catch (const rti1516e::Exception& e) {
		String^ msg = "MSG-(rti1516e::Exception: - createFederation):" + Environment::NewLine + "Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (System::Exception ^ e) {
		MessageBox::Show("MSG-(GeneralExceptionInCreateFederation):\r\n" + "System::Exception in <createFederationExecution()>. The Federate cannot create the federation." + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
#pragma endregion
};

// 4.6 - Destroy Federation
void RtiAmb_Hla1516e::destroyFederation(String ^fedexec) {
	try {
		pin_ptr<const wchar_t> fedx = PtrToStringChars(fedexec);
		rti->destroyFederationExecution(fedx);
		String^ msg = "Federation execution is finalized.";
		this->OnFederationExecutionDestroyed(gcnew RaconEventArgs(msg));
	}
#pragma region exceptions
	catch (FederatesCurrentlyJoined& e) {
		String^ msg = "There are still federates joined in the federation. Federation execution is not destroyed. " + "Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederationExecutionDoesNotExist& e) {
		String^ msg = "Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (NotConnected& e) {
		String^ msg = "MSG-(NotConnected - destroyFederation):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg));
	}
	catch (RTIinternalError& e) {
		MessageBox::Show("MSG-(RTIinternalError - destroyFederation):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	catch (System::Exception^ e) {
		MessageBox::Show("MSG-(GeneralException - destroyFederation):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
#pragma endregion
};

// 4.7 - listFederationExecutions
void RtiAmb_Hla1516e::listFederationExecutions() {
	try {
		String^ msg = "A list of currently existing federation executions is requested.";
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		rti->listFederationExecutions();
	}
#pragma region exceptions
	catch (NotConnected& e) {
		String^ msg = "MSG-(NotConnected - listFederationExecutions):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg));
	}
	catch (RTIinternalError& e) {
		MessageBox::Show("MSG-(RTIinternalError - listFederationExecutions):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	catch (System::Exception^ e) {
		MessageBox::Show("MSG-(GeneralException - listFederationExecutions):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
#pragma endregion
};

// 4.9 - Join Federation
unsigned int RtiAmb_Hla1516e::joinFederation(String ^fedexec, String ^fdName) {
	// !!! No support for additional FOM modules in join
	bool joined = false;
	int numTries = 0;
	while ((numTries++ < 20) && !joined) {
		try {
			pin_ptr<const wchar_t> fedx = PtrToStringChars(fedexec);
			pin_ptr<const wchar_t> fd = PtrToStringChars(fdName);

			rti1516e::FederateHandle federateHandle = rti->joinFederationExecution(fd, L"Federate Type (Not Implemented)", fedx);
			joined = true; 
			//String^ handle = gcnew String(federateHandle.toString().c_str());
			ULong handle = Handle2Long(federateHandle);
			String^ msg = "Federate (" + gcnew String(fd) + ") joined to the federation execution (" + gcnew String(fedx) + "). Federate Handle: " + handle.ToString();
			this->OnFederateJoined(gcnew RaconEventArgs(msg));
			return handle;
		}
#pragma region exceptions
		catch (CouldNotCreateLogicalTimeFactory& e) {
			String^ msg = "MSG-(CouldNotCreateLogicalTimeFactory - joinFederation):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
			this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		}
		catch (FederationExecutionDoesNotExist& e) {
			MessageBox::Show("MSG-(FederationExecutionDoesNotExist - joinFederation):" + Environment::NewLine + "Federation Execution does not exist. Please, restart the federate. Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
		}
		catch (ErrorReadingFDD& e) {
			MessageBox::Show("MSG-(ErrorReadingFED - joinFederation):" + Environment::NewLine + "Invalid FOM Module. Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
		}
		catch (InconsistentFDD& e) {
			MessageBox::Show("MSG-(InconsistentFDD - joinFederation):" + Environment::NewLine + "Inconsistent FOM Module. Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
		}
		catch (CouldNotOpenFDD& e) {
			MessageBox::Show("MSG-(CouldNotOpenFDD - joinFederation):" + Environment::NewLine + "Federate cannot open the FED file. Either the file does not exist or there exists an error in the file name or path provided. Please, check that FED file exists and its path is correct. Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
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
			MessageBox::Show("MSG-(FederateAlreadyExecutionMember - joinFederation):" + Environment::NewLine + "Federate already is an federation execution member." + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
		}
		catch (NotConnected& e) {
			String^ msg = "MSG-(NotConnected - joinFederation):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str());
			this->OnNotConnected(gcnew RaconEventArgs(msg));
		}
		catch (RTIinternalError& e) {
			MessageBox::Show("MSG-(RTIinternalError - joinFederation):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
		catch (System::Exception^ e) {
			MessageBox::Show("MSG-(GeneralException - joinFederation):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
		}
#pragma endregion
	} // end of while
};

// 4.10 - Resign Federation
void RtiAmb_Hla1516e::resignFederation(int resignAction) {
	String ^resign;
	try {
		switch (resignAction) {
		case 0: rti->resignFederationExecution(UNCONDITIONALLY_DIVEST_ATTRIBUTES);
			resign = "UNCONDITIONALLY_DIVEST_ATTRIBUTES";
			break;
		case 1: rti->resignFederationExecution(DELETE_OBJECTS);
			resign = "DELETE_OBJECTS";
			break;
		case 2: rti->resignFederationExecution(CANCEL_PENDING_OWNERSHIP_ACQUISITIONS);
			resign = "CANCEL_PENDING_OWNERSHIP_ACQUISITIONS";
			break;
		case 3: rti->resignFederationExecution(DELETE_OBJECTS_THEN_DIVEST);
			resign = "DELETE_OBJECTS_THEN_DIVEST";
			break;
		case 4: rti->resignFederationExecution(CANCEL_THEN_DELETE_THEN_DIVEST);
			resign = "CANCEL_THEN_DELETE_THEN_DIVEST";
			break;
		case 5: rti->resignFederationExecution(NO_ACTION);
			resign = "NO_ACTION";
			break;
		default: rti->resignFederationExecution(NO_ACTION);
			resign = "NO_ACTION";
			break;
		}
		String^ msg = "Federate resigned from the federation execution. Resign action is " + resign + ".";
		this->OnFederateResigned(gcnew RaconEventArgs(msg));
	}
#pragma region exceptions
	catch (InvalidResignAction& e) {
		String^ msg = "MSG-(InvalidResignAction - resignFederation):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (OwnershipAcquisitionPending& e) {
		String^ msg = "MSG-(OwnershipAcquisitionPending - resignFederation):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (AttributeAlreadyOwned& e) {
		MessageBox::Show("MSG-(AttributeAlreadyOwned - resignFederation):" + Environment::NewLine + "The joined federate owns instance attributes. Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
	catch (FederateNotExecutionMember& e) {
		MessageBox::Show("MSG-(FederateNotExecutionMember - resignFederation):" + Environment::NewLine + "Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
	catch (FederationExecutionDoesNotExist& e) {
		MessageBox::Show("MSG-(FederationExecutionDoesNotExist - resignFederation):" + Environment::NewLine + "Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
	catch (NotConnected& e) {
		String^ msg = "MSG-(NotConnected - resignFederation):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg));
	}
	catch (RTIinternalError& e) {
		MessageBox::Show("MSG-(RTIinternalError - resignFederation):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	catch (System::Exception^ e) {
		MessageBox::Show("MSG-(GeneralException - resignFederation):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
#pragma endregion
};

// 4.11 - registerFederationSynchronizationPoint
void RtiAmb_Hla1516e::registerFederationSynchronizationPoint(String^ synchronizationPointLabel, String^ userSuppliedTag) {
	try {
		pin_ptr<const wchar_t> label = PtrToStringChars(synchronizationPointLabel);
		char const * _pVal = (char*)Marshal::StringToHGlobalAnsi(userSuppliedTag).ToPointer();
		rti1516e::VariableLengthData tag(_pVal, ((strlen(_pVal) + 1) * sizeof(char)));
		rti->registerFederationSynchronizationPoint(label, tag);
		String^ msg = "Federate requested a register of a federation synchronization point. Label: " + synchronizationPointLabel + ". Tag: " + userSuppliedTag;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
#pragma region exceptions
	catch (SaveInProgress& e) {
		String^ msg = "MSG-(SaveInProgress - registerFederationSynchronizationPoint):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "MSG-(RestoreInProgress - registerFederationSynchronizationPoint):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "MSG-(FederateNotExecutionMember - registerFederationSynchronizationPoint):" + Environment::NewLine + "Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (NotConnected& e) {
		String^ msg = "MSG-(NotConnected - registerFederationSynchronizationPoint):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg));
	}
	catch (RTIinternalError& e) {
		MessageBox::Show("MSG-(RTIinternalError - registerFederationSynchronizationPoint):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	catch (System::Exception^ e) {
		MessageBox::Show("MSG-(GeneralException - registerFederationSynchronizationPoint):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
#pragma endregion
};
// 4.11 - registerFederationSynchronizationPoint with a set of federates
void RtiAmb_Hla1516e::registerFederationSynchronizationPoint(String^ synchronizationPointLabel, String^ userSuppliedTag, List<unsigned int>^ setOfJoinedFederateDesignators) {
	try {
		pin_ptr<const wchar_t> label = PtrToStringChars(synchronizationPointLabel);
		char const * _pVal = (char*)Marshal::StringToHGlobalAnsi(userSuppliedTag).ToPointer();
		rti1516e::VariableLengthData tag(_pVal, ((strlen(_pVal) + 1) * sizeof(char)));
		// !!! How to convert handle -> rti1516e::FederateHandle
		// !!! Error here. Needs to keep smt like
		// std::map<unsigned int, rti1516e::FederateHandle> Federates;
		// Which is not possible to populate this map currently
		// There is no way to get federate handles from federation execution, except MIM
		// MIM Federate objectclass can be subscribed to get of the list of federates at hand.

		rti1516e::FederateHandleSet federates;
		for (int i = 0; i < setOfJoinedFederateDesignators->Count; i++) {
			rti1516e::FederateHandle* handle = new rti1516e::FederateHandle();
			federates.insert(*handle);
		}

		rti->registerFederationSynchronizationPoint(label, tag, federates);
		String^ msg = "Federate requested a register of a federation synchronization point. Label: " + synchronizationPointLabel + ". Tag: " + userSuppliedTag;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
#pragma region exceptions
	catch (InvalidFederateHandle& e) {
		String^ msg = "MSG-(InvalidFederateHandle - registerFederationSynchronizationPoint2):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (SaveInProgress& e) {
		String^ msg = "MSG-(SaveInProgress - registerFederationSynchronizationPoint2):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "MSG-(RestoreInProgress - registerFederationSynchronizationPoint2):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "MSG-(FederateNotExecutionMember - registerFederationSynchronizationPoint2):" + Environment::NewLine + "Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (NotConnected& e) {
		String^ msg = "MSG-(NotConnected - registerFederationSynchronizationPoint2):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg));
	}
	catch (RTIinternalError& e) {
		MessageBox::Show("MSG-(RTIinternalError - registerFederationSynchronizationPoint2):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	catch (System::Exception^ e) {
		MessageBox::Show("MSG-(GeneralException - registerFederationSynchronizationPoint2):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
#pragma endregion
};

// 4.14 - synchronizationPointAchieved
void RtiAmb_Hla1516e::synchronizationPointAchieved(String^ synchronizationPointLabel, bool synchronizationSuccess) {
	try {
		pin_ptr<const wchar_t> label = PtrToStringChars(synchronizationPointLabel);
		rti->synchronizationPointAchieved(label, synchronizationSuccess);
		String^ msg = "Federation synchronization point is achieved. Label: " + synchronizationPointLabel + ". Success: " + synchronizationSuccess;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
#pragma region exceptions
	catch (SynchronizationPointLabelNotAnnounced& e) {
		String^ msg = "MSG-(SynchronizationPointLabelNotAnnounced - synchronizationPointAchieved):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (SaveInProgress& e) {
		String^ msg = "MSG-(SaveInProgress - synchronizationPointAchieved):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "MSG-(RestoreInProgress - synchronizationPointAchieved):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "MSG-(FederateNotExecutionMember - synchronizationPointAchieved):" + Environment::NewLine + "Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (NotConnected& e) {
		String^ msg = "MSG-(NotConnected - synchronizationPointAchieved):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg));
	}
	catch (RTIinternalError& e) {
		MessageBox::Show("MSG-(RTIinternalError - synchronizationPointAchieved):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	catch (System::Exception^ e) {
		MessageBox::Show("MSG-(GeneralException - synchronizationPointAchieved):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
#pragma endregion
};

// Request Federation Save - save as soon as possible
void RtiAmb_Hla1516e::requestFederationSave(String^ _label) {
};

// Request Federation Save with Time - save at the specified time
void RtiAmb_Hla1516e::requestFederationSave(String^ _label, Double time) {
};

// Federate Save Begun
void RtiAmb_Hla1516e::federateSaveBegun() {
};
// Federate Save Complete
void RtiAmb_Hla1516e::federateSaveComplete(bool _IsCompleted) {
};
// Request Federation Restore
void RtiAmb_Hla1516e::requestFederationRestore(String^ _label) {
};
// Federate Restore Complete
void RtiAmb_Hla1516e::federateRestoreComplete(bool _IsCompleted) {
};
#pragma endregion	// Federation Management

#pragma region Declaration Management

// 5.2 - publishOC
void RtiAmb_Hla1516e::publishObjectClass(HlaObjectClass ^oc, BindingList<HlaAttribute^>^ attributes)
{
	try {
		// !!! sifir attr set ile metodu cagirmak unpublish et anlamina geliyor. Eger bir class'in attr yoksa (yada attr var ancak PS'i N ise - privilegeToDelete) ve onu publish etmek istersek exception uretiyoruz.
		// Construct tmp list - according to the PS stat of the class
		List<HlaAttribute^> ^tmpList = gcnew List<HlaAttribute^>();
		for each (HlaAttribute^ attr in attributes) {
			if ((attr->AttributePS == PSKind::Publish) || (attr->AttributePS == PSKind::PublishSubscribe)) {
				tmpList->Add(attr);
			}
		}

		String^ tmpStr = "";
		rti1516e::ObjectClassHandle ocHandle = getObjectClassHandle(oc);
		rti1516e::AttributeHandleSet ahs;
		// Add attribute handles to AHS
		for (int j = 0; j < tmpList->Count; j++) {
			rti1516e::AttributeHandle handle = getAttributeHandle(tmpList[j], ocHandle);
			ahs.insert(handle);
			tmpStr += tmpList[j]->Name + " (" + Handle2Long(handle).ToString() + ")";
			if (j != tmpList->Count - 1) tmpStr += ", ";
		}

		rti->publishObjectClassAttributes(ocHandle, ahs); // *ahs:ordinary reference to native object

		String^ msg = "Object Class (" + oc->Name + ") is published with its all publishable attributes. Class handle: " + Handle2Long(ocHandle).ToString() + ", Attributes: {" + tmpStr + "}";
		this->OnHLAClassPublished(gcnew RaconEventArgs(msg));
	}
#pragma region exceptions
	catch (AttributeNotDefined& e) {
		String^ msg = "MSG-(AttributeNotDefined - publishObjectClass):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (ObjectClassNotDefined& e) {
		String^ msg = "MSG-(ObjectClassNotDefined - publishObjectClass):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (SaveInProgress& e) {
		String^ msg = "MSG-(SaveInProgress - publishObjectClass):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "MSG-(RestoreInProgress - publishObjectClass):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "MSG-(FederateNotExecutionMember - publishObjectClass):" + Environment::NewLine + "Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (NotConnected& e) {
		String^ msg = "MSG-(NotConnected - publishObjectClass):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg));
	}
	catch (RTIinternalError& e) {
		MessageBox::Show("MSG-(RTIinternalError - publishObjectClass):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	//catch (System::Exception^ e) {
	//	MessageBox::Show("MSG-(GeneralException - publishObjectClass):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	//}
#pragma endregion
};

// 5.4 - publishIC
void RtiAmb_Hla1516e::publishInteractionClass(HlaInteractionClass ^ic) {
	// Check that interaction is Publishable
	if ((ic->ClassPS == PSKind::Publish) || (ic->ClassPS == PSKind::PublishSubscribe))
		try {
		pin_ptr<const wchar_t> name = PtrToStringChars(ic->Name);
		rti1516e::InteractionClassHandle handle = getInteractionClassHandle(ic);
		rti->publishInteractionClass(handle);
		String^ msg = "Interaction Class (name: " + ic->Name + ", handle: " + ic->Handle + ") is published.";
		this->OnHLAClassPublished(gcnew RaconEventArgs(msg));
	}
#pragma region exceptions
	catch (InteractionClassNotDefined& e) {
		String^ msg = "MSG-(InteractionClassNotDefined - publishInteractionClass):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (SaveInProgress& e) {
		String^ msg = "MSG-(SaveInProgress - publishInteractionClass):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "MSG-(RestoreInProgress - publishInteractionClass):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "MSG-(FederateNotExecutionMember - publishInteractionClass):" + Environment::NewLine + "Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (NotConnected& e) {
		String^ msg = "MSG-(NotConnected - publishInteractionClass):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg));
	}
	catch (RTIinternalError& e) {
		MessageBox::Show("MSG-(RTIinternalError - publishInteractionClass):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	catch (System::Exception^ e) {
		MessageBox::Show("MSG-(GeneralException - publishInteractionClass):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
#pragma endregion
};

// 5.6 - subscribeOC with selected attributes
bool RtiAmb_Hla1516e::subscribeObjectClass(HlaObjectClass ^oc, BindingList<HlaAttribute^>^ attributes, Boolean active)
{
	try {
		List<HlaAttribute^> ^tmpList = gcnew List<HlaAttribute^>();
		if ((oc->ClassPS == PSKind::Subscribe) || (oc->ClassPS == PSKind::PublishSubscribe))
		{
			for each (HlaAttribute^ attr in attributes) {
				if ((attr->AttributePS == PSKind::Subscribe) || (attr->AttributePS == PSKind::PublishSubscribe)) {
					tmpList->Add(attr);
				}
			}
		}
		rti1516e::ObjectClassHandle ocHandle = getObjectClassHandle(oc);
		rti1516e::AttributeHandleSet ahs;

		String^ tmpStr = "";
		// Add attribute handles to AHS
		for (int j = 0; j < tmpList->Count; j++) {
			rti1516e::AttributeHandle handle = getAttributeHandle(tmpList[j], ocHandle);
			ahs.insert(handle);
			tmpStr += tmpList[j]->Name + " (" + Handle2Long(handle).ToString() + ")";
			if (j != tmpList->Count - 1) tmpStr += ", ";
		}

		rti->subscribeObjectClassAttributes(ocHandle, ahs);

		String^ msg = "Object Class (" + oc->Name + ") is subscribed with all its subscribable attributes. Class handle: " + Handle2Long(ocHandle).ToString() + ", Attributes: {" + tmpStr + "}";
		this->OnHLAClassPublished(gcnew RaconEventArgs(msg));
		return true;
	}
#pragma region exceptions
	catch (ObjectClassNotDefined& e) {
		String^ msg = "MSG-(ObjectClassNotDefined - subscribeObjectClass):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (AttributeNotDefined& e) {
		String^ msg = "MSG-(AttributeNotDefined - subscribeObjectClass):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (SaveInProgress& e) {
		String^ msg = "MSG-(SaveInProgress - subscribeObjectClass):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (RestoreInProgress& e) {
		String^ msg = "MSG-(RestoreInProgress - subscribeObjectClass):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "MSG-(FederateNotExecutionMember - subscribeObjectClass):" + Environment::NewLine + "Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (InvalidUpdateRateDesignator& e) {
		String^ msg = "MSG-(InvalidUpdateRateDesignator - subscribeObjectClass):" + Environment::NewLine + "Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (NotConnected& e) {
		String^ msg = "MSG-(NotConnected - subscribeObjectClass):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (RTIinternalError& e) {
		MessageBox::Show("MSG-(RTIinternalError - subscribeObjectClass):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
		return false;
	}
	catch (System::Exception^ e) {
		MessageBox::Show("MSG-(GeneralException - subscribeObjectClass):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
		return false;
	}
#pragma endregion
};

// 5.7 - unsubscribe OC 
bool RtiAmb_Hla1516e::unsubscribeObjectClass(HlaObjectClass ^oc) {
	return true;
};

// 5.8 - SubscribeIC
void RtiAmb_Hla1516e::subscribeInteractionClass(HlaInteractionClass ^ic) {
	// Check that interaction is Subscribable
	if ((ic->ClassPS == PSKind::Subscribe) || (ic->ClassPS == PSKind::PublishSubscribe))
		try {
		rti1516e::InteractionClassHandle handle = getInteractionClassHandle(ic);

		rti->subscribeInteractionClass(handle);
		String^ msg = "Interaction Class (name: " + ic->Name + ", handle: " + ic->Handle + ") is subscribed.";
		this->OnHLAClassSubscribed(gcnew RaconEventArgs(msg));
	}
#pragma region exceptions
	catch (FederateServiceInvocationsAreBeingReportedViaMOM& e) {
		String^ msg = "MSG-(FederateServiceInvocationsAreBeingReportedViaMOM - subscribeInteractionClass):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (InteractionClassNotDefined& e) {
		String^ msg = "MSG-(InteractionClassNotDefined - subscribeInteractionClass):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (SaveInProgress& e) {
		String^ msg = "MSG-(SaveInProgress - subscribeInteractionClass):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "MSG-(RestoreInProgress - subscribeInteractionClass):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "MSG-(FederateNotExecutionMember - subscribeInteractionClass):" + Environment::NewLine + "Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (NotConnected& e) {
		String^ msg = "MSG-(NotConnected - subscribeInteractionClass):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg));
	}
	catch (RTIinternalError& e) {
		MessageBox::Show("MSG-(RTIinternalError - subscribeInteractionClass):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	catch (System::Exception^ e) {
		MessageBox::Show("MSG-(GeneralException - subscribeInteractionClass):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
#pragma endregion
};

// 5.9 - unsubscribeIC
bool RtiAmb_Hla1516e::unsubscribeInteractionClass(HlaInteractionClass ^ic) {
	return true;
};

#pragma endregion	// Declaration Management		

#pragma region Object Management

// 6.10 - Update Attribute Values
bool RtiAmb_Hla1516e::updateAttributeValues(HlaObject ^object) {
	try {
		String^ msg = "";
		// Pack attribute values
		rti1516e::AttributeHandleValueMap attributes;
		// Create Tag
		char* tagData = (char *)Marshal::StringToHGlobalAnsi(object->Tag).ToPointer();
		VariableLengthData tag(tagData, ((strlen(tagData) + 1) * sizeof(char)));
		for each (HlaAttribute^ var in object->Attributes)
		{
			char* _pVal = (char*)var->Value.ToPointer();
			//msg += "Size: " + var->Length;
			rti1516e::VariableLengthData data(_pVal, var->Length);
			//rti1516e::VariableLengthData data(_pVal, ((strlen(_pVal) + 1)*sizeof(char)));
			attributes[om->Attributes[var->Handle]] = data;
		}
		// Consume Attributes
		object->Attributes->Clear();
		// Update attribute values using RTI
		rti1516e::ObjectInstanceHandle ohandle = om->Objects[object->Handle];
		(void)rti->updateAttributeValues(ohandle, attributes, tag);
		msg = "Attribute values of object instance (handle: " + object->Handle + ") are updated.";
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return true;
	}
#pragma region exceptions
	catch (ObjectInstanceNotKnown& e) {
		String^ msg = "MSG-(ObjectInstanceNotKnown - updateAttributeValues):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (AttributeNotDefined& e) {
		String^ msg = "MSG-(AttributeNotDefined - updateAttributeValues):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (AttributeNotOwned& e) {
		String^ msg = "MSG-(AttributeNotOwned - updateAttributeValues):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "MSG-(FederateNotExecutionMember - updateAttributeValues):" + Environment::NewLine + "Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (SaveInProgress& e) {
		String^ msg = "MSG-(SaveInProgress - updateAttributeValues):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "MSG-(RestoreInProgress - updateAttributeValues):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (NotConnected& e) {
		String^ msg = "MSG-(NotConnected - updateAttributeValues):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg));
	}
	catch (RTIinternalError& e) {
		MessageBox::Show("MSG-(RTIinternalError - updateAttributeValues):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	catch (System::Exception^ e) {
		MessageBox::Show("MSG-(GeneralException - updateAttributeValues):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
#pragma endregion
};

// 6.10 - Update Attribute Values with Timestamp
EventRetractionHandle^ RtiAmb_Hla1516e::updateAttributeValues(HlaObject ^object, Double timestamp) {
	try {
		// Pack attribute values
		rti1516e::AttributeHandleValueMap attributes;
		// Create Tag
		char* tagData = (char *)Marshal::StringToHGlobalAnsi(object->Tag).ToPointer();
		VariableLengthData tag(tagData, ((strlen(tagData) + 1) * sizeof(char)));
		for each (HlaAttribute^ var in object->Attributes)
		{
			char* _pVal = (char*)var->Value.ToPointer();
			rti1516e::VariableLengthData data(_pVal, var->Length);
			attributes[om->Attributes[var->Handle]] = data;
		}
		// Consume Attributes
		object->Attributes->Clear();
		// Create timestamp
		std::auto_ptr<rti1516e::HLAfloat64Time> rtitime(new HLAfloat64Time(timestamp));
		// Update attribute values using RTI
		rti1516e::ObjectInstanceHandle ohandle = om->Objects[object->Handle];
		rti1516e::MessageRetractionHandle mrh = rti->updateAttributeValues(ohandle, attributes, tag, *rtitime);
		// create retraction
		EventRetractionHandle^ retraction = gcnew EventRetractionHandle(gcnew String(mrh.toString().c_str()));
		// Report
		String^ msg = "Attribute values of object instance (handle: " + object->Handle + ") are updated." + " Retraction handle = " + gcnew String(mrh.toString().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return retraction;
	}
#pragma region exceptions
	catch (ObjectInstanceNotKnown& e) {
		String^ msg = "MSG-(ObjectInstanceNotKnown - updateAttributeValues):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (AttributeNotDefined& e) {
		String^ msg = "MSG-(AttributeNotDefined - updateAttributeValues):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (AttributeNotOwned& e) {
		String^ msg = "MSG-(AttributeNotOwned - updateAttributeValues):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "MSG-(FederateNotExecutionMember - updateAttributeValues):" + Environment::NewLine + "Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (SaveInProgress& e) {
		String^ msg = "MSG-(SaveInProgress - updateAttributeValues):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "MSG-(RestoreInProgress - updateAttributeValues):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (NotConnected& e) {
		String^ msg = "MSG-(NotConnected - updateAttributeValues):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg));
	}
	catch (RTIinternalError& e) {
		MessageBox::Show("MSG-(RTIinternalError - updateAttributeValues):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	catch (System::Exception^ e) {
		MessageBox::Show("MSG-(GeneralException - updateAttributeValues):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
#pragma endregion
};

// 6.12 - Send Interaction
bool RtiAmb_Hla1516e::sendInteraction(HlaInteraction ^interaction) {
	try {
		String ^msg = "";
		unsigned int chandle = interaction->ClassHandle;
		rti1516e::InteractionClassHandle handle = om->InteractionClasses[chandle];

		// Create Tag
		char* tagData = (char *)Marshal::StringToHGlobalAnsi(interaction->Tag).ToPointer();
		VariableLengthData tag(tagData, ((strlen(tagData) + 1) * sizeof(char)));
		// Pack parameter values
		rti1516e::ParameterHandleValueMap Parameters;
		for each (HlaParameter^ var in interaction->Parameters)
		{
			char* _pVal = (char*)var->Value.ToPointer();
			//msg += "Handle: " + var->Name + " Size: " + var->Length + sizeof(double);
			rti1516e::VariableLengthData data(_pVal, var->Length);
			//rti1516e::VariableLengthData data(_pVal, ((strlen(_pVal) + 1)*sizeof(char)));
			Parameters[om->Parameters[var->Handle]] = data;
		}

		rti->sendInteraction(handle, Parameters, tag);
		msg += "Interaction (handle: " + interaction->ClassHandle + ") is sent.";
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return true;
	}
#pragma region exceptions
	catch (InteractionClassNotPublished &e) {
		MessageBox::Show("MSG-(InteractionClassNotPublished - sendInteraction):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
		return false;
	}
	catch (InteractionParameterNotDefined &e) {
		MessageBox::Show("MSG-(InteractionParameterNotDefined - sendInteraction):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
		return false;
	}
	catch (InteractionClassNotDefined &e) {
		MessageBox::Show("MSG-(InteractionClassNotDefined - sendInteraction):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
		return false;
	}
	catch (SaveInProgress& e) {
		MessageBox::Show("MSG-(SaveInProgress - sendInteraction):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
		return false;
	}
	catch (RestoreInProgress& e) {
		MessageBox::Show("MSG-(RestoreInProgress - sendInteraction):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
		return false;
	}
	catch (FederateNotExecutionMember& e) {
		MessageBox::Show("MSG-(FederateNotExecutionMember - sendInteraction):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
		return false;
	}
	catch (NotConnected& e) {
		String^ msg = "MSG-(NotConnected - sendInteraction):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg));
	}
	catch (RTIinternalError& e) {
		MessageBox::Show("MSG-(RTIinternalError - sendInteraction):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
		return false;
	}
	catch (System::Exception^ e) {
		MessageBox::Show("MSG-(GeneralException - sendInteraction):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
		return false;
	}
#pragma endregion
};

// 6.12 - Send Interaction with Timestamp
EventRetractionHandle^ RtiAmb_Hla1516e::sendInteraction(HlaInteraction ^interaction, Double time) {
	try {
		String ^msg = "";
		unsigned int chandle = interaction->ClassHandle;
		rti1516e::InteractionClassHandle handle = om->InteractionClasses[chandle];

		// Create Tag
		char* tagData = (char *)Marshal::StringToHGlobalAnsi(interaction->Tag).ToPointer();
		VariableLengthData tag(tagData, ((strlen(tagData) + 1) * sizeof(char)));
		// Create timestamp
		std::auto_ptr<rti1516e::HLAfloat64Time> rtitime(new HLAfloat64Time(time));
		// Pack parameter values
		rti1516e::ParameterHandleValueMap Parameters;
		for each (HlaParameter^ var in interaction->Parameters)
		{
			char* _pVal = (char*)var->Value.ToPointer();
			rti1516e::VariableLengthData data(_pVal, var->Length);
			Parameters[om->Parameters[var->Handle]] = data;
		}

		rti1516e::MessageRetractionHandle mrh = rti->sendInteraction(handle, Parameters, tag, *rtitime);
		// create retraction
		EventRetractionHandle^ retraction = gcnew EventRetractionHandle(gcnew String(mrh.toString().c_str()));
		// Report
		msg += "Interaction (handle: " + interaction->ClassHandle + ") is sent." + " Retraction handle = " + gcnew String(mrh.toString().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return retraction;
	}
#pragma region exceptions
	catch (InteractionClassNotPublished &e) {
		MessageBox::Show("MSG-(InteractionClassNotPublished - sendInteraction):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	catch (InteractionParameterNotDefined &e) {
		MessageBox::Show("MSG-(InteractionParameterNotDefined - sendInteraction):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	catch (InteractionClassNotDefined &e) {
		MessageBox::Show("MSG-(InteractionClassNotDefined - sendInteraction):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	catch (SaveInProgress& e) {
		MessageBox::Show("MSG-(SaveInProgress - sendInteraction):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	catch (RestoreInProgress& e) {
		MessageBox::Show("MSG-(RestoreInProgress - sendInteraction):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	catch (FederateNotExecutionMember& e) {
		MessageBox::Show("MSG-(FederateNotExecutionMember - sendInteraction):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	catch (NotConnected& e) {
		String^ msg = "MSG-(NotConnected - sendInteraction):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg));
	}
	catch (RTIinternalError& e) {
		MessageBox::Show("MSG-(RTIinternalError - sendInteraction):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	catch (System::Exception^ e) {
		MessageBox::Show("MSG-(GeneralException - sendInteraction):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
#pragma endregion
};

// 6.14 - Delete an Object Instance
bool RtiAmb_Hla1516e::deleteObjectInstance(HlaObject ^obj) {
	try {
		// Get handle
		rti1516e::ObjectInstanceHandle ohandle = om->Objects[obj->Handle];
		// Create Tag
		char* tagData = (char *)Marshal::StringToHGlobalAnsi(obj->Tag).ToPointer();
		VariableLengthData tag(tagData, ((strlen(tagData) + 1) * sizeof(char)));
		// Delete object
		rti->deleteObjectInstance(ohandle, tag);
		// Update Nom
		om->Objects.erase(obj->Handle);
		// Report
		String^ msg = "Object (handle: " + obj->Handle + ") is deleted by this federate";
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return true;
	}
#pragma region exceptions
	catch (DeletePrivilegeNotHeld& e) {
		String^ msg = "MSG-(DeletePrivilegeNotHeld - deleteObjectInstance):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (ObjectInstanceNotKnown& e) {
		String^ msg = "MSG-(ObjectInstanceNotKnown - deleteObjectInstance):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "MSG-(FederateNotExecutionMember - deleteObjectInstance):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (SaveInProgress& e) {
		String^ msg = "MSG-(SaveInProgress - deleteObjectInstance):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "MSG-(RestoreInProgress - deleteObjectInstance):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (NotConnected& e) {
		String^ msg = "MSG-(NotConnected - deleteObjectInstance):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg));
	}
	catch (RTIinternalError& e) {
		MessageBox::Show("MSG-(RTIinternalError - deleteObjectInstance):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	catch (System::Exception^ e) {
		MessageBox::Show("MSG-(GeneralException - deleteObjectInstance):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
#pragma endregion
};

// 6.14 - Delete an Object Instance with Timestamp
EventRetractionHandle^ RtiAmb_Hla1516e::deleteObjectInstance(HlaObject ^obj, Double time) {
	try {
		// Get handle
		rti1516e::ObjectInstanceHandle ohandle = om->Objects[obj->Handle];
		// Create Tag
		char* tagData = (char *)Marshal::StringToHGlobalAnsi(obj->Tag).ToPointer();
		VariableLengthData tag(tagData, ((strlen(tagData) + 1) * sizeof(char)));
		// Create timestamp
		std::auto_ptr<rti1516e::HLAfloat64Time> rtitime(new HLAfloat64Time(time));

		// Delete object
		rti1516e::MessageRetractionHandle mrh = rti->deleteObjectInstance(ohandle, tag, *rtitime);
		// Update Nom
		om->Objects.erase(obj->Handle);
		// create retraction
		EventRetractionHandle^ retraction = gcnew EventRetractionHandle(gcnew String(mrh.toString().c_str()));
		// Report
		String^ msg = "Object (handle: " + obj->Handle + ") is deleted by this federate" + " Retraction handle = " + gcnew String(mrh.toString().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return retraction;
	}
#pragma region exceptions
	catch (DeletePrivilegeNotHeld& e) {
		String^ msg = "MSG-(DeletePrivilegeNotHeld - deleteObjectInstance):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (ObjectInstanceNotKnown& e) {
		String^ msg = "MSG-(ObjectInstanceNotKnown - deleteObjectInstance):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "MSG-(FederateNotExecutionMember - deleteObjectInstance):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (SaveInProgress& e) {
		String^ msg = "MSG-(SaveInProgress - deleteObjectInstance):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "MSG-(RestoreInProgress - deleteObjectInstance):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (NotConnected& e) {
		String^ msg = "MSG-(NotConnected - deleteObjectInstance):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg));
	}
	catch (RTIinternalError& e) {
		MessageBox::Show("MSG-(RTIinternalError - deleteObjectInstance):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	catch (System::Exception^ e) {
		MessageBox::Show("MSG-(GeneralException - deleteObjectInstance):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
#pragma endregion
};

// 6.19 - Request Attribute Value Update - oc
bool RtiAmb_Hla1516e::requestAttributeValueUpdate(HlaObjectClass ^oc, List<HlaAttribute^>^ attributes) {
	try {
		String^ tmpStr = "";

		// Create Attribute Handle Set 
		rti1516e::AttributeHandleSet ahs;
		for (int j = 0; j < attributes->Count; j++) {
			rti1516e::AttributeHandle handle = om->Attributes[attributes[j]->Handle];
			ahs.insert(handle);
			tmpStr += attributes[j]->Name + " (" + Handle2Long(handle).ToString() + ")";
			if (j != attributes->Count - 1) tmpStr += ", ";
		}

		rti1516e::ObjectClassHandle ocHandle = om->ObjectClasses[oc->Handle];
		// Create Tag
		char* tagData = (char *)Marshal::StringToHGlobalAnsi(oc->Tag).ToPointer();
		VariableLengthData tag(tagData, ((strlen(tagData) + 1) * sizeof(char)));

		this->rti->requestAttributeValueUpdate(ocHandle, ahs, tag);
		String^ msg = "Attribute value update for class (name: " + oc->Name + ", handle: " + oc->Handle + ") is requested for attributes:" + tmpStr + ".";
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return true;
	}
#pragma region exceptions
	catch (ObjectClassNotDefined& e) {
		String^ msg = "MSG-(ObjectClassNotDefined - requestAttributeValueUpdate):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (AttributeNotDefined& e) {
		String^ msg = "MSG-(AttributeNotDefined - requestAttributeValueUpdate):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "MSG-(FederateNotExecutionMember - requestAttributeValueUpdate):" + Environment::NewLine + "Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (SaveInProgress& e) {
		String^ msg = "MSG-(SaveInProgress - requestAttributeValueUpdate):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "MSG-(RestoreInProgress - requestAttributeValueUpdate):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (NotConnected& e) {
		String^ msg = "MSG-(NotConnected - requestAttributeValueUpdate):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg));
	}
	catch (RTIinternalError& e) {
		MessageBox::Show("MSG-(RTIinternalError - requestAttributeValueUpdate):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	//catch (System::Exception^ e) {
	//	MessageBox::Show("MSG-(GeneralException - publishObjectClass):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	//}
#pragma endregion
};

// 6.19 - Request Attribute Value Update - object
bool RtiAmb_Hla1516e::requestAttributeValueUpdate(HlaObject ^obj, List<HlaAttribute^>^ attributes) {
	try {
		String^ tmpStr = "";

		// Create Attribute Handle Set 
		rti1516e::AttributeHandleSet ahs;
		for (int j = 0; j < attributes->Count; j++) {
			rti1516e::AttributeHandle handle = om->Attributes[attributes[j]->Handle];
			ahs.insert(handle);
			tmpStr += attributes[j]->Name + " (" + Handle2Long(handle).ToString() + ")";
			if (j != attributes->Count - 1) tmpStr += ", ";
		}

		rti1516e::ObjectInstanceHandle oHandle = om->Objects[obj->Handle];
		// Create Tag
		char* tagData = (char *)Marshal::StringToHGlobalAnsi(obj->Tag).ToPointer();
		VariableLengthData tag(tagData, ((strlen(tagData) + 1) * sizeof(char)));

		String^ msg = "Attribute value update for object (name: " + obj->Name + ", handle: " + obj->Handle + ") is requested for attributes:" + tmpStr + ".";
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));

		this->rti->requestAttributeValueUpdate(oHandle, ahs, tag);
		return true;
	}
#pragma region exceptions
	catch (ObjectInstanceNotKnown& e) {
		String^ msg = "MSG-(ObjectInstanceNotKnown - requestAttributeValueUpdate2):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (AttributeNotDefined& e) {
		String^ msg = "MSG-(AttributeNotDefined - requestAttributeValueUpdate2):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "MSG-(FederateNotExecutionMember - requestAttributeValueUpdate2):" + Environment::NewLine + "Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (SaveInProgress& e) {
		String^ msg = "MSG-(SaveInProgress - requestAttributeValueUpdate2):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "MSG-(RestoreInProgress - requestAttributeValueUpdate2):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (NotConnected& e) {
		String^ msg = "MSG-(NotConnected - requestAttributeValueUpdate2):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg));
	}
	catch (RTIinternalError& e) {
		MessageBox::Show("MSG-(RTIinternalError - requestAttributeValueUpdate2):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	//catch (System::Exception^ e) {
	//	MessageBox::Show("MSG-(GeneralException - publishObjectClass):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	//}
#pragma endregion
};

#pragma endregion	// Object Management	

#pragma region Ownership Management
// attributeOwnershipAcquisitionIfAvailable
bool RtiAmb_Hla1516e::attributeOwnershipAcquisitionIfAvailable(HlaObject ^object, RaconAttributeSet^ _set) {
	return true;

};

// attributeOwnershipAcquisition
bool RtiAmb_Hla1516e::attributeOwnershipAcquisition(HlaObject ^object, RaconAttributeSet^ _set) {
	return true;

};

// cancelAttributeOwnershipAcquisition
bool RtiAmb_Hla1516e::cancelAttributeOwnershipAcquisition(HlaObject ^object, RaconAttributeSet^ _set) {
	return true;

};

// queryAttributeOwnership
bool RtiAmb_Hla1516e::queryAttributeOwnership(HlaObject ^object, HlaAttribute^ _attribute) {
	return true;

};

// attributeOwnershipReleaseResponse
bool RtiAmb_Hla1516e::attributeOwnershipReleaseResponse(HlaObject ^object, RaconAttributeSet^ _set) {
	return true;

};

// cancelNegotiatedAttributeOwnershipDivestiture
bool RtiAmb_Hla1516e::cancelNegotiatedAttributeOwnershipDivestiture(HlaObject ^object, RaconAttributeSet^ _set) {
	return true;

};

// isAttributeOwnedByFederate
bool RtiAmb_Hla1516e::isAttributeOwnedByFederate(HlaObject ^object, HlaAttribute^ _attribute) {
	return true;

};

// negotiatedAttributeOwnershipDivestiture
bool RtiAmb_Hla1516e::negotiatedAttributeOwnershipDivestiture(HlaObject ^object, RaconAttributeSet^ _set) {
	return true;

};

// unconditionalAttributeOwnershipDivestiture
bool RtiAmb_Hla1516e::unconditionalAttributeOwnershipDivestiture(HlaObject ^object, RaconAttributeSet^ _set) {
	return true;
};
#pragma endregion	// Ownership Management

#pragma region Data Distribution Management

// associateRegionForUpdates
bool RtiAmb_Hla1516e::associateRegionForUpdates(CHlaRegion ^region, HlaObject ^object, List<HlaAttribute^> ^attributes) {
	return true;
};

// Create region
bool RtiAmb_Hla1516e::createRegion(CHlaRegion^ region, unsigned long noOfExtents, long spaceHandle) {
	return true;

};

// Delete region
bool RtiAmb_Hla1516e::deleteRegion(CHlaRegion^ region) {
	return true;
};

// Register Object Instance With Region - selected (publishable) attributes with specific regions
bool RtiAmb_Hla1516e::registerObjectInstanceWithRegion(HlaObject ^obj, List<HlaAttribute^> ^attributes, List<CHlaRegion^> ^regions) {
	return true;

};

// requestClassAttributeValueUpdateWithRegion
bool RtiAmb_Hla1516e::requestClassAttributeValueUpdateWithRegion(HlaObjectClass ^oc, List<HlaAttribute^> ^attributes, CHlaRegion ^region) {
	return true;
};

// Send Interaction With Region
bool RtiAmb_Hla1516e::sendInteraction(HlaInteraction ^interaction, CHlaRegion ^region) {
	return true;
};

// subscribe IC with Region
bool RtiAmb_Hla1516e::subscribeInteractionClass(HlaInteractionClass ^ic, CHlaRegion ^region) {
	return true;

};

// subscribeOC with Region
bool RtiAmb_Hla1516e::subscribeObjectClassAttributesWithRegion(HlaObjectClass ^oc, List<HlaAttribute^>^ attributes, CHlaRegion ^reg, Boolean active) {
	return true;

};

// associateRegionForUpdates
bool RtiAmb_Hla1516e::unassociateRegionForUpdates(CHlaRegion ^region, HlaObject ^object) {
	return true;
};

// unsubscribe IC with Region
void RtiAmb_Hla1516e::unsubscribeInteractionClass(HlaInteractionClass ^ic, CHlaRegion ^region) {
};

// unsubscribe OC with Region
void RtiAmb_Hla1516e::unsubscribeObjectClassWithRegion(HlaObjectClass ^oc, CHlaRegion ^region) {
};

#pragma endregion // Data Distribution Management

#pragma region Time Management

// Enable Async. Delivery
bool RtiAmb_Hla1516e::enableAsynchronousDelivery() {
	return true;
};

// changeAttributeOrderType
bool RtiAmb_Hla1516e::changeAttributeOrderType(HlaObject ^obj, List<HlaAttribute^>^ attributes, OrderingHandle type) {
	return true;
};

// changeInteractionOrderType
bool RtiAmb_Hla1516e::changeInteractionOrderType(HlaInteractionClass ^ic, OrderingHandle type) {
	return true;
};

// Disable Async. Delivery
bool RtiAmb_Hla1516e::disableAsynchronousDelivery() {
	return true;
};

// Disable Time Constrained
bool RtiAmb_Hla1516e::disableTimeConstrained() {
	return true;

};

// Disable Time Regulation
bool RtiAmb_Hla1516e::disableTimeRegulation() {
	return true;

};

// Enable Time Constrained
bool RtiAmb_Hla1516e::enableTimeConstrained() {
	return true;

};

// Enable Time Constrained
bool RtiAmb_Hla1516e::enableTimeRegulation(Double federateTime, Double lookahead) {
	return true;

};

// flushQueueRequest
bool RtiAmb_Hla1516e::flushQueueRequest(Double time) {
	return true;

};

// modifyLookahead
bool RtiAmb_Hla1516e::modifyLookahead(Double lookahead) {
	return true;

};

// nextEventRequest
bool RtiAmb_Hla1516e::nextEventRequest(Double time) {
	return true;

};

// nextEventRequestAvailable
bool RtiAmb_Hla1516e::nextEventRequestAvailable(Double time) {
	return true;

};

// queryFederateTime
Double RtiAmb_Hla1516e::queryFederateTime() {
	return 0;

};

// queryLBTS
Double RtiAmb_Hla1516e::queryLBTS() {
	return 0;

};

// queryLookahead
Double RtiAmb_Hla1516e::queryLookahead() {
	return 0;
};

// queryMinNextEventTime
Double RtiAmb_Hla1516e::queryMinNextEventTime() {
	return 0;

};

// timeAdvanceRequest
bool RtiAmb_Hla1516e::timeAdvanceRequest(Double time) {
	return true;

};

// timeAdvanceRequestAvailable
bool RtiAmb_Hla1516e::timeAdvanceRequestAvailable(Double time) {
	return true;

};

// Retract
bool RtiAmb_Hla1516e::retract(EventRetractionHandle ^retraction) {
	return true;
};

#pragma endregion	// Time Management		

#pragma region Support Services

// 10.33 - enableObjectClassRelevanceAdvisorySwitch
bool RtiAmb_Hla1516e::enableObjectClassRelevanceAdvisorySwitch(void) {
	try {
		// Relevance advising is enabled by default
		rti->enableObjectClassRelevanceAdvisorySwitch();
		String^ msg = "Object Class Relevance Advisory Switch is enabled.";
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return true;
	}
#pragma region exceptions
	catch (ObjectClassRelevanceAdvisorySwitchIsOn& e) {
		String^ msg = "MSG-(ObjectClassRelevanceAdvisorySwitchIsOn - enableClassRelevanceAdvisorySwitch):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "MSG-(FederateNotExecutionMember - enableClassRelevanceAdvisorySwitch):" + Environment::NewLine + "Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (SaveInProgress& e) {
		String^ msg = "MSG-(SaveInProgress - enableClassRelevanceAdvisorySwitch):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "MSG-(RestoreInProgress - enableClassRelevanceAdvisorySwitch):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (NotConnected& e) {
		String^ msg = "MSG-(NotConnected-enableClassRelevanceAdvisorySwitch-RtiAmb_Hla1516e):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg));
	}
	catch (RTIinternalError& e) {
		MessageBox::Show("MSG-(RTIinternalError - enableClassRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	catch (System::Exception^ e) {
		MessageBox::Show("MSG-(GeneralException - enableClassRelevanceAdvisorySwitch):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
#pragma endregion
};

// 10.34 - disableObjectClassRelevanceAdvisorySwitch
bool RtiAmb_Hla1516e::disableObjectClassRelevanceAdvisorySwitch(void) {
	try {
		rti->disableObjectClassRelevanceAdvisorySwitch();
		String^ msg = "Object Class Relevance Advisory Switch is disabled.";
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return true;
	}
#pragma region exceptions
	catch (ObjectClassRelevanceAdvisorySwitchIsOff& e) {
		String^ msg = "MSG-(ObjectClassRelevanceAdvisorySwitchIsOff - disableClassRelevanceAdvisorySwitch):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "MSG-(FederateNotExecutionMember - disableClassRelevanceAdvisorySwitch):" + Environment::NewLine + "Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (SaveInProgress& e) {
		String^ msg = "MSG-(SaveInProgress - disableClassRelevanceAdvisorySwitch):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "MSG-(RestoreInProgress - disableClassRelevanceAdvisorySwitch):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (NotConnected& e) {
		String^ msg = "MSG-(NotConnected - disableClassRelevanceAdvisorySwitch):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RTIinternalError& e) {
		MessageBox::Show("MSG-(RTIinternalError - disableClassRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	catch (System::Exception^ e) {
		MessageBox::Show("MSG-(GeneralException - disableClassRelevanceAdvisorySwitch):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
#pragma endregion
};

// 10.35 - enableAttributeRelevanceAdvisorySwitch
bool RtiAmb_Hla1516e::enableAttributeRelevanceAdvisorySwitch(void) {
	try {
		rti->enableAttributeRelevanceAdvisorySwitch();
		String^ msg = "Attribute Relevance Advisory Switch is enabled.";
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return true;
	}
#pragma region exceptions
	catch (AttributeRelevanceAdvisorySwitchIsOn& e) {
		String^ msg = "MSG-(AttributeRelevanceAdvisorySwitchIsOn - enableAttributeRelevanceAdvisorySwitch):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "MSG-(FederateNotExecutionMember - enableAttributeRelevanceAdvisorySwitch):" + Environment::NewLine + "Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (SaveInProgress& e) {
		String^ msg = "MSG-(SaveInProgress - enableAttributeRelevanceAdvisorySwitch):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "MSG-(RestoreInProgress - enableAttributeRelevanceAdvisorySwitch):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (NotConnected& e) {
		String^ msg = "MSG-(NotConnected - enableAttributeRelevanceAdvisorySwitch):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg));
	}
	catch (RTIinternalError& e) {
		MessageBox::Show("MSG-(RTIinternalError - enableAttributeRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	catch (System::Exception^ e) {
		MessageBox::Show("MSG-(GeneralException - enableAttributeRelevanceAdvisorySwitch):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
#pragma endregion
};

// 10.36 - disableAttributeRelevanceAdvisorySwitch
bool RtiAmb_Hla1516e::disableAttributeRelevanceAdvisorySwitch(void) {
	try {
		rti->disableAttributeRelevanceAdvisorySwitch();
		String^ msg = "Attribute Relevance Advisory Switch is disabled.";
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return true;
	}
#pragma region exceptions
	catch (AttributeRelevanceAdvisorySwitchIsOff& e) {
		String^ msg = "MSG-(AttributeRelevanceAdvisorySwitchIsOff - disableAttributeRelevanceAdvisorySwitch):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "MSG-(FederateNotExecutionMember - disableAttributeRelevanceAdvisorySwitch):" + Environment::NewLine + "Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (SaveInProgress& e) {
		String^ msg = "MSG-(SaveInProgress - disableAttributeRelevanceAdvisorySwitch):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "MSG-(RestoreInProgress - disableAttributeRelevanceAdvisorySwitch):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (NotConnected& e) {
		String^ msg = "MSG-(NotConnected - disableAttributeRelevanceAdvisorySwitch):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg));
	}
	catch (RTIinternalError& e) {
		MessageBox::Show("MSG-(RTIinternalError - disableAttributeRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	catch (System::Exception^ e) {
		MessageBox::Show("MSG-(GeneralException - disableAttributeRelevanceAdvisorySwitch):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
#pragma endregion
};

// 10.37 - enableAttributeScopeAdvisorySwitch
bool RtiAmb_Hla1516e::enableAttributeScopeAdvisorySwitch(void) {
	try {
		rti->enableAttributeScopeAdvisorySwitch();
		String^ msg = "Attribute Scope Advisory Switch is enabled.";
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return true;
	}
#pragma region exceptions
	catch (AttributeScopeAdvisorySwitchIsOn& e) {
		String^ msg = "MSG-(AttributeScopeAdvisorySwitchIsOn - enableAttributeScopeAdvisorySwitch):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "MSG-(FederateNotExecutionMember - enableAttributeScopeAdvisorySwitch):" + Environment::NewLine + "Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (SaveInProgress& e) {
		String^ msg = "MSG-(SaveInProgress - enableAttributeScopeAdvisorySwitch):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "MSG-(RestoreInProgress - enableAttributeScopeAdvisorySwitch):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (NotConnected& e) {
		String^ msg = "MSG-(NotConnected - enableAttributeScopeAdvisorySwitch):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg));
	}
	catch (RTIinternalError& e) {
		MessageBox::Show("MSG-(RTIinternalError - enableAttributeScopeAdvisorySwitch):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	catch (System::Exception^ e) {
		MessageBox::Show("MSG-(GeneralException - enableAttributeScopeAdvisorySwitch):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
#pragma endregion
};

// 10.38 - disableAttributeScopeAdvisorySwitch
bool RtiAmb_Hla1516e::disableAttributeScopeAdvisorySwitch(void) {
	try {
		rti->disableAttributeScopeAdvisorySwitch();
		String^ msg = "Attribute Scope Advisory Switch is disabled.";
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return true;
	}
#pragma region exceptions
	catch (AttributeRelevanceAdvisorySwitchIsOff& e) {
		String^ msg = "MSG-(AttributeScopeAdvisorySwitchIsOff - disableAttributeScopeAdvisorySwitch):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "MSG-(FederateNotExecutionMember - disableAttributeScopeAdvisorySwitch):" + Environment::NewLine + "Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (SaveInProgress& e) {
		String^ msg = "MSG-(SaveInProgress - disableAttributeScopeAdvisorySwitch):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "MSG-(RestoreInProgress - disableAttributeScopeAdvisorySwitch):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (NotConnected& e) {
		String^ msg = "MSG-(NotConnected - disableAttributeScopeAdvisorySwitch):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg));
	}
	catch (RTIinternalError& e) {
		MessageBox::Show("MSG-(RTIinternalError - disableAttributeScopeAdvisorySwitch):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	catch (System::Exception^ e) {
		MessageBox::Show("MSG-(GeneralException - disableAttributeScopeAdvisorySwitch):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
#pragma endregion
};

// 10.39 - enableInteractionRelevanceAdvisorySwitch
bool RtiAmb_Hla1516e::enableInteractionRelevanceAdvisorySwitch(void) {
	try {
		rti->enableInteractionRelevanceAdvisorySwitch();
		String^ msg = "Interaction Relevance Advisory Switch is enabled.";
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return true;
	}
#pragma region exceptions
	catch (InteractionRelevanceAdvisorySwitchIsOn& e) {
		String^ msg = "MSG-(InteractionRelevanceAdvisorySwitchIsOn - enableInteractionRelevanceAdvisorySwitch):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "MSG-(FederateNotExecutionMember - enableInteractionRelevanceAdvisorySwitch):" + Environment::NewLine + "Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (SaveInProgress& e) {
		String^ msg = "MSG-(SaveInProgress - enableInteractionRelevanceAdvisorySwitch):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "MSG-(RestoreInProgress - enableInteractionRelevanceAdvisorySwitch):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (NotConnected& e) {
		String^ msg = "MSG-(NotConnected - enableInteractionRelevanceAdvisorySwitch):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg));
	}
	catch (RTIinternalError& e) {
		MessageBox::Show("MSG-(RTIinternalError - enableInteractionRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	catch (System::Exception^ e) {
		MessageBox::Show("MSG-(GeneralException - enableInteractionRelevanceAdvisorySwitch):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
#pragma endregion
};

// 10.40 - disableInteractionRelevanceAdvisorySwitch
bool RtiAmb_Hla1516e::disableInteractionRelevanceAdvisorySwitch(void) {
	try {
		rti->disableInteractionRelevanceAdvisorySwitch();
		String^ msg = "Interaction Relevance Advisory Switch is disabled.";
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return true;
	}
#pragma region exceptions
	catch (InteractionRelevanceAdvisorySwitchIsOff& e) {
		String^ msg = "MSG-(InteractionRelevanceAdvisorySwitchIsOff - disableInteractionRelevanceAdvisorySwitch):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "MSG-(FederateNotExecutionMember - disableInteractionRelevanceAdvisorySwitch):" + Environment::NewLine + "Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (SaveInProgress& e) {
		String^ msg = "MSG-(SaveInProgress - disableInteractionRelevanceAdvisorySwitch):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "MSG-(RestoreInProgress - disableInteractionRelevanceAdvisorySwitch):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (NotConnected& e) {
		String^ msg = "MSG-(NotConnected - disableInteractionRelevanceAdvisorySwitch):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg));
	}
	catch (RTIinternalError& e) {
		MessageBox::Show("MSG-(RTIinternalError - disableInteractionRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	catch (System::Exception^ e) {
		MessageBox::Show("MSG-(GeneralException - disableInteractionRelevanceAdvisorySwitch):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
#pragma endregion
};

// 10.41 - Evoke Callback
void RtiAmb_Hla1516e::evokeCallback(double approximateMinimumTimeInSeconds) {
	try {
		rti->evokeCallback(approximateMinimumTimeInSeconds);
		//rti->evokeMultipleCallbacks(0.1, 1.0);
	}
#pragma region exceptions
	catch (CallNotAllowedFromWithinCallback& e) {
		String^ msg = "MSG-(CallNotAllowedFromWithinCallback - evokeCallback):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (NotConnected& e) {
		String^ msg = "MSG-(NotConnected - evokeCallback):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg));
	}
	catch (RTIinternalError& e) {
		MessageBox::Show("MSG-(RTIinternalError - evokeCallback):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	catch (System::Exception^ e) {
		MessageBox::Show("MSG-(GeneralException - evokeCallback):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
#pragma endregion
};

// 10.42 - Evoke Multiple Callbacks
void RtiAmb_Hla1516e::evokeMultipleCallbacks(double approximateMinimumTimeInSeconds, double approximateMaximumTimeInSeconds) {
	try {
		rti->evokeMultipleCallbacks(approximateMinimumTimeInSeconds, approximateMaximumTimeInSeconds);
		//rti->evokeMultipleCallbacks(0.1, 1.0);
	}
#pragma region exceptions
	catch (CallNotAllowedFromWithinCallback& e) {
		String^ msg = "MSG-(CallNotAllowedFromWithinCallback - evokeMultipleCallbacks):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str()) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (NotConnected& e) {
		String^ msg = "MSG-(NotConnected - evokeMultipleCallbacks):" + Environment::NewLine + " Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg));
	}
	catch (RTIinternalError& e) {
		MessageBox::Show("MSG-(RTIinternalError - evokeMultipleCallbacks):" + Environment::NewLine + gcnew String(e.what().c_str()) + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	catch (System::Exception^ e) {
		MessageBox::Show("MSG-(GeneralException - evokeMultipleCallbacks):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
#pragma endregion
};

// getDimensionHandle
unsigned int RtiAmb_Hla1516e::getDimensionHandle(String^ dim, unsigned int space) {
	return 0;
};

#pragma endregion // Support Services

#pragma region Helpers
#pragma endregion // Helpers			
