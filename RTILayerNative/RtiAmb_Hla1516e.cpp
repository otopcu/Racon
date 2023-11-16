/*
Racon - RTI abstraction component for MS.NET (Racon)
https://sites.google.com/site/okantopcu/racon

Copyright © Okan Topçu, 2009-2019
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

#include "include/StdAfx.h"
#include <vcclr.h>
#include "RTI/RTI1516.h"
#include "RTI/time/HLAfloat64Interval.h"
#include "RTI/time/HLAfloat64Time.h"
#include "include/RtiAmb_Hla1516e.h"
#include <assert.h>

using namespace rti1516e;
using namespace System;
using namespace System::Runtime::InteropServices;// For Marshalling
//using namespace System::Windows::Forms;
using namespace System::Threading; // for sleep
// Racon
using namespace Racon;
using namespace Racon::RtiLayer;
using namespace Racon::RtiLayer::Native;


#pragma region Constructor
RtiAmb_Hla1516e::RtiAmb_Hla1516e(CallbackManager^ eventManager) : RtiAmb() {
	try {
		RTIambassadorFactory factory = RTIambassadorFactory();
		this->rti = factory.createRTIambassador().release();
		nom = new NomHla1516e();
		_nativeFdAmb = new FdAmb_Hla1516e(eventManager, nom);
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(RtiAmb_Hla1516e Ctor). The native rti ambassador pointer creation  is failed. This may indicate a corrupted/wrong RTI.rid file. Please ensure that RTI.rid is provided by Portico distribution. Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
};
#pragma endregion			

#pragma region Federation Management
// 4.2 - Connect
void RtiAmb_Hla1516e::connect(Racon::CallbackModel callbackModel, String ^localSetting) {
	try
	{
		pin_ptr<const wchar_t> addr = PtrToStringChars(localSetting);

		rti->connect(*this->_nativeFdAmb, (rti1516e::CallbackModel)callbackModel, addr);
		String^ msg = "Federate application is connected.";
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
		String^ msg = "RtiAmb_Hla1516e-(ConnectionFailed - connect). Check that rti (e.g. rtinode for OpenRTI) is running. Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (InvalidLocalSettingsDesignator& e) {
		String^ msg = "RtiAmb_Hla1516e-(InvalidLocalSettingsDesignator - connect). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (UnsupportedCallbackModel& e) {
		String^ msg = "RtiAmb_Hla1516e-(UnsupportedCallbackModel - connect). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (AlreadyConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(AlreadyConnected - connect). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (CallNotAllowedFromWithinCallback& e) {
		String^ msg = "RtiAmb_Hla1516e-(CallNotAllowedFromWithinCallback - connect). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - connect). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

// 4.3 - Disconnect
void RtiAmb_Hla1516e::disconnect() {
	try
	{
		rti->disconnect();
		String^ msg = "Federate application is disconnected.";
		this->OnFederateDisconnected(gcnew RaconEventArgs(msg, LogLevel::INFO));
	}
#pragma region exceptions
	catch (FederateIsExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateIsExecutionMember - disconnect). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - disconnect). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - disconnect). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - disconnect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

// 4.5 - Create Federation w/ single FOM
void RtiAmb_Hla1516e::createFederation(String ^fedexec, String ^fdd, String^ logicalTimeImplementationName) {
	try {
		pin_ptr<const wchar_t> _fedexec = PtrToStringChars(fedexec);
		pin_ptr<const wchar_t> _fdd = PtrToStringChars(fdd);
		pin_ptr<const wchar_t> _logicalTimeImplementationName = PtrToStringChars(logicalTimeImplementationName);

		rti->createFederationExecution(_fedexec, _fdd, _logicalTimeImplementationName);
		String^ msg = "Federation execution (name: " + fedexec + ") is created.FDD file: \"" + fdd + "\"";
		this->OnFederationExecutionCreated(gcnew RaconEventArgs(msg, LogLevel::INFO));
	}
#pragma region exceptions
	catch (CouldNotCreateLogicalTimeFactory& e) {
		String^ msg = "RtiAmb_Hla1516e-(CouldNotCreateLogicalTimeFactory - createFederation). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (InconsistentFDD& e) {
		String^ msg = "RtiAmb_Hla1516e-(InconsistentFDD - createFederation). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (ErrorReadingFDD& e) {
		String^ msg = "RtiAmb_Hla1516e-(ErrorReadingFDD - createFederation). Invalid FOM Module. Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (CouldNotOpenFDD& e) {
		String^ msg = "RtiAmb_Hla1516e-(CouldNotOpenFDD - createFederation). Federate cannot open the FDD file. Either the file does not exist or there exists an error in the file name or path provided. Please, check that FDD file exists and its path is correct. Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederationExecutionAlreadyExists&)
	{
		String^ msg = "RtiAmb_Hla1516e-Federation execution (name: " + fedexec + ") already exists.";
		this->OnFederationExecutionCreated(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - createFederation). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - createFederation). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - createFederation). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (const rti1516e::Exception& e) {
		String^ msg = "RtiAmb_Hla1516e-(rti1516e::Exception: - createFederation). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
#pragma endregion
};
// 4.5 - Create Federation w/ multiple FOM modules
void RtiAmb_Hla1516e::createFederation(String ^fedexec, List<String^>^ fomModules, String^ logicalTimeImplementationName) {
	try {
		pin_ptr<const wchar_t> _fedexec = PtrToStringChars(fedexec);
		pin_ptr<const wchar_t> _logicalTimeImplementationName = PtrToStringChars(logicalTimeImplementationName);
		std::vector<std::wstring> modules;
		for each (String^ var in fomModules)
		{
			pin_ptr<const wchar_t> module = PtrToStringChars(var);
			modules.push_back(module);
		}

		rti->createFederationExecution(_fedexec, modules, _logicalTimeImplementationName);

		String^ list = String::Join(", ", fomModules);// fomModules to string
		String^ msg = "Federation execution (name: " + fedexec + ") is created with multiple FOM modules: " + list;
		this->OnFederationExecutionCreated(gcnew RaconEventArgs(msg, LogLevel::INFO));
	}
#pragma region exceptions
	catch (CouldNotCreateLogicalTimeFactory& e) {
		String^ msg = "RtiAmb_Hla1516e-(CouldNotCreateLogicalTimeFactory - createFederation2). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (InconsistentFDD& e) {
		String^ msg = "RtiAmb_Hla1516e-(InconsistentFDD - createFederation2). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (ErrorReadingFDD& e) {
		String^ msg = "RtiAmb_Hla1516e-(ErrorReadingFDD - createFederation2). Invalid FOM Module. Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (CouldNotOpenFDD& e) {
		String^ msg = "RtiAmb_Hla1516e-(CouldNotOpenFDD - createFederation2). Federate cannot open the FDD file. Either the file does not exist or there exists an error in the file name or path provided. Please, check that FDD file exists and its path is correct. Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederationExecutionAlreadyExists&)
	{
		String^ msg = "RtiAmb_Hla1516e-Federation execution (name: " + fedexec + ") already exists.";
		this->OnFederationExecutionCreated(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - createFederation2). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - createFederation2). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - createFederation2). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (const rti1516e::Exception& e) {
		String^ msg = "RtiAmb_Hla1516e-(rti1516e::Exception: - createFederation2). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
#pragma endregion
};
// 4.5 - Create Federation w/ multiple FOM modules and a MIM
void RtiAmb_Hla1516e::createFederation(String ^fedexec, List<String^>^ fomModules, String ^mimModule, String^ logicalTimeImplementationName) {
	try {
		pin_ptr<const wchar_t> _fedexec = PtrToStringChars(fedexec);
		pin_ptr<const wchar_t> _logicalTimeImplementationName = PtrToStringChars(logicalTimeImplementationName);
		pin_ptr<const wchar_t> _mimModule = PtrToStringChars(mimModule);
		std::vector<std::wstring> modules;
		for each (String^ var in fomModules)
		{
			pin_ptr<const wchar_t> module = PtrToStringChars(var);
			modules.push_back(module);
		}

		rti->createFederationExecutionWithMIM(_fedexec, modules, _mimModule, _logicalTimeImplementationName);

		String^ list = String::Join(", ", fomModules);// fomModules to string
		String^ msg = "Federation execution (name: " + fedexec + ") is created with multiple FOM modules and with a MIM module. Fom modules: " + list;
		this->OnFederationExecutionCreated(gcnew RaconEventArgs(msg, LogLevel::INFO));
	}
#pragma region exceptions
	catch (CouldNotCreateLogicalTimeFactory& e) {
		String^ msg = "RtiAmb_Hla1516e-(CouldNotCreateLogicalTimeFactory - createFederation3). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (InconsistentFDD& e) {
		String^ msg = "RtiAmb_Hla1516e-(InconsistentFDD - createFederation3). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (ErrorReadingFDD& e) {
		String^ msg = "RtiAmb_Hla1516e-(ErrorReadingFDD - createFederation3). Invalid FOM Module. Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (CouldNotOpenFDD& e) {
		String^ msg = "RtiAmb_Hla1516e-(CouldNotOpenFDD - createFederation3). Federate cannot open the FDD file. Either the file does not exist or there exists an error in the file name or path provided. Please, check that FDD file exists and its path is correct. Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (DesignatorIsHLAstandardMIM& e) {
		String^ msg = "RtiAmb_Hla1516e-(DesignatorIsHLAstandardMIM - createFederation3).MIM designator shall not be HLAstandardMIM. Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (ErrorReadingMIM& e) {
		String^ msg = "RtiAmb_Hla1516e-(ErrorReadingMIM - createFederation3).Invalid MIM. Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (CouldNotOpenMIM& e) {
		String^ msg = "RtiAmb_Hla1516e-(CouldNotOpenMIM - createFederation3). Could not locate MIM indicated by supplied designator. Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederationExecutionAlreadyExists&)
	{
		String^ msg = "RtiAmb_Hla1516e-Federation execution (name: " + fedexec + ") already exists.";
		this->OnFederationExecutionCreated(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - createFederation3). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - createFederation3). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - createFederation3). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (const rti1516e::Exception& e) {
		String^ msg = "RtiAmb_Hla1516e-(rti1516e::Exception: - createFederation3). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
#pragma endregion
};

// 4.6 - Destroy Federation
void RtiAmb_Hla1516e::destroyFederation(String ^fedexec) {
	try {
		pin_ptr<const wchar_t> fedx = PtrToStringChars(fedexec);
		rti->destroyFederationExecution(fedx);
		String^ msg = "Federation execution (name: " + fedexec + ") is finalized.";
		this->OnFederationExecutionDestroyed(gcnew RaconEventArgs(msg, LogLevel::INFO));
	}
#pragma region exceptions
	catch (FederatesCurrentlyJoined& e) {
		String^ msg = "There are still federates joined in the federation. Federation execution is not destroyed. " + "Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (FederationExecutionDoesNotExist& e) {
		String^ msg = "Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - destroyFederation). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - destroyFederation). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - destroyFederation). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

// 4.7 - listFederationExecutions
void RtiAmb_Hla1516e::listFederationExecutions() {
	try {
		rti->listFederationExecutions();
		String^ msg = "A list of currently existing federation executions is requested.";
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::INFO));
	}
#pragma region exceptions
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - listFederationExecutions). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - listFederationExecutions). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - listFederationExecutions). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

// 4.9 - Join Federation (1)
unsigned int RtiAmb_Hla1516e::joinFederationExecution(String ^federateType, String ^federationExecutionName, List<String^>^ fomModules) {
	bool joined = false;
	int numTries = 0;
	while ((numTries++ < 20) && !joined) {
		try {
			pin_ptr<const wchar_t> _federateType = PtrToStringChars(federateType);
			pin_ptr<const wchar_t> _federationExecutionName = PtrToStringChars(federationExecutionName);
			rti1516e::FederateHandle _federateHandle;

			if (fomModules == nullptr)
				_federateHandle = rti->joinFederationExecution(_federateType, _federationExecutionName);
			else
			{
				std::vector<std::wstring> modules;
				for each (String^ var in fomModules)
				{
					pin_ptr<const wchar_t> module = PtrToStringChars(var);
					modules.push_back(module);
				}
				_federateHandle = rti->joinFederationExecution(_federateType, _federationExecutionName, modules);
			}

			joined = true;
			// Keep Federate Handle
			nom->FederateHandle = _federateHandle;
			ULong handle = Handle2Long(_federateHandle);
			String^ msg = "Federate joined to the federation execution (" + gcnew String(_federationExecutionName) + "). Federate Type: " + gcnew String(_federateType) + ". Federate Handle: " + handle.ToString();
			this->OnFederateJoined(gcnew RaconEventArgs(msg, LogLevel::INFO));
			return handle;
		}
#pragma region exceptions
		catch (CouldNotCreateLogicalTimeFactory& e) {
			String^ msg = "RtiAmb_Hla1516e-(CouldNotCreateLogicalTimeFactory - joinFederation). Reason: " + gcnew String(e.what().c_str());
			this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		}
		catch (FederationExecutionDoesNotExist& e) {
			String^ msg = "RtiAmb_Hla1516e-(InconsistentFDD - joinFederation). Federation Execution does not exist. Please, restart the federate. Reason: " + gcnew String(e.what().c_str());
			this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		}
		catch (InconsistentFDD& e) {
			String^ msg = "RtiAmb_Hla1516e-(InconsistentFDD - joinFederation). Inconsistent FOM Module. Reason: " + gcnew String(e.what().c_str());
			this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		}
		catch (ErrorReadingFDD& e) {
			String^ msg = "RtiAmb_Hla1516e-(ErrorReadingFED - joinFederation). Invalid FOM Module. Reason:  " + gcnew String(e.what().c_str());
			this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		}
		catch (CouldNotOpenFDD& e) {
			String^ msg = "RtiAmb_Hla1516e-(CouldNotOpenFDD - joinFederation). Federate cannot open the FED file. Either the file does not exist or there exists an error in the file name or path provided. Please, check that FED file exists and its path is correct. Reason: " + gcnew String(e.what().c_str());
			this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		}
		catch (SaveInProgress& e) {
			String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - joinFederation). Reason: " + gcnew String(e.what().c_str());
			this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		}
		catch (RestoreInProgress& e) {
			String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - joinFederation). Reason: " + gcnew String(e.what().c_str());
			this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		}
		catch (FederateAlreadyExecutionMember& e) {
			String^ msg = "RtiAmb_Hla1516e-(FederateAlreadyExecutionMember - joinFederation): " + gcnew String(e.what().c_str());
			this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		}
		catch (NotConnected& e) {
			String^ msg = "RtiAmb_Hla1516e-(NotConnected - joinFederation). Reason: " + gcnew String(e.what().c_str());
			this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		}
		catch (CallNotAllowedFromWithinCallback & e) {
			String^ msg = "RtiAmb_Hla1516e-(CallNotAllowedFromWithinCallback - joinFederation). Reason: " + gcnew String(e.what().c_str());
			this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		}
		catch (RTIinternalError& e) {
			String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - joinFederation). Reason: " + gcnew String(e.what().c_str());
			this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		}
		catch (System::Exception^ e) {
			String^ msg = "RtiAmb_Hla1516e-(Exception - joinFederation). Reason: " + gcnew String(e->ToString());
			this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		}
#pragma endregion
	} // end of while
};
// 4.9 - Join Federation (2)
unsigned int RtiAmb_Hla1516e::joinFederationExecution(String ^federateName, String ^federateType, String ^federationExecutionName, List<String^>^ fomModules) {
	bool joined = false;
	int numTries = 0;
	while ((numTries++ < 20) && !joined) {
		try {
			pin_ptr<const wchar_t> _federateName = PtrToStringChars(federateName);
			pin_ptr<const wchar_t> _federateType = PtrToStringChars(federateType);
			pin_ptr<const wchar_t> _federationExecutionName = PtrToStringChars(federationExecutionName);
			rti1516e::FederateHandle federateHandle;

			if (fomModules == nullptr)
				federateHandle = rti->joinFederationExecution(_federateName, _federateType, _federationExecutionName);
			else
			{
				std::vector<std::wstring> modules;
				for each (String^ var in fomModules)
				{
					pin_ptr<const wchar_t> module = PtrToStringChars(var);
					modules.push_back(module);
				}
				federateHandle = rti->joinFederationExecution(_federateName, _federateType, _federationExecutionName, modules);
			}
			joined = true;
			// Keep Federate Handle
			nom->FederateHandle = federateHandle;
			//rti->getFederateName(federateHandle);
			ULong handle = Handle2Long(federateHandle);
			String^ msg = "Federate joined to the federation execution (" + gcnew String(_federationExecutionName) + "). Federate Name: " + gcnew String(_federateName) + ". Federate Type: " + gcnew String(_federateType) + ". Federate Handle: " + handle.ToString();
			this->OnFederateJoined(gcnew RaconEventArgs(msg, LogLevel::INFO));
			return handle;
		}
#pragma region exceptions
		catch (CouldNotCreateLogicalTimeFactory& e) {
			String^ msg = "RtiAmb_Hla1516e-(CouldNotCreateLogicalTimeFactory - joinFederation2). Reason: " + gcnew String(e.what().c_str());
			this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		}
		catch (FederateNameAlreadyInUse & e) {
			String^ msg = "RtiAmb_Hla1516e-(FederateNameAlreadyInUse - joinFederation2). Possible solution: (1) Ensure that the provided federate name is unique in the federation execution. Reason: " + gcnew String(e.what().c_str());
			this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
			throw;
		}
		catch (FederationExecutionDoesNotExist& e) {
			String^ msg = "RtiAmb_Hla1516e-(FederationExecutionDoesNotExist - joinFederation2). Possible solution: (1) Federation Execution does not exist. Please, restart the federate. Reason: " + gcnew String(e.what().c_str());
			this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		}
		catch (InconsistentFDD& e) {
			String^ msg = "RtiAmb_Hla1516e-(InconsistentFDD - joinFederation2). Possible solution: (1) Inconsistent FOM Module. Reason: " + gcnew String(e.what().c_str());
			this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		}
		catch (ErrorReadingFDD& e) {
			String^ msg = "RtiAmb_Hla1516e-(ErrorReadingFDD - joinFederation2). Possible solution: (1) Invalid FOM Module. Reason:  " + gcnew String(e.what().c_str());
			this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		}
		catch (CouldNotOpenFDD& e) {
			String^ msg = "RtiAmb_Hla1516e-(CouldNotOpenFDD - joinFederation2). Possible solution: (1) Federate cannot open the FED file. Either the file does not exist or there exists an error in the file name or path provided. Please, check that FED file exists and its path is correct. Reason: " + gcnew String(e.what().c_str());
			this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		}
		catch (SaveInProgress& e) {
			String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - joinFederation2). Reason: " + gcnew String(e.what().c_str());
			this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		}
		catch (RestoreInProgress& e) {
			String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - joinFederation2). Reason: " + gcnew String(e.what().c_str());
			this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		}
		catch (FederateAlreadyExecutionMember& e) {
			String^ msg = "RtiAmb_Hla1516e-(FederateAlreadyExecutionMember - joinFederation2): Federation execution name is " + federationExecutionName + ". Reason: " + gcnew String(e.what().c_str());
			this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		}
		catch (NotConnected& e) {
			String^ msg = "RtiAmb_Hla1516e-(NotConnected - joinFederation2). Reason: " + gcnew String(e.what().c_str());
			this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
			throw;
		}
		catch (CallNotAllowedFromWithinCallback & e) {
			String^ msg = "RtiAmb_Hla1516e-(CallNotAllowedFromWithinCallback - joinFederation2). Reason: " + gcnew String(e.what().c_str());
			this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		}
		catch (RTIinternalError& e) {
			String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - joinFederation2). Reason: " + gcnew String(e.what().c_str());
			this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
			throw;
		}
		catch (System::Exception^ e) {
			String^ msg = "RtiAmb_Hla1516e-(Exception - joinFederation2). Reason:"  + gcnew String(e->ToString());
			this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
			throw;
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
		this->OnFederateResigned(gcnew RaconEventArgs(msg, LogLevel::INFO));
	}
#pragma region exceptions
	catch (InvalidResignAction& e) {
		String^ msg = "RtiAmb_Hla1516e-(InvalidResignAction - resignFederation). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (OwnershipAcquisitionPending& e) {
		String^ msg = "RtiAmb_Hla1516e-(OwnershipAcquisitionPending - resignFederation). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (AttributeAlreadyOwned& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - resignFederation): " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - resignFederation): " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (FederationExecutionDoesNotExist& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - resignFederation): " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - resignFederation). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - resignFederation). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - resignFederation). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
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
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - registerFederationSynchronizationPoint). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - registerFederationSynchronizationPoint). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - registerFederationSynchronizationPoint).Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - registerFederationSynchronizationPoint). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - registerFederationSynchronizationPoint). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - registerFederationSynchronizationPoint). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
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
		String^ msg = "RtiAmb_Hla1516e-(InvalidFederateHandle - registerFederationSynchronizationPoint2). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - registerFederationSynchronizationPoint2). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - registerFederationSynchronizationPoint2). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - registerFederationSynchronizationPoint2).Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - registerFederationSynchronizationPoint2). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - registerFederationSynchronizationPoint2). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - connect). registerFederationSynchronizationPoint2: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
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
		String^ msg = "RtiAmb_Hla1516e-(SynchronizationPointLabelNotAnnounced - synchronizationPointAchieved). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - synchronizationPointAchieved). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - synchronizationPointAchieved). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - synchronizationPointAchieved).Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - synchronizationPointAchieved). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - synchronizationPointAchieved). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - synchronizationPointAchieved). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
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
void RtiAmb_Hla1516e::publishObjectClass(HlaObjectClass ^oc, List<HlaAttribute^>^ attributes)
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

		rti1516e::ObjectClassHandle ocHandle = getObjectClassHandle(oc);
		rti1516e::AttributeHandleSet ahs;
		// Add attribute handles to AHS
		for (int j = 0; j < tmpList->Count; j++) {
			rti1516e::AttributeHandle handle = getAttributeHandle(tmpList[j], ocHandle);
			ahs.insert(handle);
		}

		rti->publishObjectClassAttributes(ocHandle, ahs); // *ahs:ordinary reference to native object

		String^ list = String::Join(", ", attributes);// attributes to string
		String^ msg = "Object Class (" + oc->Name + ") is published with its all publishable attributes. Class handle: " + Handle2Long(ocHandle).ToString() + ", Attributes: {" + list + "}";
		this->OnHLAClassPublished(gcnew RaconEventArgs(msg));
	}
#pragma region exceptions
	catch (AttributeNotDefined& e) {
		String^ msg = "RtiAmb_Hla1516e-(AttributeNotDefined - publishObjectClass). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (ObjectClassNotDefined& e) {
		String^ msg = "RtiAmb_Hla1516e-(ObjectClassNotDefined - publishObjectClass). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - publishObjectClass). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - publishObjectClass). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - publishObjectClass).Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - publishObjectClass). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - publishObjectClass). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - publishObjectClass). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

// 5.3 - unpublishOC
bool RtiAmb_Hla1516e::unpublishObjectClass(HlaObjectClass^ oc, List<HlaAttribute^>^ attributes)
{
	try{
		rti1516e::ObjectClassHandle ocHandle = getObjectClassHandle(oc);
		rti1516e::AttributeHandleSet ahs;
		// Add attribute handles to AHS
		for (int j = 0; j < attributes->Count; j++) {
			rti1516e::AttributeHandle handle = getAttributeHandle(attributes[j], ocHandle);
			ahs.insert(handle);
		}

		rti->unpublishObjectClassAttributes(ocHandle, ahs); // *ahs:ordinary reference to native object

		String^ list = String::Join(", ", attributes);// attributes to string
		String^ msg = "Object Class (" + oc->Name + ") is unpublished with the following attributes. Class handle: " + Handle2Long(ocHandle).ToString() + ", Attributes: {" + list + "}";
		this->OnHLAClassPublished(gcnew RaconEventArgs(msg));
		return true;
	}
#pragma region exceptions
	catch (OwnershipAcquisitionPending & e) {
		String^ msg = "RtiAmb_Hla1516e-(OwnershipAcquisitionPending - unpublishObjectClass). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (AttributeNotDefined & e) {
		String^ msg = "RtiAmb_Hla1516e-(AttributeNotDefined - unpublishObjectClass). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (ObjectClassNotDefined & e) {
		String^ msg = "RtiAmb_Hla1516e-(ObjectClassNotDefined - unpublishObjectClass). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (SaveInProgress & e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - unpublishObjectClass). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (RestoreInProgress & e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - unpublishObjectClass). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (FederateNotExecutionMember & e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - unpublishObjectClass).Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (NotConnected & e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - unpublishObjectClass). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
	catch (RTIinternalError & e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - unpublishObjectClass). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
	catch (System::Exception ^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - unpublishObjectClass). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
#pragma endregion
};

// 5.4 - publishIC
bool RtiAmb_Hla1516e::publishInteractionClass(HlaInteractionClass ^ic) {
	// Check that interaction is Publishable
	if ((ic->ClassPS == PSKind::Publish) || (ic->ClassPS == PSKind::PublishSubscribe))
		try {
		pin_ptr<const wchar_t> name = PtrToStringChars(ic->Name);
		rti1516e::InteractionClassHandle handle = getInteractionClassHandle(ic);
		rti->publishInteractionClass(handle);
		String^ msg = "Interaction Class (name: " + ic->Name + ", handle: " + ic->Handle + ") is published.";
		this->OnHLAClassPublished(gcnew RaconEventArgs(msg));
		return true;
	}
#pragma region exceptions
	catch (InteractionClassNotDefined& e) {
		String^ msg = "RtiAmb_Hla1516e-(InteractionClassNotDefined - publishInteractionClass). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - publishInteractionClass). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - publishInteractionClass). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - publishInteractionClass).Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - publishInteractionClass). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - publishInteractionClass). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - publishInteractionClass). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
#pragma endregion
};

// 5.5 - unpublishInteractionClass
bool RtiAmb_Hla1516e::unpublishInteractionClass(HlaInteractionClass^ ic) {
	try {
		rti1516e::InteractionClassHandle _handle = getInteractionClassHandle(ic);

		rti->unpublishInteractionClass(_handle);

		String^ msg = "Interaction Class (name: " + ic->Name + ", handle: " + ic->Handle + ") is unpublished.";

		this->OnHLAClassPublished(gcnew RaconEventArgs(msg));
		return true;
	}
#pragma region exceptions
	catch (InteractionClassNotDefined & e) {
		String^ msg = "RtiAmb_Hla1516e-(InteractionClassNotDefined - unpublishInteractionClass). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (SaveInProgress & e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - unpublishInteractionClass). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (RestoreInProgress & e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - unpublishInteractionClass). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (FederateNotExecutionMember & e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - unpublishInteractionClass).Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (NotConnected & e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - unpublishInteractionClass). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
	catch (RTIinternalError & e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - unpublishInteractionClass). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
	catch (System::Exception ^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - unpublishInteractionClass). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
#pragma endregion
};

// 5.6 - subscribeOC with selected attributes
bool RtiAmb_Hla1516e::subscribeObjectClass(HlaObjectClass ^oc, List<HlaAttribute^>^ attributes, Boolean active, String^ updateRate)
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
		pin_ptr<const wchar_t> _updateRateDesignator = PtrToStringChars(updateRate);

		// Add attribute handles to AHS
		for (int j = 0; j < tmpList->Count; j++) {
			rti1516e::AttributeHandle handle = getAttributeHandle(tmpList[j], ocHandle);
			ahs.insert(handle);
		}

		rti->subscribeObjectClassAttributes(ocHandle, ahs, _updateRateDesignator);

		String^ list = String::Join(", ", attributes);// attributes to string
		String^ msg = "Object Class (" + oc->Name + ") is subscribed with all its subscribable attributes. Class handle: " + Handle2Long(ocHandle).ToString() + ", Attributes: {" + list + "}";
		if (updateRate != "")	msg += ", Update Rate Designator: " + updateRate;
		this->OnHLAClassPublished(gcnew RaconEventArgs(msg));
		return true;
	}
#pragma region exceptions
	catch (ObjectClassNotDefined& e) {
		String^ msg = "RtiAmb_Hla1516e-(ObjectClassNotDefined - subscribeObjectClass). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (AttributeNotDefined& e) {
		String^ msg = "RtiAmb_Hla1516e-(AttributeNotDefined - subscribeObjectClass). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - subscribeObjectClass). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - subscribeObjectClass). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - subscribeObjectClass).Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (InvalidUpdateRateDesignator& e) {
		String^ msg = "RtiAmb_Hla1516e-(InvalidUpdateRateDesignator - subscribeObjectClass).Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - subscribeObjectClass). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - subscribeObjectClass). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - subscribeObjectClass). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

// 5.7 - unsubscribe OC 
bool RtiAmb_Hla1516e::unsubscribeObjectClass(HlaObjectClass ^oc, List<HlaAttribute^>^ attributes) {
	try {
		rti1516e::ObjectClassHandle ocHandle = getObjectClassHandle(oc);
		rti1516e::AttributeHandleSet ahs;

		// Add attribute handles to AHS
		for (int j = 0; j < attributes->Count; j++) {
			rti1516e::AttributeHandle handle = getAttributeHandle(attributes[j], ocHandle);
			ahs.insert(handle);
		}

		rti->unsubscribeObjectClassAttributes(ocHandle, ahs);

		String^ list = String::Join(", ", attributes);// attributes to string
		String^ msg = "Object Class (" + oc->Name + ") is unsubscribed with the following attributes. Class handle: " + Handle2Long(ocHandle).ToString() + ", Attributes: {" + list + "}";
		this->OnHLAClassPublished(gcnew RaconEventArgs(msg));
		return true;
	}
#pragma region exceptions
	catch (AttributeNotDefined & e) {
		String^ msg = "RtiAmb_Hla1516e-(AttributeNotDefined - unsubscribeObjectClass). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (ObjectClassNotDefined & e) {
		String^ msg = "RtiAmb_Hla1516e-(ObjectClassNotDefined - unsubscribeObjectClass). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (SaveInProgress & e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - unsubscribeObjectClass). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (RestoreInProgress & e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - unsubscribeObjectClass). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (FederateNotExecutionMember & e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - unsubscribeObjectClass).Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (NotConnected & e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - unsubscribeObjectClass). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
	catch (RTIinternalError & e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - unsubscribeObjectClass). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (System::Exception ^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - unsubscribeObjectClass). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

// 5.8 - SubscribeIC
bool RtiAmb_Hla1516e::subscribeInteractionClass(HlaInteractionClass ^ic) {
	try {
		// Check that interaction is Subscribable
		if ((ic->ClassPS == PSKind::Subscribe) || (ic->ClassPS == PSKind::PublishSubscribe))
		{
			rti1516e::InteractionClassHandle handle = getInteractionClassHandle(ic);
			rti->subscribeInteractionClass(handle);
			String^ msg = "Interaction Class (name: " + ic->Name + ", handle: " + ic->Handle + ") is subscribed.";
			this->OnHLAClassSubscribed(gcnew RaconEventArgs(msg));
		}
		return true;
	}
#pragma region exceptions
	catch (FederateServiceInvocationsAreBeingReportedViaMOM& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateServiceInvocationsAreBeingReportedViaMOM - subscribeInteractionClass). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (InteractionClassNotDefined& e) {
		String^ msg = "RtiAmb_Hla1516e-(InteractionClassNotDefined - subscribeInteractionClass). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - subscribeInteractionClass). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - subscribeInteractionClass). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - subscribeInteractionClass).Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - subscribeInteractionClass). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - subscribeInteractionClass). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - subscribeInteractionClass). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
#pragma endregion
};

// 5.9 - unsubscribeIC
bool RtiAmb_Hla1516e::unsubscribeInteractionClass(HlaInteractionClass ^ic) {
	try {
		rti1516e::InteractionClassHandle _handle = getInteractionClassHandle(ic);

		rti->unsubscribeInteractionClass(_handle);

		String^ msg = "Interaction Class (name: " + ic->Name + ", handle: " + ic->Handle + ") is unsubscribed.";

		this->OnHLAClassSubscribed(gcnew RaconEventArgs(msg));
		return true;
	}
#pragma region exceptions
	catch (InteractionClassNotDefined & e) {
		String^ msg = "RtiAmb_Hla1516e-(InteractionClassNotDefined - unsubscribeInteractionClass). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (SaveInProgress & e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - unsubscribeInteractionClass). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (RestoreInProgress & e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - unsubscribeInteractionClass). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (FederateNotExecutionMember & e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - unsubscribeInteractionClass).Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (NotConnected & e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - unsubscribeInteractionClass). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
	catch (RTIinternalError & e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - unsubscribeInteractionClass). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
	catch (System::Exception ^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - unsubscribeInteractionClass). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
#pragma endregion
};

#pragma endregion	// Declaration Management		

#pragma region Object Management

// 6.10 - Update Attribute Values
bool RtiAmb_Hla1516e::updateAttributeValues(HlaObject ^object, VariableLengthDataWrapper^ userTag) {
	try {
		String^ msg = "";
		// Pack attribute values
		rti1516e::AttributeHandleValueMap attributes;
		// Create Tag
		VariableLengthData tag(userTag->Data.ToPointer(), userTag->Size);

		for each (HlaAttribute^ var in object->Attributes)
		{
			char* _pVal = (char*)var->Value.ToPointer();
			//msg += "Size: " + var->Length;
			rti1516e::VariableLengthData data(_pVal, var->Length);
			//rti1516e::VariableLengthData data(_pVal, ((strlen(_pVal) + 1)*sizeof(char)));
			attributes[nom->Attributes[var->Handle]] = data;
		}
		// Consume Attributes
		object->Attributes->Clear();
		// Update attribute values using RTI
		rti1516e::ObjectInstanceHandle ohandle = nom->Objects[object->Handle];
		(void)rti->updateAttributeValues(ohandle, attributes, tag);
		msg = "Attribute values of object instance (handle: " + object->Handle + ") are updated. Reason: " + userTag->ToString();
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return true;
	}
#pragma region exceptions
	catch (ObjectInstanceNotKnown& e) {
		String^ msg = "RtiAmb_Hla1516e-(ObjectInstanceNotKnown - updateAttributeValues). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (AttributeNotDefined& e) {
		String^ msg = "RtiAmb_Hla1516e-(AttributeNotDefined - updateAttributeValues). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (AttributeNotOwned& e) {
		String^ msg = "RtiAmb_Hla1516e-(AttributeNotOwned - updateAttributeValues). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - updateAttributeValues).Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - updateAttributeValues). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - updateAttributeValues). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - updateAttributeValues). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - updateAttributeValues). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - updateAttributeValues). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

// 6.10 - Update Attribute Values with Timestamp
MessageRetraction^ RtiAmb_Hla1516e::updateAttributeValues(HlaObject ^object, VariableLengthDataWrapper^ userTag, Double timestamp) {
	try {
		// Pack attribute values
		rti1516e::AttributeHandleValueMap attributes;
		// Create Tag
		VariableLengthData tag(userTag->Data.ToPointer(), userTag->Size);
		for each (HlaAttribute^ var in object->Attributes)
		{
			char* _pVal = (char*)var->Value.ToPointer();
			rti1516e::VariableLengthData data(_pVal, var->Length);
			attributes[nom->Attributes[var->Handle]] = data;
		}
		// Consume Attributes
		object->Attributes->Clear();
		// Create timestamp
		std::auto_ptr<rti1516e::HLAfloat64Time> rtitime(new HLAfloat64Time(timestamp));
		// Update attribute values using RTI
		rti1516e::ObjectInstanceHandle ohandle = nom->Objects[object->Handle];
		rti1516e::MessageRetractionHandle mrh = rti->updateAttributeValues(ohandle, attributes, tag, *rtitime);
		// create retraction
		MessageRetraction^ retraction = gcnew MessageRetraction(toULong(mrh.toString()));
		nom->RetractionHandles[retraction->Handle] = mrh;
		// Report
		String^ msg = "Attribute values of object instance (handle: " + object->Handle + ") are updated." + " Retraction handle = " + gcnew String(mrh.toString().c_str()) + ". Reason: " + userTag->ToString();
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return retraction;
	}
#pragma region exceptions
	catch (ObjectInstanceNotKnown& e) {
		String^ msg = "RtiAmb_Hla1516e-(ObjectInstanceNotKnown - updateAttributeValues2). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (AttributeNotDefined& e) {
		String^ msg = "RtiAmb_Hla1516e-(AttributeNotDefined - updateAttributeValues2). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (AttributeNotOwned& e) {
		String^ msg = "RtiAmb_Hla1516e-(AttributeNotOwned - updateAttributeValues2). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - updateAttributeValues2).Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - updateAttributeValues2). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - updateAttributeValues2). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - updateAttributeValues2). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - updateAttributeValues2). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - updateAttributeValues2). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

// 6.12 - Send Interaction
bool RtiAmb_Hla1516e::sendInteraction(HlaInteraction ^interaction, VariableLengthDataWrapper^ userTag) {
	try {
		String ^msg = "";
		unsigned int chandle = interaction->ClassHandle;
		rti1516e::InteractionClassHandle handle = nom->InteractionClasses[chandle];

		// Create Tag
		VariableLengthData tag(userTag->Data.ToPointer(), userTag->Size);
		// Pack parameter values
		rti1516e::ParameterHandleValueMap Parameters;
		for each (HlaParameter^ var in interaction->Parameters)
		{
			char* _pVal = (char*)var->Value.ToPointer();
			//msg += "Handle: " + var->Name + " Size: " + var->Length + sizeof(double);
			rti1516e::VariableLengthData data(_pVal, var->Length);
			//rti1516e::VariableLengthData data(_pVal, ((strlen(_pVal) + 1)*sizeof(char)));
			Parameters[nom->Parameters[var->Handle]] = data;
		}

		rti->sendInteraction(handle, Parameters, tag);
		msg += "Interaction (handle: " + interaction->ClassHandle + ") is sent. Reason: " + userTag->ToString();
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return true;
	}
#pragma region exceptions
	catch (InteractionClassNotPublished &e) {
		String^ msg = "RtiAmb_Hla1516e-(InteractionClassNotPublished - sendInteraction): " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
	}
	catch (InteractionParameterNotDefined &e) {
		String^ msg = "RtiAmb_Hla1516e-(InteractionParameterNotDefined - sendInteraction): " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
	}
	catch (InteractionClassNotDefined &e) {
		String^ msg = "RtiAmb_Hla1516e-(InteractionClassNotDefined - sendInteraction): " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
	}
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - sendInteraction): " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - sendInteraction): " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - sendInteraction): " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - sendInteraction). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - sendInteraction). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - sendInteraction). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
#pragma endregion
};

// 6.12 - Send Interaction with Timestamp
MessageRetraction^ RtiAmb_Hla1516e::sendInteraction(HlaInteraction ^interaction, VariableLengthDataWrapper^ userTag, Double time) {
	try {
		String ^msg = "";
		unsigned int chandle = interaction->ClassHandle;
		rti1516e::InteractionClassHandle handle = nom->InteractionClasses[chandle];

		// Create Tag
		VariableLengthData tag(userTag->Data.ToPointer(), userTag->Size);
		// Create timestamp
		rti1516e::HLAfloat64Time timestamp(time);
		// Pack parameter values
		rti1516e::ParameterHandleValueMap Parameters;
		for each (HlaParameter^ var in interaction->Parameters)
		{
			char* _pVal = (char*)var->Value.ToPointer();
			rti1516e::VariableLengthData data(_pVal, var->Length);
			Parameters[nom->Parameters[var->Handle]] = data;
		}

		rti1516e::MessageRetractionHandle mrh = rti->sendInteraction(handle, Parameters, tag, timestamp);
		// create retraction
		MessageRetraction^ retraction = gcnew MessageRetraction(toULong(mrh.toString()));
		nom->RetractionHandles[retraction->Handle] = mrh;
		// Report
		msg += "Interaction (handle: " + interaction->ClassHandle + ") is sent with timestamp: " + timestamp.getTime() + ". Retraction handle: " + gcnew String(mrh.toString().c_str()) + ". Tag: " + userTag->ToString();
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return retraction;
	}
#pragma region exceptions
	catch (InvalidLogicalTime &e) {
		String^ msg = "RtiAmb_Hla1516e-(InvalidLogicalTime - sendInteraction2): One possible reason is that if lookahead is not zero, then timestamp must be greater than or equal to (logical time + lookahead), otherwise timestamp must be greater than (logical time + lookahead). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (InteractionClassNotPublished &e) {
		String^ msg = "RtiAmb_Hla1516e-(InteractionClassNotPublished - sendInteraction2): " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (InteractionParameterNotDefined &e) {
		String^ msg = "RtiAmb_Hla1516e-(InteractionParameterNotDefined - sendInteraction2): " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (InteractionClassNotDefined &e) {
		String^ msg = "RtiAmb_Hla1516e-(InteractionParameterNotDefined - sendInteraction2): " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(InteractionParameterNotDefined - sendInteraction2): " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(InteractionParameterNotDefined - sendInteraction2): " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(InteractionParameterNotDefined - sendInteraction2): " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - sendInteraction2). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - sendInteraction2). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - sendInteraction2). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

// 6.14 - Delete an Object Instance
bool RtiAmb_Hla1516e::deleteObjectInstance(HlaObject ^obj, VariableLengthDataWrapper^ userTag) {
	try {
		// Get handle
		rti1516e::ObjectInstanceHandle ohandle = nom->Objects[obj->Handle];
		// Create Tag
		VariableLengthData tag(userTag->Data.ToPointer(), userTag->Size);
		// Delete object
		rti->deleteObjectInstance(ohandle, tag);
		// Update Nom
		nom->Objects.erase(obj->Handle);
		// Report
		String^ msg = "Object (handle: " + obj->Handle + ") is deleted by this federate. Tag: " + userTag->ToString();
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return true;
	}
#pragma region exceptions
	catch (DeletePrivilegeNotHeld& e) {
		String^ msg = "RtiAmb_Hla1516e-(DeletePrivilegeNotHeld - deleteObjectInstance). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (ObjectInstanceNotKnown& e) {
		String^ msg = "RtiAmb_Hla1516e-(ObjectInstanceNotKnown - deleteObjectInstance). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - deleteObjectInstance). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - deleteObjectInstance). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - deleteObjectInstance). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - deleteObjectInstance). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - deleteObjectInstance). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - deleteObjectInstance). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

// 6.14 - Delete an Object Instance with Timestamp
MessageRetraction^ RtiAmb_Hla1516e::deleteObjectInstance(HlaObject ^obj, VariableLengthDataWrapper^ userTag, Double theTime) {
	try {
		// Get handle
		rti1516e::ObjectInstanceHandle ohandle = nom->Objects[obj->Handle];
		// Create Tag
		VariableLengthData tag(userTag->Data.ToPointer(), userTag->Size);
		// Create logical timestamp
		HLAfloat64Time timestamp(theTime);
		// Delete object
		rti1516e::MessageRetractionHandle mrh = rti->deleteObjectInstance(ohandle, tag, timestamp);
		// Update Nom
		nom->Objects.erase(obj->Handle);
		// create retraction
		//MessageRetraction^ retraction = gcnew MessageRetraction(toULong(mrh.toString()));
		MessageRetraction^ retraction = gcnew MessageRetraction(0);
		nom->RetractionHandles[retraction->Handle] = mrh;
		// Report
		String^ msg = "Object (handle: " + obj->Handle + ") is deleted by this federate. Tag: " + userTag->ToString() + ". Retraction handle: " + gcnew String(mrh.toString().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return retraction;
	}
#pragma region exceptions
	catch (InvalidLogicalTime& e) {
		String^ msg = "RtiAmb_Hla1516e-(InvalidLogicalTime - deleteObjectInstance2). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (DeletePrivilegeNotHeld& e) {
		String^ msg = "RtiAmb_Hla1516e-(DeletePrivilegeNotHeld - deleteObjectInstance2). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (ObjectInstanceNotKnown& e) {
		String^ msg = "RtiAmb_Hla1516e-(ObjectInstanceNotKnown - deleteObjectInstance2). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - deleteObjectInstance2). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - deleteObjectInstance2). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - deleteObjectInstance2). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - deleteObjectInstance2). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - deleteObjectInstance2). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - deleteObjectInstance2). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

// 6.19 - Request Attribute Value Update - oc
bool RtiAmb_Hla1516e::requestAttributeValueUpdate(HlaObjectClass ^oc, List<HlaAttribute^>^ attributes, VariableLengthDataWrapper^ userTag) {
	try {
		// Create Attribute Handle Set 
		rti1516e::AttributeHandleSet ahs;
		for (int j = 0; j < attributes->Count; j++) {
			rti1516e::AttributeHandle handle = nom->Attributes[attributes[j]->Handle];
			ahs.insert(handle);
		}

		rti1516e::ObjectClassHandle ocHandle = nom->ObjectClasses[oc->Handle];
		// Create Tag
		VariableLengthData tag(userTag->Data.ToPointer(), userTag->Size);

		this->rti->requestAttributeValueUpdate(ocHandle, ahs, tag);

		String^ list = String::Join(", ", attributes);// attributes to string
		String^ msg = "Attribute value update for class (name: " + oc->Name + ", handle: " + oc->Handle + ") is requested for attributes:" + list + ".";
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return true;
	}
#pragma region exceptions
	catch (ObjectClassNotDefined& e) {
		String^ msg = "RtiAmb_Hla1516e-(ObjectClassNotDefined - requestAttributeValueUpdate). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (AttributeNotDefined& e) {
		String^ msg = "RtiAmb_Hla1516e-(AttributeNotDefined - requestAttributeValueUpdate). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - requestAttributeValueUpdate).Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - requestAttributeValueUpdate). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - requestAttributeValueUpdate). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - requestAttributeValueUpdate). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - requestAttributeValueUpdate). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - requestAttributeValueUpdate). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

// 6.19 - Request Attribute Value Update - object
bool RtiAmb_Hla1516e::requestAttributeValueUpdate(HlaObject ^obj, List<HlaAttribute^>^ attributes, VariableLengthDataWrapper^ userTag) {
	try {
		// Create Attribute Handle Set 
		rti1516e::AttributeHandleSet ahs;
		for (int j = 0; j < attributes->Count; j++) {
			rti1516e::AttributeHandle handle = nom->Attributes[attributes[j]->Handle];
			ahs.insert(handle);
		}

		rti1516e::ObjectInstanceHandle oHandle = nom->Objects[obj->Handle];
		// Create Tag
		VariableLengthData tag(userTag->Data.ToPointer(), userTag->Size);

		this->rti->requestAttributeValueUpdate(oHandle, ahs, tag);

		String^ list = String::Join(", ", attributes);// attributes to string
		String^ msg = "Attribute value update for object (name: " + obj->Name + ", handle: " + obj->Handle + ") is requested for attributes:" + list + ".";
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return true;
	}
#pragma region exceptions
	catch (ObjectInstanceNotKnown& e) {
		String^ msg = "RtiAmb_Hla1516e-(ObjectInstanceNotKnown - requestAttributeValueUpdate2). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (AttributeNotDefined& e) {
		String^ msg = "RtiAmb_Hla1516e-(AttributeNotDefined - requestAttributeValueUpdate2). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - requestAttributeValueUpdate2).Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - requestAttributeValueUpdate2). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - requestAttributeValueUpdate2). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - requestAttributeValueUpdate2). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - requestAttributeValueUpdate2). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - requestAttributeValueUpdate2). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

#pragma endregion	// Object Management	

#pragma region Ownership Management
// 7.2 - unconditionalAttributeOwnershipDivestiture
bool RtiAmb_Hla1516e::unconditionalAttributeOwnershipDivestiture(HlaObject ^object, List<HlaAttribute^>^ attributes) {
	try {
		// Create Attribute Handle Set 
		rti1516e::AttributeHandleSet ahs;
		for (int j = 0; j < attributes->Count; j++) {
			rti1516e::AttributeHandle handle = nom->Attributes[attributes[j]->Handle];
			ahs.insert(handle);
		}
		rti1516e::ObjectInstanceHandle _oHandle = nom->Objects[object->Handle];
		(void)rti->unconditionalAttributeOwnershipDivestiture(_oHandle, ahs);

		String^ list = String::Join(", ", attributes);// attributes to string
		String^ msg = "The ownership(s) of attribute(s) of object (handle: " + object->Handle + ") is/are released unconditionally. the attributes: " + list;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return true;
	}
#pragma region exceptions
	catch (AttributeNotOwned& e) {
		String^ msg = "RtiAmb_Hla1516e-(AttributeNotOwned - unconditionalAttributeOwnershipDivestiture). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (AttributeNotDefined& e) {
		String^ msg = "RtiAmb_Hla1516e-(AttributeNotDefined - unconditionalAttributeOwnershipDivestiture). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (ObjectInstanceNotKnown& e) {
		String^ msg = "RtiAmb_Hla1516e-(ObjectInstanceNotKnown - unconditionalAttributeOwnershipDivestiture). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - unconditionalAttributeOwnershipDivestiture). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - unconditionalAttributeOwnershipDivestiture). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - unconditionalAttributeOwnershipDivestiture).Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - unconditionalAttributeOwnershipDivestiture). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - unconditionalAttributeOwnershipDivestiture). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - unconditionalAttributeOwnershipDivestiture). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

// 7.3 - negotiatedAttributeOwnershipDivestiture
bool RtiAmb_Hla1516e::negotiatedAttributeOwnershipDivestiture(HlaObject ^object, List<HlaAttribute^>^ attributes, VariableLengthDataWrapper^ userTag) {
	try {
		// Create Attribute Handle Set 
		rti1516e::AttributeHandleSet _ahs;
		for (int j = 0; j < attributes->Count; j++) {
			rti1516e::AttributeHandle handle = nom->Attributes[attributes[j]->Handle];
			_ahs.insert(handle);
		}
		// Create Tag
		VariableLengthData tag(userTag->Data.ToPointer(), userTag->Size);

		rti1516e::ObjectInstanceHandle _oHandle = nom->Objects[object->Handle];
		(void)rti->negotiatedAttributeOwnershipDivestiture(_oHandle, _ahs, tag);

		String^ list = String::Join(", ", attributes);// attributes to string
		String^ msg = "The release for the negotiated Attribute Ownership Divestiture of object (handle: " + object->Handle + ") is attempted. The attributes: " + list;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return true;
	}
#pragma region exceptions
	catch (AttributeAlreadyBeingDivested& e) {
		String^ msg = "RtiAmb_Hla1516e-(AttributeAlreadyBeingDivested - negotiatedAttributeOwnershipDivestiture). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (AttributeNotOwned& e) {
		String^ msg = "RtiAmb_Hla1516e-(AttributeNotOwned - negotiatedAttributeOwnershipDivestiture). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (AttributeNotDefined& e) {
		String^ msg = "RtiAmb_Hla1516e-(AttributeNotDefined - negotiatedAttributeOwnershipDivestiture). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (ObjectInstanceNotKnown& e) {
		String^ msg = "RtiAmb_Hla1516e-(ObjectInstanceNotKnown - negotiatedAttributeOwnershipDivestiture). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - negotiatedAttributeOwnershipDivestiture). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - negotiatedAttributeOwnershipDivestiture). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - negotiatedAttributeOwnershipDivestiture).Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - negotiatedAttributeOwnershipDivestiture). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - negotiatedAttributeOwnershipDivestiture). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - negotiatedAttributeOwnershipDivestiture). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

// 7.6 - confirmDivestiture
bool RtiAmb_Hla1516e::confirmDivestiture(HlaObject ^object, List<HlaAttribute^>^ attributes, VariableLengthDataWrapper^ userTag) {
	try {
		// Create Attribute Handle Set 
		rti1516e::AttributeHandleSet _ahs;
		for (int j = 0; j < attributes->Count; j++) {
			rti1516e::AttributeHandle handle = nom->Attributes[attributes[j]->Handle];
			_ahs.insert(handle);
		}
		// Create Tag
		VariableLengthData tag(userTag->Data.ToPointer(), userTag->Size);

		rti1516e::ObjectInstanceHandle _oHandle = nom->Objects[object->Handle];
		(void)rti->confirmDivestiture(_oHandle, _ahs, tag);

		String^ list = String::Join(", ", attributes);// attributes to string
		String^ msg = "The joined federate wants to vomplete negotiated divestiture for the attributes of object (handle: " + object->Handle + "). The attributes: " + list;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return true;
	}
#pragma region exceptions
	catch (NoAcquisitionPending& e) {
		String^ msg = "RtiAmb_Hla1516e-(NoAcquisitionPending - confirmDivestiture). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (AttributeDivestitureWasNotRequested& e) {
		String^ msg = "RtiAmb_Hla1516e-(AttributeDivestitureWasNotRequested - confirmDivestiture). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (AttributeNotOwned& e) {
		String^ msg = "RtiAmb_Hla1516e-(AttributeNotOwned - confirmDivestiture). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (AttributeNotDefined& e) {
		String^ msg = "RtiAmb_Hla1516e-(AttributeNotDefined - confirmDivestiture). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (ObjectInstanceNotKnown& e) {
		String^ msg = "RtiAmb_Hla1516e-(ObjectInstanceNotKnown - confirmDivestiture). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - confirmDivestiture). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - confirmDivestiture). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - confirmDivestiture).Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - confirmDivestiture). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - confirmDivestiture). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - confirmDivestiture). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

// 7.8 - attributeOwnershipAcquisition
bool RtiAmb_Hla1516e::attributeOwnershipAcquisition(HlaObject ^object, List<HlaAttribute^>^ attributes, VariableLengthDataWrapper^ userTag) {
	try {
		// Create Attribute Handle Set 
		rti1516e::AttributeHandleSet _ahs;
		for (int j = 0; j < attributes->Count; j++) {
			rti1516e::AttributeHandle handle = nom->Attributes[attributes[j]->Handle];
			_ahs.insert(handle);
		}
		// Create Tag
		VariableLengthData tag(userTag->Data.ToPointer(), userTag->Size);

		rti1516e::ObjectInstanceHandle _oHandle = nom->Objects[object->Handle];
		(void)rti->attributeOwnershipAcquisition(_oHandle, _ahs, tag);

		String^ list = String::Join(", ", attributes);// attributes to string
		String^ msg = "The ownership of the attributes of the object (handle: " + object->Handle + ") is requested. The desired attributes: " + list;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return true;
	}
#pragma region exceptions
	catch (AttributeNotPublished& e) {
		String^ msg = "RtiAmb_Hla1516e-(AttributeNotPublished - attributeOwnershipAcquisition). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (ObjectClassNotPublished& e) {
		String^ msg = "RtiAmb_Hla1516e-(ObjectClassNotPublished - attributeOwnershipAcquisition). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (FederateOwnsAttributes& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateOwnsAttributes - attributeOwnershipAcquisition). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (AttributeNotDefined& e) {
		String^ msg = "RtiAmb_Hla1516e-(AttributeNotDefined - attributeOwnershipAcquisition). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (ObjectInstanceNotKnown& e) {
		String^ msg = "RtiAmb_Hla1516e-(ObjectInstanceNotKnown - attributeOwnershipAcquisition). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - attributeOwnershipAcquisition). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - attributeOwnershipAcquisition). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - attributeOwnershipAcquisition).Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - attributeOwnershipAcquisition). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - attributeOwnershipAcquisition). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - attributeOwnershipAcquisition). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
#pragma endregion

};

// 7.9 - attributeOwnershipAcquisitionIfAvailable
bool RtiAmb_Hla1516e::attributeOwnershipAcquisitionIfAvailable(HlaObject ^object, List<HlaAttribute^>^ attributes) {
	try {
		// Create Attribute Handle Set 
		rti1516e::AttributeHandleSet _ahs;
		for (int j = 0; j < attributes->Count; j++) {
			rti1516e::AttributeHandle handle = nom->Attributes[attributes[j]->Handle];
			_ahs.insert(handle);
		}

		rti1516e::ObjectInstanceHandle _oHandle = nom->Objects[object->Handle];
		(void)rti->attributeOwnershipAcquisitionIfAvailable(_oHandle, _ahs);

		String^ list = String::Join(", ", attributes);// attributes to string
		String^ msg = "The ownership of the attributes of the object (handle: " + object->Handle + ") is requested. The desired attributes: " + list;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return true;
	}
#pragma region exceptions
	catch (AttributeAlreadyBeingAcquired& e) {
		String^ msg = "RtiAmb_Hla1516e-(AttributeAlreadyBeingAcquired - attributeOwnershipAcquisitionIfAvailable). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (AttributeNotPublished& e) {
		String^ msg = "RtiAmb_Hla1516e-(AttributeNotPublished - attributeOwnershipAcquisitionIfAvailable). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (ObjectClassNotPublished& e) {
		String^ msg = "RtiAmb_Hla1516e-(ObjectClassNotPublished - attributeOwnershipAcquisitionIfAvailable). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (FederateOwnsAttributes& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateOwnsAttributes - attributeOwnershipAcquisitionIfAvailable). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (AttributeNotDefined& e) {
		String^ msg = "RtiAmb_Hla1516e-(AttributeNotDefined - attributeOwnershipAcquisitionIfAvailable). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (ObjectInstanceNotKnown& e) {
		String^ msg = "RtiAmb_Hla1516e-(ObjectInstanceNotKnown - attributeOwnershipAcquisitionIfAvailable). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - attributeOwnershipAcquisitionIfAvailable). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - attributeOwnershipAcquisitionIfAvailable). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - attributeOwnershipAcquisitionIfAvailable).Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - attributeOwnershipAcquisitionIfAvailable). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - attributeOwnershipAcquisitionIfAvailable). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - attributeOwnershipAcquisitionIfAvailable). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
#pragma endregion
};

// 7.12 - attributeOwnershipReleaseDenied
bool RtiAmb_Hla1516e::attributeOwnershipReleaseDenied(HlaObject ^object, List<HlaAttribute^>^ attributes) {
	try {
		// Create Attribute Handle Set 
		rti1516e::AttributeHandleSet _ahs;
		for (int j = 0; j < attributes->Count; j++) {
			rti1516e::AttributeHandle handle = nom->Attributes[attributes[j]->Handle];
			_ahs.insert(handle);
		}

		rti1516e::ObjectInstanceHandle _oHandle = nom->Objects[object->Handle];
		(void)rti->attributeOwnershipReleaseDenied(_oHandle, _ahs);

		String^ list = String::Join(", ", attributes);// attributes to string
		String^ msg = "The federate is unwilling to divest itself of ownership of the attributes of the object (handle: " + object->Handle + "). The attributes: " + list;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return true;
	}
#pragma region exceptions
	catch (AttributeNotOwned& e) {
		String^ msg = "RtiAmb_Hla1516e-(AttributeNotOwned - attributeOwnershipReleaseDenied). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (AttributeNotDefined& e) {
		String^ msg = "RtiAmb_Hla1516e-(AttributeNotDefined - attributeOwnershipReleaseDenied). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (ObjectInstanceNotKnown& e) {
		String^ msg = "RtiAmb_Hla1516e-(ObjectInstanceNotKnown - attributeOwnershipReleaseDenied). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - attributeOwnershipReleaseDenied). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - attributeOwnershipReleaseDenied). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - attributeOwnershipReleaseDenied).Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - attributeOwnershipReleaseDenied). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - attributeOwnershipReleaseDenied). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - attributeOwnershipReleaseDenied). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
#pragma endregion
};

// 7.13 - attributeOwnershipDivestitureIfWanted
bool RtiAmb_Hla1516e::attributeOwnershipDivestitureIfWanted(HlaObject ^object, List<HlaAttribute^>^ attributes, List<HlaAttribute^>^ % attributesDivested) {
	try {
		// Create Attribute Handle Set 
		rti1516e::AttributeHandleSet _ahs;
		for (int j = 0; j < attributes->Count; j++) {
			rti1516e::AttributeHandle handle = nom->Attributes[attributes[j]->Handle];
			_ahs.insert(handle);
		}
		rti1516e::AttributeHandleSet _ahsDivested; // filled by RTI

		rti1516e::ObjectInstanceHandle _oHandle = nom->Objects[object->Handle];

		(void)rti->attributeOwnershipDivestitureIfWanted(_oHandle, _ahs, _ahsDivested);

		// Populate divested attributes returned by RTI
		for (auto const& value : _ahsDivested)
		{
			HlaAttribute^ attribute = gcnew HlaAttribute();
			attribute->Handle = Handle2Long(value);
			attributesDivested->Add(attribute);
		};

		String^ list = String::Join(", ", attributes);
		String^ list2 = String::Join(", ", attributesDivested);
		String^ msg = "The federate is willing to divest itself of ownership of the attributes of the object (handle: " + object->Handle + "). The attributes: " + list + ". Theattributes for which ownership has actually been divested are " + list2;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return true;
	}
#pragma region exceptions
	catch (AttributeNotOwned& e) {
		String^ msg = "RtiAmb_Hla1516e-(AttributeNotOwned - attributeOwnershipDivestitureIfWanted). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (AttributeNotDefined& e) {
		String^ msg = "RtiAmb_Hla1516e-(AttributeNotDefined - attributeOwnershipDivestitureIfWanted). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (ObjectInstanceNotKnown& e) {
		String^ msg = "RtiAmb_Hla1516e-(ObjectInstanceNotKnown - attributeOwnershipDivestitureIfWanted). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - attributeOwnershipDivestitureIfWanted). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - attributeOwnershipDivestitureIfWanted). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - attributeOwnershipDivestitureIfWanted).Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - attributeOwnershipDivestitureIfWanted). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - attributeOwnershipDivestitureIfWanted). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - attributeOwnershipDivestitureIfWanted). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
#pragma endregion
};

// 7.14 - cancelNegotiatedAttributeOwnershipDivestiture
bool RtiAmb_Hla1516e::cancelNegotiatedAttributeOwnershipDivestiture(HlaObject ^object, List<HlaAttribute^>^ attributes) {
	try {
		// Create Attribute Handle Set 
		rti1516e::AttributeHandleSet _ahs;
		for (int j = 0; j < attributes->Count; j++) {
			rti1516e::AttributeHandle handle = nom->Attributes[attributes[j]->Handle];
			_ahs.insert(handle);
		}

		rti1516e::ObjectInstanceHandle _oHandle = nom->Objects[object->Handle];
		(void)rti->cancelNegotiatedAttributeOwnershipDivestiture(_oHandle, _ahs);

		String^ list = String::Join(", ", attributes);// attributes to string
		String^ msg = "Negotiated Attribute Ownership Divestiture for object (handle: " + object->Handle + ") is cancelled. The attributes: " + list;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return true;
	}
#pragma region exceptions
	catch (AttributeDivestitureWasNotRequested& e) {
		String^ msg = "RtiAmb_Hla1516e-(AttributeDivestitureWasNotRequested - cancelNegotiatedAttributeOwnershipDivestiture). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (AttributeNotOwned& e) {
		String^ msg = "RtiAmb_Hla1516e-(AttributeNotOwned - cancelNegotiatedAttributeOwnershipDivestiture). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (AttributeNotDefined& e) {
		String^ msg = "RtiAmb_Hla1516e-(AttributeNotDefined - cancelNegotiatedAttributeOwnershipDivestiture). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (ObjectInstanceNotKnown& e) {
		String^ msg = "RtiAmb_Hla1516e-(ObjectInstanceNotKnown - cancelNegotiatedAttributeOwnershipDivestiture). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - cancelNegotiatedAttributeOwnershipDivestiture). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - cancelNegotiatedAttributeOwnershipDivestiture). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - cancelNegotiatedAttributeOwnershipDivestiture).Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - cancelNegotiatedAttributeOwnershipDivestiture). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - cancelNegotiatedAttributeOwnershipDivestiture). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - cancelNegotiatedAttributeOwnershipDivestiture). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
#pragma endregion
};

// 7.15 - cancelAttributeOwnershipAcquisition
bool RtiAmb_Hla1516e::cancelAttributeOwnershipAcquisition(HlaObject ^object, List<HlaAttribute^>^ attributes) {
	try {
		// Create Attribute Handle Set 
		rti1516e::AttributeHandleSet _ahs;
		for (int j = 0; j < attributes->Count; j++) {
			rti1516e::AttributeHandle handle = nom->Attributes[attributes[j]->Handle];
			_ahs.insert(handle);
		}

		rti1516e::ObjectInstanceHandle _oHandle = nom->Objects[object->Handle];
		(void)rti->cancelAttributeOwnershipAcquisition(_oHandle, _ahs);

		String^ list = String::Join(", ", attributes);// attributes to string
		String^ msg = "Negotiated Attribute Ownership Divestiture for object (handle: " + object->Handle + ") is cancelled. The attributes: " + list;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return true;
	}
#pragma region exceptions
	catch (AttributeAcquisitionWasNotRequested& e) {
		String^ msg = "RtiAmb_Hla1516e-(AttributeAcquisitionWasNotRequested - cancelAttributeOwnershipAcquisition). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (AttributeAlreadyOwned& e) {
		String^ msg = "RtiAmb_Hla1516e-(AttributeAlreadyOwned - cancelAttributeOwnershipAcquisition). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (AttributeNotDefined& e) {
		String^ msg = "RtiAmb_Hla1516e-(AttributeNotDefined - cancelAttributeOwnershipAcquisition). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (ObjectInstanceNotKnown& e) {
		String^ msg = "RtiAmb_Hla1516e-(ObjectInstanceNotKnown - cancelAttributeOwnershipAcquisition). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - cancelAttributeOwnershipAcquisition). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - cancelAttributeOwnershipAcquisition). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - cancelAttributeOwnershipAcquisition).Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - cancelAttributeOwnershipAcquisition). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - cancelAttributeOwnershipAcquisition). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - cancelAttributeOwnershipAcquisition). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
#pragma endregion
};

// 7.17 - queryAttributeOwnership
bool RtiAmb_Hla1516e::queryAttributeOwnership(HlaObject ^object, HlaAttribute^ attribute) {
	try {
		rti1516e::ObjectInstanceHandle _oHandle = nom->Objects[object->Handle];
		rti1516e::AttributeHandle _ahandle = nom->Attributes[attribute->Handle];

		(void)rti->queryAttributeOwnership(_oHandle, _ahandle);

		String^ msg = "Ownership of the attribute (handle: " + attribute->Handle + ") is queried.";
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return true;
	}
#pragma region exceptions
	catch (AttributeNotDefined& e) {
		String^ msg = "RtiAmb_Hla1516e-(AttributeNotDefined - queryAttributeOwnership). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (ObjectInstanceNotKnown& e) {
		String^ msg = "RtiAmb_Hla1516e-(ObjectInstanceNotKnown - queryAttributeOwnership). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - queryAttributeOwnership). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - queryAttributeOwnership). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - queryAttributeOwnership).Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - queryAttributeOwnership). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - queryAttributeOwnership). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - queryAttributeOwnership). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
#pragma endregion
};

// 7.19 - isAttributeOwnedByFederate
bool RtiAmb_Hla1516e::isAttributeOwnedByFederate(HlaObject ^object, HlaAttribute^ attribute) {
	try {
		rti1516e::ObjectInstanceHandle _oHandle = nom->Objects[object->Handle];
		rti1516e::AttributeHandle _ahandle = nom->Attributes[attribute->Handle];

		bool result = rti->isAttributeOwnedByFederate(_oHandle, _ahandle);

		String^ msg = "The ownership of the attribute (handle: " + attribute->Handle + ") of object (handle: " + object->Handle + ") is owned by this federate is queried. The result is " + result;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return result;
	}
#pragma region exceptions
	catch (AttributeNotDefined& e) {
		String^ msg = "RtiAmb_Hla1516e-(AttributeNotDefined - isAttributeOwnedByFederate). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (ObjectInstanceNotKnown& e) {
		String^ msg = "RtiAmb_Hla1516e-(ObjectInstanceNotKnown - isAttributeOwnedByFederate). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - isAttributeOwnedByFederate). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - isAttributeOwnedByFederate). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - isAttributeOwnedByFederate).Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - isAttributeOwnedByFederate). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - isAttributeOwnedByFederate). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - isAttributeOwnedByFederate). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
#pragma endregion
};

#pragma endregion	// Ownership Management

#pragma region Data Distribution Management

// 9.2 - Create region
unsigned int RtiAmb_Hla1516e::createRegion(HlaRegion ^region, List<HlaDimension^> ^dimensions) {
	try {
		String^ tmpStr = "";

		// Create Attribute Handle Set 
		rti1516e::DimensionHandleSet set;
		for (int j = 0; j < dimensions->Count; j++) {
			rti1516e::DimensionHandle handle = nom->Dimensions[dimensions[j]->Handle];
			set.insert(handle);
			tmpStr += dimensions[j]->Name + " (" + dimensions[j]->Handle + ")";
			if (j != dimensions->Count - 1) tmpStr += ", ";
		}

		rti1516e::RegionHandle regionHandle = rti->createRegion(set);
		region->Handle = Handle2Long(regionHandle);

		// Update Nom
		if (nom->Regions.find(region->Handle) == nom->Regions.end()) // if not found
			nom->Regions[region->Handle] = regionHandle;

		String^ msg = "Region (name: " + region->Name + ", handle: " + region->Handle + ") is created with dimensions: " + tmpStr + ".";
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));

		return region->Handle;
	}
#pragma region exceptions
	catch (InvalidDimensionHandle& e) {
		String^ msg = "RtiAmb_Hla1516e-(InvalidDimensionHandle - createRegion). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - createRegion). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - createRegion). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - createRegion). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - createRegion). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - createRegion). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - createRegion). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

// 9.4 - Delete region
void RtiAmb_Hla1516e::deleteRegion(HlaRegion^ region) {
	try {
		rti->deleteRegion(nom->Regions[region->Handle]);

		String^ msg = "Region (name: " + region->Name + ", handle: " + region->Handle + ") is deleted.";
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
#pragma region exceptions
	catch (RegionInUseForUpdateOrSubscription& e) {
		String^ msg = "RtiAmb_Hla1516e-(RegionInUseForUpdateOrSubscription - deleteRegion). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RegionNotCreatedByThisFederate& e) {
		String^ msg = "RtiAmb_Hla1516e-(RegionNotCreatedByThisFederate - deleteRegion). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (InvalidRegion& e) {
		String^ msg = "RtiAmb_Hla1516e-(InvalidRegion - deleteRegion). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - deleteRegion). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - deleteRegion). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - deleteRegion). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - deleteRegion). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - deleteRegion). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - deleteRegion). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

// 9.5 - Register Object Instance With Region - selected (publishable) attributes with specific regions
void RtiAmb_Hla1516e::registerObjectInstanceWithRegions(HlaObject ^obj, RtiLayer::AttributeHandleSetRegionHandleSetPairVector ^list) {
	try {
		rti1516e::ObjectClassHandle _ocHandle = nom->ObjectClasses[obj->Type->Handle];
		rti1516e::ObjectInstanceHandle _ohandle;
		rti1516e::AttributeHandleSetRegionHandleSetPairVector _pairs;

		// Convert Rom => Nom
		for each (KeyValuePair<List<HlaAttribute^>^, List<HlaRegion^>^> ^pair in list->Pairs)
		{
			rti1516e::AttributeHandleSet ahs;
			for each (HlaAttribute^ attribute in pair->Key)
				ahs.insert(nom->Attributes[attribute->Handle]);
			rti1516e::RegionHandleSet rhs;
			for each (HlaRegion^ region in pair->Value)
				rhs.insert(nom->Regions[region->Handle]);
			rti1516e::AttributeHandleSetRegionHandleSetPair _pair(ahs, rhs); // create a pair
			_pairs.push_back(_pair); // insert pair to vector
		}

		String^ name = "Object Handle = " + obj->Handle + ". ";
		if (obj->Name == "")
		{
			_ohandle = rti->registerObjectInstanceWithRegions(_ocHandle, _pairs);
		}
		else
		{
			pin_ptr<const wchar_t> _oName = PtrToStringChars(obj->Name);
			_ohandle = rti->registerObjectInstanceWithRegions(_ocHandle, _pairs, _oName);
			name += "Object Name = " + obj->Name + ". ";
		}

		obj->Handle = toULong(gcnew String(_ohandle.toString().c_str()));
		nom->Objects[obj->Handle] = _ohandle; // update Nom

		String^ msg = "Object (class name: " + obj->Type->Name + ") is registered with regions. Attribute-Region pairs: " + list->ToString() + name;
		this->OnObjectRegistered(gcnew RaconEventArgs(msg));
	}
#pragma region exceptions
	catch (ObjectInstanceNameInUse& e) {
		String^ msg = "MSG-(ObjectInstanceNameInUse - registerObjectInstanceWithRegions):" + " Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (ObjectInstanceNameNotReserved& e) {
		String^ msg = "MSG-(ObjectInstanceNameNotReserved - registerObjectInstanceWithRegions):" + " Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (InvalidRegionContext& e) {
		String^ msg = "MSG-(InvalidRegionContext - registerObjectInstanceWithRegions):" + " Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RegionNotCreatedByThisFederate& e) {
		String^ msg = "MSG-(RegionNotCreatedByThisFederate - registerObjectInstanceWithRegions):" + " Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (InvalidRegion& e) {
		String^ msg = "MSG-(InvalidRegion - registerObjectInstanceWithRegions):" + " Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (AttributeNotPublished& e) {
		String^ msg = "MSG-(AttributeNotPublished - registerObjectInstanceWithRegions):" + " Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (ObjectClassNotPublished& e) {
		String^ msg = "MSG-(ObjectClassNotPublished - registerObjectInstanceWithRegions):" + " Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (AttributeNotDefined& e) {
		String^ msg = "MSG-(AttributeNotDefined - registerObjectInstanceWithRegions):" + " Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (ObjectClassNotDefined& e) {
		String^ msg = "MSG-(ObjectClassNotDefined - registerObjectInstanceWithRegions):" + " Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "MSG-(FederateNotExecutionMember - registerObjectInstanceWithRegions):" + " Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (SaveInProgress& e) {
		String^ msg = "MSG-(SaveInProgress - registerObjectInstanceWithRegions):" + " Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "MSG-(RestoreInProgress - registerObjectInstanceWithRegions):" + " Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (NotConnected& e) {
		String^ msg = "MSG-(NotConnected - registerObjectInstanceWithRegions):" + " Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla13-(RTIinternalError - registerObjectInstanceWithRegions). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - registerObjectInstanceWithRegions). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

// 9.6 - associateRegionsForUpdates
bool RtiAmb_Hla1516e::associateRegionsForUpdates(HlaObject ^obj, RtiLayer::AttributeHandleSetRegionHandleSetPairVector ^list) {
	try {
		rti1516e::ObjectInstanceHandle _ohandle = nom->Objects[obj->Handle];
		rti1516e::AttributeHandleSetRegionHandleSetPairVector _pairs;

		// Convert Rom => Nom
		for each (KeyValuePair<List<HlaAttribute^>^, List<HlaRegion^>^> ^pair in list->Pairs)
		{
			rti1516e::AttributeHandleSet ahs;
			for each (HlaAttribute^ attribute in pair->Key)
				ahs.insert(nom->Attributes[attribute->Handle]);
			rti1516e::RegionHandleSet rhs;
			for each (HlaRegion^ region in pair->Value)
				rhs.insert(nom->Regions[region->Handle]);
			rti1516e::AttributeHandleSetRegionHandleSetPair _pair(ahs, rhs); // create a pair
			_pairs.push_back(_pair); // insert pair to vector
		}

		rti->associateRegionsForUpdates(_ohandle, _pairs);

		String^ msg = "Attributes of object (handle: " + obj->Handle + ") are associated with Regions. Attribute - Region pairs : " + list->ToString();
		this->OnHLAClassSubscribed(gcnew RaconEventArgs(msg));
		return true;
	}
#pragma region exceptions
	catch (InvalidRegionContext& e) {
		String^ msg = "MSG-(InvalidRegionContext - associateRegionsForUpdates):" + " Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RegionNotCreatedByThisFederate& e) {
		String^ msg = "MSG-(RegionNotCreatedByThisFederate - associateRegionsForUpdates):" + " Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (InvalidRegion& e) {
		String^ msg = "MSG-(InvalidRegion - associateRegionsForUpdates):" + " Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (AttributeNotDefined& e) {
		String^ msg = "MSG-(AttributeNotDefined - associateRegionsForUpdates):" + " Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (ObjectInstanceNotKnown& e) {
		String^ msg = "MSG-(ObjectInstanceNotKnown - associateRegionsForUpdates):" + " Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "MSG-(FederateNotExecutionMember - associateRegionsForUpdates):" + " Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (SaveInProgress& e) {
		String^ msg = "MSG-(SaveInProgress - associateRegionsForUpdates):" + " Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "MSG-(RestoreInProgress - associateRegionsForUpdates):" + " Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (NotConnected& e) {
		String^ msg = "MSG-(NotConnected - associateRegionsForUpdates):" + " Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla13-(RTIinternalError - associateRegionsForUpdates). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - associateRegionsForUpdates). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

// 9.7 - unassociateRegionForUpdates
bool RtiAmb_Hla1516e::unassociateRegionsForUpdates(HlaObject ^obj, RtiLayer::AttributeHandleSetRegionHandleSetPairVector ^list) {
	try {
		rti1516e::ObjectInstanceHandle _ohandle = nom->Objects[obj->Handle];
		rti1516e::AttributeHandleSetRegionHandleSetPairVector _pairs;

		// Convert Rom => Nom
		for each (KeyValuePair<List<HlaAttribute^>^, List<HlaRegion^>^> ^pair in list->Pairs)
		{
			rti1516e::AttributeHandleSet ahs;
			for each (HlaAttribute^ attribute in pair->Key)
				ahs.insert(nom->Attributes[attribute->Handle]);
			rti1516e::RegionHandleSet rhs;
			for each (HlaRegion^ region in pair->Value)
				rhs.insert(nom->Regions[region->Handle]);
			rti1516e::AttributeHandleSetRegionHandleSetPair _pair(ahs, rhs); // create a pair
			_pairs.push_back(_pair); // insert pair to vector
		}

		rti->unassociateRegionsForUpdates(_ohandle, _pairs);

		String^ msg = "Attributes of object (handle: " + obj->Handle + ") are unassociated with Regions. Attribute - Region pairs : " + list->ToString();
		this->OnHLAClassSubscribed(gcnew RaconEventArgs(msg));
		return true;
	}
#pragma region exceptions
	catch (RegionNotCreatedByThisFederate& e) {
		String^ msg = "MSG-(RegionNotCreatedByThisFederate - unassociateRegionsForUpdates):" + " Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (InvalidRegion& e) {
		String^ msg = "MSG-(InvalidRegion - unassociateRegionsForUpdates):" + " Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (AttributeNotDefined& e) {
		String^ msg = "MSG-(AttributeNotDefined - unassociateRegionsForUpdates):" + " Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (ObjectInstanceNotKnown& e) {
		String^ msg = "MSG-(ObjectInstanceNotKnown - unassociateRegionsForUpdates):" + " Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (SaveInProgress& e) {
		String^ msg = "MSG-(SaveInProgress - unassociateRegionsForUpdates):" + " Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "MSG-(RestoreInProgress - unassociateRegionsForUpdates):" + " Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "MSG-(FederateNotExecutionMember - unassociateRegionsForUpdates):" + " Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (NotConnected& e) {
		String^ msg = "MSG-(NotConnected - unassociateRegionsForUpdates):" + " Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla13-(RTIinternalError - unassociateRegionsForUpdates). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - unassociateRegionsForUpdates). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

// 9.8 - subscribeOC with Region
bool RtiAmb_Hla1516e::subscribeObjectClassAttributesWithRegions(HlaObjectClass ^oc, RtiLayer::AttributeHandleSetRegionHandleSetPairVector ^list, bool indicator, String^ updateRate) {
	try {
		rti1516e::AttributeHandleSetRegionHandleSetPairVector _pairs;

		// Convert Rom => Nom
		for each (KeyValuePair<List<HlaAttribute^>^, List<HlaRegion^>^> ^pair in list->Pairs)
		{
			rti1516e::AttributeHandleSet ahs;
			for each (HlaAttribute^ attribute in pair->Key)
				if ((attribute->AttributePS == PSKind::Subscribe) || (attribute->AttributePS == PSKind::PublishSubscribe))
					ahs.insert(nom->Attributes[attribute->Handle]);
			rti1516e::RegionHandleSet rhs;
			for each (HlaRegion^ region in pair->Value)
				rhs.insert(nom->Regions[region->Handle]);
			rti1516e::AttributeHandleSetRegionHandleSetPair _pair(ahs, rhs); // create a pair
			_pairs.push_back(_pair); // insert pair to vector
		}
		pin_ptr<const wchar_t> _updateRateDesignator = PtrToStringChars(updateRate);
		rti1516e::ObjectClassHandle _handle = getObjectClassHandle(oc);

		rti->subscribeObjectClassAttributesWithRegions(_handle, _pairs, indicator, _updateRateDesignator);

		String^ msg = "Object Class (" + oc->Name + ") is subscribed with all its subscribeable attributes. Class handle: " + Handle2Long(_handle).ToString() + ",  Attribute-Region pairs: " + list->ToString();
		if (updateRate != "")	msg += ", Update Rate Designator: " + updateRate;
		this->OnHLAClassPublished(gcnew RaconEventArgs(msg));
		return true;
	}
#pragma region exceptions
	catch (InvalidRegionContext& e) {
		String^ msg = "MSG-(InvalidRegionContext - subscribeObjectClassAttributesWithRegions):" + " Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (RegionNotCreatedByThisFederate& e) {
		String^ msg = "MSG-(RegionNotCreatedByThisFederate - subscribeObjectClassAttributesWithRegions):" + " Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (InvalidRegion& e) {
		String^ msg = "MSG-(InvalidRegion - subscribeObjectClassAttributesWithRegions):" + " Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (AttributeNotDefined& e) {
		String^ msg = "RtiAmb_Hla1516e-(AttributeNotDefined - subscribeObjectClassAttributesWithRegions). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (ObjectClassNotDefined& e) {
		String^ msg = "RtiAmb_Hla1516e-(ObjectClassNotDefined - subscribeObjectClassAttributesWithRegions). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (InvalidUpdateRateDesignator& e) {
		String^ msg = "RtiAmb_Hla1516e-(InvalidUpdateRateDesignator - subscribeObjectClassAttributesWithRegions). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - subscribeObjectClassAttributesWithRegions). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - subscribeObjectClassAttributesWithRegions). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - subscribeObjectClassAttributesWithRegions).Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - subscribeObjectClassAttributesWithRegions). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - subscribeObjectClassAttributesWithRegions). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - subscribeObjectClassAttributesWithRegions). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
#pragma endregion
};

// 9.9 - unsubscribe OC with Region
bool RtiAmb_Hla1516e::unsubscribeObjectClassWithRegions(HlaObjectClass ^oc, RtiLayer::AttributeHandleSetRegionHandleSetPairVector ^list) {
	try {
		rti1516e::ObjectClassHandle _handle = nom->ObjectClasses[oc->Handle];
		rti1516e::AttributeHandleSetRegionHandleSetPairVector _pairs;

		// Convert Rom => Nom
		for each (KeyValuePair<List<HlaAttribute^>^, List<HlaRegion^>^> ^pair in list->Pairs)
		{
			rti1516e::AttributeHandleSet ahs;
			for each (HlaAttribute^ attribute in pair->Key)
				if ((attribute->AttributePS == PSKind::Subscribe) || (attribute->AttributePS == PSKind::PublishSubscribe))
					ahs.insert(nom->Attributes[attribute->Handle]);
			rti1516e::RegionHandleSet rhs;
			for each (HlaRegion^ region in pair->Value)
				rhs.insert(nom->Regions[region->Handle]);
			rti1516e::AttributeHandleSetRegionHandleSetPair _pair(ahs, rhs); // create a pair
			_pairs.push_back(_pair); // insert pair to vector
		}

		rti->unsubscribeObjectClassAttributesWithRegions(_handle, _pairs);

		String^ msg = "Object Class (" + oc->Name + ") is unsubscribed with all its subscribeable attributes. Class handle: " + Handle2Long(_handle).ToString() + ",  Attribute-Region pairs: " + list->ToString();
		this->OnHLAClassPublished(gcnew RaconEventArgs(msg));
		return true;
	}
#pragma region exceptions
	catch (RegionNotCreatedByThisFederate& e) {
		String^ msg = "MSG-(RegionNotCreatedByThisFederate - unsubscribeObjectClassWithRegions):" + " Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (InvalidRegion& e) {
		String^ msg = "MSG-(InvalidRegion - unsubscribeObjectClassWithRegions):" + " Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (AttributeNotDefined& e) {
		String^ msg = "RtiAmb_Hla1516e-(AttributeNotDefined - unsubscribeObjectClassWithRegions). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (ObjectClassNotDefined& e) {
		String^ msg = "RtiAmb_Hla1516e-(ObjectClassNotDefined - unsubscribeObjectClassWithRegions). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - unsubscribeObjectClassWithRegions). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - unsubscribeObjectClassWithRegions). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - unsubscribeObjectClassWithRegions).Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - unsubscribeObjectClassWithRegions). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - unsubscribeObjectClassWithRegions). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - unsubscribeObjectClassWithRegions). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
#pragma endregion
};


// 9.10 - subscribe IC with Region
bool RtiAmb_Hla1516e::subscribeInteractionClass(HlaInteractionClass ^ic, List<HlaRegion^>^ regions, bool indicator) {
	try {
		// Check that interaction is Subscribable
		if ((ic->ClassPS == PSKind::Subscribe) || (ic->ClassPS == PSKind::PublishSubscribe))
		{
			rti1516e::InteractionClassHandle _handle = getInteractionClassHandle(ic);
			rti1516e::RegionHandleSet _regions;

			// Convert Rom => Nom
			for each (HlaRegion ^region in regions)
			{
				_regions.insert(nom->Regions[region->Handle]);
			}

			rti->subscribeInteractionClassWithRegions(_handle, _regions, indicator);

			String^ msg = "Interaction Class (name: " + ic->Name + ", handle: " + ic->Handle + ") is subscribed with regions.";

			this->OnHLAClassPublished(gcnew RaconEventArgs(msg));
			return true;
		}

	}
#pragma region exceptions
	catch (FederateServiceInvocationsAreBeingReportedViaMOM& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateServiceInvocationsAreBeingReportedViaMOM - subscribeInteractionClass). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false; // ??? or true ???
	}
	catch (InvalidRegionContext& e) {
		String^ msg = "MSG-(InvalidRegionContext - subscribeInteractionClass):" + " Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (RegionNotCreatedByThisFederate& e) {
		String^ msg = "MSG-(RegionNotCreatedByThisFederate - subscribeInteractionClass):" + " Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (InvalidRegion& e) {
		String^ msg = "MSG-(InvalidRegion - subscribeInteractionClass):" + " Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (InteractionClassNotDefined& e) {
		String^ msg = "RtiAmb_Hla1516e-(ObjectClassNotDefined - subscribeInteractionClass). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - subscribeInteractionClass). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - subscribeInteractionClass). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - subscribeInteractionClass).Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - subscribeInteractionClass). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - subscribeInteractionClass). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - subscribeInteractionClass). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
#pragma endregion
};

// 9.11 -unsubscribe IC with Region
bool RtiAmb_Hla1516e::unsubscribeInteractionClass(HlaInteractionClass ^ic, List<HlaRegion^>^ regions) {
	try {
		rti1516e::InteractionClassHandle _handle = getInteractionClassHandle(ic);
		rti1516e::RegionHandleSet _regions;

		// Convert Rom => Nom
		for each (HlaRegion ^region in regions)
		{
			_regions.insert(nom->Regions[region->Handle]);
		}

		rti->unsubscribeInteractionClassWithRegions(_handle, _regions);

		String^ msg = "Interaction Class (name: " + ic->Name + ", handle: " + ic->Handle + ") is unsubcribed with regions.";

		this->OnHLAClassPublished(gcnew RaconEventArgs(msg));
		return true;
	}
#pragma region exceptions
	catch (RegionNotCreatedByThisFederate& e) {
		String^ msg = "MSG-(RegionNotCreatedByThisFederate - unsubscribeInteractionClass):" + " Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (InvalidRegion& e) {
		String^ msg = "MSG-(InvalidRegion - unsubscribeInteractionClass):" + " Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (InteractionClassNotDefined& e) {
		String^ msg = "RtiAmb_Hla1516e-(ObjectClassNotDefined - unsubscribeInteractionClass). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - unsubscribeInteractionClass). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - unsubscribeInteractionClass). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - unsubscribeInteractionClass).Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - unsubscribeInteractionClass). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - unsubscribeInteractionClass). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - unsubscribeInteractionClass). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
#pragma endregion
};

// 9.12 - Send Interaction With Region
bool RtiAmb_Hla1516e::sendInteractionWithRegions(HlaInteraction ^interaction, List<HlaRegion^>^ regions, VariableLengthDataWrapper^ userTag) {
	try {
		rti1516e::InteractionClassHandle _handle = nom->InteractionClasses[interaction->ClassHandle];

		// Create Tag
		VariableLengthData tag(userTag->Data.ToPointer(), userTag->Size);
		// Pack parameter values
		rti1516e::ParameterHandleValueMap _parameters;
		for each (HlaParameter^ var in interaction->Parameters)
		{
			char* _pVal = (char*)var->Value.ToPointer();
			rti1516e::VariableLengthData data(_pVal, var->Length);
			_parameters[nom->Parameters[var->Handle]] = data;
		}

		// Convert Rom => Nom
		rti1516e::RegionHandleSet _regions;
		for each (HlaRegion ^region in regions)
			_regions.insert(nom->Regions[region->Handle]);

		rti->sendInteractionWithRegions(_handle, _parameters, _regions, tag);
		// Report
		String ^msg = "Interaction (handle: " + interaction->ClassHandle + ") is sent with regions";
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return true;
	}
#pragma region exceptions
	catch (InvalidLogicalTime &e) {
		String^ msg = "RtiAmb_Hla1516e-(InvalidLogicalTime - sendInteractionWithRegions): One possible reason is that if lookahead is not zero, then timestamp must be greater than or equal to (logical time + lookahead), otherwise timestamp must be greater than (logical time + lookahead). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
	}
	catch (InvalidRegionContext &e) {
		String^ msg = "RtiAmb_Hla1516e-(InvalidRegionContext - sendInteractionWithRegions2): " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
	}
	catch (RegionNotCreatedByThisFederate &e) {
		String^ msg = "RtiAmb_Hla1516e-(RegionNotCreatedByThisFederate - sendInteractionWithRegions2): " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
	}
	catch (InvalidRegion &e) {
		String^ msg = "RtiAmb_Hla1516e-(InvalidRegion - sendInteractionWithRegions2): " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
	}
	catch (InteractionClassNotPublished &e) {
		String^ msg = "RtiAmb_Hla1516e-(InteractionClassNotPublished - sendInteractionWithRegions2): " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
	}
	catch (InteractionParameterNotDefined &e) {
		String^ msg = "RtiAmb_Hla1516e-(InteractionParameterNotDefined - sendInteractionWithRegions2): " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
	}
	catch (InteractionClassNotDefined &e) {
		String^ msg = "RtiAmb_Hla1516e-(InteractionParameterNotDefined - sendInteractionWithRegions2): " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
	}
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(InteractionParameterNotDefined - sendInteractionWithRegions2): " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(InteractionParameterNotDefined - sendInteractionWithRegions2): " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(InteractionParameterNotDefined - sendInteractionWithRegions2): " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - sendInteractionWithRegions2). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - sendInteractionWithRegions2). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - sendInteractionWithRegions2). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
#pragma endregion
};

// 9.12 - Send Interaction With Region and time
MessageRetraction^ RtiAmb_Hla1516e::sendInteractionWithRegions(HlaInteraction ^interaction, List<HlaRegion^>^ regions, VariableLengthDataWrapper^ userTag, double time) {
	try {
		String ^msg = "";
		rti1516e::InteractionClassHandle _handle = nom->InteractionClasses[interaction->ClassHandle];

		// Create Tag
		VariableLengthData tag(userTag->Data.ToPointer(), userTag->Size);
		// Create timestamp
		rti1516e::HLAfloat64Time _timestamp(time);
		// Pack parameter values
		rti1516e::ParameterHandleValueMap _parameters;
		for each (HlaParameter^ var in interaction->Parameters)
		{
			char* _pVal = (char*)var->Value.ToPointer();
			rti1516e::VariableLengthData data(_pVal, var->Length);
			_parameters[nom->Parameters[var->Handle]] = data;
		}

		// Convert Rom => Nom
		rti1516e::RegionHandleSet _regions;
		for each (HlaRegion ^region in regions)
			_regions.insert(nom->Regions[region->Handle]);

		rti1516e::MessageRetractionHandle _mrh = rti->sendInteractionWithRegions(_handle, _parameters, _regions, tag, _timestamp);
		// create retraction
		MessageRetraction^ retraction = gcnew MessageRetraction(toULong(_mrh.toString()));
		nom->RetractionHandles[retraction->Handle] = _mrh;
		// Report
		msg += "Interaction (handle: " + interaction->ClassHandle + ") is sent with regions and with a timestamp: " + _timestamp.getTime() + ". Retraction handle: " + gcnew String(_mrh.toString().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return retraction;
	}
#pragma region exceptions
	catch (InvalidLogicalTime &e) {
		String^ msg = "RtiAmb_Hla1516e-(InvalidLogicalTime - sendInteractionWithRegions): One possible reason is that if lookahead is not zero, then timestamp must be greater than or equal to (logical time + lookahead), otherwise timestamp must be greater than (logical time + lookahead). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return nullptr;
	}
	catch (InvalidRegionContext &e) {
		String^ msg = "RtiAmb_Hla1516e-(InvalidRegionContext - sendInteractionWithRegions): " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return nullptr;
	}
	catch (RegionNotCreatedByThisFederate &e) {
		String^ msg = "RtiAmb_Hla1516e-(RegionNotCreatedByThisFederate - sendInteractionWithRegions): " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return nullptr;
	}
	catch (InvalidRegion &e) {
		String^ msg = "RtiAmb_Hla1516e-(InvalidRegion - sendInteractionWithRegions): " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return nullptr;
	}
	catch (InteractionClassNotPublished &e) {
		String^ msg = "RtiAmb_Hla1516e-(InteractionClassNotPublished - sendInteractionWithRegions): " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return nullptr;
	}
	catch (InteractionParameterNotDefined &e) {
		String^ msg = "RtiAmb_Hla1516e-(InteractionParameterNotDefined - sendInteractionWithRegions): " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return nullptr;
	}
	catch (InteractionClassNotDefined &e) {
		String^ msg = "RtiAmb_Hla1516e-(InteractionParameterNotDefined - sendInteractionWithRegions): " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return nullptr;
	}
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(InteractionParameterNotDefined - sendInteractionWithRegions): " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(InteractionParameterNotDefined - sendInteractionWithRegions): " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return nullptr;
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(InteractionParameterNotDefined - sendInteractionWithRegions): " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return nullptr;
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - sendInteractionWithRegions). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return nullptr;
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - sendInteractionWithRegions). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return nullptr;
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - sendInteractionWithRegions). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return nullptr;
	}
#pragma endregion
};

// 9.13 - requestClassAttributeValueUpdateWithRegion
bool RtiAmb_Hla1516e::requestClassAttributeValueUpdateWithRegions(HlaObjectClass ^oc, RtiLayer::AttributeHandleSetRegionHandleSetPairVector ^list, VariableLengthDataWrapper^ userTag) {
	try {
		rti1516e::ObjectClassHandle _handle = nom->ObjectClasses[oc->Handle];
		rti1516e::AttributeHandleSetRegionHandleSetPairVector _pairs;
		// Create Tag
		VariableLengthData tag(userTag->Data.ToPointer(), userTag->Size);

		// Convert Rom => Nom
		for each (KeyValuePair<List<HlaAttribute^>^, List<HlaRegion^>^> ^pair in list->Pairs)
		{
			rti1516e::AttributeHandleSet ahs;
			for each (HlaAttribute^ attribute in pair->Key)
				if ((attribute->AttributePS == PSKind::Subscribe) || (attribute->AttributePS == PSKind::PublishSubscribe))
					ahs.insert(nom->Attributes[attribute->Handle]);
			rti1516e::RegionHandleSet rhs;
			for each (HlaRegion^ region in pair->Value)
				rhs.insert(nom->Regions[region->Handle]);
			rti1516e::AttributeHandleSetRegionHandleSetPair _pair(ahs, rhs); // create a pair
			_pairs.push_back(_pair); // insert pair to vector
		}

		rti->requestAttributeValueUpdateWithRegions(_handle, _pairs, tag);

		String^ msg = "Object Class (" + oc->Name + ") is unsubscribed with all its subscribeable attributes. Class handle: " + Handle2Long(_handle).ToString() + ",  Attribute-Region pairs: " + list->ToString();
		this->OnHLAClassPublished(gcnew RaconEventArgs(msg));
		return true;
	}
#pragma region exceptions
	catch (InvalidRegionContext& e) {
		String^ msg = "MSG-(InvalidRegionContext - requestClassAttributeValueUpdateWithRegions):" + " Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (RegionNotCreatedByThisFederate& e) {
		String^ msg = "MSG-(RegionNotCreatedByThisFederate - requestClassAttributeValueUpdateWithRegions):" + " Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (InvalidRegion& e) {
		String^ msg = "MSG-(InvalidRegion - requestClassAttributeValueUpdateWithRegions):" + " Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (AttributeNotDefined& e) {
		String^ msg = "RtiAmb_Hla1516e-(AttributeNotDefined - requestClassAttributeValueUpdateWithRegions). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (ObjectClassNotDefined& e) {
		String^ msg = "RtiAmb_Hla1516e-(ObjectClassNotDefined - requestClassAttributeValueUpdateWithRegions). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - requestClassAttributeValueUpdateWithRegions). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - requestClassAttributeValueUpdateWithRegions). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - requestClassAttributeValueUpdateWithRegions).Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return false;
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - requestClassAttributeValueUpdateWithRegions). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - requestClassAttributeValueUpdateWithRegions). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - requestClassAttributeValueUpdateWithRegions). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
#pragma endregion
};

#pragma endregion // Data Distribution Management

#pragma region Time Management

// 8.2 Enable Time Regulation
bool RtiAmb_Hla1516e::enableTimeRegulation(Double theLookahead) {
	try {
		// Create lookahead - time interval
		HLAfloat64Interval lookahead(theLookahead);

		rti->enableTimeRegulation(lookahead);
		String^ msg = "Time Regulation is enabled. Lookahead = " + lookahead.getInterval() + " secs.";
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return true;
	}
#pragma region exceptions
	catch (InvalidLookahead& e) {
		String^ msg = "RtiAmb_Hla1516e-(InvalidLookahead - enableTimeRegulation). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (InTimeAdvancingState& e) {
		String^ msg = "RtiAmb_Hla1516e-(InTimeAdvancingState - enableTimeRegulation). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RequestForTimeConstrainedPending& e) {
		String^ msg = "RtiAmb_Hla1516e-(RequestForTimeConstrainedPending - enableTimeRegulation). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (TimeRegulationAlreadyEnabled& e) {
		String^ msg = "RtiAmb_Hla1516e-(TimeRegulationAlreadyEnabled - enableTimeRegulation). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - enableTimeRegulation). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - enableTimeRegulation). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - enableTimeRegulation). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - enableTimeRegulation). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - enableTimeRegulation). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - enableTimeRegulation). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

// 8.4 - Disable Time Regulation
bool RtiAmb_Hla1516e::disableTimeRegulation() {
	try {
		rti->disableTimeRegulation();
		String^ msg = "Time Regulation is disabled.";
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return true;
	}
#pragma region exceptions
	catch (TimeRegulationIsNotEnabled& e) {
		String^ msg = "RtiAmb_Hla1516e-(TimeConstrainedIsNotEnabled - disableTimeRegulation). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - disableTimeRegulation). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - disableTimeRegulation). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - disableTimeRegulation). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - disableTimeRegulation). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - disableTimeRegulation). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - disableTimeRegulation). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

// 8.5 - Enable Time Constrained
bool RtiAmb_Hla1516e::enableTimeConstrained() {
	try {
		rti->enableTimeConstrained();
		String^ msg = "Time-Constrained is enabled.";
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return true;
	}
#pragma region exceptions
	catch (InTimeAdvancingState& e) {
		String^ msg = "RtiAmb_Hla1516e-(InTimeAdvancingState - enableTimeConstrained). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RequestForTimeConstrainedPending& e) {
		String^ msg = "RtiAmb_Hla1516e-(RequestForTimeConstrainedPending - enableTimeConstrained). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (TimeConstrainedAlreadyEnabled& e) {
		String^ msg = "RtiAmb_Hla1516e-(TimeConstrainedAlreadyEnabled - enableTimeConstrained). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - enableTimeConstrained). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - enableTimeConstrained). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - enableTimeConstrained). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - enableTimeConstrained). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - enableTimeConstrained). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - enableTimeConstrained). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

// 8.7 - Disable Time Constrained
bool RtiAmb_Hla1516e::disableTimeConstrained() {
	try {
		rti->disableTimeConstrained();
		String^ msg = "Time-Constrained is disabled.";
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return true;
	}
#pragma region exceptions
	catch (TimeConstrainedIsNotEnabled& e) {
		String^ msg = "RtiAmb_Hla1516e-(TimeConstrainedIsNotEnabled - disableTimeConstrained). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - disableTimeConstrained). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - disableTimeConstrained). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - disableTimeConstrained). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - disableTimeConstrained). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - disableTimeConstrained). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - disableTimeConstrained). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

// 8.8 - timeAdvanceRequest
bool RtiAmb_Hla1516e::timeAdvanceRequest(Double time) {
	try {
		// Create logical time
		HLAfloat64Time theTime(time);

		rti->timeAdvanceRequest(theTime);
		String^ msg = "Time advance is requested for logical time = " + theTime.getTime();
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return true;
	}
#pragma region exceptions
	catch (LogicalTimeAlreadyPassed& e) {
		String^ msg = "RtiAmb_Hla1516e-(LogicalTimeAlreadyPassed - timeAdvanceRequest). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (InvalidLogicalTime& e) {
		String^ msg = "RtiAmb_Hla1516e-(InvalidLogicalTime - timeAdvanceRequest). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (InTimeAdvancingState& e) {
		String^ msg = "RtiAmb_Hla1516e-(InTimeAdvancingState - timeAdvanceRequest). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RequestForTimeRegulationPending& e) {
		String^ msg = "RtiAmb_Hla1516e-(RequestForTimeRegulationPending - timeAdvanceRequest). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RequestForTimeConstrainedPending& e) {
		String^ msg = "RtiAmb_Hla1516e-(RequestForTimeConstrainedPending - timeAdvanceRequest). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - timeAdvanceRequest). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - timeAdvanceRequest). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - timeAdvanceRequest). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - timeAdvanceRequest). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - timeAdvanceRequest). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - timeAdvanceRequest). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

// 8.9 - timeAdvanceRequestAvailable
bool RtiAmb_Hla1516e::timeAdvanceRequestAvailable(Double time) {
	try {
		// Create lookahead - logical time
		HLAfloat64Time theTime(time);

		rti->timeAdvanceRequestAvailable(theTime);
		String^ msg = "Time advance (available) is requested for logical time = " + theTime.getTime();
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return true;
	}
#pragma region exceptions
	catch (LogicalTimeAlreadyPassed& e) {
		String^ msg = "RtiAmb_Hla1516e-(LogicalTimeAlreadyPassed - timeAdvanceRequestAvailable). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (InvalidLogicalTime& e) {
		String^ msg = "RtiAmb_Hla1516e-(InvalidLogicalTime - timeAdvanceRequestAvailable). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (InTimeAdvancingState& e) {
		String^ msg = "RtiAmb_Hla1516e-(InTimeAdvancingState - timeAdvanceRequestAvailable). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RequestForTimeRegulationPending& e) {
		String^ msg = "RtiAmb_Hla1516e-(RequestForTimeRegulationPending - timeAdvanceRequestAvailable). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RequestForTimeConstrainedPending& e) {
		String^ msg = "RtiAmb_Hla1516e-(RequestForTimeConstrainedPending - timeAdvanceRequestAvailable). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - timeAdvanceRequestAvailable). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - timeAdvanceRequestAvailable). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - timeAdvanceRequestAvailable). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - timeAdvanceRequestAvailable). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - timeAdvanceRequestAvailable). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - timeAdvanceRequestAvailable). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

// 8.10 - nextMessageRequest
bool RtiAmb_Hla1516e::nextMessageRequest(Double time) {
	try {
		// Create lookahead - logical time
		HLAfloat64Time theTime(time);

		rti->nextMessageRequest(theTime);
		String^ msg = "Advance of federate's logical time to the timestamp of the next relevant TSO event is requested. The cut off point at which to stop advancing logical time is " + theTime.getTime();
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return true;
	}
#pragma region exceptions
	catch (LogicalTimeAlreadyPassed& e) {
		String^ msg = "RtiAmb_Hla1516e-(LogicalTimeAlreadyPassed - nextMessageRequest). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (InvalidLogicalTime& e) {
		String^ msg = "RtiAmb_Hla1516e-(InvalidLogicalTime - nextMessageRequest). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (InTimeAdvancingState& e) {
		String^ msg = "RtiAmb_Hla1516e-(InTimeAdvancingState - nextMessageRequest). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RequestForTimeRegulationPending& e) {
		String^ msg = "RtiAmb_Hla1516e-(RequestForTimeRegulationPending - nextMessageRequest). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RequestForTimeConstrainedPending& e) {
		String^ msg = "RtiAmb_Hla1516e-(RequestForTimeConstrainedPending - nextMessageRequest). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - nextMessageRequest). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - nextMessageRequest). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - nextMessageRequest). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - nextMessageRequest). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - nextMessageRequest). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - nextMessageRequest). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

// 8.11 - nextMessageRequestAvailable
bool RtiAmb_Hla1516e::nextMessageRequestAvailable(Double time) {
	try {
		// Create lookahead - logical time
		HLAfloat64Time theTime(time);

		rti->nextMessageRequestAvailable(theTime);
		String^ msg = "Advance of federate's logical time (nextEventRequestAvailable) to the time stamp of the next relevant TSO event is requested. The cut off point at which to stop advancing logical time is " + theTime.getTime();
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return true;
	}
#pragma region exceptions
	catch (LogicalTimeAlreadyPassed& e) {
		String^ msg = "RtiAmb_Hla1516e-(LogicalTimeAlreadyPassed - nextMessageRequestAvailable). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (InvalidLogicalTime& e) {
		String^ msg = "RtiAmb_Hla1516e-(InvalidLogicalTime - nextMessageRequestAvailable). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (InTimeAdvancingState& e) {
		String^ msg = "RtiAmb_Hla1516e-(InTimeAdvancingState - nextMessageRequestAvailable). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RequestForTimeRegulationPending& e) {
		String^ msg = "RtiAmb_Hla1516e-(RequestForTimeRegulationPending - nextMessageRequestAvailable). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RequestForTimeConstrainedPending& e) {
		String^ msg = "RtiAmb_Hla1516e-(RequestForTimeConstrainedPending - nextMessageRequestAvailable). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - nextMessageRequestAvailable). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - nextMessageRequestAvailable). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - nextMessageRequestAvailable). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - nextMessageRequestAvailable). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - nextMessageRequestAvailable). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - nextMessageRequestAvailable). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

// 8.12 - flushQueueRequest
bool RtiAmb_Hla1516e::flushQueueRequest(Double time) {
	try {
		// Create lookahead - logical time
		HLAfloat64Time theTime(time);

		rti->flushQueueRequest(theTime);
		String^ msg = "A flush of the federate's event queues is initiated. The maximum logical time which to advance upon completion of the flush is " + theTime.getTime();
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return true;
	}
#pragma region exceptions
	catch (LogicalTimeAlreadyPassed& e) {
		String^ msg = "RtiAmb_Hla1516e-(LogicalTimeAlreadyPassed - flushQueueRequest). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (InvalidLogicalTime& e) {
		String^ msg = "RtiAmb_Hla1516e-(InvalidLogicalTime - flushQueueRequest). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (InTimeAdvancingState& e) {
		String^ msg = "RtiAmb_Hla1516e-(InTimeAdvancingState - flushQueueRequest). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RequestForTimeRegulationPending& e) {
		String^ msg = "RtiAmb_Hla1516e-(RequestForTimeRegulationPending - flushQueueRequest). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RequestForTimeConstrainedPending& e) {
		String^ msg = "RtiAmb_Hla1516e-(RequestForTimeConstrainedPending - flushQueueRequest). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - flushQueueRequest). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - flushQueueRequest). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - flushQueueRequest). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - flushQueueRequest). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - flushQueueRequest). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - flushQueueRequest). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

// 8.14 - Enable Async. Delivery
bool RtiAmb_Hla1516e::enableAsynchronousDelivery() {
	try {
		rti->enableAsynchronousDelivery();
		String^ msg = "Asynchronous delivery is enabled.";
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return true;
	}
#pragma region exceptions
	catch (AsynchronousDeliveryAlreadyEnabled& e) {
		String^ msg = "RtiAmb_Hla1516e-(AsynchronousDeliveryAlreadyEnabled - enableAsynchronousDelivery). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - enableAsynchronousDelivery). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - enableAsynchronousDelivery). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - enableAsynchronousDelivery). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - enableAsynchronousDelivery). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - enableAsynchronousDelivery). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - connect). enableAsynchronousDelivery: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

// 8.15 - Disable Async. Delivery
bool RtiAmb_Hla1516e::disableAsynchronousDelivery() {
	try {
		rti->disableAsynchronousDelivery();
		String^ msg = "Asynchronous delivery is disabled.";
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return true;
	}
#pragma region exceptions
	catch (AsynchronousDeliveryAlreadyDisabled& e) {
		String^ msg = "RtiAmb_Hla1516e-(AsynchronousDeliveryAlreadyDisabled - disableAsynchronousDelivery). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - disableAsynchronousDelivery). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - disableAsynchronousDelivery). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - disableAsynchronousDelivery). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - disableAsynchronousDelivery). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - disableAsynchronousDelivery). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - disableAsynchronousDelivery). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

// 8.16 - queryGALT
bool RtiAmb_Hla1516e::queryGALT(Double% Galt) {
	try {
		HLAfloat64Time time;
		bool res = rti->queryGALT(time);// time is OUT parameter set to indicate the federate's current logical time
		Galt = time.getTime(); // return GALT value
		String^ msg = "The federate's Greatest Available Logical Time (GALT) ";
		msg += res ? "is defined" : "is undefined";// shortcut for concat with ?: operator 
		msg += " and GALT is " + Galt;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return res; // indicates GLAT is defined or not
	}
#pragma region exceptions
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - queryGALT). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - queryGALT). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - queryGALT). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - queryGALT). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - queryGALT). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - queryGALT). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

// 8.17 - queryLogicalTime
Double RtiAmb_Hla1516e::queryLogicalTime() {
	try {
		HLAfloat64Time time;
		rti->queryLogicalTime(time);// theTime is OUT parameter set to indicate the federate's current logical time

		String^ msg = "The federate's current logical time is " + time.getTime();
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return time.getTime();
	}
#pragma region exceptions
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - queryLogicalTime). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - queryLogicalTime). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - queryLogicalTime). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - queryLogicalTime). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - queryLogicalTime). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - queryLogicalTime). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

// 8.18 - queryLITS
bool RtiAmb_Hla1516e::queryLITS(Double% Lits) {
	try {
		HLAfloat64Time time;
		bool res = rti->queryLITS(time);// time is OUT parameter set to indicate the federate's current logical time
		Lits = time.getTime(); // return LITS value
		String^ msg = "The federate's List Incoming Time Stamp (LITS) ";
		msg += res ? "is defined" : "is undefined"; // shortcut for concat with ?: operator
		msg += " and LITS is " + Lits;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return res; // indicates LITS is defined or not
	}
#pragma region exceptions
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - queryLITS). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - queryLITS). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - queryLITS). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - queryLITS). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - queryLITS). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - connect). queryLITS: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

// 8.19 - modifyLookahead
bool RtiAmb_Hla1516e::modifyLookahead(Double theLookahead) {
	try {
		// Create lookahead - time interval
		HLAfloat64Interval lookahead(theLookahead);

		rti->modifyLookahead(lookahead);
		String^ msg = "Lookahead is modified as " + lookahead.getInterval();
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return true;
	}
#pragma region exceptions
	catch (InvalidLookahead& e) {
		String^ msg = "RtiAmb_Hla1516e-(InvalidLookahead - modifyLookahead). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (InTimeAdvancingState& e) {
		String^ msg = "RtiAmb_Hla1516e-(InTimeAdvancingState - modifyLookahead). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (TimeRegulationIsNotEnabled& e) {
		String^ msg = "RtiAmb_Hla1516e-(TimeRegulationIsNotEnabled - modifyLookahead). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - modifyLookahead). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - modifyLookahead). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - modifyLookahead). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - modifyLookahead). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - modifyLookahead). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - modifyLookahead). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

// 8.20 - queryLookahead
Double RtiAmb_Hla1516e::queryLookahead() {
	try {
		HLAfloat64Interval theLookahead;
		rti->queryLookahead(theLookahead);

		String^ msg = "The current lookahead interval of the local federate is " + theLookahead.getInterval();
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return theLookahead.getInterval();
	}
#pragma region exceptions
	catch (TimeRegulationIsNotEnabled& e) {
		String^ msg = "RtiAmb_Hla1516e-(TimeRegulationIsNotEnabled - queryLookahead). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - queryLookahead). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - queryLookahead). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - queryLookahead). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - queryLookahead). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - queryLookahead). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - queryLookahead). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

// 8.21 - Retract
bool RtiAmb_Hla1516e::retract(MessageRetraction ^retraction) {
	try {
		//rti->retract(*(retraction->Handle));
		rti1516e::MessageRetractionHandle handle = nom->RetractionHandles[retraction->Handle];
		rti->retract(handle);
		String^ msg = "Unscheduleding is done for event; Rectraction Handle = " + retraction->Handle;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return true;
	}
#pragma region exceptions
	catch (MessageCanNoLongerBeRetracted& e) {
		String^ msg = "RtiAmb_Hla1516e-(MessageCanNoLongerBeRetracted - retract). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (InvalidMessageRetractionHandle& e) {
		String^ msg = "RtiAmb_Hla1516e-(InvalidMessageRetractionHandle - retract). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (TimeRegulationIsNotEnabled& e) {
		String^ msg = "RtiAmb_Hla1516e-(TimeRegulationIsNotEnabled - retract). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - retract). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - retract). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - retract). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - retract). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - retract). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - retract). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

// 8.23 - changeAttributeOrderType
bool RtiAmb_Hla1516e::changeAttributeOrderType(HlaObject ^obj, List<HlaAttribute^>^ attributes, unsigned int type) {
	try {
		rti1516e::ObjectInstanceHandle handle = nom->Objects[obj->Handle];
		rti1516e::OrderType orderType = (rti1516e::OrderType)type;
		String^ tmpStr = "";
		// Create Attribute Handle Set 
		rti1516e::AttributeHandleSet ahs;
		for (int j = 0; j < attributes->Count; j++) {
			rti1516e::AttributeHandle handle = nom->Attributes[attributes[j]->Handle];
			ahs.insert(handle);
			tmpStr += attributes[j]->Name + " (" + Handle2Long(handle).ToString() + ")";
			if (j != attributes->Count - 1) tmpStr += ", ";
		}
		rti->changeAttributeOrderType(handle, ahs, orderType);
		String^ orderStr = "";
		if (orderType == rti1516e::OrderType::RECEIVE)
			orderStr = "Receive";
		else orderStr = "TimeStamp";
		String^ msg = "The ordering policy is changed to " + orderStr + " for the object (handle: " + obj->Handle + ") attributes: " + tmpStr;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return true;
	}
#pragma region exceptions
	catch (AttributeNotOwned& e) {
		String^ msg = "RtiAmb_Hla1516e-(AttributeNotOwned - changeAttributeOrderType). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (AttributeNotDefined& e) {
		String^ msg = "RtiAmb_Hla1516e-(AttributeNotDefined - changeAttributeOrderType). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (ObjectInstanceNotKnown& e) {
		String^ msg = "RtiAmb_Hla1516e-(ObjectInstanceNotKnown - changeAttributeOrderType). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - changeAttributeOrderType). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - changeAttributeOrderType). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - changeAttributeOrderType). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - changeAttributeOrderType). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - changeAttributeOrderType). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - changeAttributeOrderType). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

// 8.24 - changeInteractionOrderType
bool RtiAmb_Hla1516e::changeInteractionOrderType(HlaInteractionClass ^ic, unsigned int type) {
	try {
		rti1516e::InteractionClassHandle handle = nom->InteractionClasses[ic->Handle];
		rti1516e::OrderType orderType = (rti1516e::OrderType)type;
		rti->changeInteractionOrderType(handle, orderType);
		String^ orderStr = "";
		if (orderType == rti1516e::OrderType::RECEIVE)
			orderStr = "Receive";
		else orderStr = "TimeStamp";
		String^ msg = "The ordering policy for interaction class (" + ic->Name + ") is changed to " + orderStr;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return true;
	}
#pragma region exceptions
	catch (InteractionClassNotPublished& e) {
		String^ msg = "RtiAmb_Hla1516e-(InteractionClassNotPublished - changeInteractionOrderType). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (InteractionClassNotDefined& e) {
		String^ msg = "RtiAmb_Hla1516e-(InteractionClassNotDefined - changeInteractionOrderType). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - changeInteractionOrderType). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - changeInteractionOrderType). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - changeInteractionOrderType). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - changeInteractionOrderType). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - changeInteractionOrderType). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - changeInteractionOrderType). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

#pragma endregion	// Time Management		

#pragma region Support Services

// 10.2 getAutomaticResignDirective
Racon::ResignAction RtiAmb_Hla1516e::getAutomaticResignDirective() {
	try {
		rti1516e::ResignAction action = rti->getAutomaticResignDirective();
		// report
		String^ msg = "Automatic resign directive: " + ((Racon::ResignAction)(int)action).ToString();
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return (Racon::ResignAction)(int)action;
	}
#pragma region exceptions
	catch (FederateNotExecutionMember & e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - getAutomaticResignDirective): Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (NotConnected & e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - getAutomaticResignDirective): Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RTIinternalError & e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - getAutomaticResignDirective). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (System::Exception ^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - getAutomaticResignDirective). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

// 10.3 setAutomaticResignDirective
bool RtiAmb_Hla1516e::setAutomaticResignDirective(int action) {
	try {
		rti1516e::ResignAction directive = (rti1516e::ResignAction)action;
		rti->setAutomaticResignDirective(directive);
		// report
		String^ msg = "Automatic resign directive is set to: " + action.ToString();
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return true;
	}
#pragma region exceptions
	catch (FederateNotExecutionMember & e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - getAutomaticResignDirective): Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (NotConnected & e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - getAutomaticResignDirective): Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RTIinternalError & e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - getAutomaticResignDirective). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (System::Exception ^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - getAutomaticResignDirective). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

// 10.13 getUpdateRateValue
double RtiAmb_Hla1516e::getUpdateRateValue(String^ name) {
	try {
		pin_ptr<const wchar_t> _name = PtrToStringChars(name);
		double rate = rti->getUpdateRateValue(_name);

		// report
		String^ msg = "Update rate value for (name: " + name + "): " + rate;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return rate;
	}
#pragma region exceptions
	catch (InvalidUpdateRateDesignator& e) {
		String^ msg = "RtiAmb_Hla1516e-(InvalidUpdateRateDesignator - getUpdateRateValue): Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - getUpdateRateValue): Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - getUpdateRateValue): Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - getUpdateRateValue). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - getUpdateRateValue). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

// 10.14 getUpdateRateValueForAttribute
double RtiAmb_Hla1516e::getUpdateRateValueForAttribute(HlaObject^ objectInstance, HlaAttribute^ attribute) {
	try {
		rti1516e::ObjectInstanceHandle ohandle = nom->Objects[objectInstance->Handle];
		rti1516e::AttributeHandle handle = nom->Attributes[attribute->Handle];

		double rate = rti->getUpdateRateValueForAttribute(ohandle, handle);

		// report
		String^ msg = "Update rate value for (object: " + objectInstance->Name + ", attribute: " + attribute->Name + "): " + rate;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return rate;
	}
#pragma region exceptions
	catch (ObjectInstanceNotKnown& e) {
		String^ msg = "RtiAmb_Hla1516e-(ObjectInstanceNotKnown - getUpdateRateValueForAttribute): Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (AttributeNotDefined& e) {
		String^ msg = "RtiAmb_Hla1516e-(AttributeNotDefined - getUpdateRateValueForAttribute): Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - getUpdateRateValueForAttribute): Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - getUpdateRateValueForAttribute): Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - getUpdateRateValueForAttribute). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - getUpdateRateValueForAttribute). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

// 10.30 - setRangeBounds
void RtiAmb_Hla1516e::setRangeBounds(unsigned int regionHandle, unsigned int dimensionHandle, unsigned int lowerBound, unsigned int upperBound) {
	try {
		rti1516e::RangeBounds range;
		range.setLowerBound(lowerBound);
		range.setUpperBound(upperBound);
		// set range bounds
		rti->setRangeBounds(nom->Regions[regionHandle], nom->Dimensions[dimensionHandle], range);
		// report
		String^ msg = "The lower and upper range bounds of region (handle: " + regionHandle + ") is set for dimension (handle: " + dimensionHandle + ").";
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
#pragma region exceptions
	catch (InvalidRangeBound& e) {
		String^ msg = "RtiAmb_Hla1516e-(InvalidRangeBound - setRangeBounds): Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (RegionDoesNotContainSpecifiedDimension& e) {
		String^ msg = "RtiAmb_Hla1516e-(RegionDoesNotContainSpecifiedDimension - setRangeBounds): Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (RegionNotCreatedByThisFederate& e) {
		String^ msg = "RtiAmb_Hla1516e-(RegionNotCreatedByThisFederate - setRangeBounds): Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (InvalidRegion& e) {
		String^ msg = "RtiAmb_Hla1516e-(InvalidRegion - setRangeBounds): Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - setRangeBounds): Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - setRangeBounds): Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - setRangeBounds): Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - setRangeBounds): Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - setRangeBounds). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - setRangeBounds). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

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
		String^ msg = "RtiAmb_Hla1516e-(ObjectClassRelevanceAdvisorySwitchIsOn - enableObjectClassRelevanceAdvisorySwitch). enableObjectClassRelevanceAdvisorySwitch is already On. Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - enableClassRelevanceAdvisorySwitch).Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - enableClassRelevanceAdvisorySwitch). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - enableClassRelevanceAdvisorySwitch). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected-enableClassRelevanceAdvisorySwitch-RtiAmb_Hla1516e). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - enableClassRelevanceAdvisorySwitch). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - enableClassRelevanceAdvisorySwitch). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
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
		String^ msg = "RtiAmb_Hla1516e-(ObjectClassRelevanceAdvisorySwitchIsOff - disableClassRelevanceAdvisorySwitch). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - disableClassRelevanceAdvisorySwitch).Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - disableClassRelevanceAdvisorySwitch). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - disableClassRelevanceAdvisorySwitch). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - disableClassRelevanceAdvisorySwitch). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - disableClassRelevanceAdvisorySwitch). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - disableClassRelevanceAdvisorySwitch). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
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
		String^ msg = "RtiAmb_Hla1516e-(AttributeRelevanceAdvisorySwitchIsOn - enableAttributeRelevanceAdvisorySwitch). enableAttributeRelevanceAdvisorySwitch is already On. Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - enableAttributeRelevanceAdvisorySwitch).Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - enableAttributeRelevanceAdvisorySwitch). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - enableAttributeRelevanceAdvisorySwitch). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - enableAttributeRelevanceAdvisorySwitch). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - enableAttributeRelevanceAdvisorySwitch). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - enableAttributeRelevanceAdvisorySwitch). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
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
		String^ msg = "RtiAmb_Hla1516e-(AttributeRelevanceAdvisorySwitchIsOff - disableAttributeRelevanceAdvisorySwitch). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - disableAttributeRelevanceAdvisorySwitch).Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - disableAttributeRelevanceAdvisorySwitch). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - disableAttributeRelevanceAdvisorySwitch). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - disableAttributeRelevanceAdvisorySwitch). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - disableAttributeRelevanceAdvisorySwitch). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - disableAttributeRelevanceAdvisorySwitch). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
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
		String^ msg = "RtiAmb_Hla1516e-(AttributeScopeAdvisorySwitchIsOn - enableAttributeScopeAdvisorySwitch). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - enableAttributeScopeAdvisorySwitch).Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - enableAttributeScopeAdvisorySwitch). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - enableAttributeScopeAdvisorySwitch). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - enableAttributeScopeAdvisorySwitch). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - enableAttributeScopeAdvisorySwitch). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - enableAttributeScopeAdvisorySwitch). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
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
		String^ msg = "RtiAmb_Hla1516e-(AttributeScopeAdvisorySwitchIsOff - disableAttributeScopeAdvisorySwitch). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - disableAttributeScopeAdvisorySwitch).Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - disableAttributeScopeAdvisorySwitch). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - disableAttributeScopeAdvisorySwitch). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - disableAttributeScopeAdvisorySwitch). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - disableAttributeScopeAdvisorySwitch). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - disableAttributeScopeAdvisorySwitch). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
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
		String^ msg = "RtiAmb_Hla1516e-(InteractionRelevanceAdvisorySwitchIsOn - enableInteractionRelevanceAdvisorySwitch). InteractionRelevanceAdvisorySwitch is already On. Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - enableInteractionRelevanceAdvisorySwitch).Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - enableInteractionRelevanceAdvisorySwitch). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - enableInteractionRelevanceAdvisorySwitch). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - enableInteractionRelevanceAdvisorySwitch). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - enableInteractionRelevanceAdvisorySwitch). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - enableInteractionRelevanceAdvisorySwitch). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
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
		String^ msg = "RtiAmb_Hla1516e-(InteractionRelevanceAdvisorySwitchIsOff - disableInteractionRelevanceAdvisorySwitch). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla1516e-(FederateNotExecutionMember - disableInteractionRelevanceAdvisorySwitch).Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - disableInteractionRelevanceAdvisorySwitch). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - disableInteractionRelevanceAdvisorySwitch). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - disableInteractionRelevanceAdvisorySwitch). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - disableInteractionRelevanceAdvisorySwitch). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - disableInteractionRelevanceAdvisorySwitch). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
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
		String^ msg = "RtiAmb_Hla1516e-(CallNotAllowedFromWithinCallback - evokeCallback). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - evokeCallback). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - evokeCallback). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - evokeCallback). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
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
		String^ msg = "RtiAmb_Hla1516e-(CallNotAllowedFromWithinCallback - evokeMultipleCallbacks). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (NotConnected& e) {
		String^ msg = "RtiAmb_Hla1516e-(NotConnected - evokeMultipleCallbacks). Reason: " + gcnew String(e.what().c_str());
		this->OnNotConnected(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - evokeMultipleCallbacks). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - evokeMultipleCallbacks). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

// 10.43 enableCallbacks
bool RtiAmb_Hla1516e::enableCallbacks() {
	try {
		rti->enableCallbacks();
		// report
		String^ msg = "Callbacks are enabled.";
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return true;
	}
#pragma region exceptions
	catch (SaveInProgress & e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - enableCallbacks): Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress & e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - enableCallbacks): Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RTIinternalError & e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - enableCallbacks). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (System::Exception ^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - enableCallbacks). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

// 10.44 disableCallbacks
bool RtiAmb_Hla1516e::disableCallbacks() {
	try {
		rti->disableCallbacks();
		// report
		String^ msg = "Callbacks are disabled.";
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
		return true;
	}
#pragma region exceptions
	catch (SaveInProgress & e) {
		String^ msg = "RtiAmb_Hla1516e-(SaveInProgress - disableCallback): Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RestoreInProgress & e) {
		String^ msg = "RtiAmb_Hla1516e-(RestoreInProgress - disableCallback): Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RTIinternalError & e) {
		String^ msg = "RtiAmb_Hla1516e-(RTIinternalError - disableCallback). Reason: " + gcnew String(e.what().c_str());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (System::Exception ^ e) {
		String^ msg = "RtiAmb_Hla1516e-(Exception - disableCallback). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

//// getDimensionHandle
//unsigned int RtiAmb_Hla1516e::getDimensionHandle(String^ dim, unsigned int space) {
//  return 0;
//};

#pragma endregion // Support Services

#pragma region Helpers
#pragma endregion // Helpers			
