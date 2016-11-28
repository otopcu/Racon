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

#pragma once
#include <iostream> // for using std

using namespace System;
using namespace System::Threading;
using namespace System::Runtime::Remoting::Messaging;
using namespace System::Collections::Generic;
using namespace Racon::RtiLayer;

namespace Racon
{
	namespace RtiLayer {
		public ref class CallbackManager
		{
			// Constructor
		public:
			CallbackManager();
			// Custom Event Handlers - Delegates (function pointers)
#pragma region Delegates
		public:
			//delegate void ObjectDiscoveredHandler(System::Object ^ sender, CHlaObjectClassEventArgs^ fe);
#pragma endregion	// Delegates

#pragma region Events
		public:
			// Events related to the RTI-initiated services
			// FM Events
			virtual event EventHandler<HlaFederationManagementEventArgs^>^ InitiateFederateSave;
			virtual event EventHandler<HlaFederationManagementEventArgs^>^ InitiateFederateRestore;
			virtual event EventHandler<HlaFederationManagementEventArgs^>^ FederationRestorationRequestConfirmed;
			virtual event EventHandler<HlaFederationManagementEventArgs^>^ FederationSaved;
			virtual event EventHandler<HlaFederationManagementEventArgs^>^ FederationRestored;
			virtual event EventHandler<HlaFederationManagementEventArgs^>^ FederationRestoreBegun;
			// DM Events
			virtual event EventHandler<HlaDeclarationManagementEventArgs^>^ StartRegistrationForObjectClassAdvised;
			virtual event EventHandler<HlaDeclarationManagementEventArgs^>^ StopRegistrationForObjectClassAdvised;
			virtual event EventHandler<HlaDeclarationManagementEventArgs^>^ TurnInteractionsOffAdvised;
			virtual event EventHandler<HlaDeclarationManagementEventArgs^>^ TurnInteractionsOnAdvised;
			// OM Events
			virtual event EventHandler<HlaObjectEventArgs^>^ ObjectDiscovered;
			virtual event EventHandler<HlaInteractionEventArgs^>^ InteractionReceived;
			virtual event EventHandler<HlaObjectEventArgs^>^ AttributeValueUpdateRequested;
			virtual event EventHandler<HlaObjectEventArgs^>^ ObjectRemoved;
			virtual event EventHandler<HlaObjectEventArgs^>^ ObjectAttributesReflected;
			virtual event EventHandler<HlaObjectEventArgs^>^ TurnUpdatesOnForObjectInstanceAdvised;
			virtual event EventHandler<HlaObjectEventArgs^>^ TurnUpdatesOffForObjectInstanceAdvised;
			// OwM Events
			virtual event EventHandler<HlaOwnershipManagementEventArgs^>^ AttributeOwnershipReleaseRequested;
			virtual event EventHandler<HlaOwnershipManagementEventArgs^>^ AttributeOwnershipInformed;
			virtual event EventHandler<HlaOwnershipManagementEventArgs^>^ AttributeOwnershipAcquisitionNotified;
			virtual event EventHandler<HlaOwnershipManagementEventArgs^>^ AttributeOwnershipDivestitureNotified;
			virtual event EventHandler<HlaOwnershipManagementEventArgs^>^ AttributeOwnershipUnavailable;
			virtual event EventHandler<HlaOwnershipManagementEventArgs^>^ AttributeOwnershipAcquisitionCancellationConfirmed;
			virtual event EventHandler<HlaOwnershipManagementEventArgs^>^ AttributeOwnershipAssumptionRequested;
			// TM Events
			virtual event EventHandler<HlaTimeManagementEventArgs^>^ TimeConstrainedEnabled;
			virtual event EventHandler<HlaTimeManagementEventArgs^>^ TimeRegulationEnabled;
			virtual event EventHandler<HlaTimeManagementEventArgs^>^ TimeAdvanceGrant;
			virtual event EventHandler<HlaTimeManagementEventArgs^>^ RequestRetraction;

			// Event Queues
		public: // Protected olmali
			Queue<RaconEventArgs^>^ FdAmbEventQueue;
#pragma endregion // Events

#pragma region Triggers
		protected:
			// FM
			virtual void OnInitiateFederateSave(HlaFederationManagementEventArgs^);
			virtual void OnInitiateFederateRestore(HlaFederationManagementEventArgs^);
			virtual void OnAcceptanceOfRequestFederationRestore(HlaFederationManagementEventArgs^);
			virtual void OnFederationSaved(HlaFederationManagementEventArgs^);
			virtual void OnFederationRestored(HlaFederationManagementEventArgs^);
			virtual void OnFederationRestoreBegun(HlaFederationManagementEventArgs^);
			// DM
			virtual void OnStartRegistrationForObjectClassAdvised(HlaDeclarationManagementEventArgs^);
			virtual void OnStopRegistrationForObjectClassAdvised(HlaDeclarationManagementEventArgs^);
			virtual void OnTurnInteractionsOnAdvised(HlaDeclarationManagementEventArgs^);
			virtual void OnTurnInteractionsOffAdvised(HlaDeclarationManagementEventArgs^);
			// OM
			virtual void OnObjectDiscovered(HlaObjectEventArgs^);
			virtual void OnInteractionReceived(HlaInteractionEventArgs^);
			virtual void OnAttributeValueUpdateRequested(HlaObjectEventArgs^);
			virtual void OnObjectRemoved(HlaObjectEventArgs^);
			virtual void OnObjectAttributesReflected(HlaObjectEventArgs^);
			virtual void OnTurnUpdatesOnForObjectInstanceAdvised(HlaObjectEventArgs^);
			virtual void OnTurnUpdatesOffForObjectInstanceAdvised(HlaObjectEventArgs^);
			// OwM
			virtual void OnAttributeOwnershipReleaseRequested(HlaOwnershipManagementEventArgs^);
			virtual void OnAttributeOwnershipInformed(HlaOwnershipManagementEventArgs^);
			virtual void OnAttributeOwnershipAcquisitionNotified(HlaOwnershipManagementEventArgs^);
			virtual void OnAttributeOwnershipDivestitureNotified(HlaOwnershipManagementEventArgs^);
			virtual void OnAttributeOwnershipUnavailable(HlaOwnershipManagementEventArgs^);
			virtual void OnAttributeOwnershipAcquisitionCancellationConfirmed(HlaOwnershipManagementEventArgs^);
			virtual void OnAttributeOwnershipAssumptionRequested(HlaOwnershipManagementEventArgs^);
			// TM
			virtual void OnTimeConstrainedEnabled(HlaTimeManagementEventArgs^);
			virtual void OnTimeRegulationEnabled(HlaTimeManagementEventArgs^);
			virtual void OnTimeAdvanceGrant(HlaTimeManagementEventArgs^);
			virtual void OnRequestRetraction(HlaTimeManagementEventArgs^);

#pragma endregion // Event Raisers-Triggers 

#pragma region Methods
		public:
			// Use function "FreeIntptrMemory(Intptr)" to free the buffer
			void FreeIntPtrMemory(IntPtr ptr) {
				try
				{
					void *p = (void*)ptr.ToPointer();
					free(p);
				}
				catch (Exception^ e)
				{
					Console::WriteLine("CallbackManager-FreeIntPtrMemory():" + e->Message);
				}
				catch (std::exception)
				{
					Console::WriteLine("CallbackManager-FreeIntPtrMemory(): std:exception");
				}
			}
		private:
			//[OneWay] void raiseEventObjectDiscovered(CHlaObjectClassEventArgs ^);
#pragma endregion

			// Async Calls - The delegate must have the same signature as the method it will call asynchronously.
			//delegate void AsyncMethodCallerOD(CHlaObjectClassEventArgs ^);
		};
	}
}

