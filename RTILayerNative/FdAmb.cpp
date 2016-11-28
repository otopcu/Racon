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

#include "stdafx.h"
#include "CallbackManager.h"

// Constructor
CallbackManager::CallbackManager()
{
//#if defined(HLA13)
//	_nativeFdAmb = new FdAmb_Hla13(this);
//#elif defined(HLA1516_2010)
//	_nativeFdAmb = new FdAmb_Hla13(this);
//#endif
	// Create FdAmb Event Queue
	FdAmbEventQueue = gcnew Queue<RaconEventArgs^>();
}

// OnInitiateFederateSave
void CallbackManager::OnInitiateFederateSave(Racon::RtiLayer::HlaFederationManagementEventArgs^ e)	{
  this->InitiateFederateSave(this, e);// Raise the event. c++/cli checks it is null or not.
};
// OnInitiateFederateRestore
void CallbackManager::OnInitiateFederateRestore(HlaFederationManagementEventArgs^ e)	{
  this->InitiateFederateRestore(this, e);// Raise the event. c++/cli checks it is null or not.
};
// OnAcceptanceOfRequestFederationRestore
void CallbackManager::OnAcceptanceOfRequestFederationRestore(HlaFederationManagementEventArgs^ e)	{
  this->FederationRestorationRequestConfirmed(this, e);
};
// OnFederationSaved
void CallbackManager::OnFederationSaved(HlaFederationManagementEventArgs^ e)	{
  this->FederationSaved(this, e);
};
// OnFederationRestored
void CallbackManager::OnFederationRestored(HlaFederationManagementEventArgs^ e)	{
  this->FederationRestored(this, e);
};
// OnFederationRestoreBegun
void CallbackManager::OnFederationRestoreBegun(HlaFederationManagementEventArgs^ e)	{
  this->FederationRestoreBegun(this, e);
};
// OnStartRegistrationForObjectClassAdvised
void CallbackManager::OnStartRegistrationForObjectClassAdvised(HlaDeclarationManagementEventArgs^ e)	{
	this->StartRegistrationForObjectClassAdvised(this, e);
};
// OnStopRegistrationForObjectClassAdvised
void CallbackManager::OnStopRegistrationForObjectClassAdvised(HlaDeclarationManagementEventArgs^ e)	{
	this->StopRegistrationForObjectClassAdvised(this, e);
};
// OnTurnInteractionsOffAdvised
void CallbackManager::OnTurnInteractionsOffAdvised(HlaDeclarationManagementEventArgs^ e)	{
	this->TurnInteractionsOffAdvised(this, e);
};
// OnTurnInteractionsOnAdvised
void CallbackManager::OnTurnInteractionsOnAdvised(HlaDeclarationManagementEventArgs^ e)	{
	this->TurnInteractionsOnAdvised(this, e);
};
// Object Discovered
void CallbackManager::OnObjectDiscovered(HlaObjectEventArgs^ e)	{
  this->ObjectDiscovered(this, e);
};
// Object Removed
void CallbackManager::OnObjectRemoved(HlaObjectEventArgs^ e)	{
  this->ObjectRemoved(this, e);
};
// Object Attributes Reflected
void CallbackManager::OnObjectAttributesReflected(HlaObjectEventArgs^ e)	
{
  this->ObjectAttributesReflected(this, e);
};
// Interaction Received
void CallbackManager::OnInteractionReceived(HlaInteractionEventArgs^ e)
{
  this->InteractionReceived(this, e);
};
// Attribute Value Update Requested
void CallbackManager::OnAttributeValueUpdateRequested(HlaObjectEventArgs^ e)	
{
  this->AttributeValueUpdateRequested(this, e);
}
// TurnUpdatesOnForObjectInstanceAdvised
void CallbackManager::OnTurnUpdatesOnForObjectInstanceAdvised(HlaObjectEventArgs^ e)
{
	this->TurnUpdatesOnForObjectInstanceAdvised(this, e);
}
// TurnUpdatesOffForObjectInstanceAdvised
void CallbackManager::OnTurnUpdatesOffForObjectInstanceAdvised(HlaObjectEventArgs^ e)
{
	this->TurnUpdatesOffForObjectInstanceAdvised(this, e);
}
// OnAttributeOwnershipReleaseRequested
void CallbackManager::OnAttributeOwnershipReleaseRequested(HlaOwnershipManagementEventArgs^ e)	
{
  this->AttributeOwnershipReleaseRequested(this, e);
}
// OnAttributeOwnershipInformed
void CallbackManager::OnAttributeOwnershipInformed(HlaOwnershipManagementEventArgs^ e)	
{
  this->AttributeOwnershipInformed(this, e);
}
// OnAttributeOwnershipAcquisitionNotified
void CallbackManager::OnAttributeOwnershipAcquisitionNotified(HlaOwnershipManagementEventArgs^ e)	
{
  this->AttributeOwnershipAcquisitionNotified(this, e);
}
// OnAttributeOwnershipDivestitureNotified
void CallbackManager::OnAttributeOwnershipDivestitureNotified(HlaOwnershipManagementEventArgs^ e)	
{
  this->AttributeOwnershipDivestitureNotified(this, e);
}
// OnAttributeOwnershipUnavailable
void CallbackManager::OnAttributeOwnershipUnavailable(HlaOwnershipManagementEventArgs^ e)	
{
  this->AttributeOwnershipUnavailable(this, e);
}
// OnAttributeOwnershipAcquisitionCancellationConfirmed
void CallbackManager::OnAttributeOwnershipAcquisitionCancellationConfirmed(HlaOwnershipManagementEventArgs^ e)	
{
  this->AttributeOwnershipAcquisitionCancellationConfirmed(this, e);
}
// OnAttributeOwnershipAssumptionRequested
void CallbackManager::OnAttributeOwnershipAssumptionRequested(HlaOwnershipManagementEventArgs^ e)	
{
  this->AttributeOwnershipAssumptionRequested(this, e);
}
// OnTimeConstrainedEnabled
void CallbackManager::OnTimeConstrainedEnabled(HlaTimeManagementEventArgs^ e)
{
	this->TimeConstrainedEnabled(this, e);
}
// OnTimeRegulationEnabled
void CallbackManager::OnTimeRegulationEnabled(HlaTimeManagementEventArgs^ e)
{
	this->TimeRegulationEnabled(this, e);
}
// OnTimeAdvanceGrant
void CallbackManager::OnTimeAdvanceGrant(HlaTimeManagementEventArgs^ e)
{
	this->TimeAdvanceGrant(this, e);
}
// OnRequestRetraction
void CallbackManager::OnRequestRetraction(HlaTimeManagementEventArgs^ e)
{
	this->RequestRetraction(this, e);
}

//// Interaction Received
//void CallbackManager::OnInteractionReceived(RTI::InteractionClassHandle theInteraction, const RTI::ParameterHandleValuePairSet& theParameters)	
//{
//	try{
//		// Create Event Arguments
//		CHlaInteractionEventArgs ^eArgs = gcnew CHlaInteractionEventArgs();
//		eArgs->InteractionClassHandle = theInteraction;
//
//		RTI::ULong valueLength;
//		for (unsigned int i=0; i < theParameters.size(); i++){
//			// Create a buffer that has the capacity 2048 bytes) for attributes.
//			char *buffer = (char*)malloc(theParameters.size()*sizeof(char[2048]));
//			theParameters.getValue(i, (char*) &buffer[i], valueLength);
//			IntPtr p = (IntPtr)&buffer[i];
//			//Console::WriteLine("(R)p:"+ k->ToString());
//			//Console::WriteLine("(R)p(adr):"+p.ToString());
//			// Pass the value 
//			eArgs->ParameterValues->Add(theParameters.getHandle(i), p);
//		}
//		this->InteractionReceived(this, eArgs);
//	}
//	catch (Exception^ e){
//		MessageBox::Show("MSG-(GeneralException - OnInteractionReceived):" + Environment::NewLine + e->ToString() + Environment::NewLine, "FdAmb", MessageBoxButtons::OK, MessageBoxIcon::Error);
//	}
//};
