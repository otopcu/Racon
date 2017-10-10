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
#include "RtiAmb_Hla13.h"

using namespace System;
using namespace System::Runtime::InteropServices;// For Marshalling
using namespace System::Threading; // for sleep
// Racon
using namespace Racon;
using namespace Racon::RtiLayer;
using namespace Racon::RtiLayer::Native;

// Constructor
#pragma region Constructor
RtiAmb_Hla13::RtiAmb_Hla13(CallbackManager^ eventManager) : RtiAmb() {
  try {
    rti = new RTI::RTIambassador();
    nom = new NomHla13();
    _nativeFdAmb = new FdAmb_Hla13(eventManager);
  }
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - Ctor). The native rti ambassador pointer creation  is failed. This may indicate a corrupted or a wrong RTI.rid file. Please ensure that RTI.rid is provided by DMSO HLA RTI13-NG distribution. Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}

};
#pragma endregion			

#pragma region Federation Management
// Connect
void RtiAmb_Hla13::connect(Racon::CallbackModel callbackModel, String ^localSetting) {
  // For IEEE1516-2010 - Pseudo Connect - Returns always connected
  this->OnFederateConnected(gcnew RaconEventArgs(""));
};
// Disconnect
void RtiAmb_Hla13::disconnect() {
  // For IEEE1516-2010 - Pseudo Disconnect - Returns always connected
  this->OnFederateDisconnected(gcnew RaconEventArgs(""));
};

// Create Federation
void RtiAmb_Hla13::createFederation(String ^fedexec, String ^fdd, String ^logicalTimeImplementationName) {
  try {
    String^ msg = "Federation execution (name: " + fedexec + ") with FED ("+ fdd + ")";
    rti->createFederationExecution((char*)Marshal::StringToHGlobalAnsi(fedexec).ToPointer(), (char*)Marshal::StringToHGlobalAnsi(fdd).ToPointer());
    msg += " is created.";
    this->OnFederationExecutionCreated(gcnew RaconEventArgs(msg));
  }
  catch (RTI::FederationExecutionAlreadyExists&) {
    String^ msg = "Federation execution (name: " + fedexec + ") already exists.";
    this->OnFederationExecutionCreated(gcnew RaconEventArgs(msg));
  }
  catch (RTI::CouldNotOpenFED& e) {
		String^ msg = "RtiAmb_Hla13-(CouldNotOpenFED - createFederation). Federate cannot open the FED file. Either the file does not exist or there exists an error in the file name or path provided. Please, check that FED file exists and its path is correct. Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
  }
  catch (RTI::ErrorReadingFED& e) {
		String^ msg = "RtiAmb_Hla13-(ErrorReadingFED - createFederation). Federate cannot read the FED file. The FED file appears to be corrupt. Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
  }
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - createFederation). The Federate cannot create the federation. Please check that RTI is running. Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
};

// 4.7 - listFederationExecutions //not supported in HLA 1.3 specification
void RtiAmb_Hla13::listFederationExecutions() {
	try {
		String^ msg = "listFederationExecutions() call is not supported in HLA 1.3 specification.";
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
#pragma region exceptions
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

// Join Federation
unsigned int RtiAmb_Hla13::joinFederationExecution(String ^fdName, String ^fedexec) {
  bool joined = false;
  int numTries = 0;
  while ((numTries++ < 20) && !joined) {
    try {
      RTI::FederateHandle federateHandle = rti->joinFederationExecution((const char*)(Marshal::StringToHGlobalAnsi(fdName)).ToPointer(), (char*)Marshal::StringToHGlobalAnsi(fedexec).ToPointer(), _nativeFdAmb);
      joined = true;
      String^ msg = "Federate joined to the federation execution. Federate handle: " + federateHandle;
      this->OnFederateJoined(gcnew RaconEventArgs(msg));
			return federateHandle;
    }
    catch (RTI::FederateAlreadyExecutionMember& e) {
			String^ msg = "RtiAmb_Hla13-(FederateAlreadyExecutionMember - joinFederation). Federate already is an federation execution member. Reason: ";
			this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
    }
    catch (RTI::FederationExecutionDoesNotExist& e) {
			String^ msg = "RtiAmb_Hla13-(FederationExecutionDoesNotExist - joinFederation). Federation Execution does not exist. Please, restart the federate. Reason: " + gcnew String(e._reason);
			this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
    }
    catch (RTI::CouldNotOpenFED& e) {
			String^ msg = "RtiAmb_Hla13-(CouldNotOpenFED - joinFederation). Federate cannot open the FED file. Either the file does not exist or there exists an error in the file name or path provided. Please, check that FED file exists and its path is correct. Reason: " + gcnew String(e._reason);
			this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
    }
    catch (RTI::ErrorReadingFED& e) {
			String^ msg = "RtiAmb_Hla13-(ErrorReadingFED - joinFederation). Federate cannot read the FED file. The FED file appears to be corrupt. Reason: " + gcnew String(e._reason);
			this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
    }
		catch (System::Exception^ e) {
			String^ msg = "RtiAmb_Hla13-(Exception - joinFederation). Reason: " + gcnew String(e->ToString());
			this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		}
		//finally{
    //	// free the unmanaged string.
    //	//Marshal::FreeHGlobal(IntPtr(federationTxt));
    //};
  } // end of while
};

// Resign Federation
void RtiAmb_Hla13::resignFederation(int resignAction) {
  String ^resign;
  try {
    switch (resignAction) {
    case 0: rti->resignFederationExecution(RTI::RELEASE_ATTRIBUTES);
      resign = "RELEASE_ATTRIBUTES";
      break;
    case 1: rti->resignFederationExecution(RTI::DELETE_OBJECTS);
      resign = "DELETE_OBJECTS";
      break;
    case 2: rti->resignFederationExecution(RTI::DELETE_OBJECTS_AND_RELEASE_ATTRIBUTES);
      resign = "DELETE_OBJECTS_AND_RELEASE_ATTRIBUTES";
      break;
    case 3: rti->resignFederationExecution(RTI::NO_ACTION);
      resign = "NO_ACTION";
      break;
    case 4: rti->resignFederationExecution(RTI::NO_ACTION);
      resign = "NO_ACTION (4-Not defined in HLA13)";
      break;
    case 5: rti->resignFederationExecution(RTI::NO_ACTION);
      resign = "NO_ACTION (5-Not defined in HLA13)";
      break;
    default: rti->resignFederationExecution(RTI::NO_ACTION);
      resign = "NO_ACTION (6-Not defined in HLA13)";
      break;
    }
    //rti->resignFederationExecution(RTI::DELETE_OBJECTS_AND_RELEASE_ATTRIBUTES);
    String^ msg = "Federate resigned from the federation execution. Resign action is " + resign + ".";
    this->OnFederateResigned(gcnew RaconEventArgs(msg));
  }
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}

};

// Destroy Federation
void RtiAmb_Hla13::destroyFederation(String ^fedexec) {
  try {
    rti->destroyFederationExecution((const char*)(Marshal::StringToHGlobalAnsi(fedexec)).ToPointer());
    String^ msg = "Federation execution is finalized.";
    this->OnFederationExecutionDestroyed(gcnew RaconEventArgs(msg));
  }
  catch (RTI::FederatesCurrentlyJoined&) {
    String^ msg = "There are still federates joined in the federation. Federation execution is not destroyed.";
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
  }
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
};

// 4.11 - registerFederationSynchronizationPoint
void RtiAmb_Hla13::registerFederationSynchronizationPoint(String^ synchronizationPointLabel, String^ userSuppliedTag) {
	try {
		rti->registerFederationSynchronizationPoint((char*)Marshal::StringToHGlobalAnsi(synchronizationPointLabel).ToPointer(), (char*)Marshal::StringToHGlobalAnsi(userSuppliedTag).ToPointer());
		String^ msg = "Federate requested a register of a federation synchronization point. Label: " + synchronizationPointLabel + ". Tag: " + userSuppliedTag;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
#pragma region exceptions
	catch (RTI::FederateNotExecutionMember& e) {
		String^ msg = "MSG-(FederateNotExecutionMember - registerFederationSynchronizationPoint):" + Environment::NewLine + " Reason: " + gcnew String(e._reason) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RTI::ConcurrentAccessAttempted& e) {
		String^ msg = "MSG-(ConcurrentAccessAttempted - registerFederationSynchronizationPoint):" + Environment::NewLine + " Reason: " + gcnew String(e._reason) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RTI::SaveInProgress& e) {
		String^ msg = "MSG-(SaveInProgress - registerFederationSynchronizationPoint):" + Environment::NewLine + " Reason: " + gcnew String(e._reason) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RTI::RestoreInProgress& e) {
		String^ msg = "MSG-(RestoreInProgress - registerFederationSynchronizationPoint):" + Environment::NewLine + " Reason: " + gcnew String(e._reason) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RTI::RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

// 4.11 - registerFederationSynchronizationPoint with a set of federates
void RtiAmb_Hla13::registerFederationSynchronizationPoint(String^ synchronizationPointLabel, String^ userSuppliedTag, List<unsigned int>^ setOfJoinedFederateDesignators) {
	try {
		RTI::FederateHandleSet* federates = RTI::FederateHandleSetFactory::create(setOfJoinedFederateDesignators->Count);

		for (int i = 0; i < setOfJoinedFederateDesignators->Count; i++) {
			federates->add(setOfJoinedFederateDesignators[i]);
		}

		rti->registerFederationSynchronizationPoint((char*)Marshal::StringToHGlobalAnsi(synchronizationPointLabel).ToPointer(), (char*)Marshal::StringToHGlobalAnsi(userSuppliedTag).ToPointer(), *federates);
		String^ msg = "Federate requested a register of a federation synchronization point. Label: " + synchronizationPointLabel + ". Tag: " + userSuppliedTag;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
#pragma region exceptions
	catch (RTI::FederateNotExecutionMember& e) {
		String^ msg = "MSG-(FederateNotExecutionMember - registerFederationSynchronizationPoint):" + Environment::NewLine + " Reason: " + gcnew String(e._reason) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RTI::ConcurrentAccessAttempted& e) {
		String^ msg = "MSG-(ConcurrentAccessAttempted - registerFederationSynchronizationPoint):" + Environment::NewLine + " Reason: " + gcnew String(e._reason) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RTI::SaveInProgress& e) {
		String^ msg = "MSG-(SaveInProgress - registerFederationSynchronizationPoint):" + Environment::NewLine + " Reason: " + gcnew String(e._reason) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RTI::RestoreInProgress& e) {
		String^ msg = "MSG-(RestoreInProgress - registerFederationSynchronizationPoint):" + Environment::NewLine + " Reason: " + gcnew String(e._reason) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RTI::RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

// 4.14 - synchronizationPointAchieved
void RtiAmb_Hla13::synchronizationPointAchieved(String^ synchronizationPointLabel, bool synchronizationSuccess) {
	try {
		rti->synchronizationPointAchieved((char*)Marshal::StringToHGlobalAnsi(synchronizationPointLabel).ToPointer());
		String^ msg = "Federation synchronization point is achieved. Label: " + synchronizationPointLabel ;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
#pragma region exceptions
	catch (RTI::SynchronizationPointLabelWasNotAnnounced& e) {
		String^ msg = "MSG-(SynchronizationPointLabelWasNotAnnounced - synchronizationPointAchieved):" + Environment::NewLine + " Reason: " + gcnew String(e._reason) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RTI::FederateNotExecutionMember& e) {
		String^ msg = "MSG-(FederateNotExecutionMember - synchronizationPointAchieved):" + Environment::NewLine + " Reason: " + gcnew String(e._reason) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RTI::ConcurrentAccessAttempted& e) {
		String^ msg = "MSG-(ConcurrentAccessAttempted - synchronizationPointAchieved):" + Environment::NewLine + " Reason: " + gcnew String(e._reason) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RTI::SaveInProgress& e) {
		String^ msg = "MSG-(SaveInProgress - synchronizationPointAchieved):" + Environment::NewLine + " Reason: " + gcnew String(e._reason) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RTI::RestoreInProgress& e) {
		String^ msg = "MSG-(RestoreInProgress - synchronizationPointAchieved):" + Environment::NewLine + " Reason: " + gcnew String(e._reason) + Environment::NewLine;
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
	}
	catch (RTI::RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

// Request Federation Save - save as soon as possible
void RtiAmb_Hla13::requestFederationSave(String^ _label) {
  try {
    rti->requestFederationSave((char*)Marshal::StringToHGlobalAnsi(_label).ToPointer());
    String^ msg = "This federate requested a federation save (with label: " + _label + ")." + Environment::NewLine;
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
  }
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
};

// Request Federation Save with Time - save at the specified time
void RtiAmb_Hla13::requestFederationSave(String^ _label, Double time) {
  try {
    RTIfedTime time(time);
    RTI::FedTime& theTime = time;

    rti->requestFederationSave((char*)Marshal::StringToHGlobalAnsi(_label).ToPointer(), theTime);
    String^ msg = "This federate requested a federation save with label: " + _label + " and time = " + time.getTime() + "." + Environment::NewLine;
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
  }
#pragma region exceptions
  catch (RTI::FederationTimeAlreadyPassed &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::InvalidFederationTime &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::ConcurrentAccessAttempted& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

// Federate Save Begun
void RtiAmb_Hla13::federateSaveBegun() {
  try {
    rti->federateSaveBegun();
    String^ msg = "This federate informed RTI that it has begun saving." + Environment::NewLine;
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
  }
  catch (RTI::SaveNotInitiated& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). There are no current outstanding save requests of the local federate. Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
  }
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
};

// Federate Save Complete
void RtiAmb_Hla13::federateSaveComplete(bool _IsCompleted) {
  try {
    String^ msg = "";
    if (_IsCompleted)
    {
      rti->federateSaveComplete();
      msg = "This federate informed RTI that it completed saving successfully." + Environment::NewLine;
    }
    else
    {
      rti->federateSaveNotComplete();
      msg = "This federate informed RTI that it diidn't complete saving." + Environment::NewLine;
    }
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
  }
  catch (RTI::SaveNotInitiated& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). There are no current outstanding save requests or the federate failed to indicate the beginning of its save (i.e., via the federateSaveBegun() method). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
  }
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
};

// Request Federation Restore
void RtiAmb_Hla13::requestFederationRestore(String^ _label) {
  try {
    rti->requestFederationRestore((char*)Marshal::StringToHGlobalAnsi(_label).ToPointer());
    String^ msg = "This federate requested a federation restore (with label: " + _label + ")." + Environment::NewLine;
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
  }
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
};

// Federate Restore Complete
void RtiAmb_Hla13::federateRestoreComplete(bool _IsCompleted) {
  try {
    String^ msg = "";
    if (_IsCompleted)
    {
      rti->federateRestoreComplete();
      msg = "This federate informed RTI that it restored." + Environment::NewLine;
    }
    else
    {
      rti->federateRestoreNotComplete();
      msg = "This federate informed RTI that it couldn't complete restore." + Environment::NewLine;
    }
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
  }
  catch (RTI::RestoreNotRequested& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). There is no outstanding request for a federate restoration.Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
  }
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
};
#pragma endregion	// Federation Management

#pragma region Declaration Management
// publishOC
void RtiAmb_Hla13::publishObjectClass(HlaObjectClass ^oc, List<HlaAttribute^>^ attributes)
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
    RTI::AttributeHandleSet* ahs = RTI::AttributeHandleSetFactory::create(tmpList->Count);
    // Add attribute handles to AHS
    for (int j = 0; j < tmpList->Count; j++) {
      ahs->add(tmpList[j]->Handle);
    }

    // Check that ahs is not null 
    if (ahs) {
      rti->publishObjectClass(oc->Handle, *ahs);

      // Report attributes to the user
      String^ tmpStr = "";
      for (int j = 0; j < tmpList->Count; j++) {
        tmpStr += tmpList[j]->Name;
        if (j != tmpList->Count - 1) tmpStr += ", ";
      };
      String^ msg = "Object Class (name: " + oc->Name + ") is published. Class handle: " + oc->Handle + ", Attributes: {" + tmpStr + "}";
      this->OnHLAClassPublished(gcnew RaconEventArgs(msg));
      // Release Mem
      ahs->empty(); delete ahs;
    }
  }
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
};

// subscribeOC with selected attributes
bool RtiAmb_Hla13::subscribeObjectClass(HlaObjectClass ^oc, List<HlaAttribute^>^ attributes, Boolean active) {
  this->getClassHandleFromRti(oc);
  // Check that OC is Subscribable
  if ((oc->ClassPS == PSKind::Subscribe) || (oc->ClassPS == PSKind::PublishSubscribe))
    try {
    // Construct tmp list - according to the PS stat of the class
    List<HlaAttribute^> ^tmpList = gcnew List<HlaAttribute^>();
    for each (HlaAttribute^ attr in attributes) {
      attr->Handle = this->getAttributeHandle(attr, oc);
      if ((attr->AttributePS == PSKind::Subscribe) || (attr->AttributePS == PSKind::PublishSubscribe)) {
        tmpList->Add(attr);
      }
    }
    RTI::AttributeHandleSet* ahs = RTI::AttributeHandleSetFactory::create(tmpList->Count);
    // Add attribute handles to AHS
    for (int j = 0; j < tmpList->Count; j++) {
      ahs->add(tmpList[j]->Handle);
    }
    // Check that ahs is not null
    if (ahs) {
      rti->subscribeObjectClassAttributes(oc->Handle, *ahs, (RTI::Boolean)active);
      String^ tmpStr = "";
      // Report attributes to the user
      for (int j = 0; j < tmpList->Count; j++) {
        tmpStr += tmpList[j]->Name;
        if (j != tmpList->Count - 1) tmpStr += ", ";
      };
      String^ msg = "Object Class (name: " + oc->Name + ") is subscribed with some selected attributes. Class handle: " + oc->Handle + ", Attributes: {" + tmpStr + "}";
      this->OnHLAClassSubscribed(gcnew RaconEventArgs(msg));
      // Release Mem
      ahs->empty(); delete ahs;
    }
    return true;
  }
#pragma region exceptions
  catch (RTI::ObjectClassNotDefined& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
  }
  catch (RTI::AttributeNotDefined &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
  }
  catch (RTI::FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
  }
  catch (RTI::ConcurrentAccessAttempted& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
  }
  catch (RTI::SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
  }
  catch (RTI::RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
  }
  catch (RTI::RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
  }
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

// publishIC
void RtiAmb_Hla13::publishInteractionClass(HlaInteractionClass ^ic) {
  // Check that interaction is Publishable
  if ((ic->ClassPS == PSKind::Publish) || (ic->ClassPS == PSKind::PublishSubscribe))
    try {
    // publish
    rti->publishInteractionClass(ic->Handle);
    String^ msg = "Interaction Class (name: " + ic->Name + ") is published. Class handle: " + ic->Handle;
    this->OnHLAClassPublished(gcnew RaconEventArgs(msg));
  }
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
};

// SubscribeIC
void RtiAmb_Hla13::subscribeInteractionClass(HlaInteractionClass ^ic) {

  String^ msg = "";
  // Check that interaction is Subscribable
  if ((ic->ClassPS == PSKind::Subscribe) || (ic->ClassPS == PSKind::PublishSubscribe))
    try {
    rti->subscribeInteractionClass(ic->Handle);
    msg = "Interaction Class (name: " + ic->Name + ", handle: " + ic->Handle + ") is subscribed.";
    //}
    this->OnHLAClassSubscribed(gcnew RaconEventArgs(msg));
  }
#pragma region exceptions
  catch (RTI::InteractionClassNotDefined& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RegionNotKnown& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::InvalidRegionContext& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). One or more of the specified attribute handles is not valid within the context of the specified region. Please, check that the interaction class is associated with a routing space in FED file. Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
  }
  catch (RTI::FederateLoggingServiceCalls& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::ConcurrentAccessAttempted& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

// unsubscribeIC
bool RtiAmb_Hla13::unsubscribeInteractionClass(HlaInteractionClass ^ic) {
  try {
    rti->unsubscribeInteractionClass(ic->Handle);
    String^ msg = "Interaction Class (name: " + ic->Name + ") is Unsubscribed. Class handle: " + ic->Handle;
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return true;
  }
#pragma region exceptions
  catch (RTI::InteractionClassNotDefined& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::InteractionClassNotSubscribed& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::ConcurrentAccessAttempted& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

// unsubscribe OC 
bool RtiAmb_Hla13::unsubscribeObjectClass(HlaObjectClass ^oc) {
  try {
    rti->unsubscribeObjectClass(oc->Handle);
    String^ msg = "Object Class (name: " + oc->Name + ", handle:" + oc->Handle + ") is unsubscribed.";
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return true;
  }
#pragma region exceptions
  catch (RTI::ObjectClassNotDefined& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
  }
  catch (RTI::ObjectClassNotSubscribed &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
  }
  catch (RTI::FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
  }
  catch (RTI::ConcurrentAccessAttempted& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
  }
  catch (RTI::SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
  }
  catch (RTI::RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
  }
  catch (RTI::RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
  }
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
#pragma endregion
};

#pragma endregion	// Declaration Management		

#pragma region Object Management

// Delete an Object Instance
bool RtiAmb_Hla13::deleteObjectInstance(HlaObject ^obj, String^ tag) {
  try {
    rti->deleteObjectInstance(obj->Handle, (char*)Marshal::StringToHGlobalAnsi(tag).ToPointer());
    String^ msg = "Object (handle: " + obj->Handle + ") is deleted by this federate. Reason: " + tag;
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return true;
  }
#pragma region exceptions
  catch (RTI::ObjectNotKnown &e) {
    String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
  }
  catch (RTI::DeletePrivilegeNotHeld &e) {
    String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
  }
  catch (RTI::InvalidFederationTime &e) {
    String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
  }
  catch (RTI::FederateNotExecutionMember& e) {
    String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
  }
  catch (RTI::ConcurrentAccessAttempted& e) {
    String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
  }
  catch (RTI::SaveInProgress& e) {
    String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
  }
  catch (RTI::RestoreInProgress& e) {
    String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
  }
  catch (RTI::RTIinternalError& e) {
    String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
  }
  catch (System::Exception^ e) {
    String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
  }
#pragma endregion
};

// Delete an Object Instance with Time
MessageRetraction^ RtiAmb_Hla13::deleteObjectInstance(HlaObject ^obj, Double time, String^ tag) {
  try {
    RTIfedTime time(time);
    RTI::FedTime& theTime = time;

    RTI::EventRetractionHandle theHandle = rti->deleteObjectInstance(obj->Handle, theTime, (char*)Marshal::StringToHGlobalAnsi(tag).ToPointer());
		MessageRetraction^ retraction = gcnew MessageRetraction(theHandle.theSerialNumber, theHandle.sendingFederate);
    String^ msg = "Object (handle: " + obj->Handle + ") is scheduled for deletion by this federate at time = " + time.getTime() + ". Retraction Serial No = " + retraction->SerialNumber + ". Reason: " + tag;
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return retraction;
  }
#pragma region exceptions
  catch (RTI::ObjectNotKnown &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::DeletePrivilegeNotHeld &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::InvalidFederationTime &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::ConcurrentAccessAttempted& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

// Register One Object
bool RtiAmb_Hla13::registerObject(HlaObject ^obj) {
  try {
    obj->Handle = rti->registerObjectInstance(obj->Type->Handle);
    String^ msg = "Object (class name: " + obj->Type->Name + ") is registered. Object handle: " + obj->Handle;
    this->OnObjectRegistered(gcnew RaconEventArgs(msg));
    return true;
  }
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
};

// Register One Object with an object name
bool RtiAmb_Hla13::registerObject(HlaObject ^obj, String^ name) {
  try {
    obj->Handle = rti->registerObjectInstance(obj->Type->Handle, (char*)Marshal::StringToHGlobalAnsi(name).ToPointer());
    String^ msg = "Object (class name: " + obj->Type->Name + ") is registered. Object handle: " + obj->Handle + " Object name: " + obj->Name;
    this->OnObjectRegistered(gcnew RaconEventArgs(msg));
    return true;
  }
  catch (RTI::ObjectAlreadyRegistered&) {
    String^ msg = "MSG-(ObjectAlreadyRegistered - registerObject2): The symbolic name associated with the object has already been registered within the federation.";
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return false;
  }
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
};

// Send Interaction
bool RtiAmb_Hla13::sendInteraction(HlaInteraction ^interaction) {
  try {
    String ^msg = "";

    // Pack parameter values
    RTI::ParameterHandleValuePairSet *phvps = RTI::ParameterSetFactory::create(interaction->Parameters->Count);
    for each (HlaParameter^ var in interaction->Parameters)
    {
      char* _pVal = (char*)var->Value.ToPointer();
      msg += var->Name + " (" + var->Length + ") ";// +sizeof(double);
      phvps->add(var->Handle, (char*)_pVal, var->Length);
      //phvps->add(var->Handle, (char*)_pVal, ((strlen(_pVal) + 1)*sizeof(char)));
      Marshal::FreeHGlobal(IntPtr(_pVal));
    }

    rti->sendInteraction(interaction->ClassHandle, *phvps, (char*)(Marshal::StringToHGlobalAnsi(interaction->Tag)).ToPointer());
    msg += "Interaction (handle: " + interaction->ClassHandle + ") is sent.";
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return true;
  }
#pragma region exceptions
  catch (RTI::InteractionClassNotDefined &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::InteractionClassNotPublished &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::InteractionParameterNotDefined &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::ConcurrentAccessAttempted& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
  }
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
#pragma endregion
};

// Send Interaction with Time stamp
MessageRetraction^ RtiAmb_Hla13::sendInteraction(HlaInteraction ^interaction, Double timestamp) {
  try {
    String ^msg = "";
    RTIfedTime time(timestamp);
    RTI::FedTime& theTime = time;
    // Pack parameter values
    RTI::ParameterHandleValuePairSet *phvps = RTI::ParameterSetFactory::create(interaction->Parameters->Count);
    for each (HlaParameter^ var in interaction->Parameters)
    {
      char* _pVal = (char*)var->Value.ToPointer();
      //msg += "Size: " + var->Length;
      phvps->add(var->Handle, (char*)_pVal, var->Length);
      Marshal::FreeHGlobal(IntPtr(_pVal));
    }
    RTI::EventRetractionHandle erh = rti->sendInteraction(interaction->ClassHandle, *phvps, theTime, (char*)(Marshal::StringToHGlobalAnsi(interaction->Tag)).ToPointer());

		MessageRetraction^ retraction = gcnew MessageRetraction(erh.theSerialNumber, erh.sendingFederate);

    msg += "Interaction (handle: " + interaction->ClassHandle + ") is sent with timestamp: " + time.getTime() + ". Retraction serial no = " + retraction->SerialNumber + ". Retraction sending federate = " + retraction->SendingFederate;
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return retraction;
  }
#pragma region exceptions
  catch (RTI::InteractionClassNotDefined &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::InteractionClassNotPublished &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::InteractionParameterNotDefined &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::InvalidFederationTime &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::ConcurrentAccessAttempted& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

// Update Attribute Values
bool RtiAmb_Hla13::updateAttributeValues(HlaObject ^object) {
  try {
    // Pack attribute values
    RTI::AttributeHandleValuePairSet *ahvps = RTI::AttributeSetFactory::create(object->Attributes->Count);
    for each (HlaAttribute^ var in object->Attributes)
    {
      char* _pVal = (char*)var->Value.ToPointer();
      ahvps->add(var->Handle, (char*)_pVal, var->Length);
      //ahvps->add(var->Handle, (char*)_pVal, ((strlen(_pVal) + 1)*sizeof(char)));
      //Marshal::FreeHGlobal(IntPtr(_pVal));
    }

    (void)rti->updateAttributeValues(object->Handle, *ahvps, (char*)Marshal::StringToHGlobalAnsi(object->Tag).ToPointer());
    String^ msg = "Attribute values for object (handle: " + object->Handle + ") are updated.";
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return true;
  }
#pragma region exceptions
  catch (RTI::ObjectNotKnown &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
  }
  catch (RTI::AttributeNotDefined &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
  }
  catch (RTI::AttributeNotOwned &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
  }
  catch (RTI::InvalidFederationTime &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
  }
  catch (RTI::FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
  }
  catch (RTI::ConcurrentAccessAttempted& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
  }
  catch (RTI::SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
  }
  catch (RTI::RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
  }
  catch (RTI::RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
  }
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
#pragma endregion
};

// Update Attribute Values with Timestamp
MessageRetraction^ RtiAmb_Hla13::updateAttributeValues(HlaObject ^object, Double timestamp) {
  try {
    RTIfedTime time(timestamp);
    RTI::FedTime& theTime = time;
    // Pack attribute values
    RTI::AttributeHandleValuePairSet *ahvps = RTI::AttributeSetFactory::create(object->Attributes->Count);
    for each (HlaAttribute^ var in object->Attributes)
    {
      char* _pVal = (char*)var->Value.ToPointer();
      ahvps->add(var->Handle, (char*)_pVal, var->Length);
      //Marshal::FreeHGlobal(IntPtr(_pVal));
    }

    RTI::EventRetractionHandle erh = rti->updateAttributeValues(object->Handle, *ahvps, theTime, (char*)Marshal::StringToHGlobalAnsi(object->Tag).ToPointer());
		MessageRetraction^ retraction = gcnew MessageRetraction(erh.theSerialNumber, erh.sendingFederate);

    String^ msg = "Attribute values for object (handle: " + object->Handle + ") are updated. Retraction Serial No = " + retraction->SerialNumber + ".";
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return retraction;
  }
#pragma region exceptions
  catch (RTI::ObjectNotKnown &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::AttributeNotDefined &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::AttributeNotOwned &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::InvalidFederationTime &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::ConcurrentAccessAttempted& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

// Request Update for All Attributes of All Objects Related to a Specific Object Class
bool RtiAmb_Hla13::requestAttributeValueUpdate(HlaObjectClass ^oc, List<HlaAttribute^>^ attributes) {
  // Request update for all the attribute values of all objects in the specified object class
  try {
    // Create Attribute Handle Set 
    RTI::AttributeHandleSet* ahs = RTI::AttributeHandleSetFactory::create(attributes->Count);
    String^ attStr = "";
    // Add attribute handles to AHS
    for (int j = 0; j < attributes->Count; j++) {
      ahs->add(attributes[j]->Handle);
      attStr += attributes[j]->Name;
      if (j != attributes->Count - 1) attStr += ", ";
    }

    this->rti->requestClassAttributeValueUpdate(oc->Handle, *ahs);
    String^ msg = "Attribute value update for class (name: " + oc->Name + ", handle: " + oc->Handle + ") is requested for attributes:" + attStr + ".";
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    ahs->empty(); delete ahs;
    return true;
  }
#pragma region exceptions
  catch (RTI::ObjectClassNotDefined &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
  }
  catch (RTI::AttributeNotDefined &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
  }
  catch (RTI::FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::ConcurrentAccessAttempted& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

// Request Update for All Attributes of a specific object
bool RtiAmb_Hla13::requestAttributeValueUpdate(HlaObject ^obj, List<HlaAttribute^>^ attributes) {
  // Request update for all the attribute values of all objects in the specified object class
  try {
    // Create Attribute Handle Set 
    RTI::AttributeHandleSet* ahs = RTI::AttributeHandleSetFactory::create(attributes->Count);
    String^ attStr = "";
    // Add attribute handles to AHS
    for (int j = 0; j < attributes->Count; j++) {
      ahs->add(attributes[j]->Handle);
      attStr += attributes[j]->Name;
      if (j != attributes->Count - 1) attStr += ", ";
    }
    this->rti->requestObjectAttributeValueUpdate(obj->Handle, *ahs);
    String^ msg = "Attribute value update for object (name: " + obj->Handle + ", handle: " + obj->Handle + ") is requested for attributes:" + attStr + ".";
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    ahs->empty(); delete ahs;
    return true;
  }
#pragma region exceptions
  catch (RTI::ObjectNotKnown &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
  }
  catch (RTI::AttributeNotDefined &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
  }
  catch (RTI::FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::ConcurrentAccessAttempted& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};
#pragma endregion	// Object Management	

#pragma region Ownership Management

// attributeOwnershipAcquisitionIfAvailable
bool RtiAmb_Hla13::attributeOwnershipAcquisitionIfAvailable(HlaObject ^object, List<HlaAttribute^>^ _set) {
  try {
    // Create Attribute Handle Set 
    RTI::AttributeHandleSet* ahs = RTI::AttributeHandleSetFactory::create(_set->Count);
    // Add Attribute Handles to set
    for each (HlaAttribute^ attr in _set)
      ahs->add(attr->Handle);
    (void)rti->attributeOwnershipAcquisitionIfAvailable(object->Handle, *ahs);
    String^ msg = "Ownership of attributes that are unowned of object (handle: " + object->Handle + ") are requested.";
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return true;
  }
  catch (RTI::ObjectNotKnown&e) {
    String^ msg = "MSG-(ObjectNotKnown - attributeOwnershipAcquisitionIfAvailable):" + Environment::NewLine + " Reason: " + gcnew String(e._reason) + Environment::NewLine;
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return false;
  }
  catch (RTI::ObjectClassNotPublished &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). The operation attempted requires that the object class be currently published by the federate.Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
    return false;
  }
  catch (RTI::AttributeNotDefined &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
  }
  catch (RTI::AttributeNotPublished &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). One or more of the specified attributes are not currently published by the local federate.Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
    return false;
  }
  catch (RTI::FederateOwnsAttributes &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). One or more attributes of the specified object are owned by the local federate.Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
    return false;
  }
  catch (RTI::AttributeAlreadyBeingAcquired &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). The federate is already in the process of acquiring one or more of the specified attribute-instances.Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
    return false;
  }
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
    return false;
	}
};

// attributeOwnershipAcquisition
bool RtiAmb_Hla13::attributeOwnershipAcquisition(HlaObject ^object, List<HlaAttribute^>^ _set, String ^tag) {
  try {
    // Create Attribute Handle Set 
    RTI::AttributeHandleSet* ahs = RTI::AttributeHandleSetFactory::create(_set->Count);
    // Add Attribute Handles to set
    for each (HlaAttribute^ attr in _set)
      ahs->add(attr->Handle);

    (void)rti->attributeOwnershipAcquisition(object->Handle, *ahs, (char*)Marshal::StringToHGlobalAnsi(object->Tag).ToPointer());

    String^ list = String::Join(", ", _set);// attributes to string
    String^ msg = "Ownership of attributes of object (handle: " + object->Handle + ") are requested. the attributes: " + list;
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return true;
  }
  catch (RTI::ObjectNotKnown&e) {
    String^ msg = "MSG-(ObjectNotKnown - attributeOwnershipAcquisition):" + Environment::NewLine + " Reason: " + gcnew String(e._reason) + Environment::NewLine;
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return false;
  }
  catch (RTI::ObjectClassNotPublished &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). The operation attempted requires that the object class be currently published by the federate.Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
    return false;
  }
  catch (RTI::AttributeNotDefined &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
  }
  catch (RTI::AttributeNotPublished &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). One or more of the specified attributes are not currently published by the local federate.Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
    return false;
  }
  catch (RTI::FederateOwnsAttributes &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). One or more attributes of the specified object are owned by the local federate.Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
    return false;
  }
  catch (RTI::FederateNotExecutionMember &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
  }
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
};

// cancelAttributeOwnershipAcquisition
bool RtiAmb_Hla13::cancelAttributeOwnershipAcquisition(HlaObject ^object, List<HlaAttribute^>^_set) {
  try {
    // Create Attribute Handle Set 
    RTI::AttributeHandleSet* ahs = RTI::AttributeHandleSetFactory::create(_set->Count);
    // Add Attribute Handles to set
    for each (HlaAttribute^ attr in _set)
      ahs->add(attr->Handle);
    (void)rti->cancelAttributeOwnershipAcquisition(object->Handle, *ahs);
    String^ msg = "Federate requested the cancellation of a previously requested ownership acquisition.";
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return true;
  }
  catch (RTI::ObjectNotKnown&e) {
    String^ msg = "MSG-(ObjectNotKnown - cancelAttributeOwnershipAcquisition):" + Environment::NewLine + " Reason: " + gcnew String(e._reason) + Environment::NewLine;
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return false;
  }
  catch (RTI::AttributeNotDefined &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
  }
  catch (RTI::AttributeAlreadyOwned &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). One or more of the instance-attributes is already owned by the local federate.Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
    return false;
  }
  catch (RTI::AttributeAcquisitionWasNotRequested &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). One or more of the instance-attributes is not the subject of a currently outstanding attributeOwnershipAcquisition() request.Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
    return false;
  }
  catch (RTI::FederateNotExecutionMember &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
  }
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
	}
};

// queryAttributeOwnership
bool RtiAmb_Hla13::queryAttributeOwnership(HlaObject ^object, HlaAttribute^ _attribute) {
  try {
    (void)rti->queryAttributeOwnership(object->Handle, _attribute->Handle);
    String^ msg = "Ownership for attribute (handle: " + _attribute->Handle + ") is queried.";
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return true;
  }
  catch (RTI::ObjectNotKnown&e) {
    String^ msg = "MSG-(ObjectNotKnown - queryAttributeOwnership):" + Environment::NewLine + " Reason: " + gcnew String(e._reason) + Environment::NewLine;
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return false;
  }
  catch (RTI::AttributeNotDefined &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
  }
  catch (RTI::FederateNotExecutionMember &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
  }
  catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
  }
};

// attributeOwnershipReleaseResponse
bool RtiAmb_Hla13::attributeOwnershipReleaseResponse(HlaObject ^object, List<HlaAttribute^>^ _set) {
  try {
    // Create Attribute Handle Set 
    RTI::AttributeHandleSet* ahs = RTI::AttributeHandleSetFactory::create(_set->Count);
    // Add Attribute Handles to set
    for each (HlaAttribute^ attr in _set)
      ahs->add(attr->Handle);
    (void)rti->attributeOwnershipReleaseResponse(object->Handle, *ahs);
    String^ msg = "Ownership for attributes of object (handle: " + object->Handle + ") is/are released.";
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return true;
  }
  catch (RTI::ObjectNotKnown&e) {
    String^ msg = "MSG-(ObjectNotKnown - attributeOwnershipReleaseResponse):" + Environment::NewLine + " Reason: " + gcnew String(e._reason) + Environment::NewLine;
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return false;
  }
  catch (RTI::AttributeNotDefined &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
  }
  catch (RTI::FederateWasNotAskedToReleaseAttribute &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). One or more of the attribute-instances are not the subject of a currently outstanding requestAttributeOwnershipRelease() notification.Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
    return false;
  }
  catch (RTI::FederateNotExecutionMember &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
  }
  catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
  }
};

// cancelNegotiatedAttributeOwnershipDivestiture
bool RtiAmb_Hla13::cancelNegotiatedAttributeOwnershipDivestiture(HlaObject ^object, List<HlaAttribute^>^ _set) {
  try {
    // Create Attribute Handle Set 
    RTI::AttributeHandleSet* ahs = RTI::AttributeHandleSetFactory::create(_set->Count);
    // Add Attribute Handles to set
    for each (HlaAttribute^ attr in _set)
      ahs->add(attr->Handle);
    (void)rti->cancelNegotiatedAttributeOwnershipDivestiture(object->Handle, *ahs);
    String^ msg = "Negotiated Attribute Ownership Divestiture for object (handle: " + object->Handle + ") is cancelled.";
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return true;
  }
  catch (RTI::ObjectNotKnown&e) {
    String^ msg = "MSG-(ObjectNotKnown - cancelNegotiatedAttributeOwnershipDivestiture):" + Environment::NewLine + " Reason: " + gcnew String(e._reason) + Environment::NewLine;
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return false;
  }
  catch (RTI::AttributeNotDefined &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
  }
  catch (RTI::AttributeDivestitureWasNotRequested &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). One or more of the attribute-instances are not the subject of a currently outstanding negotiatedAttributeOwnership-Divestiture() notification.Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
    return false;
  }
  catch (RTI::FederateNotExecutionMember &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
  }
  catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
  }
};

// isAttributeOwnedByFederate
bool RtiAmb_Hla13::isAttributeOwnedByFederate(HlaObject ^object, HlaAttribute^ attribute) {
  try {
    bool result = rti->isAttributeOwnedByFederate(object->Handle, attribute->Handle);
    String^ msg = "The ownership of the attribute (handle: " + attribute->Handle + ") of object (handle: " + object->Handle + ") is owned by this federate is queried. The result is " + result;
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return result;
  }
  catch (RTI::ObjectNotKnown&e) {
    String^ msg = "MSG-(ObjectNotKnown - isAttributeOwnedByFederate):" + Environment::NewLine + " Reason: " + gcnew String(e._reason) + Environment::NewLine;
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return false;
  }
  catch (RTI::AttributeNotDefined &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
  }
  catch (RTI::FederateNotExecutionMember &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
  }
  catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
  }
};

// negotiatedAttributeOwnershipDivestiture
bool RtiAmb_Hla13::negotiatedAttributeOwnershipDivestiture(HlaObject ^object, List<HlaAttribute^>^ _set, String^ tag) {
  try {
    // Create Attribute Handle Set 
    RTI::AttributeHandleSet* ahs = RTI::AttributeHandleSetFactory::create(_set->Count);
    // Add Attribute Handles to set
    for each (HlaAttribute^ attr in _set)
      ahs->add(attr->Handle);
    (void)rti->negotiatedAttributeOwnershipDivestiture(object->Handle, *ahs, (char*)Marshal::StringToHGlobalAnsi(object->Tag).ToPointer());
    String^ msg = "The release for the negotiated Attribute Ownership Divestiture of object (handle: " + object->Handle + ") is attempted.";
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return true;
  }
  catch (RTI::ObjectNotKnown&e) {
    String^ msg = "MSG-(ObjectNotKnown - negotiatedAttributeOwnershipDivestiture):" + Environment::NewLine + " Reason: " + gcnew String(e._reason) + Environment::NewLine;
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return false;
  }
  catch (RTI::AttributeNotDefined &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
  }
  catch (RTI::AttributeNotOwned &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). One or more of the specified attribute-instances is not owned by the local federate.Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
    return false;
  }
  catch (RTI::AttributeAlreadyBeingDivested &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). A negotiated divestiture is already in progress for one or more of the attribute-instances.Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
    return false;
  }
  catch (RTI::FederateNotExecutionMember &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
  }
  catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
  }
};

// unconditionalAttributeOwnershipDivestiture
bool RtiAmb_Hla13::unconditionalAttributeOwnershipDivestiture(HlaObject ^object, List<HlaAttribute^>^ _set) {
  try {
    // Create Attribute Handle Set 
    RTI::AttributeHandleSet* ahs = RTI::AttributeHandleSetFactory::create(_set->Count);
    // Add Attribute Handles to set
    for each (HlaAttribute^ attr in _set)
      ahs->add(attr->Handle);
    (void)rti->unconditionalAttributeOwnershipDivestiture(object->Handle, *ahs);
    String^ msg = "The ownership(s) of attribute(s) of object (handle: " + object->Handle + ") is/are released unconditionally.";
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return true;
  }
  catch (RTI::ObjectNotKnown&e) {
    String^ msg = "MSG-(ObjectNotKnown - unconditionalAttributeOwnershipDivestiture):" + Environment::NewLine + " Reason: " + gcnew String(e._reason) + Environment::NewLine;
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return false;
  }
  catch (RTI::AttributeNotDefined &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
  }
  catch (RTI::AttributeNotOwned &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). One or more of the specified attribute-instances is not owned by the local federate.Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
    return false;
  }
  catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
  }
};

#pragma endregion	// Ownership Management

#pragma region Data Distribution Management

// associateRegionForUpdates
bool RtiAmb_Hla13::associateRegionForUpdates(HlaRegion ^region, HlaObject ^object, List<HlaAttribute^> ^attributes) {
  try {
    RTI::AttributeHandleSet* ahs = RTI::AttributeHandleSetFactory::create(attributes->Count);
    for each (HlaAttribute^ attr in attributes)
      ahs->add(attr->Handle);
    // Check that ahs is not null
    if (ahs) {
      rti->associateRegionForUpdates(*nom->Regions[(char*)Marshal::StringToHGlobalAnsi(region->Name).ToPointer()], object->Handle, *ahs);
      String^ tmpStr = "";
      // Report attributes to the user
      for (int j = 0; j < attributes->Count; j++) {
        tmpStr += attributes[j]->Name;
        if (j != attributes->Count - 1) tmpStr += ", ";
      };
      String^ msg = "Attributes {" + tmpStr + "} of object (handle: " + object->Handle + ") are associated with Region (" + region->Name + ").";
      this->OnHLAClassSubscribed(gcnew RaconEventArgs(msg));
      // Release Mem
      ahs->empty(); delete ahs;
      return true;
    }
  }
#pragma region exceptions
  catch (RTI::ObjectNotKnown &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Please ensure that you try to associate with a registered object. Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
  }
  catch (RTI::AttributeNotDefined &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RegionNotKnown &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::InvalidRegionContext &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::ConcurrentAccessAttempted& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

// Create region
bool RtiAmb_Hla13::createRegion(String^ region, unsigned long noOfExtents, long spaceHandle) {
  try {
    char* _name = (char*)Marshal::StringToHGlobalAnsi(region).ToPointer();
    //Create region
    RTI::Region* _region = rti->createRegion(spaceHandle, noOfExtents);
    //region->Handle = rti->createRegion(spaceHandle, noOfExtents);

    // Update Nom
    if (nom->Regions.find(_name) == nom->Regions.end()) // if not found
      nom->Regions[_name] = _region;

    String^ msg = "Region (" + region + ") is created.";
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return true;
  }
#pragma region exceptions
  catch (RTI::SpaceNotDefined &e) {
		String^ msg = "RtiAmb_Hla13-(SpaceNotDefined - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::InvalidExtents &e) {
		String^ msg = "RtiAmb_Hla13-(InvalidExtents - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla13-(FederateNotExecutionMember - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::ConcurrentAccessAttempted& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
  }
  catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
  }
#pragma endregion

};

// Delete region
void RtiAmb_Hla13::deleteRegion(HlaRegion^ region) {
  try {
    char* _name = (char*)Marshal::StringToHGlobalAnsi(region->Name).ToPointer();

    rti->deleteRegion(nom->Regions[_name]);
    String^ msg = "Region (" + region->Name + ") is deleted.";
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
  }
#pragma region exceptions
  catch (RTI::RegionNotKnown &e) {
		String^ msg = "RtiAmb_Hla13-(RegionNotKnown - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RegionInUse &e) {
		String^ msg = "RtiAmb_Hla13-(RegionInUse - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla13-(FederateNotExecutionMember - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::ConcurrentAccessAttempted& e) {
		String^ msg = "RtiAmb_Hla13-(ConcurrentAccessAttempted - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(SaveInProgress - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(RestoreInProgress - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla13-(RTIinternalError - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
#pragma endregion
};

// Register Object Instance With Region - selected (publishable) attributes with specific regions
void RtiAmb_Hla13::registerObjectInstanceWithRegions(HlaObject ^obj, List<HlaAttribute^> ^attributes, List<HlaRegion^> ^regions) {
  try {
    RTI::ObjectClassHandle theClass = obj->Type->Handle;
    RTI::Region **theRegions = (RTI::Region**)malloc(regions->Count * sizeof(RTI::Region *));
    RTI::Handle* theAttributes = new RTI::Handle[attributes->Count];
    int count = 0;
    String^ pairs = "";
    for each (HlaAttribute^ attr in attributes) {
      theRegions[count] = nom->Regions[(char*)Marshal::StringToHGlobalAnsi(regions[count]->Name).ToPointer()];
      //theRegions[count] = regions[count]->Handle;
      theAttributes[count] = attr->Handle;
      pairs += "<" + attr->Name + "," + regions[count]->Name + ">, ";
      count++;
    }
    pairs += ". ";
    String^ name = "Object Handle = " + obj->Handle + ". ";
    if (obj->Name == "")
    {
      obj->Handle = rti->registerObjectInstanceWithRegion(theClass, theAttributes, theRegions, count);
    }
    else
    {
      const char *theObject = (char*)Marshal::StringToHGlobalAnsi(obj->Name).ToPointer();
      obj->Handle = rti->registerObjectInstanceWithRegion(theClass, theObject, theAttributes, theRegions, count);
      name += "Object Name = " + obj->Name + ". ";
    }

    String^ msg = "Object (class name: " + obj->Type->Name + ") is registered with regions." + " Attribute-Region pairs: " + pairs + name;
    this->OnObjectRegistered(gcnew RaconEventArgs(msg));
    free(theRegions);
  }
#pragma region exceptions
  catch (RTI::ObjectClassNotDefined& e) {
		String^ msg = "RtiAmb_Hla13-(ObjectClassNotDefined - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::ObjectClassNotPublished &e) {
		String^ msg = "RtiAmb_Hla13-(ObjectClassNotPublished - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::AttributeNotDefined &e) {
		String^ msg = "RtiAmb_Hla13-(AttributeNotDefined - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::AttributeNotPublished &e) {
		String^ msg = "RtiAmb_Hla13-(AttributeNotPublished - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RegionNotKnown &e) {
		String^ msg = "RtiAmb_Hla13-(RegionNotKnown - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::InvalidRegionContext& e) {
		String^ msg = "RtiAmb_Hla13-(InvalidRegionContext - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::ObjectAlreadyRegistered& e) {
		String^ msg = "RtiAmb_Hla13-(ObjectAlreadyRegistered - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla13-(FederateNotExecutionMember - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::ConcurrentAccessAttempted& e) {
		String^ msg = "RtiAmb_Hla13-(ConcurrentAccessAttempted - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(SaveInProgress - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(RestoreInProgress - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla13-(RTIinternalError - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
  }
#pragma endregion
};

//// Register Object Instance With Region - all publishable attributes - WITH OBJECT NAME
//bool RtiAmb_Hla13::registerObjectInstanceWithRegion(HlaObject ^obj, CHlaRegion ^reg, String^ name) {
//	try{
//		const char *theObject = (char*)Marshal::StringToHGlobalAnsi(name).ToPointer();
//		RTI::ObjectClassHandle theClass = obj->Type->ClassHandle;
//		RTI::Region **theRegions = (RTI::Region**)malloc(obj->Type->Attributes->Count * sizeof(RTI::Region *));
//		RTI::Handle* theAttributes = new RTI::Handle[obj->Type->Attributes->Count];
//		int count = 0;
//		String^ tmpStr = "";
//		for each (HlaAttribute^ attr in obj->Type->Attributes){
//			if ((attr->AttributePS == PSKind::Publish) || (attr->AttributePS == PSKind::PublishSubscribe)){
//				theRegions[count] = reg->Handle;
//				theAttributes[count] = attr->Handle;
//				tmpStr += attr->Name + ", ";
//				count++;
//			}
//		}
//		obj->Handle = rti->registerObjectInstanceWithRegion(theClass, theObject, theAttributes, theRegions, count);
//		String^ msg = "Object (class name: " + obj->Type->ClassName + ") is registered with region: " + reg->Name + "of all selected attributes. Object handle: " + obj->Handle + ". Object name: " + name;
//		this->OnObjectRegistered(gcnew RaconEventArgs(msg));
//		return true;
//	}
//#pragma region exceptions
//	catch (RTI::ObjectClassNotDefined& e){
//		MessageBox::Show("MSG-(ObjectClassNotDefined - registerObjectInstanceWithRegion2):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
//	}
//	catch (RTI::ObjectClassNotPublished &e){
//		MessageBox::Show("MSG-(ObjectClassNotPublished - registerObjectInstanceWithRegion2):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
//	}
//	catch (RTI::AttributeNotDefined &e){
//		MessageBox::Show("MSG-(AttributeNotDefined - registerObjectInstanceWithRegion2):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
//	}
//	catch (RTI::AttributeNotPublished &e){
//		MessageBox::Show("MSG-(AttributeNotPublished - registerObjectInstanceWithRegion2):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
//	}
//	catch (RTI::RegionNotKnown &e){
//		MessageBox::Show("MSG-(RegionNotKnown - registerObjectInstanceWithRegion2):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
//	}
//	catch (RTI::InvalidRegionContext& e){
//		MessageBox::Show("MSG-(InvalidRegionContext - registerObjectInstanceWithRegion2):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
//	}
//	catch (RTI::ObjectAlreadyRegistered& e){
//		MessageBox::Show("MSG-(ObjectAlreadyRegistered - registerObjectInstanceWithRegion2):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
//	}
//	catch (RTI::FederateNotExecutionMember& e){
//		MessageBox::Show("MSG-(FederateNotExecutionMember - registerObjectInstanceWithRegion2):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
//	}
//	catch (RTI::ConcurrentAccessAttempted& e){
//		MessageBox::Show("MSG-(ConcurrentAccessAttempted - registerObjectInstanceWithRegion2):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
//	}
//	catch (RTI::SaveInProgress& e){
//		MessageBox::Show("MSG-(SaveInProgress - registerObjectInstanceWithRegion2):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
//	}
//	catch (RTI::RestoreInProgress& e){
//		MessageBox::Show("MSG-(RestoreInProgress - registerObjectInstanceWithRegion2):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
//	}
//	catch (RTI::RTIinternalError& e){
//		MessageBox::Show("MSG-(RTIinternalError - registerObjectInstanceWithRegion2):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
//	}
//	catch (Exception^ e){
//		MessageBox::Show("MSG-(GeneralException - registerObjectInstanceWithRegion2):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
//		return false;
//	}
//#pragma endregion
//};
//// Registering object with a specific region with some of its attributes
//bool RtiAmb_Hla13::registerObjectInstanceWithRegion(HlaObject ^obj, List<HlaAttribute^>^ attributes, CHlaRegion ^reg) {
//	try{
//		RTI::ObjectClassHandle theClass = obj->Type->ClassHandle;
//		RTI::Region **theRegions = (RTI::Region**)malloc(attributes->Count * sizeof(RTI::Region *));
//		RTI::Handle* theAttributes = new RTI::Handle[attributes->Count];
//		int count = 0;
//		String^ tmpStr = "";
//		for each (HlaAttribute^ attr in attributes){
//			if ((attr->AttributePS == PSKind::Publish) || (attr->AttributePS == PSKind::PublishSubscribe)){
//				theRegions[count] = reg->Handle;
//				theAttributes[count] = attr->Handle;
//				tmpStr += attr->Name + "(" + attr->Handle + "), ";
//				count++;
//			}
//		}
//		obj->Handle = rti->registerObjectInstanceWithRegion(theClass, theAttributes, theRegions, count);
//		String^ msg = "Object (class name: " + obj->Type->ClassName + ") is registered with region: " + reg->Name + " of selected publishable attributes (" + tmpStr + "). Object handle: " + obj->Handle;
//		this->OnObjectRegistered(gcnew RaconEventArgs(msg));
//		return true;
//	}
//#pragma region exceptions
//	catch (RTI::ObjectClassNotDefined& e){
//		MessageBox::Show("MSG-(ObjectClassNotDefined - registerObjectInstanceWithRegion3):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
//		return false;
//	}
//	catch (RTI::ObjectClassNotPublished &e){
//		MessageBox::Show("MSG-(ObjectClassNotPublished - registerObjectInstanceWithRegion3):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
//		return false;
//	}
//	catch (RTI::AttributeNotDefined &e){
//		MessageBox::Show("MSG-(AttributeNotDefined - registerObjectInstanceWithRegion3):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
//		return false;
//	}
//	catch (RTI::AttributeNotPublished &e){
//		MessageBox::Show("MSG-(AttributeNotPublished - registerObjectInstanceWithRegion3):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
//		return false;
//	}
//	catch (RTI::RegionNotKnown &e){
//		MessageBox::Show("MSG-(RegionNotKnown - registerObjectInstanceWithRegion3):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
//		return false;
//	}
//	catch (RTI::InvalidRegionContext& e){
//		MessageBox::Show("MSG-(InvalidRegionContext - registerObjectInstanceWithRegion3):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
//		return false;
//	}
//	catch (RTI::ObjectAlreadyRegistered& e){
//		MessageBox::Show("MSG-(ObjectAlreadyRegistered - registerObjectInstanceWithRegion3):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
//		return false;
//	}
//	catch (RTI::FederateNotExecutionMember& e){
//		MessageBox::Show("MSG-(FederateNotExecutionMember - registerObjectInstanceWithRegion3):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
//		return false;
//	}
//	catch (RTI::ConcurrentAccessAttempted& e){
//		MessageBox::Show("MSG-(ConcurrentAccessAttempted - registerObjectInstanceWithRegion3):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
//		return false;
//	}
//	catch (RTI::SaveInProgress& e){
//		MessageBox::Show("MSG-(SaveInProgress - registerObjectInstanceWithRegion3):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
//		return false;
//	}
//	catch (RTI::RestoreInProgress& e){
//		MessageBox::Show("MSG-(RestoreInProgress - registerObjectInstanceWithRegion3):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
//		return false;
//	}
//	catch (RTI::RTIinternalError& e){
//		MessageBox::Show("MSG-(RTIinternalError - registerObjectInstanceWithRegion3):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
//		return false;
//	}
//	catch (Exception^ e){
//		MessageBox::Show("MSG-(GeneralException - registerObjectInstanceWithRegion3):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
//		return false;
//	}
//#pragma endregion
//};
//
//// Registering object with a specific region with some of its publishable attributes - WITH OBJECT NAME
//bool RtiAmb_Hla13::registerObjectInstanceWithRegion(HlaObject ^obj, List<HlaAttribute^>^ attributes, CHlaRegion ^reg, String^ name) {
//	try{
//		const char *theObject = (char*)Marshal::StringToHGlobalAnsi(name).ToPointer();
//		RTI::ObjectClassHandle theClass = obj->Type->ClassHandle;
//		RTI::Region **theRegions = (RTI::Region**)malloc(attributes->Count * sizeof(RTI::Region *));
//		RTI::Handle* theAttributes = new RTI::Handle[attributes->Count];
//		int count = 0;
//		String^ tmpStr = "";
//		for each (HlaAttribute^ attr in attributes){
//			if ((attr->AttributePS == PSKind::Publish) || (attr->AttributePS == PSKind::PublishSubscribe)){
//				theRegions[count] = reg->Handle;
//				theAttributes[count] = attr->Handle;
//				tmpStr += attr->Name + "(" + attr->Handle + "), ";
//				count++;
//			}
//		}
//		obj->Handle = rti->registerObjectInstanceWithRegion(theClass, theObject, theAttributes, theRegions, count);
//		String^ msg = "Object (class name: " + obj->Type->ClassName + ") is registered with region: " + reg->Name + "of all selected attributes. Object handle: " + obj->Handle + ". Object name: " + name;
//		this->OnObjectRegistered(gcnew RaconEventArgs(msg));
//		return true;
//	}
//#pragma region exceptions
//	catch (RTI::ObjectClassNotDefined& e){
//		MessageBox::Show("MSG-(ObjectClassNotDefined - registerObjectInstanceWithRegion4):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
//	}
//	catch (RTI::ObjectClassNotPublished &e){
//		MessageBox::Show("MSG-(ObjectClassNotPublished - registerObjectInstanceWithRegion4):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
//	}
//	catch (RTI::AttributeNotDefined &e){
//		MessageBox::Show("MSG-(AttributeNotDefined - registerObjectInstanceWithRegion4):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
//	}
//	catch (RTI::AttributeNotPublished &e){
//		MessageBox::Show("MSG-(AttributeNotPublished - registerObjectInstanceWithRegion4):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
//	}
//	catch (RTI::RegionNotKnown &e){
//		MessageBox::Show("MSG-(RegionNotKnown - registerObjectInstanceWithRegion4):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
//	}
//	catch (RTI::InvalidRegionContext& e){
//		MessageBox::Show("MSG-(InvalidRegionContext - registerObjectInstanceWithRegion4):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
//	}
//	catch (RTI::ObjectAlreadyRegistered& e){
//		MessageBox::Show("MSG-(ObjectAlreadyRegistered - registerObjectInstanceWithRegion4):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
//	}
//	catch (RTI::FederateNotExecutionMember& e){
//		MessageBox::Show("MSG-(FederateNotExecutionMember - registerObjectInstanceWithRegion4):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
//	}
//	catch (RTI::ConcurrentAccessAttempted& e){
//		MessageBox::Show("MSG-(ConcurrentAccessAttempted - registerObjectInstanceWithRegion4):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
//	}
//	catch (RTI::SaveInProgress& e){
//		MessageBox::Show("MSG-(SaveInProgress - registerObjectInstanceWithRegion4):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
//	}
//	catch (RTI::RestoreInProgress& e){
//		MessageBox::Show("MSG-(RestoreInProgress - registerObjectInstanceWithRegion4):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
//	}
//	catch (RTI::RTIinternalError& e){
//		MessageBox::Show("MSG-(RTIinternalError - registerObjectInstanceWithRegion):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
//	}
//	catch (Exception^ e){
//		MessageBox::Show("MSG-(GeneralException - registerObjectInstanceWithRegion4):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
//		return false;
//	}
//#pragma endregion
//};

// subscribeOC with Region
bool RtiAmb_Hla13::subscribeObjectClassAttributesWithRegions(HlaObjectClass ^oc, List<HlaAttribute^>^ attributes, HlaRegion ^region, Boolean active) {
  this->getClassHandleFromRti(oc);
  // Check that OC is Subscribable
  if ((oc->ClassPS == PSKind::Subscribe) || (oc->ClassPS == PSKind::PublishSubscribe))
    try {
    // Construct tmp list - according to the PS stat of the class
    List<HlaAttribute^> ^tmpList = gcnew List<HlaAttribute^>();
    for each (HlaAttribute^ attr in attributes) {
      attr->Handle = this->getAttributeHandle(attr, oc);
      if ((attr->AttributePS == PSKind::Subscribe) || (attr->AttributePS == PSKind::PublishSubscribe)) {
        tmpList->Add(attr);
      }
    }
    RTI::AttributeHandleSet* ahs = RTI::AttributeHandleSetFactory::create(tmpList->Count);
    // Add attribute handles to AHS
    for (int j = 0; j < tmpList->Count; j++) {
      ahs->add(tmpList[j]->Handle);
    }
    // Check that ahs is not null
    if (ahs) {
      rti->subscribeObjectClassAttributesWithRegion(oc->Handle, *nom->Regions[(char*)Marshal::StringToHGlobalAnsi(region->Name).ToPointer()], *ahs, (RTI::Boolean)active);
      //rti->subscribeObjectClassAttributesWithRegion(oc->Handle, *(reg->Handle), *ahs, (RTI::Boolean)active);
      String^ tmpStr = "";
      // Report attributes to the user
      for (int j = 0; j < tmpList->Count; j++) {
        tmpStr += tmpList[j]->Name;
        if (j != tmpList->Count - 1) tmpStr += ", ";
      };
      String^ msg = "Object Class (name: " + oc->Name + ") is subscribed with Region: " + region->Name + ". Class handle: " + oc->Handle + ", Attributes: {" + tmpStr + "}";
      this->OnHLAClassSubscribed(gcnew RaconEventArgs(msg));
      // Release Mem
      ahs->empty(); delete ahs;
      return true;
    }
  }
#pragma region exceptions
  catch (RTI::ObjectClassNotDefined& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::AttributeNotDefined &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RegionNotKnown &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::InvalidRegionContext& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Check your FED file that the attribute you try to subscribe is associated with the related routing space.Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
  }
  catch (RTI::FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::ConcurrentAccessAttempted& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

// requestClassAttributeValueUpdateWithRegion
bool RtiAmb_Hla13::requestClassAttributeValueUpdateWithRegion(HlaObjectClass ^oc, List<HlaAttribute^> ^attributes, HlaRegion ^region) {
  try {
    RTI::AttributeHandleSet* ahs = RTI::AttributeHandleSetFactory::create(attributes->Count);
    // Add attribute handles to AHS
    for (int j = 0; j < attributes->Count; j++)
      ahs->add(attributes[j]->Handle);
    // Check that ahs is not null
    if (ahs) {
      rti->requestClassAttributeValueUpdateWithRegion(oc->Handle, *ahs, *nom->Regions[(char*)Marshal::StringToHGlobalAnsi(region->Name).ToPointer()]);
      String^ tmpStr = "";
      // Report attributes to the user
      for (int j = 0; j < attributes->Count; j++) {
        tmpStr += attributes[j]->Name;
        if (j != attributes->Count - 1) tmpStr += ", ";
      };
      String^ msg = "The attribute value update for attributes {" + tmpStr + "} of class (name: " + oc->Name + ") is requested for region (" + region->Name + ").";
      this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
      // Release Mem
      ahs->empty(); delete ahs;
      return true;
    }
  }
#pragma region exceptions
  catch (RTI::ObjectClassNotDefined &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
    return false;
	}
  catch (RTI::AttributeNotDefined &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
    return false;
  }
  catch (RTI::RegionNotKnown &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
    return false;
  }
  catch (RTI::InvalidRegionContext &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
    return false;
  }
  catch (RTI::FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
    return false;
  }
  catch (RTI::ConcurrentAccessAttempted& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
    return false;
  }
  catch (RTI::SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
    return false;
  }
  catch (RTI::RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
    return false;
  }
  catch (RTI::RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
    return false;
  }
  catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
    return false;
  }
#pragma endregion
};

// Send Interaction With Region
bool RtiAmb_Hla13::sendInteractionWithRegion(HlaInteraction ^interaction, HlaRegion ^region) {
  try {
    String ^msg = "";
    if (region)
    {
      // Pack parameter values
      RTI::ParameterHandleValuePairSet *phvps = RTI::ParameterSetFactory::create(interaction->Parameters->Count);
      for each (HlaParameter^ var in interaction->Parameters)
      {
        char* _pVal = (char*)var->Value.ToPointer();
        phvps->add(var->Handle, (char*)_pVal, var->Length);
        Marshal::FreeHGlobal(IntPtr(_pVal));
      }
      rti->sendInteractionWithRegion(interaction->ClassHandle, *phvps, (char*)(Marshal::StringToHGlobalAnsi(interaction->Tag)).ToPointer(), *nom->Regions[(char*)Marshal::StringToHGlobalAnsi(region->Name).ToPointer()]);
      msg = "Interaction (handle: " + interaction->ClassHandle + ") is sent via region (" + region->Name + ").";
    }
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return true;
  }
  catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
  };
};

// subscribe IC with Region
bool RtiAmb_Hla13::subscribeInteractionClass(HlaInteractionClass ^ic, HlaRegion ^region) {

  String^ msg = "";
  // Check that interaction is Subscribable
  if ((ic->ClassPS == PSKind::Subscribe) || (ic->ClassPS == PSKind::PublishSubscribe))
    try {
    // Get Class Handle
    getClassHandleFromRti(ic);
    // Get All Attribute Handles
    getParameterHandlesFromRti(ic);
    // Subscribe according to the region
    if (region) {
      //MessageBox::Show("MSG-(debug): Region:" + region->Name + " Class handle: " + ic->ClassHandle + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
      rti->subscribeInteractionClassWithRegion(ic->Handle, *nom->Regions[(char*)Marshal::StringToHGlobalAnsi(region->Name).ToPointer()]);
      msg = "Interaction Class (name: " + ic->Name + ", handle: " + ic->Handle + ") is subscribed with region.";
      this->OnHLAClassSubscribed(gcnew RaconEventArgs(msg));
      return true;
    }
  }
#pragma region exceptions
  catch (RTI::InteractionClassNotDefined& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RegionNotKnown& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::InvalidRegionContext& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). One or more of the specified attribute handles is not valid within the context of the specified region.Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
  }
  catch (RTI::FederateLoggingServiceCalls& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::ConcurrentAccessAttempted& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};


// associateRegionForUpdates
bool RtiAmb_Hla13::unassociateRegionForUpdates(HlaRegion ^region, HlaObject ^object) {
  try {
    rti->unassociateRegionForUpdates(*nom->Regions[(char*)Marshal::StringToHGlobalAnsi(region->Name).ToPointer()], object->Handle);
    String^ msg = "The association between object (handle: " + object->Handle + ") and region (" + region->Name + ") is removed.";
    this->OnHLAClassSubscribed(gcnew RaconEventArgs(msg));
    return true;
  }
#pragma region exceptions
  catch (RTI::ObjectNotKnown &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RegionNotKnown &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::InvalidRegionContext &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::ConcurrentAccessAttempted& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

// unsubscribe IC with Region
void RtiAmb_Hla13::unsubscribeInteractionClass(HlaInteractionClass ^ic, HlaRegion ^region) {
  try {
    if (region) rti->unsubscribeInteractionClassWithRegion(ic->Handle, *nom->Regions[(char*)Marshal::StringToHGlobalAnsi(region->Name).ToPointer()]);
    String^ msg = "Interaction Class (name: " + ic->Name + ", handle:" + ic->Handle + ") is unsubscribed from region (" + region->Name + ").";
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
  }

  catch (RTI::InteractionClassNotDefined& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::InteractionClassNotSubscribed& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RegionNotKnown& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::ConcurrentAccessAttempted& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
};

// unsubscribe OC with Region
bool RtiAmb_Hla13::unsubscribeObjectClassWithRegions(HlaObjectClass ^oc, HlaRegion ^region) {
  try {
    rti->unsubscribeObjectClassWithRegion(oc->Handle, *nom->Regions[(char*)Marshal::StringToHGlobalAnsi(region->Name).ToPointer()]);
    //if (region) rti->unsubscribeObjectClassWithRegion(oc->Handle, *region->Handle);
    String^ msg = "Object Class (name: " + oc->Name + ", handle:" + oc->Handle + ") is unsubscribed from region (" + region->Name + ").";
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return true;
  }
#pragma region exceptions
  catch (RTI::ObjectClassNotDefined& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RegionNotKnown &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::ObjectClassNotSubscribed &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::ConcurrentAccessAttempted& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

#pragma endregion // Data Distribution Management

#pragma region Time Management

// Enable Async. Delivery
bool RtiAmb_Hla13::enableAsynchronousDelivery() {
  try {
    rti->enableAsynchronousDelivery();
    String^ msg = "Asynchronous delivery is enabled.";
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return true;
  }
  catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
  }
};

// changeAttributeOrderType
bool RtiAmb_Hla13::changeAttributeOrderType(HlaObject ^obj, List<HlaAttribute^>^ attributes, unsigned int type) {
  try {
    RTI::AttributeHandleSet* ahs = RTI::AttributeHandleSetFactory::create(attributes->Count);
    for each (HlaAttribute^ attr in attributes)
      ahs->add(attr->Handle);

    rti->changeAttributeOrderType(obj->Handle, *ahs, type);
    String^ tmpStr = "";
    for (int j = 0; j < attributes->Count; j++) {
      tmpStr += attributes[j]->Name;
      if (j != attributes->Count - 1) tmpStr += ", ";
    };
    String^ msg = "The ordering policy for attributes {" + tmpStr + "} of object (handle: " + obj->Handle + ") is changed to " + (type) ? "(Receive)" : "(TimeStamp)";
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    ahs->empty(); delete ahs;
    return true;
  }
#pragma region exceptions
  catch (RTI::ObjectNotKnown &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::AttributeNotDefined &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::AttributeNotOwned &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::InvalidOrderingHandle &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::ConcurrentAccessAttempted& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
  }
  catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
  }
#pragma endregion
};

// changeInteractionOrderType
bool RtiAmb_Hla13::changeInteractionOrderType(HlaInteractionClass ^ic, unsigned int type) {
  try {
    rti->changeInteractionOrderType(ic->Handle, type);
    String^ msg = "The ordering policy for interaction class (" + ic->Name + ") is changed to " + (type) ? "(Receive)" : "(TimeStamp)";
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return true;
  }
#pragma region exceptions
  catch (RTI::InteractionClassNotDefined &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::InteractionClassNotPublished &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::InvalidOrderingHandle &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::ConcurrentAccessAttempted& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
  }
  catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
  }
#pragma endregion
};

// Disable Async. Delivery
bool RtiAmb_Hla13::disableAsynchronousDelivery() {
  try {
    rti->disableAsynchronousDelivery();
    String^ msg = "Asynchronous delivery is disabled.";
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return true;
  }
  catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
  }
};

// Disable Time Constrained
bool RtiAmb_Hla13::disableTimeConstrained() {
  try {
    rti->disableTimeConstrained();
    String^ msg = "Time Constrained is disabled.";
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return true;
  }
  catch (RTI::TimeConstrainedWasNotEnabled &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::ConcurrentAccessAttempted& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
  }
};

// Disable Time Regulation
bool RtiAmb_Hla13::disableTimeRegulation() {
  try {
    rti->disableTimeRegulation();
    String^ msg = "Time Regulation is disabled.";
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return true;
  }
  catch (RTI::TimeRegulationWasNotEnabled &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::ConcurrentAccessAttempted& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
  }
};

// Enable Time Constrained
bool RtiAmb_Hla13::enableTimeConstrained() {
  try {
    rti->enableTimeConstrained();
    String^ msg = "Time Constrained is enabled.";
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return true;
  }
  catch (RTI::TimeConstrainedAlreadyEnabled &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::EnableTimeConstrainedPending &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::TimeAdvanceAlreadyInProgress &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::ConcurrentAccessAttempted& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
  }
};

// Enable Time Constrained
bool RtiAmb_Hla13::enableTimeRegulation(Double federateTime, Double lookahead) {
  try {
    RTIfedTime time(lookahead);
    RTI::FedTime& theLookahead = time;
    RTIfedTime time2(federateTime);
    RTI::FedTime& theFederateTime = time2;

    //RTI::FedTime* theFederateTime = (RTI::FedTimeFactory::makeZero());
    //RTI::FedTime* theFederateTime = new RTIfedTime(federateTime);
    //RTI::FedTime* theLookahead = new RTIfedTime(lookahead);
    rti->enableTimeRegulation(theFederateTime, theLookahead);
    String^ msg = "Time Regulation is enabled. FederateTime = " + time2.getTime() + " secs. Lookahead = " + time.getTime() + " secs.";
    //char* pstr(0);
    //theFederateTime->getPrintableString(pstr);
    //String^ msg = "Time Regulation is enabled. FederateTime = " + *pstr + " secs. Lookahead = " + time.getTime() + " secs.";
    //delete pstr;
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return true;
  }
  catch (RTI::TimeRegulationAlreadyEnabled &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::EnableTimeRegulationPending &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::TimeAdvanceAlreadyInProgress &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::InvalidFederationTime &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::InvalidLookahead &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::ConcurrentAccessAttempted& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
  }
};

// flushQueueRequest
bool RtiAmb_Hla13::flushQueueRequest(Double time) {
  try {
    RTIfedTime time(time);
    RTI::FedTime& theTime = time;

    rti->flushQueueRequest(theTime);
    String^ msg = "A flush of the federate's event queues is initiated. The maximum logical time which to advance upon completion of the flush is " + time.getTime();
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return true;
  }
  catch (RTI::InvalidFederationTime &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::FederationTimeAlreadyPassed &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::TimeAdvanceAlreadyInProgress &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::EnableTimeConstrainedPending &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::EnableTimeRegulationPending &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::ConcurrentAccessAttempted& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
  }
};

// modifyLookahead
bool RtiAmb_Hla13::modifyLookahead(Double lookahead) {
  try {
    RTIfedTime time(lookahead);
    RTI::FedTime& theTime = time;

    rti->modifyLookahead(theTime);
    String^ msg = "Lookahead is modified as " + time.getTime();
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return true;
  }
  catch (RTI::InvalidLookahead &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::ConcurrentAccessAttempted& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
  }
};

// nextEventRequest
bool RtiAmb_Hla13::nextEventRequest(Double time) {
  try {
    RTIfedTime time(time);
    RTI::FedTime& theTime = time;

    rti->nextEventRequest(theTime);
    String^ msg = "Advance of federate's logical time to the time stamp of the next relevant TSO event is requested. The cut off point at which to stop advancing logical time is " + time.getTime();
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return true;
  }
  catch (RTI::InvalidFederationTime &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::FederationTimeAlreadyPassed &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::TimeAdvanceAlreadyInProgress &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::EnableTimeConstrainedPending &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::EnableTimeRegulationPending &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::ConcurrentAccessAttempted& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
  }
};

// nextEventRequestAvailable
bool RtiAmb_Hla13::nextEventRequestAvailable(Double time) {
  try {
    RTIfedTime time(time);
    RTI::FedTime& theTime = time;

    rti->nextEventRequestAvailable(theTime);
    String^ msg = "Advance of federate's logical time (nextEventRequestAvailable) to the time stamp of the next relevant TSO event is requested. The cut off point at which to stop advancing logical time is " + time.getTime();
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return true;
  }
  catch (RTI::InvalidFederationTime &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::FederationTimeAlreadyPassed &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::TimeAdvanceAlreadyInProgress &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::EnableTimeConstrainedPending &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::EnableTimeRegulationPending &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::ConcurrentAccessAttempted& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
  }
};

// queryFederateTime
Double RtiAmb_Hla13::queryFederateTime() {
  try {
    RTI::FedTime& theTime = *(RTI::FedTimeFactory::makeZero());
    rti->queryFederateTime(theTime); // theTime is OUT parameter set to indicate the federate's current logical time
    RTIfedTime time(theTime);
    String^ msg = "The federate's current logical time is " + time.getTime();
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return time.getTime();
  }
  catch (RTI::FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::ConcurrentAccessAttempted& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
  }
};

// queryLBTS
Double RtiAmb_Hla13::queryLBTS() {
  try {
    RTI::FedTime& theTime = *(RTI::FedTimeFactory::makeZero());
    rti->queryLBTS(theTime); // theTime is OUT parameter set to indicate the federate's current logical time
    RTIfedTime time(theTime);
    String^ msg = "The federation’s current lowerbound time-stamp is " + time.getTime();
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return time.getTime();
  }
  catch (RTI::FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::ConcurrentAccessAttempted& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
  }
};

// queryLookahead
Double RtiAmb_Hla13::queryLookahead() {
  try {
    RTI::FedTime& theTime = *(RTI::FedTimeFactory::makeZero());
    rti->queryLookahead(theTime);
    RTIfedTime time(theTime);
    String^ msg = "The current lookahead interval of the local federate is " + time.getTime();
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return time.getTime();
  }
  catch (RTI::FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::ConcurrentAccessAttempted& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
  }
};

// queryMinNextEventTime
Double RtiAmb_Hla13::queryMinNextEventTime() {
  try {
    RTI::FedTime& theTime = *(RTI::FedTimeFactory::makeZero());
    rti->queryMinNextEventTime(theTime);
    RTIfedTime time(theTime);
    String^ msg = "The minimum time stamp of the next(potential) time - stamp - ordered event that may be subsequently delivered to the federate is " + time.getTime();
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return time.getTime();
  }
  catch (RTI::FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::ConcurrentAccessAttempted& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
  }
  catch (RTI::RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
  }
};

// timeAdvanceRequest
bool RtiAmb_Hla13::timeAdvanceRequest(Double time) {
  try {
    RTIfedTime time(time);
    RTI::FedTime& theTime = time;

    rti->timeAdvanceRequest(theTime);
    String^ msg = "Time advance is requested for Time = " + time.getTime();
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return true;
  }
  catch (RTI::InvalidFederationTime &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::FederationTimeAlreadyPassed &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::TimeAdvanceAlreadyInProgress &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::EnableTimeConstrainedPending &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::EnableTimeRegulationPending &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::ConcurrentAccessAttempted& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
  }
};

// timeAdvanceRequestAvailable
bool RtiAmb_Hla13::timeAdvanceRequestAvailable(Double time) {
  try {
    RTIfedTime time(time);
    RTI::FedTime& theTime = time;

    rti->timeAdvanceRequestAvailable(theTime);
    String^ msg = "Time advance (available) is requested for Time = " + time.getTime();
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return true;
  }
  catch (RTI::InvalidFederationTime &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::FederationTimeAlreadyPassed &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::TimeAdvanceAlreadyInProgress &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::EnableTimeConstrainedPending &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::EnableTimeRegulationPending &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::ConcurrentAccessAttempted& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
  }
};

// Retract
bool RtiAmb_Hla13::retract(MessageRetraction ^retraction) {
  try {
    //rti->retract(*(retraction->Handle));
    RTI::EventRetractionHandle Handle;
    Handle.sendingFederate = retraction->SendingFederate;
    Handle.theSerialNumber = retraction->SerialNumber;
    rti->retract(Handle);
    String^ msg = "Unscheduleding is done for event; Rectraction Serial No = " + Handle.theSerialNumber + " and Sending Federate = " + Handle.sendingFederate;
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return true;
  }
#pragma region exceptions
  catch (RTI::InvalidRetractionHandle &e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). the event-retraction handle must be obtained from updateAttributeValues(), sendInteraction(), deleteObject()) of the event to unscheduleReason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
    return false;
  }
  catch (RTI::FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
  }
  catch (RTI::ConcurrentAccessAttempted& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
  }
  catch (RTI::SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
  }
  catch (RTI::RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
  }
  catch (RTI::RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
		return false;
  }
  catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
  }
#pragma endregion
};

#pragma endregion	// Time Management		

#pragma region Support Services
// enableAttributeRelevanceAdvisorySwitch
bool RtiAmb_Hla13::enableAttributeRelevanceAdvisorySwitch(void) {
  try {
    rti->enableAttributeRelevanceAdvisorySwitch();
    String^ msg = "Attribute Relevance Advisory Switch is enabled.";
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return true;
  }
  catch (RTI::FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::ConcurrentAccessAttempted& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
  }
};

// enableAttributeRelevanceAdvisorySwitch
bool RtiAmb_Hla13::enableAttributeScopeAdvisorySwitch(void) {
  try {
    rti->enableAttributeScopeAdvisorySwitch();
    String^ msg = "Attribute Scope Advisory Switch is enabled.";
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return true;
  }
  catch (RTI::FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::ConcurrentAccessAttempted& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
  }
};

// enableInteractionRelevanceAdvisorySwitch
bool RtiAmb_Hla13::enableInteractionRelevanceAdvisorySwitch(void) {
  try {
    rti->enableInteractionRelevanceAdvisorySwitch();
    String^ msg = "Interaction Relevance Advisory Switch is enabled.";
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return true;
  }
  catch (RTI::FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::ConcurrentAccessAttempted& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RTIinternalError& e) {
    String^ msg = "MSG-(RTIinternalError-enableInteractionRelevanceAdvisorySwitch-RtiAmb_Hla13):" + Environment::NewLine + " Reason: " + gcnew String(e._reason) + Environment::NewLine;
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
  }
  catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
  }
};

// enableClassRelevanceAdvisorySwitch
bool RtiAmb_Hla13::enableObjectClassRelevanceAdvisorySwitch() {
  try {
    // Relevance advising is enabled by default
    rti->enableClassRelevanceAdvisorySwitch();
    String^ msg = "Object Class Relevance Advisory Switch is enabled.";
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return true;
  }
  catch (RTI::FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::ConcurrentAccessAttempted& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RTIinternalError& e) {
    String^ msg = "MSG-(RTIinternalError-enableClassRelevanceAdvisorySwitch-RtiAmb_Hla13):" + Environment::NewLine + " Reason: " + gcnew String(e._reason) + Environment::NewLine;
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
  }
  catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
  }
};

// disableAttributeRelevanceAdvisorySwitch
bool RtiAmb_Hla13::disableAttributeRelevanceAdvisorySwitch() {
  try {
    rti->disableAttributeRelevanceAdvisorySwitch();
    String^ msg = "Attribute Relevance Advisory Switch is disabled.";
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return true;
  }
  catch (RTI::FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::ConcurrentAccessAttempted& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
  }
};

// disableAttributeScopeAdvisorySwitch
bool RtiAmb_Hla13::disableAttributeScopeAdvisorySwitch() {
  try {
    rti->disableAttributeScopeAdvisorySwitch();
    String^ msg = "Attribute Scope Advisory Switch is disabled.";
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return true;
  }
  catch (RTI::FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::ConcurrentAccessAttempted& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
  }
};

// disableClassRelevanceAdvisorySwitch
bool RtiAmb_Hla13::disableObjectClassRelevanceAdvisorySwitch() {
  try {
    rti->disableClassRelevanceAdvisorySwitch();
    String^ msg = "Object Class Relevance Advisory Switch is disabled.";
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return true;
  }
  catch (RTI::FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::ConcurrentAccessAttempted& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
  }
};

// disableInteractionRelevanceAdvisorySwitch
bool RtiAmb_Hla13::disableInteractionRelevanceAdvisorySwitch() {
  try {
    rti->disableInteractionRelevanceAdvisorySwitch();
    String^ msg = "Interaction Relevance Advisory Switch is disabled.";
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return true;
  }
  catch (RTI::FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::ConcurrentAccessAttempted& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::SaveInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RestoreInProgress& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
		return false;
  }
};

// getClassHandleFromRti - OC
void RtiAmb_Hla13::getClassHandleFromRti(HlaObjectClass ^oc) {
  try {
    // Get Object ClassHandle
    oc->Handle = rti->getObjectClassHandle((const char*)(Marshal::StringToHGlobalAnsi(oc->Name)).ToPointer());
  }
  catch (RTI::NameNotFound& e) {
		String^ msg = "RtiAmb_Hla13-(Notspecified - Notspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
};

// getClassHandleFromRti - IC
void RtiAmb_Hla13::getClassHandleFromRti(HlaInteractionClass ^ic) {
  try {
    // Get Interaction ClassHandle
    ic->Handle = rti->getInteractionClassHandle((const char*)(Marshal::StringToHGlobalAnsi(ic->Name)).ToPointer());
  }
  catch (RTI::NameNotFound& e) {
		String^ msg = "RtiAmb_Hla13-(Exceptionnotspecified - methodnotspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
};

// getAttributeHandle
Handle RtiAmb_Hla13::getAttributeHandle(HlaAttribute^ attr, HlaObjectClass ^oc) {
  try {
    return rti->getAttributeHandle((const char*)(Marshal::StringToHGlobalAnsi(attr->Name)).ToPointer(), oc->Handle);
  }
#pragma region exceptions
  catch (RTI::ObjectClassNotDefined &e) {
		String^ msg = "RtiAmb_Hla13-(Exceptionnotspecified - methodnotspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::NameNotFound &e) {
		String^ msg = "RtiAmb_Hla13-(Exceptionnotspecified - methodnotspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla13-(Exceptionnotspecified - methodnotspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::ConcurrentAccessAttempted& e) {
		String^ msg = "RtiAmb_Hla13-(Exceptionnotspecified - methodnotspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla13-(Exceptionnotspecified - methodnotspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};
// getAttributeHandlesFromRti
void RtiAmb_Hla13::getAttributeHandlesFromRti(HlaObjectClass ^oc) {
  try {
    // Get AHs for all attributes
    for each (HlaAttribute^ attr in oc->Attributes)
      attr->Handle = rti->getAttributeHandle((const char*)(Marshal::StringToHGlobalAnsi(attr->Name)).ToPointer(), oc->Handle);
  }
  catch (RTI::NameNotFound& e) {
		String^ msg = "RtiAmb_Hla13-(Exceptionnotspecified - methodnotspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
};

// getParameterHandlesFromRti
void RtiAmb_Hla13::getParameterHandlesFromRti(HlaInteractionClass ^ic) {
  try {
    // Get Parameter Handles
    for each (HlaParameter^ parameter in ic->Parameters)
      parameter->Handle = rti->getParameterHandle((const char*)(Marshal::StringToHGlobalAnsi(parameter->Name)).ToPointer(), ic->Handle);
    //for each (KeyValuePair<String^, HlaParameter^> kvp in ic->Parameters)
    //	kvp.Value->ParameterHandle = rti->getParameterHandle((const char*)(Marshal::StringToHGlobalAnsi(kvp.Value->ParameterName)).ToPointer(), ic->ClassHandle);
  }
  catch (RTI::NameNotFound& e) {
		String^ msg = "RtiAmb_Hla13-(Exceptionnotspecified - methodnotspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
};


// getDimensionHandle
unsigned int RtiAmb_Hla13::getDimensionHandle(String^ dim, unsigned int space) {
  try {
    long handle = rti->getDimensionHandle((const char*)(Marshal::StringToHGlobalAnsi(dim)).ToPointer(), space);
    String^ msg = "Dimension (" + dim + ") Handle: " + handle;
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return handle;
  }
#pragma region exceptions
  catch (RTI::SpaceNotDefined &e) {
		String^ msg = "RtiAmb_Hla13-(Exceptionnotspecified - methodnotspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::NameNotFound &e) {
		String^ msg = "RtiAmb_Hla13-(Exceptionnotspecified - methodnotspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::FederateNotExecutionMember& e) {
		String^ msg = "RtiAmb_Hla13-(Exceptionnotspecified - methodnotspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::ConcurrentAccessAttempted& e) {
		String^ msg = "RtiAmb_Hla13-(Exceptionnotspecified - methodnotspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (RTI::RTIinternalError& e) {
		String^ msg = "RtiAmb_Hla13-(Exceptionnotspecified - methodnotspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion

};

// getSpaceHandle
unsigned int RtiAmb_Hla13::getSpaceHandle(String ^rs) {
  try {
    long handle = rti->getRoutingSpaceHandle((const char*)(Marshal::StringToHGlobalAnsi(rs)).ToPointer());
    String^ msg = "Routing Space (" + rs + ") Handle: " + handle;
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return handle;
  }
  catch (RTI::NameNotFound& e) {
		String^ msg = "RtiAmb_Hla13-(Exceptionnotspecified - methodnotspecified). Reason: " + gcnew String(e._reason);
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
	}
  catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
};

// Tick - zero argument
void RtiAmb_Hla13::evokeCallback(double approximateMinimumTimeInSeconds) {
	try {
		rti->tick();// The Zero argument tick() always return FALSE
	}
#pragma region exceptions
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};
// Tick - multiple argument
void RtiAmb_Hla13::evokeMultipleCallbacks(double approximateMinimumTimeInSeconds, double approximateMaximumTimeInSeconds) {
	try {
		rti->tick(approximateMinimumTimeInSeconds, approximateMaximumTimeInSeconds);
	}
#pragma region exceptions
	catch (System::Exception^ e) {
		String^ msg = "RtiAmb_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
		this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
	}
#pragma endregion
};

#pragma endregion // Support Services			
