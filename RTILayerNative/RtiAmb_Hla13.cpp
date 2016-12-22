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
#include "RtiAmb_Hla13.h"

using namespace Racon::RtiLayer;
using namespace System;
using namespace System::Runtime::InteropServices;// For Marshalling
using namespace System::Windows::Forms;
using namespace System::Threading; // for sleep
// Racon
using namespace Racon::RtiLayer;
using namespace Racon::RtiLayer::Native;

// Constructor
#pragma region Constructor
RtiAmb_Hla13::RtiAmb_Hla13(CallbackManager^ eventManager) : CRtiAmb() {
  try {
    rti = new RTI::RTIambassador();
    _nativeFdAmb = new FdAmb_Hla13(eventManager);
  }
  catch (Exception ^ e) {
    MessageBox::Show("MSG-(RtiAmb_Hla13 Ctor):" + Environment::NewLine + gcnew String(e->Message) + Environment::NewLine + "The native rti ambassador pointer creation  is failed. This may indicate a corrupted/a wrong RTI.rid file. Please ensure that RTI.rid is provided by DMSO HLA RTI13-NG distribution" + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
  }

};
#pragma endregion			

#pragma region Federation Management
// Connect
void RtiAmb_Hla13::connect(String ^localSetting) {
  // For IEEE1516-2010 - Pseudo Connect - Returns always connected
  this->OnFederateConnected(gcnew RaconEventArgs(""));
};
// Disconnect
void RtiAmb_Hla13::disconnect() {
  // For IEEE1516-2010 - Pseudo Disconnect - Returns always connected
  this->OnFederateDisconnected(gcnew RaconEventArgs(""));
};

// Create Federation
void RtiAmb_Hla13::createFederation(String ^fedexec, String ^fdd) {
  try {
    String^ msg = "Federation execution (name: " + fedexec + ") with FED ("+ fdd + ")";
    //this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    rti->createFederationExecution((char*)Marshal::StringToHGlobalAnsi(fedexec).ToPointer(), (char*)Marshal::StringToHGlobalAnsi(fdd).ToPointer());
    msg += " is created.";
    this->OnFederationExecutionCreated(gcnew RaconEventArgs(msg));
  }
  catch (RTI::FederationExecutionAlreadyExists&) {
    String^ msg = "Federation execution (name: " + fedexec + ") already exists.";
    this->OnFederationExecutionCreated(gcnew RaconEventArgs(msg));
  }
  catch (RTI::CouldNotOpenFED& e) {
    MessageBox::Show("MSG-(CouldNotOpenFED - createFederation):\r\n" + "Federate cannot open the FED file. Either the file does not exist or there exists an error in the file name or path provided. Please, check that FED file exists and its path is correct.Reason: " + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
  }
  catch (RTI::ErrorReadingFED& e) {
	  String^ msg = "Federation execution (name: " + fedexec + "), FED (" + fdd + ")";
    MessageBox::Show("MSG-(ErrorReadingFED - createFederation):\r\n" + "Federate cannot read the FED file. The FED file appears to be corrupt. Reason: " + gcnew String(e._reason) + Environment::NewLine + msg, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
  }
  catch (Exception ^ e) {
    MessageBox::Show("MSG-(GeneralExceptionInCreateFederation):\r\n" + "Exception in <createFederationExecution()>. The Federate cannot create the federation. Please check that RTI is running." + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
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
		MessageBox::Show("MSG-(GeneralException - listFederationExecutions):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RtiAmb_Hla1516e", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
#pragma endregion
};

// Join Federation
unsigned int RtiAmb_Hla13::joinFederation(String ^fedexec, String ^fdName) {
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
    catch (RTI::FederateAlreadyExecutionMember&) {
      MessageBox::Show("MSG-(FederateAlreadyExecutionMember - joinFederation):" + Environment::NewLine + "Federate already is an federation execution member." + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    }
    catch (RTI::FederationExecutionDoesNotExist& e) {
      MessageBox::Show("MSG-(FederationExecutionDoesNotExist - joinFederation):" + Environment::NewLine + "Federation Execution does not exist. Please, restart the federate. Reason: " + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    }
    catch (RTI::CouldNotOpenFED& e) {
      MessageBox::Show("MSG-(CouldNotOpenFED - joinFederation):" + Environment::NewLine + "Federate cannot open the FED file. Either the file does not exist or there exists an error in the file name or path provided. Please, check that FED file exists and its path is correct. Reason: " + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    }
    catch (RTI::ErrorReadingFED& e) {
      MessageBox::Show("MSG-(ErrorReadingFED - joinFederation):" + Environment::NewLine + "Federate cannot read the FED file. The FED file appears to be corrupt. Reason: " + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    }
    catch (Exception^ e) {
      MessageBox::Show("MSG-(GeneralException - joinFederation):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
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
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - resignFederation):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
  };

};

// Destroy Federation
void RtiAmb_Hla13::destroyFederation(String ^fedexec) {
  try {
    rti->destroyFederationExecution((const char*)(Marshal::StringToHGlobalAnsi(fedexec)).ToPointer());
    String^ msg = "Federation execution is finalized.";
    this->OnFederationExecutionDestroyed(gcnew RaconEventArgs(msg));
  }
  catch (RTI::FederatesCurrentlyJoined&) {
    String^ msg = "There are still federates joined in the federation. Federation execution is not destroyed." + Environment::NewLine;
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - destroyFederation):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
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
		MessageBox::Show("MSG-(RTIinternalError - registerFederationSynchronizationPoint):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RtiAmb_Hla13", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	catch (System::Exception^ e) {
		MessageBox::Show("MSG-(GeneralException - registerFederationSynchronizationPoint):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RtiAmb_Hla13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
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
		MessageBox::Show("MSG-(RTIinternalError - registerFederationSynchronizationPoint):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RtiAmb_Hla13", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	catch (System::Exception^ e) {
		MessageBox::Show("MSG-(GeneralException - registerFederationSynchronizationPoint):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RtiAmb_Hla13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
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
		MessageBox::Show("MSG-(RTIinternalError - synchronizationPointAchieved):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RtiAmb_Hla13", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	catch (System::Exception^ e) {
		MessageBox::Show("MSG-(GeneralException - synchronizationPointAchieved):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RtiAmb_Hla13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
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
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - requestFederationSave):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
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
    MessageBox::Show("MSG-(FederationTimeAlreadyPassed - requestFederationSave(time)):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::InvalidFederationTime &e) {
    MessageBox::Show("MSG-(InvalidFederationTime - requestFederationSave(time)):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::FederateNotExecutionMember& e) {
    MessageBox::Show("MSG-(FederateNotExecutionMember - requestFederationSave(time)):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::ConcurrentAccessAttempted& e) {
    MessageBox::Show("MSG-(ConcurrentAccessAttempted - requestFederationSave(time)):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::SaveInProgress& e) {
    MessageBox::Show("MSG-(SaveInProgress - requestFederationSave(time)):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RestoreInProgress& e) {
    MessageBox::Show("MSG-(RestoreInProgress - requestFederationSave(time)):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RTIinternalError& e) {
    MessageBox::Show("MSG-(RTIinternalError - requestFederationSave(time)):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - requestFederationSave(time)):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
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
    MessageBox::Show("MSG-(SaveNotInitiated - requestFederationSave):" + Environment::NewLine + " Reason: " + Environment::NewLine + "There are no current outstanding save requests of the local federate.", "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - requestFederationSave):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
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
    MessageBox::Show("MSG-(SaveNotInitiated - requestFederationSave):" + Environment::NewLine + " Reason: " + Environment::NewLine + "There are no current outstanding save requests or the federate failed to indicate the beginning of its save (i.e., via the federateSaveBegun() method).", "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - requestFederationSave):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
  }
};

// Request Federation Restore
void RtiAmb_Hla13::requestFederationRestore(String^ _label) {
  try {
    rti->requestFederationRestore((char*)Marshal::StringToHGlobalAnsi(_label).ToPointer());
    String^ msg = "This federate requested a federation restore (with label: " + _label + ")." + Environment::NewLine;
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - requestFederationRestore):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
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
    MessageBox::Show("MSG-(RestoreNotRequested - federateRestoreComplete):" + Environment::NewLine + " Reason: " + Environment::NewLine + "There is no outstanding request for a federate restoration.", "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - requestFederationSave):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
  }
};
#pragma endregion	// Federation Management

#pragma region Declaration Management
// publishOC
void RtiAmb_Hla13::publishObjectClass(HlaObjectClass ^oc, BindingList<HlaAttribute^>^ attributes)
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
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - publishObjectClass):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
};

// subscribeOC with selected attributes
bool RtiAmb_Hla13::subscribeObjectClass(HlaObjectClass ^oc, BindingList<HlaAttribute^>^ attributes, Boolean active) {
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
    MessageBox::Show("MSG-(ObjectClassNotDefined - subscribeObjectClassAttributes2):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
    return false;
  }
  catch (RTI::AttributeNotDefined &e) {
    MessageBox::Show("MSG-(AttributeNotDefined - subscribeObjectClassAttributes2):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
    return false;
  }
  catch (RTI::FederateNotExecutionMember& e) {
    MessageBox::Show("MSG-(FederateNotExecutionMember - subscribeObjectClassAttributes2):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
    return false;
  }
  catch (RTI::ConcurrentAccessAttempted& e) {
    MessageBox::Show("MSG-(ConcurrentAccessAttempted - subscribeObjectClassAttributes2):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
    return false;
  }
  catch (RTI::SaveInProgress& e) {
    MessageBox::Show("MSG-(SaveInProgress - subscribeObjectClassAttributes2):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
    return false;
  }
  catch (RTI::RestoreInProgress& e) {
    MessageBox::Show("MSG-(RestoreInProgress - subscribeObjectClassAttributes2):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
    return false;
  }
  catch (RTI::RTIinternalError& e) {
    MessageBox::Show("MSG-(RTIinternalError - subscribeObjectClassAttributes2):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
    return false;
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - subscribeObjectClassAttributes2):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    return false;
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
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - publishInteractionClass):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
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
    MessageBox::Show("MSG-(InteractionClassNotDefined - subscribeInteractionClass):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RegionNotKnown& e) {
    MessageBox::Show("MSG-(RegionNotKnown - subscribeInteractionClass):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::InvalidRegionContext& e) {
    MessageBox::Show("MSG-(InvalidRegionContext - subscribeInteractionClass):" + "One or more of the specified attribute handles is not valid within the context of the specified region." + Environment::NewLine + "Please, check that the interaction class is associated with a routing space in FED file." + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador - HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::FederateLoggingServiceCalls& e) {
    MessageBox::Show("MSG-(FederateLoggingServiceCalls - subscribeInteractionClass):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::FederateNotExecutionMember& e) {
    MessageBox::Show("MSG-(FederateNotExecutionMember - subscribeInteractionClass):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::ConcurrentAccessAttempted& e) {
    MessageBox::Show("MSG-(ConcurrentAccessAttempted - subscribeInteractionClass):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RestoreInProgress& e) {
    MessageBox::Show("MSG-(RestoreInProgress - subscribeInteractionClass):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::SaveInProgress& e) {
    MessageBox::Show("MSG-(SaveInProgress - subscribeInteractionClass):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RTIinternalError& e) {
    MessageBox::Show("MSG-(RTIinternalError - subscribeInteractionClass):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - subscribeInteractionClass):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
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
    MessageBox::Show("MSG-(InteractionClassNotDefined - unsubscribeInteractionClass):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::InteractionClassNotSubscribed& e) {
    MessageBox::Show("MSG-(InteractionClassNotSubscribed - unsubscribeInteractionClass):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::FederateNotExecutionMember& e) {
    MessageBox::Show("MSG-(FederateNotExecutionMember - unsubscribeInteractionClass):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::ConcurrentAccessAttempted& e) {
    MessageBox::Show("MSG-(ConcurrentAccessAttempted - unsubscribeInteractionClass):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::SaveInProgress& e) {
    MessageBox::Show("MSG-(SaveInProgress - unsubscribeInteractionClass):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RestoreInProgress& e) {
    MessageBox::Show("MSG-(RestoreInProgress - unsubscribeInteractionClass):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RTIinternalError& e) {
    MessageBox::Show("MSG-(RTIinternalError - unsubscribeInteractionClass):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - unsubscribeInteractionClass):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
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
    MessageBox::Show("MSG-(ObjectClassNotDefined - unsubscribeObjectClass):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
    return false;
  }
  catch (RTI::ObjectClassNotSubscribed &e) {
    MessageBox::Show("MSG-(ObjectClassNotSubscribed - unsubscribeObjectClass):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
    return false;
  }
  catch (RTI::FederateNotExecutionMember& e) {
    MessageBox::Show("MSG-(FederateNotExecutionMember - unsubscribeObjectClass):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
    return false;
  }
  catch (RTI::ConcurrentAccessAttempted& e) {
    MessageBox::Show("MSG-(ConcurrentAccessAttempted - unsubscribeObjectClass):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
    return false;
  }
  catch (RTI::SaveInProgress& e) {
    MessageBox::Show("MSG-(SaveInProgress - unsubscribeObjectClass):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
    return false;
  }
  catch (RTI::RestoreInProgress& e) {
    MessageBox::Show("MSG-(RestoreInProgress - unsubscribeObjectClass):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
    return false;
  }
  catch (RTI::RTIinternalError& e) {
    MessageBox::Show("MSG-(RTIinternalError - unsubscribeObjectClass):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
    return false;
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - unsubscribeObjectClass):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    return false;
  }
#pragma endregion
};

#pragma endregion	// Declaration Management		

#pragma region Object Management

// Delete an Object Instance
bool RtiAmb_Hla13::deleteObjectInstance(HlaObject ^obj) {
  try {
    rti->deleteObjectInstance(obj->Handle, (char*)Marshal::StringToHGlobalAnsi(obj->Tag).ToPointer());
    String^ msg = "Object (handle: " + obj->Handle + ") is deleted by this federate";
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return true;
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - deleteObjectInstance):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    return false;
  }
};

// Delete an Object Instance with Time
EventRetractionHandle^ RtiAmb_Hla13::deleteObjectInstance(HlaObject ^obj, Double time) {
  try {
    RTIfedTime time(time);
    RTI::FedTime& theTime = time;

    RTI::EventRetractionHandle theHandle = rti->deleteObjectInstance(obj->Handle, theTime, (char*)Marshal::StringToHGlobalAnsi(obj->Tag).ToPointer());
    EventRetractionHandle^ retraction = gcnew EventRetractionHandle(theHandle.theSerialNumber, theHandle.sendingFederate);
    String^ msg = "Object (handle: " + obj->Handle + ") is scheduled for deletion by this federate at time = " + time.getTime() + ". Retraction Serial No = " + retraction->SerialNumber + ".";
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return retraction;
  }
#pragma region exceptions
  catch (RTI::ObjectNotKnown &e) {
    MessageBox::Show("MSG-(ObjectNotKnown - deleteObjectInstance2):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RtiAmb_Hla13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::DeletePrivilegeNotHeld &e) {
    MessageBox::Show("MSG-(DeletePrivilegeNotHeld - deleteObjectInstance2):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::InvalidFederationTime &e) {
    MessageBox::Show("MSG-(InvalidFederationTime - deleteObjectInstance2):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::FederateNotExecutionMember& e) {
    MessageBox::Show("MSG-(FederateNotExecutionMember - deleteObjectInstance2):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::ConcurrentAccessAttempted& e) {
    MessageBox::Show("MSG-(ConcurrentAccessAttempted - deleteObjectInstance2):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::SaveInProgress& e) {
    MessageBox::Show("MSG-(SaveInProgress - deleteObjectInstance2):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RestoreInProgress& e) {
    MessageBox::Show("MSG-(RestoreInProgress - deleteObjectInstance2):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RTIinternalError& e) {
    MessageBox::Show("MSG-(RTIinternalError - deleteObjectInstance2):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - deleteObjectInstance2):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
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
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - registerObject):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
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
    String^ msg = "MSG-(ObjectAlreadyRegistered - registerObject2): The symbolic name associated with the object has already been registered within the federation." + Environment::NewLine;
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return false;
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - registerObject2):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
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
    MessageBox::Show("MSG-(InteractionClassNotDefined - sendInteraction):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::InteractionClassNotPublished &e) {
    MessageBox::Show("MSG-(InteractionClassNotPublished - sendInteraction):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::InteractionParameterNotDefined &e) {
    MessageBox::Show("MSG-(InteractionParameterNotDefined - sendInteraction):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::FederateNotExecutionMember& e) {
    MessageBox::Show("MSG-(FederateNotExecutionMember - sendInteraction):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::ConcurrentAccessAttempted& e) {
    MessageBox::Show("MSG-(ConcurrentAccessAttempted - sendInteraction):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::SaveInProgress& e) {
    MessageBox::Show("MSG-(SaveInProgress - sendInteraction):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RestoreInProgress& e) {
    MessageBox::Show("MSG-(RestoreInProgress - sendInteraction):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RTIinternalError& e) {
    MessageBox::Show("MSG-(RTIinternalError - sendInteraction):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
    return false;
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - sendInteraction):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    return false;
  }
#pragma endregion
};

// Send Interaction with Time stamp
EventRetractionHandle^ RtiAmb_Hla13::sendInteraction(HlaInteraction ^interaction, Double timestamp) {
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

    EventRetractionHandle^ retraction = gcnew EventRetractionHandle(erh.theSerialNumber, erh.sendingFederate);

    msg += "Interaction (handle: " + interaction->ClassHandle + ") is sent with timestamp: " + time.getTime() + ". Retraction serial no = " + retraction->SerialNumber + ". Retraction sending federate = " + retraction->SendingFederate;
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return retraction;
  }
#pragma region exceptions
  catch (RTI::InteractionClassNotDefined &e) {
    MessageBox::Show("MSG-(InteractionClassNotDefined - sendInteractionWithTime):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::InteractionClassNotPublished &e) {
    MessageBox::Show("MSG-(InteractionClassNotPublished - sendInteractionWithTime):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::InteractionParameterNotDefined &e) {
    MessageBox::Show("MSG-(InteractionParameterNotDefined - sendInteractionWithTime):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::InvalidFederationTime &e) {
    MessageBox::Show("MSG-(InvalidFederationTime - sendInteractionWithTime):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::FederateNotExecutionMember& e) {
    MessageBox::Show("MSG-(FederateNotExecutionMember - sendInteractionWithTime):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::ConcurrentAccessAttempted& e) {
    MessageBox::Show("MSG-(ConcurrentAccessAttempted - sendInteractionWithTime):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::SaveInProgress& e) {
    MessageBox::Show("MSG-(SaveInProgress - sendInteractionWithTime):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RestoreInProgress& e) {
    MessageBox::Show("MSG-(RestoreInProgress - sendInteractionWithTime):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RTIinternalError& e) {
    MessageBox::Show("MSG-(RTIinternalError - sendInteractionWithTime):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - sendInteractionWithTime):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
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
    MessageBox::Show("MSG-(ObjectNotKnown - updateAttributeValues):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
    return false;
  }
  catch (RTI::AttributeNotDefined &e) {
    MessageBox::Show("MSG-(AttributeNotDefined - updateAttributeValues):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
    return false;
  }
  catch (RTI::AttributeNotOwned &e) {
    MessageBox::Show("MSG-(AttributeNotOwned - updateAttributeValues):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
    return false;
  }
  catch (RTI::InvalidFederationTime &e) {
    MessageBox::Show("MSG-(InvalidFederationTime - updateAttributeValues):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
    return false;
  }
  catch (RTI::FederateNotExecutionMember& e) {
    MessageBox::Show("MSG-(FederateNotExecutionMember - updateAttributeValues):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
    return false;
  }
  catch (RTI::ConcurrentAccessAttempted& e) {
    MessageBox::Show("MSG-(ConcurrentAccessAttempted - updateAttributeValues):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
    return false;
  }
  catch (RTI::SaveInProgress& e) {
    MessageBox::Show("MSG-(SaveInProgress - updateAttributeValues):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
    return false;
  }
  catch (RTI::RestoreInProgress& e) {
    MessageBox::Show("MSG-(RestoreInProgress - updateAttributeValues):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
    return false;
  }
  catch (RTI::RTIinternalError& e) {
    MessageBox::Show("MSG-(RTIinternalError - updateAttributeValues):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
    return false;
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - updateAttributeValues):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    return false;
  }
#pragma endregion
};

// Update Attribute Values with Timestamp
EventRetractionHandle^ RtiAmb_Hla13::updateAttributeValues(HlaObject ^object, Double timestamp) {
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
    EventRetractionHandle^ retraction = gcnew EventRetractionHandle(erh.theSerialNumber, erh.sendingFederate);

    String^ msg = "Attribute values for object (handle: " + object->Handle + ") are updated. Retraction Serial No = " + retraction->SerialNumber + ".";
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return retraction;
  }
#pragma region exceptions
  catch (RTI::ObjectNotKnown &e) {
    MessageBox::Show("MSG-(ObjectNotKnown - updateAttributeValues2):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::AttributeNotDefined &e) {
    MessageBox::Show("MSG-(AttributeNotDefined - updateAttributeValues2):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::AttributeNotOwned &e) {
    MessageBox::Show("MSG-(AttributeNotOwned - updateAttributeValues2):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::InvalidFederationTime &e) {
    MessageBox::Show("MSG-(InvalidFederationTime - updateAttributeValues2):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::FederateNotExecutionMember& e) {
    MessageBox::Show("MSG-(FederateNotExecutionMember - updateAttributeValues2):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::ConcurrentAccessAttempted& e) {
    MessageBox::Show("MSG-(ConcurrentAccessAttempted - updateAttributeValues2):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::SaveInProgress& e) {
    MessageBox::Show("MSG-(SaveInProgress - updateAttributeValues2):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RestoreInProgress& e) {
    MessageBox::Show("MSG-(RestoreInProgress - updateAttributeValues2):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RTIinternalError& e) {
    MessageBox::Show("MSG-(RTIinternalError - updateAttributeValues2):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - updateAttributeValues2):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
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
  catch (RTI::ObjectClassNotDefined &) {
    MessageBox::Show("MSG-(ObjectClassNotDefined - requestClassAttributeValueUpdate):" + Environment::NewLine + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    return false;
  }
  catch (RTI::AttributeNotDefined &) {
    MessageBox::Show("MSG-(AttributeNotDefined - requestClassAttributeValueUpdate):" + Environment::NewLine + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    return false;
  }
  catch (RTI::FederateNotExecutionMember& e) {
    MessageBox::Show("MSG-(FederateNotExecutionMember - requestClassAttributeValueUpdate):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::ConcurrentAccessAttempted& e) {
    MessageBox::Show("MSG-(ConcurrentAccessAttempted - requestClassAttributeValueUpdate):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::SaveInProgress& e) {
    MessageBox::Show("MSG-(SaveInProgress - requestClassAttributeValueUpdate):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RestoreInProgress& e) {
    MessageBox::Show("MSG-(RestoreInProgress - requestClassAttributeValueUpdate):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RTIinternalError& e) {
    MessageBox::Show("MSG-(RTIinternalError - requestClassAttributeValueUpdate):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - requestClassAttributeValueUpdate):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
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
  catch (RTI::ObjectNotKnown &) {
    MessageBox::Show("MSG-(ObjectNotKnown - requestObjectAttributeValueUpdate):" + Environment::NewLine + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    return false;
  }
  catch (RTI::AttributeNotDefined &) {
    MessageBox::Show("MSG-(AttributeNotDefined - requestObjectAttributeValueUpdate):" + Environment::NewLine + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    return false;
  }
  catch (RTI::FederateNotExecutionMember& e) {
    MessageBox::Show("MSG-(FederateNotExecutionMember - requestObjectAttributeValueUpdate):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::ConcurrentAccessAttempted& e) {
    MessageBox::Show("MSG-(ConcurrentAccessAttempted - requestObjectAttributeValueUpdate):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::SaveInProgress& e) {
    MessageBox::Show("MSG-(SaveInProgress - requestObjectAttributeValueUpdate):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RestoreInProgress& e) {
    MessageBox::Show("MSG-(RestoreInProgress - requestObjectAttributeValueUpdate):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RTIinternalError& e) {
    MessageBox::Show("MSG-(RTIinternalError - requestObjectAttributeValueUpdate):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - requestObjectAttributeValueUpdate):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
  }
#pragma endregion
};
#pragma endregion	// Object Management	

#pragma region Ownership Management
// attributeOwnershipAcquisitionIfAvailable
bool RtiAmb_Hla13::attributeOwnershipAcquisitionIfAvailable(HlaObject ^object, RaconAttributeSet^ _set) {
  try {
    // Create Attribute Handle Set 
    RTI::AttributeHandleSet* ahs = RTI::AttributeHandleSetFactory::create(_set->Attributes->Count);
    // Add Attribute Handles to set
    for each (RTI::Handle attr in _set->Attributes)
      ahs->add(attr);
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
    MessageBox::Show("MSG-(ObjectClassNotPublished - attributeOwnershipAcquisitionIfAvailable):" + Environment::NewLine + "The operation attempted requires that the object class be currently published by the federate." + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    return false;
  }
  catch (RTI::AttributeNotDefined &e) {
    MessageBox::Show("MSG-(AttributeNotDefined - attributeOwnershipAcquisitionIfAvailable):" + Environment::NewLine + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    return false;
  }
  catch (RTI::AttributeNotPublished &e) {
    MessageBox::Show("MSG-(FederateOwnsAttributes - attributeOwnershipAcquisitionIfAvailable):" + Environment::NewLine + "One or more of the specified attributes are not currently published by the local federate." + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    return false;
  }
  catch (RTI::FederateOwnsAttributes &e) {
    MessageBox::Show("MSG-(FederateOwnsAttributes - attributeOwnershipAcquisitionIfAvailable):" + Environment::NewLine + "One or more attributes of the specified object are owned by the local federate." + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    return false;
  }
  catch (RTI::AttributeAlreadyBeingAcquired &e) {
    MessageBox::Show("MSG-(AttributeAlreadyBeingAcquired - attributeOwnershipAcquisitionIfAvailable):" + Environment::NewLine + "The federate is already in the process of acquiring one or more of the specified attribute-instances." + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    return false;
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - attributeOwnershipAcquisitionIfAvailable):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    return false;
  }
};

// attributeOwnershipAcquisition
bool RtiAmb_Hla13::attributeOwnershipAcquisition(HlaObject ^object, RaconAttributeSet^ _set) {
  try {
    // Create Attribute Handle Set 
    RTI::AttributeHandleSet* ahs = RTI::AttributeHandleSetFactory::create(_set->Attributes->Count);
    // Add Attribute Handles to set
    for each (RTI::Handle attr in _set->Attributes)
      ahs->add(attr);

    (void)rti->attributeOwnershipAcquisition(object->Handle, *ahs, (char*)Marshal::StringToHGlobalAnsi(object->Tag).ToPointer());
    String^ msg = "Ownership of attributes of object (handle: " + object->Handle + ") are requested.";
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return true;
  }
  catch (RTI::ObjectNotKnown&e) {
    String^ msg = "MSG-(ObjectNotKnown - attributeOwnershipAcquisition):" + Environment::NewLine + " Reason: " + gcnew String(e._reason) + Environment::NewLine;
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return false;
  }
  catch (RTI::ObjectClassNotPublished &e) {
    MessageBox::Show("MSG-(ObjectClassNotPublished - attributeOwnershipAcquisition):" + Environment::NewLine + "The operation attempted requires that the object class be currently published by the federate." + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    return false;
  }
  catch (RTI::AttributeNotDefined &e) {
    MessageBox::Show("MSG-(AttributeNotDefined - attributeOwnershipAcquisition):" + Environment::NewLine + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    return false;
  }
  catch (RTI::AttributeNotPublished &e) {
    MessageBox::Show("MSG-(FederateOwnsAttributes - attributeOwnershipAcquisition):" + Environment::NewLine + "One or more of the specified attributes are not currently published by the local federate." + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    return false;
  }
  catch (RTI::FederateOwnsAttributes &e) {
    MessageBox::Show("MSG-(FederateOwnsAttributes - attributeOwnershipAcquisition):" + Environment::NewLine + "One or more attributes of the specified object are owned by the local federate." + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    return false;
  }
  catch (RTI::FederateNotExecutionMember &e) {
    MessageBox::Show("MSG-(FederateNotExecutionMember, - attributeOwnershipAcquisition):" + Environment::NewLine + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    return false;
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - attributeOwnershipAcquisition):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    return false;
  }
};

// cancelAttributeOwnershipAcquisition
bool RtiAmb_Hla13::cancelAttributeOwnershipAcquisition(HlaObject ^object, RaconAttributeSet^ _set) {
  try {
    // Create Attribute Handle Set 
    RTI::AttributeHandleSet* ahs = RTI::AttributeHandleSetFactory::create(_set->Attributes->Count);
    // Add Attribute Handles to set
    for each (RTI::Handle attr in _set->Attributes)
      ahs->add(attr);
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
    MessageBox::Show("MSG-(AttributeNotDefined - cancelAttributeOwnershipAcquisition):" + Environment::NewLine + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    return false;
  }
  catch (RTI::AttributeAlreadyOwned &e) {
    MessageBox::Show("MSG-(AttributeAlreadyOwned - cancelAttributeOwnershipAcquisition):" + Environment::NewLine + "One or more of the instance-attributes is already owned by the local federate." + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    return false;
  }
  catch (RTI::AttributeAcquisitionWasNotRequested &e) {
    MessageBox::Show("MSG-(AttributeAcquisitionWasNotRequested - attributeOwnershipAcquisition):" + Environment::NewLine + "One or more of the instance-attributes is not the subject of a currently outstanding attributeOwnershipAcquisition() request." + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    return false;
  }
  catch (RTI::FederateNotExecutionMember &e) {
    MessageBox::Show("MSG-(FederateNotExecutionMember, - attributeOwnershipAcquisition):" + Environment::NewLine + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    return false;
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - attributeOwnershipAcquisition):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
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
    MessageBox::Show("MSG-(AttributeNotDefined - queryAttributeOwnership):" + Environment::NewLine + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    return false;
  }
  catch (RTI::FederateNotExecutionMember &e) {
    MessageBox::Show("MSG-(FederateNotExecutionMember, - queryAttributeOwnership):" + Environment::NewLine + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    return false;
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - queryAttributeOwnership):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    return false;
  }
};

// attributeOwnershipReleaseResponse
bool RtiAmb_Hla13::attributeOwnershipReleaseResponse(HlaObject ^object, RaconAttributeSet^ _set) {
  try {
    // Create Attribute Handle Set 
    RTI::AttributeHandleSet* ahs = RTI::AttributeHandleSetFactory::create(_set->Attributes->Count);
    // Add Attribute Handles to set
    for each (RTI::Handle attr in _set->Attributes)
      ahs->add(attr);
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
    MessageBox::Show("MSG-(AttributeNotDefined - attributeOwnershipReleaseResponse):" + Environment::NewLine + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    return false;
  }
  catch (RTI::FederateWasNotAskedToReleaseAttribute &e) {
    MessageBox::Show("MSG-(FederateWasNotAskedToReleaseAttribute - attributeOwnershipReleaseResponse):" + Environment::NewLine + "One or more of the attribute-instances are not the subject of a currently outstanding requestAttributeOwnershipRelease() notification." + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    return false;
  }
  catch (RTI::FederateNotExecutionMember &e) {
    MessageBox::Show("MSG-(FederateNotExecutionMember, - attributeOwnershipReleaseResponse):" + Environment::NewLine + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    return false;
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - queryAttributeOwnership):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    return false;
  }
};

// cancelNegotiatedAttributeOwnershipDivestiture
bool RtiAmb_Hla13::cancelNegotiatedAttributeOwnershipDivestiture(HlaObject ^object, RaconAttributeSet^ _set) {
  try {
    // Create Attribute Handle Set 
    RTI::AttributeHandleSet* ahs = RTI::AttributeHandleSetFactory::create(_set->Attributes->Count);
    // Add Attribute Handles to set
    for each (RTI::Handle attr in _set->Attributes)
      ahs->add(attr);
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
    MessageBox::Show("MSG-(AttributeNotDefined - cancelNegotiatedAttributeOwnershipDivestiture):" + Environment::NewLine + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    return false;
  }
  catch (RTI::AttributeDivestitureWasNotRequested &e) {
    MessageBox::Show("MSG-(AttributeDivestitureWasNotRequested - cancelNegotiatedAttributeOwnershipDivestiture):" + Environment::NewLine + "One or more of the attribute-instances are not the subject of a currently outstanding negotiatedAttributeOwnership-Divestiture() notification." + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    return false;
  }
  catch (RTI::FederateNotExecutionMember &e) {
    MessageBox::Show("MSG-(FederateNotExecutionMember, - cancelNegotiatedAttributeOwnershipDivestiture):" + Environment::NewLine + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    return false;
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - cancelNegotiatedAttributeOwnershipDivestiture):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    return false;
  }
};

// isAttributeOwnedByFederate
bool RtiAmb_Hla13::isAttributeOwnedByFederate(HlaObject ^object, HlaAttribute^ _attribute) {
  try {
    (void)rti->isAttributeOwnedByFederate(object->Handle, _attribute->Handle);
    String^ msg = "A query, whether the ownership of attribute (handle: " + _attribute->Handle + ") of object (handle: " + object->Handle + ") is this federate or not, is sent to RTI.";
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return true;
  }
  catch (RTI::ObjectNotKnown&e) {
    String^ msg = "MSG-(ObjectNotKnown - isAttributeOwnedByFederate):" + Environment::NewLine + " Reason: " + gcnew String(e._reason) + Environment::NewLine;
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return false;
  }
  catch (RTI::AttributeNotDefined &e) {
    MessageBox::Show("MSG-(AttributeNotDefined - isAttributeOwnedByFederate):" + Environment::NewLine + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    return false;
  }
  catch (RTI::FederateNotExecutionMember &e) {
    MessageBox::Show("MSG-(FederateNotExecutionMember, - isAttributeOwnedByFederate):" + Environment::NewLine + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    return false;
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - isAttributeOwnedByFederate):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    return false;
  }
};

// negotiatedAttributeOwnershipDivestiture
bool RtiAmb_Hla13::negotiatedAttributeOwnershipDivestiture(HlaObject ^object, RaconAttributeSet^ _set) {
  try {
    // Create Attribute Handle Set 
    RTI::AttributeHandleSet* ahs = RTI::AttributeHandleSetFactory::create(_set->Attributes->Count);
    // Add Attribute Handles to set
    for each (RTI::Handle attr in _set->Attributes)
      ahs->add(attr);
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
    MessageBox::Show("MSG-(AttributeNotDefined - negotiatedAttributeOwnershipDivestiture):" + Environment::NewLine + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    return false;
  }
  catch (RTI::AttributeNotOwned &e) {
    MessageBox::Show("MSG-(AttributeNotOwned - negotiatedAttributeOwnershipDivestiture):" + Environment::NewLine + "One or more of the specified attribute-instances is not owned by the local federate." + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    return false;
  }
  catch (RTI::AttributeAlreadyBeingDivested &e) {
    MessageBox::Show("MSG-(AttributeAlreadyBeingDivested - attributeOwnershipAcquisition):" + Environment::NewLine + "A negotiated divestiture is already in progress for one or more of the attribute-instances." + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    return false;
  }
  catch (RTI::FederateNotExecutionMember &e) {
    MessageBox::Show("MSG-(FederateNotExecutionMember, - negotiatedAttributeOwnershipDivestiture):" + Environment::NewLine + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    return false;
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - negotiatedAttributeOwnershipDivestiture):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    return false;
  }
};

// unconditionalAttributeOwnershipDivestiture
bool RtiAmb_Hla13::unconditionalAttributeOwnershipDivestiture(HlaObject ^object, RaconAttributeSet^ _set) {
  try {
    // Create Attribute Handle Set 
    RTI::AttributeHandleSet* ahs = RTI::AttributeHandleSetFactory::create(_set->Attributes->Count);
    // Add Attribute Handles to set
    for each (RTI::Handle attr in _set->Attributes)
      ahs->add(attr);
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
    MessageBox::Show("MSG-(AttributeNotDefined - unconditionalAttributeOwnershipDivestiture):" + Environment::NewLine + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    return false;
  }
  catch (RTI::AttributeNotOwned &e) {
    MessageBox::Show("MSG-(AttributeNotOwned - unconditionalAttributeOwnershipDivestiture):" + Environment::NewLine + "One or more of the specified attribute-instances is not owned by the local federate." + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    return false;
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - unconditionalAttributeOwnershipDivestiture):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    return false;
  }
};
#pragma endregion	// Ownership Management

#pragma region Data Distribution Management

// associateRegionForUpdates
bool RtiAmb_Hla13::associateRegionForUpdates(CHlaRegion ^region, HlaObject ^object, List<HlaAttribute^> ^attributes) {
  try {
    RTI::AttributeHandleSet* ahs = RTI::AttributeHandleSetFactory::create(attributes->Count);
    for each (HlaAttribute^ attr in attributes)
      ahs->add(attr->Handle);
    // Check that ahs is not null
    if (ahs) {
      rti->associateRegionForUpdates(*(region->Handle), object->Handle, *ahs);
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
    MessageBox::Show("MSG-(ObjectNotKnown - associateRegionForUpdates):" + Environment::NewLine + "Please ensure that you try to associate with a registered object" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::AttributeNotDefined &e) {
    MessageBox::Show("MSG-(AttributeNotDefined - associateRegionForUpdates):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RegionNotKnown &e) {
    MessageBox::Show("MSG-(RegionNotKnown - associateRegionForUpdates):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::InvalidRegionContext &e) {
    MessageBox::Show("MSG-(InvalidRegionContext - associateRegionForUpdates):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::FederateNotExecutionMember& e) {
    MessageBox::Show("MSG-(FederateNotExecutionMember - associateRegionForUpdates):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::ConcurrentAccessAttempted& e) {
    MessageBox::Show("MSG-(ConcurrentAccessAttempted - associateRegionForUpdates):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::SaveInProgress& e) {
    MessageBox::Show("MSG-(SaveInProgress - associateRegionForUpdates):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RestoreInProgress& e) {
    MessageBox::Show("MSG-(RestoreInProgress - associateRegionForUpdates):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RTIinternalError& e) {
    MessageBox::Show("MSG-(RTIinternalError - associateRegionForUpdates):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - associateRegionForUpdates):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
  }
#pragma endregion
};

// Create region
bool RtiAmb_Hla13::createRegion(CHlaRegion^ region, unsigned long noOfExtents, long spaceHandle) {
  try {
    //Create region
    region->Handle = rti->createRegion(spaceHandle, noOfExtents);
    String^ msg = "Region (" + region->Name + ") is created.";
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return true;
  }
#pragma region exceptions
  catch (RTI::SpaceNotDefined &e) {
    MessageBox::Show("MSG-(SpaceNotDefined - createRegion):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::InvalidExtents &e) {
    MessageBox::Show("MSG-(InvalidExtents - createRegion):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::FederateNotExecutionMember& e) {
    MessageBox::Show("MSG-(FederateNotExecutionMember - createRegion):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::ConcurrentAccessAttempted& e) {
    MessageBox::Show("MSG-(ConcurrentAccessAttempted - createRegion):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::SaveInProgress& e) {
    MessageBox::Show("MSG-(SaveInProgress - createRegion):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RestoreInProgress& e) {
    MessageBox::Show("MSG-(RestoreInProgress - createRegion):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RTIinternalError& e) {
    MessageBox::Show("MSG-(RTIinternalError - createRegion):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
    return false;
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - createRegion):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    return false;
  }
#pragma endregion

};

// Delete region
bool RtiAmb_Hla13::deleteRegion(CHlaRegion^ region) {
  try {
    rti->deleteRegion(region->Handle);
    String^ msg = "Region (" + region->Name + ") is deleted.";
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return true;
  }
#pragma region exceptions
  catch (RTI::RegionNotKnown &e) {
    MessageBox::Show("MSG-(RegionNotKnown - deleteRegion):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RegionInUse &e) {
    MessageBox::Show("MSG-(RegionInUse - deleteRegion):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::FederateNotExecutionMember& e) {
    MessageBox::Show("MSG-(FederateNotExecutionMember - deleteRegion):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::ConcurrentAccessAttempted& e) {
    MessageBox::Show("MSG-(ConcurrentAccessAttempted - deleteRegion):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::SaveInProgress& e) {
    MessageBox::Show("MSG-(SaveInProgress - deleteRegion):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RestoreInProgress& e) {
    MessageBox::Show("MSG-(RestoreInProgress - deleteRegion):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RTIinternalError& e) {
    MessageBox::Show("MSG-(RTIinternalError - deleteRegion):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - deleteRegion):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    return false;
  }
#pragma endregion
};

// Register Object Instance With Region - selected (publishable) attributes with specific regions
bool RtiAmb_Hla13::registerObjectInstanceWithRegion(HlaObject ^obj, List<HlaAttribute^> ^attributes, List<CHlaRegion^> ^regions) {
  try {
    RTI::ObjectClassHandle theClass = obj->Type->Handle;
    RTI::Region **theRegions = (RTI::Region**)malloc(regions->Count * sizeof(RTI::Region *));
    RTI::Handle* theAttributes = new RTI::Handle[attributes->Count];
    int count = 0;
    String^ pairs = "";
    for each (HlaAttribute^ attr in attributes) {
      theRegions[count] = regions[count]->Handle;
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
    return true;
  }
#pragma region exceptions
  catch (RTI::ObjectClassNotDefined& e) {
    MessageBox::Show("MSG-(ObjectClassNotDefined - registerObjectInstanceWithRegion):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::ObjectClassNotPublished &e) {
    MessageBox::Show("MSG-(ObjectClassNotPublished - registerObjectInstanceWithRegion):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::AttributeNotDefined &e) {
    MessageBox::Show("MSG-(AttributeNotDefined - registerObjectInstanceWithRegion):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::AttributeNotPublished &e) {
    MessageBox::Show("MSG-(AttributeNotPublished - registerObjectInstanceWithRegion):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RegionNotKnown &e) {
    MessageBox::Show("MSG-(RegionNotKnown - registerObjectInstanceWithRegion):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::InvalidRegionContext& e) {
    MessageBox::Show("MSG-(InvalidRegionContext - registerObjectInstanceWithRegion):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::ObjectAlreadyRegistered& e) {
    MessageBox::Show("MSG-(ObjectAlreadyRegistered - registerObjectInstanceWithRegion):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::FederateNotExecutionMember& e) {
    MessageBox::Show("MSG-(FederateNotExecutionMember - registerObjectInstanceWithRegion):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::ConcurrentAccessAttempted& e) {
    MessageBox::Show("MSG-(ConcurrentAccessAttempted - registerObjectInstanceWithRegion):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::SaveInProgress& e) {
    MessageBox::Show("MSG-(SaveInProgress - registerObjectInstanceWithRegion):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RestoreInProgress& e) {
    MessageBox::Show("MSG-(RestoreInProgress - registerObjectInstanceWithRegion):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RTIinternalError& e) {
    MessageBox::Show("MSG-(RTIinternalError - registerObjectInstanceWithRegion):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - registerObjectInstanceWithRegion):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    return false;
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

// requestClassAttributeValueUpdateWithRegion
bool RtiAmb_Hla13::requestClassAttributeValueUpdateWithRegion(HlaObjectClass ^oc, List<HlaAttribute^> ^attributes, CHlaRegion ^region) {
  try {
    RTI::AttributeHandleSet* ahs = RTI::AttributeHandleSetFactory::create(attributes->Count);
    // Add attribute handles to AHS
    for (int j = 0; j < attributes->Count; j++)
      ahs->add(attributes[j]->Handle);
    // Check that ahs is not null
    if (ahs) {
      rti->requestClassAttributeValueUpdateWithRegion(oc->Handle, *ahs, *(region->Handle));
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
    MessageBox::Show("MSG-(ObjectClassNotDefined - requestClassAttributeValueUpdateWithRegion):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::AttributeNotDefined &e) {
    MessageBox::Show("MSG-(AttributeNotDefined - requestClassAttributeValueUpdateWithRegion):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RegionNotKnown &e) {
    MessageBox::Show("MSG-(RegionNotKnown - requestClassAttributeValueUpdateWithRegion):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::InvalidRegionContext &e) {
    MessageBox::Show("MSG-(InvalidRegionContext - requestClassAttributeValueUpdateWithRegion):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::FederateNotExecutionMember& e) {
    MessageBox::Show("MSG-(FederateNotExecutionMember - requestClassAttributeValueUpdateWithRegion):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::ConcurrentAccessAttempted& e) {
    MessageBox::Show("MSG-(ConcurrentAccessAttempted - requestClassAttributeValueUpdateWithRegion):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::SaveInProgress& e) {
    MessageBox::Show("MSG-(SaveInProgress - requestClassAttributeValueUpdateWithRegion):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RestoreInProgress& e) {
    MessageBox::Show("MSG-(RestoreInProgress - requestClassAttributeValueUpdateWithRegion):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RTIinternalError& e) {
    MessageBox::Show("MSG-(RTIinternalError - requestClassAttributeValueUpdateWithRegion):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - requestClassAttributeValueUpdateWithRegion):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
  }
#pragma endregion
};

// Send Interaction With Region
bool RtiAmb_Hla13::sendInteraction(HlaInteraction ^interaction, CHlaRegion ^region) {
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
      rti->sendInteractionWithRegion(interaction->ClassHandle, *phvps, (char*)(Marshal::StringToHGlobalAnsi(interaction->Tag)).ToPointer(), *(region->Handle));
      msg = "Interaction (handle: " + interaction->ClassHandle + ") is sent via region (" + region->Name + ").";
    }
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return true;
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - sendInteraction):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    return false;
  };
};

// subscribe IC with Region
bool RtiAmb_Hla13::subscribeInteractionClass(HlaInteractionClass ^ic, CHlaRegion ^region) {

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
      rti->subscribeInteractionClassWithRegion(ic->Handle, *region->Handle);
      msg = "Interaction Class (name: " + ic->Name + ", handle: " + ic->Handle + ") is subscribed with region.";
      this->OnHLAClassSubscribed(gcnew RaconEventArgs(msg));
      return true;
    }
  }
#pragma region exceptions
  catch (RTI::InteractionClassNotDefined& e) {
    MessageBox::Show("MSG-(InteractionClassNotDefined - subscribeInteractionClass):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RegionNotKnown& e) {
    MessageBox::Show("MSG-(RegionNotKnown - subscribeInteractionClass):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::InvalidRegionContext& e) {
    MessageBox::Show("MSG-(InvalidRegionContext - subscribeInteractionClass):" + "One or more of the specified attribute handles is not valid within the context of the specified region." + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador - HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::FederateLoggingServiceCalls& e) {
    MessageBox::Show("MSG-(FederateLoggingServiceCalls - subscribeInteractionClass):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::FederateNotExecutionMember& e) {
    MessageBox::Show("MSG-(FederateNotExecutionMember - subscribeInteractionClass):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::ConcurrentAccessAttempted& e) {
    MessageBox::Show("MSG-(ConcurrentAccessAttempted - subscribeInteractionClass):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RestoreInProgress& e) {
    MessageBox::Show("MSG-(RestoreInProgress - subscribeInteractionClass):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::SaveInProgress& e) {
    MessageBox::Show("MSG-(SaveInProgress - subscribeInteractionClass):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RTIinternalError& e) {
    MessageBox::Show("MSG-(RTIinternalError - subscribeInteractionClass):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - subscribeInteractionClass):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
#pragma endregion
};

// subscribeOC with Region
bool RtiAmb_Hla13::subscribeObjectClassAttributesWithRegion(HlaObjectClass ^oc, List<HlaAttribute^>^ attributes, CHlaRegion ^reg, Boolean active) {
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
      rti->subscribeObjectClassAttributesWithRegion(oc->Handle, *(reg->Handle), *ahs, (RTI::Boolean)active);
      String^ tmpStr = "";
      // Report attributes to the user
      for (int j = 0; j < tmpList->Count; j++) {
        tmpStr += tmpList[j]->Name;
        if (j != tmpList->Count - 1) tmpStr += ", ";
      };
      String^ msg = "Object Class (name: " + oc->Name + ") is subscribed with Region: " + reg->Name + ". Class handle: " + oc->Handle + ", Attributes: {" + tmpStr + "}";
      this->OnHLAClassSubscribed(gcnew RaconEventArgs(msg));
      // Release Mem
      ahs->empty(); delete ahs;
      return true;
    }
  }
#pragma region exceptions
  catch (RTI::ObjectClassNotDefined& e) {
    MessageBox::Show("MSG-(ObjectClassNotDefined - subscribeObjectClassAttributesWithRegion):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::AttributeNotDefined &e) {
    MessageBox::Show("MSG-(AttributeNotDefined - subscribeObjectClassAttributesWithRegion):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RegionNotKnown &e) {
    MessageBox::Show("MSG-(RegionNotKnown - subscribeObjectClassAttributesWithRegion):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::InvalidRegionContext& e) {
    MessageBox::Show("MSG-(InvalidRegionContext - subscribeObjectClassAttributesWithRegion):" + Environment::NewLine + "Check your FED file that the attribute you try to subscribe is associated with the related routing space." + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::FederateNotExecutionMember& e) {
    MessageBox::Show("MSG-(FederateNotExecutionMember - subscribeObjectClassAttributesWithRegion):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::ConcurrentAccessAttempted& e) {
    MessageBox::Show("MSG-(ConcurrentAccessAttempted - subscribeObjectClassAttributesWithRegion):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::SaveInProgress& e) {
    MessageBox::Show("MSG-(SaveInProgress - subscribeObjectClassAttributesWithRegion):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RestoreInProgress& e) {
    MessageBox::Show("MSG-(RestoreInProgress - subscribeObjectClassAttributesWithRegion):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RTIinternalError& e) {
    MessageBox::Show("MSG-(RTIinternalError - subscribeObjectClassAttributesWithRegion):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - subscribeObjectClassAttributesWithRegion):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
  }
#pragma endregion
};

// associateRegionForUpdates
bool RtiAmb_Hla13::unassociateRegionForUpdates(CHlaRegion ^region, HlaObject ^object) {
  try {
    rti->unassociateRegionForUpdates(*(region->Handle), object->Handle);
    String^ msg = "The association between object (handle: " + object->Handle + ") and region (" + region->Name + ") is removed.";
    this->OnHLAClassSubscribed(gcnew RaconEventArgs(msg));
    return true;
  }
#pragma region exceptions
  catch (RTI::ObjectNotKnown &e) {
    MessageBox::Show("MSG-(ObjectNotKnown - unassociateRegionForUpdates):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RegionNotKnown &e) {
    MessageBox::Show("MSG-(RegionNotKnown - unassociateRegionForUpdates):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::InvalidRegionContext &e) {
    MessageBox::Show("MSG-(InvalidRegionContext - unassociateRegionForUpdates):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::FederateNotExecutionMember& e) {
    MessageBox::Show("MSG-(FederateNotExecutionMember - unassociateRegionForUpdates):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::ConcurrentAccessAttempted& e) {
    MessageBox::Show("MSG-(ConcurrentAccessAttempted - unassociateRegionForUpdates):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::SaveInProgress& e) {
    MessageBox::Show("MSG-(SaveInProgress - unassociateRegionForUpdates):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RestoreInProgress& e) {
    MessageBox::Show("MSG-(RestoreInProgress - unassociateRegionForUpdates):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RTIinternalError& e) {
    MessageBox::Show("MSG-(RTIinternalError - unassociateRegionForUpdates):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - unassociateRegionForUpdates):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
  }
#pragma endregion
};

// unsubscribe IC with Region
void RtiAmb_Hla13::unsubscribeInteractionClass(HlaInteractionClass ^ic, CHlaRegion ^region) {
  try {
    if (region) rti->unsubscribeInteractionClassWithRegion(ic->Handle, *region->Handle);
    String^ msg = "Interaction Class (name: " + ic->Name + ", handle:" + ic->Handle + ") is unsubscribed from region (" + region->Name + ").";
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
  }

  catch (RTI::InteractionClassNotDefined& e) {
    MessageBox::Show("MSG-(InteractionClassNotDefined - unsubscribeInteractionClass):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::InteractionClassNotSubscribed& e) {
    MessageBox::Show("MSG-(InteractionClassNotSubscribed - unsubscribeInteractionClass):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RegionNotKnown& e) {
    MessageBox::Show("MSG-(RegionNotKnown - unsubscribeInteractionClass):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::FederateNotExecutionMember& e) {
    MessageBox::Show("MSG-(FederateNotExecutionMember - unsubscribeInteractionClass):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::ConcurrentAccessAttempted& e) {
    MessageBox::Show("MSG-(ConcurrentAccessAttempted - unsubscribeInteractionClass):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::SaveInProgress& e) {
    MessageBox::Show("MSG-(SaveInProgress - unsubscribeInteractionClass):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RestoreInProgress& e) {
    MessageBox::Show("MSG-(RestoreInProgress - unsubscribeInteractionClass):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RTIinternalError& e) {
    MessageBox::Show("MSG-(RTIinternalError - unsubscribeInteractionClass):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - unsubscribeInteractionClass):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
};

// unsubscribe OC with Region
void RtiAmb_Hla13::unsubscribeObjectClassWithRegion(HlaObjectClass ^oc, CHlaRegion ^region) {
  try {
    if (region) rti->unsubscribeObjectClassWithRegion(oc->Handle, *region->Handle);
    String^ msg = "Object Class (name: " + oc->Name + ", handle:" + oc->Handle + ") is unsubscribed from region (" + region->Name + ").";
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
  }
#pragma region exceptions
  catch (RTI::ObjectClassNotDefined& e) {
    MessageBox::Show("MSG-(ObjectClassNotDefined - unsubscribeObjectClassAttributesWithRegion):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RegionNotKnown &e) {
    MessageBox::Show("MSG-(RegionNotKnown - unsubscribeObjectClassAttributesWithRegion):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::ObjectClassNotSubscribed &e) {
    MessageBox::Show("MSG-(ObjectClassNotSubscribed - unsubscribeObjectClassAttributesWithRegion):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::FederateNotExecutionMember& e) {
    MessageBox::Show("MSG-(FederateNotExecutionMember - unsubscribeObjectClassAttributesWithRegion):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::ConcurrentAccessAttempted& e) {
    MessageBox::Show("MSG-(ConcurrentAccessAttempted - unsubscribeObjectClassAttributesWithRegion):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::SaveInProgress& e) {
    MessageBox::Show("MSG-(SaveInProgress - unsubscribeObjectClassAttributesWithRegion):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RestoreInProgress& e) {
    MessageBox::Show("MSG-(RestoreInProgress - unsubscribeObjectClassAttributesWithRegion):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RTIinternalError& e) {
    MessageBox::Show("MSG-(RTIinternalError - unsubscribeObjectClassAttributesWithRegion):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - unsubscribeObjectClassAttributesWithRegion):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
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
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - enableAsynchronousDelivery):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    return false;
  }
};

// changeAttributeOrderType
bool RtiAmb_Hla13::changeAttributeOrderType(HlaObject ^obj, List<HlaAttribute^>^ attributes, OrderingHandle type) {
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
    MessageBox::Show("MSG-(ObjectNotKnown - changeAttributeOrderType):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::AttributeNotDefined &e) {
    MessageBox::Show("MSG-(AttributeNotDefined - changeAttributeOrderType):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::AttributeNotOwned &e) {
    MessageBox::Show("MSG-(AttributeNotOwned - changeAttributeOrderType):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::InvalidOrderingHandle &e) {
    MessageBox::Show("MSG-(InvalidOrderingHandle - changeAttributeOrderType):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::FederateNotExecutionMember& e) {
    MessageBox::Show("MSG-(FederateNotExecutionMember - changeAttributeOrderType):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::ConcurrentAccessAttempted& e) {
    MessageBox::Show("MSG-(ConcurrentAccessAttempted - changeAttributeOrderType):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::SaveInProgress& e) {
    MessageBox::Show("MSG-(SaveInProgress - changeAttributeOrderType):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RestoreInProgress& e) {
    MessageBox::Show("MSG-(RestoreInProgress - changeAttributeOrderType):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RTIinternalError& e) {
    MessageBox::Show("MSG-(RTIinternalError - changeAttributeOrderType):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
    return false;
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - changeAttributeOrderType):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    return false;
  }
#pragma endregion
};

// changeInteractionOrderType
bool RtiAmb_Hla13::changeInteractionOrderType(HlaInteractionClass ^ic, OrderingHandle type) {
  try {
    rti->changeInteractionOrderType(ic->Handle, type);
    String^ msg = "The ordering policy for interaction class (" + ic->Name + ") is changed to " + (type) ? "(Receive)" : "(TimeStamp)";
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
    return true;
  }
#pragma region exceptions
  catch (RTI::InteractionClassNotDefined &e) {
    MessageBox::Show("MSG-(InteractionClassNotDefined - changeInteractionOrderType):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::InteractionClassNotPublished &e) {
    MessageBox::Show("MSG-(InteractionClassNotPublished - changeInteractionOrderType):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::InvalidOrderingHandle &e) {
    MessageBox::Show("MSG-(InvalidOrderingHandle - changeInteractionOrderType):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::FederateNotExecutionMember& e) {
    MessageBox::Show("MSG-(FederateNotExecutionMember - changeInteractionOrderType):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::ConcurrentAccessAttempted& e) {
    MessageBox::Show("MSG-(ConcurrentAccessAttempted - changeInteractionOrderType):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::SaveInProgress& e) {
    MessageBox::Show("MSG-(SaveInProgress - changeInteractionOrderType):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RestoreInProgress& e) {
    MessageBox::Show("MSG-(RestoreInProgress - changeInteractionOrderType):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RTIinternalError& e) {
    MessageBox::Show("MSG-(RTIinternalError - changeInteractionOrderType):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
    return false;
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - changeInteractionOrderType):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
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
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - disableAsynchronousDelivery):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
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
    MessageBox::Show("MSG-(TimeConstrainedWasNotEnabled - disableTimeConstrained):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::FederateNotExecutionMember& e) {
    MessageBox::Show("MSG-(FederateNotExecutionMember - disableTimeConstrained):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::ConcurrentAccessAttempted& e) {
    MessageBox::Show("MSG-(ConcurrentAccessAttempted - disableTimeConstrained):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::SaveInProgress& e) {
    MessageBox::Show("MSG-(SaveInProgress - disableTimeConstrained):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RestoreInProgress& e) {
    MessageBox::Show("MSG-(RestoreInProgress - disableTimeConstrained):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RTIinternalError& e) {
    MessageBox::Show("MSG-(RTIinternalError - disableTimeConstrained):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - disableTimeConstrained):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
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
    MessageBox::Show("MSG-(TimeRegulationWasNotEnabled - disableTimeRegulation):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::FederateNotExecutionMember& e) {
    MessageBox::Show("MSG-(FederateNotExecutionMember - disableTimeRegulation):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::ConcurrentAccessAttempted& e) {
    MessageBox::Show("MSG-(ConcurrentAccessAttempted - disableTimeRegulation):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::SaveInProgress& e) {
    MessageBox::Show("MSG-(SaveInProgress - disableTimeRegulation):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RestoreInProgress& e) {
    MessageBox::Show("MSG-(RestoreInProgress - disableTimeRegulation):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RTIinternalError& e) {
    MessageBox::Show("MSG-(RTIinternalError - disableTimeRegulation):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - disableTimeRegulation):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
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
    MessageBox::Show("MSG-(TimeConstrainedAlreadyEnabled - enableTimeConstrained):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::EnableTimeConstrainedPending &e) {
    MessageBox::Show("MSG-(EnableTimeConstrainedPending - enableTimeConstrained):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::TimeAdvanceAlreadyInProgress &e) {
    MessageBox::Show("MSG-(TimeAdvanceAlreadyInProgress - enableTimeConstrained):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::FederateNotExecutionMember& e) {
    MessageBox::Show("MSG-(FederateNotExecutionMember - enableTimeConstrained):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::ConcurrentAccessAttempted& e) {
    MessageBox::Show("MSG-(ConcurrentAccessAttempted - enableTimeConstrained):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::SaveInProgress& e) {
    MessageBox::Show("MSG-(SaveInProgress - enableTimeConstrained):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RestoreInProgress& e) {
    MessageBox::Show("MSG-(RestoreInProgress - enableTimeConstrained):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RTIinternalError& e) {
    MessageBox::Show("MSG-(RTIinternalError - enableTimeConstrained):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - enableTimeConstrained):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
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
    MessageBox::Show("MSG-(TimeRegulationAlreadyEnabled - enableTimeRegulation):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::EnableTimeRegulationPending &e) {
    MessageBox::Show("MSG-(EnableTimeRegulationPending - enableTimeRegulation):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::TimeAdvanceAlreadyInProgress &e) {
    MessageBox::Show("MSG-(TimeAdvanceAlreadyInProgress - enableTimeRegulation):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::InvalidFederationTime &e) {
    MessageBox::Show("MSG-(InvalidFederationTime - enableTimeRegulation):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::InvalidLookahead &e) {
    MessageBox::Show("MSG-(InvalidLookahead - enableTimeRegulation):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::FederateNotExecutionMember& e) {
    MessageBox::Show("MSG-(FederateNotExecutionMember - enableTimeRegulation):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::ConcurrentAccessAttempted& e) {
    MessageBox::Show("MSG-(ConcurrentAccessAttempted - enableTimeRegulation):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::SaveInProgress& e) {
    MessageBox::Show("MSG-(SaveInProgress - enableTimeRegulation):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RestoreInProgress& e) {
    MessageBox::Show("MSG-(RestoreInProgress - enableTimeRegulation):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RTIinternalError& e) {
    MessageBox::Show("MSG-(RTIinternalError - enableTimeRegulation):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - enableTimeRegulation):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
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
    MessageBox::Show("MSG-(InvalidFederationTime - flushQueueRequest):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::FederationTimeAlreadyPassed &e) {
    MessageBox::Show("MSG-(FederationTimeAlreadyPassed - flushQueueRequest):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::TimeAdvanceAlreadyInProgress &e) {
    MessageBox::Show("MSG-(TimeAdvanceAlreadyInProgress - flushQueueRequest):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::EnableTimeConstrainedPending &e) {
    MessageBox::Show("MSG-(EnableTimeConstrainedPending - flushQueueRequest):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::EnableTimeRegulationPending &e) {
    MessageBox::Show("MSG-(EnableTimeRegulationPending - flushQueueRequest):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::FederateNotExecutionMember& e) {
    MessageBox::Show("MSG-(FederateNotExecutionMember - flushQueueRequest):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::ConcurrentAccessAttempted& e) {
    MessageBox::Show("MSG-(ConcurrentAccessAttempted - nextEventRequest):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::SaveInProgress& e) {
    MessageBox::Show("MSG-(SaveInProgress - flushQueueRequest):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RestoreInProgress& e) {
    MessageBox::Show("MSG-(RestoreInProgress - flushQueueRequest):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RTIinternalError& e) {
    MessageBox::Show("MSG-(RTIinternalError - flushQueueRequest):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - flushQueueRequest):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
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
    MessageBox::Show("MSG-(InvalidLookahead - modifyLookahead):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::FederateNotExecutionMember& e) {
    MessageBox::Show("MSG-(FederateNotExecutionMember - modifyLookahead):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::ConcurrentAccessAttempted& e) {
    MessageBox::Show("MSG-(ConcurrentAccessAttempted - modifyLookahead):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::SaveInProgress& e) {
    MessageBox::Show("MSG-(SaveInProgress - modifyLookahead):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RestoreInProgress& e) {
    MessageBox::Show("MSG-(RestoreInProgress - modifyLookahead):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RTIinternalError& e) {
    MessageBox::Show("MSG-(RTIinternalError - modifyLookahead):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - modifyLookahead):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
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
    MessageBox::Show("MSG-(InvalidFederationTime - nextEventRequest):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::FederationTimeAlreadyPassed &e) {
    MessageBox::Show("MSG-(FederationTimeAlreadyPassed - nextEventRequest):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::TimeAdvanceAlreadyInProgress &e) {
    MessageBox::Show("MSG-(TimeAdvanceAlreadyInProgress - nextEventRequest):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::EnableTimeConstrainedPending &e) {
    MessageBox::Show("MSG-(EnableTimeConstrainedPending - nextEventRequest):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::EnableTimeRegulationPending &e) {
    MessageBox::Show("MSG-(EnableTimeRegulationPending - nextEventRequest):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::FederateNotExecutionMember& e) {
    MessageBox::Show("MSG-(FederateNotExecutionMember - nextEventRequest):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::ConcurrentAccessAttempted& e) {
    MessageBox::Show("MSG-(ConcurrentAccessAttempted - nextEventRequest):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::SaveInProgress& e) {
    MessageBox::Show("MSG-(SaveInProgress - nextEventRequest):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RestoreInProgress& e) {
    MessageBox::Show("MSG-(RestoreInProgress - nextEventRequest):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RTIinternalError& e) {
    MessageBox::Show("MSG-(RTIinternalError - nextEventRequest):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - nextEventRequest):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
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
    MessageBox::Show("MSG-(InvalidFederationTime - nextEventRequestAvailable):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::FederationTimeAlreadyPassed &e) {
    MessageBox::Show("MSG-(FederationTimeAlreadyPassed - nextEventRequestAvailable):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::TimeAdvanceAlreadyInProgress &e) {
    MessageBox::Show("MSG-(TimeAdvanceAlreadyInProgress - nextEventRequestAvailable):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::EnableTimeConstrainedPending &e) {
    MessageBox::Show("MSG-(EnableTimeConstrainedPending - nextEventRequestAvailable):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::EnableTimeRegulationPending &e) {
    MessageBox::Show("MSG-(EnableTimeRegulationPending - nextEventRequestAvailable):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::FederateNotExecutionMember& e) {
    MessageBox::Show("MSG-(FederateNotExecutionMember - nextEventRequestAvailable):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::ConcurrentAccessAttempted& e) {
    MessageBox::Show("MSG-(ConcurrentAccessAttempted - nextEventRequestAvailable):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::SaveInProgress& e) {
    MessageBox::Show("MSG-(SaveInProgress - nextEventRequestAvailable):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RestoreInProgress& e) {
    MessageBox::Show("MSG-(RestoreInProgress - nextEventRequestAvailable):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RTIinternalError& e) {
    MessageBox::Show("MSG-(RTIinternalError - nextEventRequestAvailable):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - nextEventRequestAvailable):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
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
    MessageBox::Show("MSG-(FederateNotExecutionMember - queryFederateTime):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::ConcurrentAccessAttempted& e) {
    MessageBox::Show("MSG-(ConcurrentAccessAttempted - queryFederateTime):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::SaveInProgress& e) {
    MessageBox::Show("MSG-(SaveInProgress - queryFederateTime):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RestoreInProgress& e) {
    MessageBox::Show("MSG-(RestoreInProgress - queryFederateTime):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RTIinternalError& e) {
    MessageBox::Show("MSG-(RTIinternalError - queryFederateTime):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - queryFederateTime):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
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
    MessageBox::Show("MSG-(FederateNotExecutionMember - queryLBTS):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::ConcurrentAccessAttempted& e) {
    MessageBox::Show("MSG-(ConcurrentAccessAttempted - queryLBTS):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::SaveInProgress& e) {
    MessageBox::Show("MSG-(SaveInProgress - queryLBTS):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RestoreInProgress& e) {
    MessageBox::Show("MSG-(RestoreInProgress - queryLBTS):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RTIinternalError& e) {
    MessageBox::Show("MSG-(RTIinternalError - queryLBTS):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - queryLBTS):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
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
    MessageBox::Show("MSG-(FederateNotExecutionMember - queryLookahead):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::ConcurrentAccessAttempted& e) {
    MessageBox::Show("MSG-(ConcurrentAccessAttempted - queryLookahead):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::SaveInProgress& e) {
    MessageBox::Show("MSG-(SaveInProgress - queryLookahead):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RestoreInProgress& e) {
    MessageBox::Show("MSG-(RestoreInProgress - queryLookahead):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RTIinternalError& e) {
    MessageBox::Show("MSG-(RTIinternalError - queryLookahead):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - queryLookahead):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
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
    MessageBox::Show("MSG-(FederateNotExecutionMember - queryMinNextEventTime):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::ConcurrentAccessAttempted& e) {
    MessageBox::Show("MSG-(ConcurrentAccessAttempted - queryMinNextEventTime):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::SaveInProgress& e) {
    MessageBox::Show("MSG-(SaveInProgress - queryMinNextEventTime):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RestoreInProgress& e) {
    MessageBox::Show("MSG-(RestoreInProgress - queryMinNextEventTime):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RTIinternalError& e) {
    MessageBox::Show("MSG-(RTIinternalError - queryMinNextEventTime):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - queryMinNextEventTime):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
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
    MessageBox::Show("MSG-(InvalidFederationTime - timeAdvanceRequest):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::FederationTimeAlreadyPassed &e) {
    MessageBox::Show("MSG-(FederationTimeAlreadyPassed - timeAdvanceRequest):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::TimeAdvanceAlreadyInProgress &e) {
    MessageBox::Show("MSG-(TimeAdvanceAlreadyInProgress - timeAdvanceRequest):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::EnableTimeConstrainedPending &e) {
    MessageBox::Show("MSG-(EnableTimeConstrainedPending - timeAdvanceRequest):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::EnableTimeRegulationPending &e) {
    MessageBox::Show("MSG-(EnableTimeRegulationPending - timeAdvanceRequest):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::FederateNotExecutionMember& e) {
    MessageBox::Show("MSG-(FederateNotExecutionMember - timeAdvanceRequest):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::ConcurrentAccessAttempted& e) {
    MessageBox::Show("MSG-(ConcurrentAccessAttempted - timeAdvanceRequest):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::SaveInProgress& e) {
    MessageBox::Show("MSG-(SaveInProgress - timeAdvanceRequest):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RestoreInProgress& e) {
    MessageBox::Show("MSG-(RestoreInProgress - timeAdvanceRequest):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RTIinternalError& e) {
    MessageBox::Show("MSG-(RTIinternalError - timeAdvanceRequest):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - timeAdvanceRequest):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
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
    MessageBox::Show("MSG-(InvalidFederationTime - timeAdvanceRequestAvailable):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::FederationTimeAlreadyPassed &e) {
    MessageBox::Show("MSG-(FederationTimeAlreadyPassed - timeAdvanceRequestAvailable):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::TimeAdvanceAlreadyInProgress &e) {
    MessageBox::Show("MSG-(TimeAdvanceAlreadyInProgress - timeAdvanceRequestAvailable):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::EnableTimeConstrainedPending &e) {
    MessageBox::Show("MSG-(EnableTimeConstrainedPending - timeAdvanceRequestAvailable):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::EnableTimeRegulationPending &e) {
    MessageBox::Show("MSG-(EnableTimeRegulationPending - timeAdvanceRequestAvailable):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::FederateNotExecutionMember& e) {
    MessageBox::Show("MSG-(FederateNotExecutionMember - timeAdvanceRequestAvailable):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::ConcurrentAccessAttempted& e) {
    MessageBox::Show("MSG-(ConcurrentAccessAttempted - timeAdvanceRequestAvailable):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::SaveInProgress& e) {
    MessageBox::Show("MSG-(SaveInProgress - timeAdvanceRequestAvailable):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RestoreInProgress& e) {
    MessageBox::Show("MSG-(RestoreInProgress - timeAdvanceRequestAvailable):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RTIinternalError& e) {
    MessageBox::Show("MSG-(RTIinternalError - timeAdvanceRequestAvailable):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - timeAdvanceRequestAvailable):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    return false;
  }
};

// Retract
bool RtiAmb_Hla13::retract(EventRetractionHandle ^retraction) {
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
    MessageBox::Show("MSG-(InvalidRetractionHandle - retract):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine + Environment::NewLine + "the event-retraction handle must be obtained from updateAttributeValues(), sendInteraction(), deleteObject()) of the event to unschedule", "RTIAmbassador - HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
    return false;
  }
  catch (RTI::FederateNotExecutionMember& e) {
    MessageBox::Show("MSG-(FederateNotExecutionMember - retract):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
    return false;
  }
  catch (RTI::ConcurrentAccessAttempted& e) {
    MessageBox::Show("MSG-(ConcurrentAccessAttempted - retract):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
    return false;
  }
  catch (RTI::SaveInProgress& e) {
    MessageBox::Show("MSG-(SaveInProgress - retract):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
    return false;
  }
  catch (RTI::RestoreInProgress& e) {
    MessageBox::Show("MSG-(RestoreInProgress - retract):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
    return false;
  }
  catch (RTI::RTIinternalError& e) {
    MessageBox::Show("MSG-(RTIinternalError - retract):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
    return false;
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - retract):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
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
    MessageBox::Show("MSG-(FederateNotExecutionMember - enableAttributeRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::ConcurrentAccessAttempted& e) {
    MessageBox::Show("MSG-(ConcurrentAccessAttempted - enableAttributeRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::SaveInProgress& e) {
    MessageBox::Show("MSG-(SaveInProgress - enableAttributeRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RestoreInProgress& e) {
    MessageBox::Show("MSG-(RestoreInProgress - enableAttributeRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RTIinternalError& e) {
    MessageBox::Show("MSG-(RTIinternalError - enableAttributeRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - enableAttributeRelevanceAdvisorySwitch):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
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
    MessageBox::Show("MSG-(FederateNotExecutionMember - enableAttributeScopeAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::ConcurrentAccessAttempted& e) {
    MessageBox::Show("MSG-(ConcurrentAccessAttempted - enableAttributeScopeAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::SaveInProgress& e) {
    MessageBox::Show("MSG-(SaveInProgress - enableAttributeScopeAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RestoreInProgress& e) {
    MessageBox::Show("MSG-(RestoreInProgress - enableAttributeScopeAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RTIinternalError& e) {
    MessageBox::Show("MSG-(RTIinternalError - enableAttributeScopeAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - enableAttributeScopeAdvisorySwitch):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
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
    MessageBox::Show("MSG-(FederateNotExecutionMember - enableInteractionRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::ConcurrentAccessAttempted& e) {
    MessageBox::Show("MSG-(ConcurrentAccessAttempted - enableInteractionRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::SaveInProgress& e) {
    MessageBox::Show("MSG-(SaveInProgress - enableInteractionRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RestoreInProgress& e) {
    MessageBox::Show("MSG-(RestoreInProgress - enableInteractionRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RTIinternalError& e) {
    String^ msg = "MSG-(RTIinternalError-enableInteractionRelevanceAdvisorySwitch-RtiAmb_Hla13):" + Environment::NewLine + " Reason: " + gcnew String(e._reason) + Environment::NewLine;
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - enableInteractionRelevanceAdvisorySwitch):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
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
    MessageBox::Show("MSG-(FederateNotExecutionMember - enableClassRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::ConcurrentAccessAttempted& e) {
    MessageBox::Show("MSG-(ConcurrentAccessAttempted - enableClassRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::SaveInProgress& e) {
    MessageBox::Show("MSG-(SaveInProgress - enableClassRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RestoreInProgress& e) {
    MessageBox::Show("MSG-(RestoreInProgress - enableClassRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RTIinternalError& e) {
    String^ msg = "MSG-(RTIinternalError-enableClassRelevanceAdvisorySwitch-RtiAmb_Hla13):" + Environment::NewLine + " Reason: " + gcnew String(e._reason) + Environment::NewLine;
    this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - enableClassRelevanceAdvisorySwitch):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
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
    MessageBox::Show("MSG-(FederateNotExecutionMember - disableAttributeRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::ConcurrentAccessAttempted& e) {
    MessageBox::Show("MSG-(ConcurrentAccessAttempted - disableAttributeRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::SaveInProgress& e) {
    MessageBox::Show("MSG-(SaveInProgress - disableAttributeRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RestoreInProgress& e) {
    MessageBox::Show("MSG-(RestoreInProgress - disableAttributeRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RTIinternalError& e) {
    MessageBox::Show("MSG-(RTIinternalError - disableAttributeRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - disableAttributeRelevanceAdvisorySwitch):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
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
    MessageBox::Show("MSG-(FederateNotExecutionMember - disableAttributeScopeAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::ConcurrentAccessAttempted& e) {
    MessageBox::Show("MSG-(ConcurrentAccessAttempted - disableAttributeScopeAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::SaveInProgress& e) {
    MessageBox::Show("MSG-(SaveInProgress - disableAttributeScopeAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RestoreInProgress& e) {
    MessageBox::Show("MSG-(RestoreInProgress - disableAttributeScopeAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RTIinternalError& e) {
    MessageBox::Show("MSG-(RTIinternalError - disableAttributeScopeAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - disableAttributeScopeAdvisorySwitch):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
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
    MessageBox::Show("MSG-(FederateNotExecutionMember - disableClassRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::ConcurrentAccessAttempted& e) {
    MessageBox::Show("MSG-(ConcurrentAccessAttempted - disableClassRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::SaveInProgress& e) {
    MessageBox::Show("MSG-(SaveInProgress - disableClassRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RestoreInProgress& e) {
    MessageBox::Show("MSG-(RestoreInProgress - disableClassRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RTIinternalError& e) {
    MessageBox::Show("MSG-(RTIinternalError - disableClassRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - disableClassRelevanceAdvisorySwitch):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
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
    MessageBox::Show("MSG-(FederateNotExecutionMember - disableInteractionRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::ConcurrentAccessAttempted& e) {
    MessageBox::Show("MSG-(ConcurrentAccessAttempted - disableInteractionRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::SaveInProgress& e) {
    MessageBox::Show("MSG-(SaveInProgress - disableInteractionRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RestoreInProgress& e) {
    MessageBox::Show("MSG-(RestoreInProgress - disableInteractionRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RTIinternalError& e) {
    MessageBox::Show("MSG-(RTIinternalError - disableInteractionRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - disableInteractionRelevanceAdvisorySwitch):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
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
    MessageBox::Show("MSG-(NameNotFound - getClassHandleFromRti (Object Class)):" + Environment::NewLine + " Reason: " + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - getClassHandleFromRti-1):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
};

// getClassHandleFromRti - IC
void RtiAmb_Hla13::getClassHandleFromRti(HlaInteractionClass ^ic) {
  try {
    // Get Interaction ClassHandle
    ic->Handle = rti->getInteractionClassHandle((const char*)(Marshal::StringToHGlobalAnsi(ic->Name)).ToPointer());
  }
  catch (RTI::NameNotFound& e) {
    MessageBox::Show("MSG-(NameNotFound - getClassHandleFromRti-2 (Interaction Class)):" + Environment::NewLine + " Reason: " + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - getClassHandleFromRti-2):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
};

// getAttributeHandle
Handle RtiAmb_Hla13::getAttributeHandle(HlaAttribute^ attr, HlaObjectClass ^oc) {
  try {
    return rti->getAttributeHandle((const char*)(Marshal::StringToHGlobalAnsi(attr->Name)).ToPointer(), oc->Handle);
  }
#pragma region exceptions
  catch (RTI::ObjectClassNotDefined &e) {
    MessageBox::Show("MSG-(ObjectClassNotDefined - getAttributeHandle):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::NameNotFound &e) {
    MessageBox::Show("MSG-(NameNotFound - getAttributeHandle):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::FederateNotExecutionMember& e) {
    MessageBox::Show("MSG-(FederateNotExecutionMember - getAttributeHandle):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::ConcurrentAccessAttempted& e) {
    MessageBox::Show("MSG-(ConcurrentAccessAttempted - getAttributeHandle):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RTIinternalError& e) {
    MessageBox::Show("MSG-(RTIinternalError - getAttributeHandle):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - getAttributeHandle):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
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
    MessageBox::Show("MSG-(NameNotFound - getAttributeHandlesFromRti):" + Environment::NewLine + " Reason: " + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - getAttributeHandlesFromRti):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
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
    MessageBox::Show("MSG-(NameNotFound - getParameterHandlesFromRti):" + Environment::NewLine + " Reason: " + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - getParameterHandlesFromRti):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
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
    MessageBox::Show("MSG-(SpaceNotDefined - getDimensionHandle):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::NameNotFound &e) {
    MessageBox::Show("MSG-(NameNotFound - getDimensionHandle):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::FederateNotExecutionMember& e) {
    MessageBox::Show("MSG-(FederateNotExecutionMember - getDimensionHandle):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::ConcurrentAccessAttempted& e) {
    MessageBox::Show("MSG-(ConcurrentAccessAttempted - getDimensionHandle):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (RTI::RTIinternalError& e) {
    MessageBox::Show("MSG-(RTIinternalError - getDimensionHandle):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - getDimensionHandle):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
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
    MessageBox::Show("MSG-(NameNotFound - getSpaceHandle):" + Environment::NewLine + " Reason: " + gcnew String(e._reason) + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
  }
  catch (Exception^ e) {
    MessageBox::Show("MSG-(GeneralException - getSpaceHandle):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Error);
  }
};

// Tick - zero argument
void RtiAmb_Hla13::evokeCallback(double approximateMinimumTimeInSeconds) {
	try {
		rti->tick();// The Zero argument tick() always return FALSE
	}
#pragma region exceptions
	catch (Exception^ e) {
		MessageBox::Show("MSG-(GeneralException - evokeCallback):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
#pragma endregion
};
// Tick - multiple argument
void RtiAmb_Hla13::evokeMultipleCallbacks(double approximateMinimumTimeInSeconds, double approximateMaximumTimeInSeconds) {
	try {
		rti->tick(approximateMinimumTimeInSeconds, approximateMaximumTimeInSeconds);
	}
#pragma region exceptions
	catch (Exception^ e) {
		MessageBox::Show("MSG-(GeneralException - evokeMultipleCallbacks):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RTIAmbassador-HLA13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
	}
#pragma endregion
};

#pragma endregion // Support Services			
