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

// Managed Wrapper Class for HLA13 - Portico
#pragma once
#include "RtiAmb_Hla13.h"

namespace Racon
{
	namespace RtiLayer {
		namespace Native {
			public ref class RtiAmb_Portico_Hla13 : public RtiAmb_Hla13 {

#pragma region Fields
			protected:
#pragma endregion			

#pragma region Ctor/Dtor
			public:
				RtiAmb_Portico_Hla13(CallbackManager^ eventManager) : RtiAmb_Hla13(eventManager)
				{
					RtiVersion = "Portico v2.1.0 win32";
				};

				~RtiAmb_Portico_Hla13()
				{
					rti->~RTIambassador();
					_nativeFdAmb->~FdAmb_Hla13();
				};
#pragma endregion						

#pragma region Methods
#pragma region Fedaration Management
			public:
#pragma endregion			

#pragma region Declaration Management
			public:
#pragma endregion			

#pragma region Object Management
			public:
#pragma endregion	// Object Management

#pragma region Ownership Management
			public:
#pragma endregion // Ownership Management

#pragma region Data Distribution Management
			public:
#pragma endregion	// Data Distribution Management

#pragma region Time Management
			public:
#pragma endregion

#pragma region Support Services
			public:
				// Not Supported by Portico !!! Check
			 // enableAttributeRelevanceAdvisorySwitch
				bool enableAttributeRelevanceAdvisorySwitch(void) override
				{// Not Supported in Portico 0.8
					try {
						rti->enableAttributeRelevanceAdvisorySwitch();
						String^ msg = "enableAttributeRelevanceAdvisorySwitch is not implemented in Portico.";
						this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
						return false;
					}
					catch (RTI::FederateNotExecutionMember& e) {
						String^ msg = "RtiAmb_Portico_Hla13-(FederateNotExecutionMember - enableAttributeRelevanceAdvisorySwitch). Reason: " + gcnew String(e._reason);
						this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
					}
					catch (RTI::ConcurrentAccessAttempted& e) {
						String^ msg = "RtiAmb_Portico_Hla13-(FederateNotExecutionMember - enableAttributeRelevanceAdvisorySwitch). Reason: " + gcnew String(e._reason);
						this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
					}
					catch (RTI::SaveInProgress& e) {
						String^ msg = "RtiAmb_Portico_Hla13-(FederateNotExecutionMember - enableAttributeRelevanceAdvisorySwitch). Reason: " + gcnew String(e._reason);
						this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
					}
					catch (RTI::RestoreInProgress& e) {
						String^ msg = "RtiAmb_Portico_Hla13-(FederateNotExecutionMember - enableAttributeRelevanceAdvisorySwitch). Reason: " + gcnew String(e._reason);
						this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
					}
					catch (RTI::RTIinternalError& e) {
						String^ msg = "RtiAmb_Portico_Hla13-(RTI::RTIinternalError - connect). Reason: " + gcnew String(e._reason);
						this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
					}
					catch (System::Exception^ e) {
						String^ msg = "RtiAmb_Portico_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
						this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
						return false;
					}
				};

				// enableAttributeRelevanceAdvisorySwitch
				bool enableAttributeScopeAdvisorySwitch(void)override {
					try {
						rti->enableAttributeScopeAdvisorySwitch();
						String^ msg = "Attribute Scope Advisory Switch is enabled.";
						this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
						return true;
					}
					catch (RTI::FederateNotExecutionMember& e) {
						String^ msg = "RtiAmb_Portico_Hla13-(FederateNotExecutionMember - enableAttributeRelevanceAdvisorySwitch). Reason: " + gcnew String(e._reason);
						this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
					}
					catch (RTI::ConcurrentAccessAttempted& e) {
						String^ msg = "RtiAmb_Portico_Hla13-(FederateNotExecutionMember - enableAttributeRelevanceAdvisorySwitch). Reason: " + gcnew String(e._reason);
						this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
					}
					catch (RTI::SaveInProgress& e) {
						String^ msg = "RtiAmb_Portico_Hla13-(FederateNotExecutionMember - enableAttributeRelevanceAdvisorySwitch). Reason: " + gcnew String(e._reason);
						this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
					}
					catch (RTI::RestoreInProgress& e) {
						String^ msg = "RtiAmb_Portico_Hla13-(FederateNotExecutionMember - enableAttributeRelevanceAdvisorySwitch). Reason: " + gcnew String(e._reason);
						this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
					}
					catch (RTI::RTIinternalError& e) {
						String^ msg = "RtiAmb_Portico_Hla13-(RTI::RTIinternalError - connect). Reason: " + gcnew String(e._reason);
						this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
					}
					catch (System::Exception^ e) {
						String^ msg = "RtiAmb_Portico_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
						this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
						return false;
					}
				};

				// enableInteractionRelevanceAdvisorySwitch
				bool enableInteractionRelevanceAdvisorySwitch(void) override {
					try {
						rti->enableInteractionRelevanceAdvisorySwitch();
						String^ msg = "Interaction Relevance Advisory Switch is enabled.";
						this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
						return true;
					}
					catch (RTI::FederateNotExecutionMember& e) {
						String^ msg = "RtiAmb_Portico_Hla13-(FederateNotExecutionMember - enableAttributeRelevanceAdvisorySwitch). Reason: " + gcnew String(e._reason);
						this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
					}
					catch (RTI::ConcurrentAccessAttempted& e) {
						String^ msg = "RtiAmb_Portico_Hla13-(FederateNotExecutionMember - enableAttributeRelevanceAdvisorySwitch). Reason: " + gcnew String(e._reason);
						this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
					}
					catch (RTI::SaveInProgress& e) {
						String^ msg = "RtiAmb_Portico_Hla13-(FederateNotExecutionMember - enableAttributeRelevanceAdvisorySwitch). Reason: " + gcnew String(e._reason);
						this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
					}
					catch (RTI::RestoreInProgress& e) {
						String^ msg = "RtiAmb_Portico_Hla13-(FederateNotExecutionMember - enableAttributeRelevanceAdvisorySwitch). Reason: " + gcnew String(e._reason);
						this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
					}
					catch (RTI::RTIinternalError& e) {
						String^ msg = "RtiAmb_Portico_Hla13-(RTI::RTIinternalError - connect). Reason: " + gcnew String(e._reason);
						this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
					}
					catch (System::Exception^ e) {
						String^ msg = "RtiAmb_Portico_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
						this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
						return false;
					}
				};

				// enableClassRelevanceAdvisorySwitch
				bool enableClassRelevanceAdvisorySwitch() override {
					try {
						// Relevance advising is enabled by default
						rti->enableClassRelevanceAdvisorySwitch();
						String^ msg = "Object Class Relevance Advisory Switch is enabled.";
						this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
						return true;
					}
					catch (RTI::FederateNotExecutionMember& e) {
						String^ msg = "RtiAmb_Portico_Hla13-(FederateNotExecutionMember - enableAttributeRelevanceAdvisorySwitch). Reason: " + gcnew String(e._reason);
						this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
					}
					catch (RTI::ConcurrentAccessAttempted& e) {
						String^ msg = "RtiAmb_Portico_Hla13-(FederateNotExecutionMember - enableAttributeRelevanceAdvisorySwitch). Reason: " + gcnew String(e._reason);
						this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
					}
					catch (RTI::SaveInProgress& e) {
						String^ msg = "RtiAmb_Portico_Hla13-(FederateNotExecutionMember - enableAttributeRelevanceAdvisorySwitch). Reason: " + gcnew String(e._reason);
						this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
					}
					catch (RTI::RestoreInProgress& e) {
						String^ msg = "RtiAmb_Portico_Hla13-(FederateNotExecutionMember - enableAttributeRelevanceAdvisorySwitch). Reason: " + gcnew String(e._reason);
						this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
					}
					catch (RTI::RTIinternalError& e) {
						String^ msg = "RtiAmb_Portico_Hla13-(RTI::RTIinternalError - connect). Reason: " + gcnew String(e._reason);
						this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
					}
					catch (System::Exception^ e) {
						String^ msg = "RtiAmb_Portico_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
						this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
						return false;
					}
				};

				// disableAttributeRelevanceAdvisorySwitch
				bool disableAttributeRelevanceAdvisorySwitch() override {
					try {
						rti->disableAttributeRelevanceAdvisorySwitch();
						String^ msg = "Attribute Relevance Advisory Switch is disabled.";
						this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
						return true;
					}
					catch (RTI::FederateNotExecutionMember& e) {
						String^ msg = "RtiAmb_Portico_Hla13-(FederateNotExecutionMember - enableAttributeRelevanceAdvisorySwitch). Reason: " + gcnew String(e._reason);
						this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
					}
					catch (RTI::ConcurrentAccessAttempted& e) {
						String^ msg = "RtiAmb_Portico_Hla13-(FederateNotExecutionMember - enableAttributeRelevanceAdvisorySwitch). Reason: " + gcnew String(e._reason);
						this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
					}
					catch (RTI::SaveInProgress& e) {
						String^ msg = "RtiAmb_Portico_Hla13-(FederateNotExecutionMember - enableAttributeRelevanceAdvisorySwitch). Reason: " + gcnew String(e._reason);
						this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
					}
					catch (RTI::RestoreInProgress& e) {
						String^ msg = "RtiAmb_Portico_Hla13-(FederateNotExecutionMember - enableAttributeRelevanceAdvisorySwitch). Reason: " + gcnew String(e._reason);
						this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
					}
					catch (RTI::RTIinternalError& e) {
						String^ msg = "RtiAmb_Portico_Hla13-(RTI::RTIinternalError - connect). Reason: " + gcnew String(e._reason);
						this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
					}
					catch (System::Exception^ e) {
						String^ msg = "RtiAmb_Portico_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
						this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
						return false;
					}
				};

				// disableAttributeScopeAdvisorySwitch
				bool disableAttributeScopeAdvisorySwitch() override {
					try {
						rti->disableAttributeScopeAdvisorySwitch();
						String^ msg = "Attribute Scope Advisory Switch is disabled.";
						this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
						return true;
					}
					catch (RTI::FederateNotExecutionMember& e) {
						String^ msg = "RtiAmb_Portico_Hla13-(FederateNotExecutionMember - enableAttributeRelevanceAdvisorySwitch). Reason: " + gcnew String(e._reason);
						this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
					}
					catch (RTI::ConcurrentAccessAttempted& e) {
						String^ msg = "RtiAmb_Portico_Hla13-(FederateNotExecutionMember - enableAttributeRelevanceAdvisorySwitch). Reason: " + gcnew String(e._reason);
						this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
					}
					catch (RTI::SaveInProgress& e) {
						String^ msg = "RtiAmb_Portico_Hla13-(FederateNotExecutionMember - enableAttributeRelevanceAdvisorySwitch). Reason: " + gcnew String(e._reason);
						this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
					}
					catch (RTI::RestoreInProgress& e) {
						String^ msg = "RtiAmb_Portico_Hla13-(FederateNotExecutionMember - enableAttributeRelevanceAdvisorySwitch). Reason: " + gcnew String(e._reason);
						this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
					}
					catch (RTI::RTIinternalError& e) {
						String^ msg = "RtiAmb_Portico_Hla13-(RTI::RTIinternalError - connect). Reason: " + gcnew String(e._reason);
						this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
					}
					catch (System::Exception^ e) {
						String^ msg = "RtiAmb_Portico_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
						this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
						return false;
					}
				};

				// disableClassRelevanceAdvisorySwitch
				bool disableClassRelevanceAdvisorySwitch() override {
					try {
						rti->disableClassRelevanceAdvisorySwitch();
						String^ msg = "Object Class Relevance Advisory Switch is disabled.";
						this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
						return true;
					}
					catch (RTI::FederateNotExecutionMember& e) {
						String^ msg = "RtiAmb_Portico_Hla13-(FederateNotExecutionMember - enableAttributeRelevanceAdvisorySwitch). Reason: " + gcnew String(e._reason);
						this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
					}
					catch (RTI::ConcurrentAccessAttempted& e) {
						String^ msg = "RtiAmb_Portico_Hla13-(FederateNotExecutionMember - enableAttributeRelevanceAdvisorySwitch). Reason: " + gcnew String(e._reason);
						this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
					}
					catch (RTI::SaveInProgress& e) {
						String^ msg = "RtiAmb_Portico_Hla13-(FederateNotExecutionMember - enableAttributeRelevanceAdvisorySwitch). Reason: " + gcnew String(e._reason);
						this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
					}
					catch (RTI::RestoreInProgress& e) {
						String^ msg = "RtiAmb_Portico_Hla13-(FederateNotExecutionMember - enableAttributeRelevanceAdvisorySwitch). Reason: " + gcnew String(e._reason);
						this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
					}
					catch (RTI::RTIinternalError& e) {
						String^ msg = "RtiAmb_Portico_Hla13-(RTI::RTIinternalError - connect). Reason: " + gcnew String(e._reason);
						this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
					}
					catch (System::Exception^ e) {
						String^ msg = "RtiAmb_Portico_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
						this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
						return false;
					}
				};

				// disableInteractionRelevanceAdvisorySwitch
				bool disableInteractionRelevanceAdvisorySwitch() override {
					try {
						rti->disableInteractionRelevanceAdvisorySwitch();
						String^ msg = "Interaction Relevance Advisory Switch is disabled.";
						this->OnRTIEventOccured(gcnew RaconEventArgs(msg));
						return true;
					}
					catch (RTI::FederateNotExecutionMember& e) {
						String^ msg = "RtiAmb_Portico_Hla13-(FederateNotExecutionMember - enableAttributeRelevanceAdvisorySwitch). Reason: " + gcnew String(e._reason);
						this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
					}
					catch (RTI::ConcurrentAccessAttempted& e) {
						String^ msg = "RtiAmb_Portico_Hla13-(FederateNotExecutionMember - enableAttributeRelevanceAdvisorySwitch). Reason: " + gcnew String(e._reason);
						this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
					}
					catch (RTI::SaveInProgress& e) {
						String^ msg = "RtiAmb_Portico_Hla13-(FederateNotExecutionMember - enableAttributeRelevanceAdvisorySwitch). Reason: " + gcnew String(e._reason);
						this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
					}
					catch (RTI::RestoreInProgress& e) {
						String^ msg = "RtiAmb_Portico_Hla13-(FederateNotExecutionMember - enableAttributeRelevanceAdvisorySwitch). Reason: " + gcnew String(e._reason);
						this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::WARN));
					}
					catch (RTI::RTIinternalError& e) {
						String^ msg = "RtiAmb_Portico_Hla13-(RTI::RTIinternalError - connect). Reason: " + gcnew String(e._reason);
						this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
					}
					catch (System::Exception^ e) {
						String^ msg = "RtiAmb_Portico_Hla13-(Exception - connect). Reason: " + gcnew String(e->ToString());
						this->OnRTIEventOccured(gcnew RaconEventArgs(msg, LogLevel::ERROR));
						return false;
					}
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

