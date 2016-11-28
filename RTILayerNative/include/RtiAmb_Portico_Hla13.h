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

// Managed Wrapper Class for HLA13 - Portico
#pragma once
#include "RtiAmb_Hla13.h"
using namespace System::Windows::Forms;

namespace Racon
{
	namespace RtiLayer {
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
					MessageBox::Show("MSG-(FederateNotExecutionMember - enableAttributeRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RtiAmb_Portico_Hla13", MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
				catch (RTI::ConcurrentAccessAttempted& e) {
					MessageBox::Show("MSG-(ConcurrentAccessAttempted - enableAttributeRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RtiAmb_Portico_Hla13", MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
				catch (RTI::SaveInProgress& e) {
					MessageBox::Show("MSG-(SaveInProgress - enableAttributeRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RtiAmb_Portico_Hla13", MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
				catch (RTI::RestoreInProgress& e) {
					MessageBox::Show("MSG-(RestoreInProgress - enableAttributeRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RtiAmb_Portico_Hla13", MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
				catch (RTI::RTIinternalError& e) {
					MessageBox::Show("MSG-(RTIinternalError - enableAttributeRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RtiAmb_Portico_Hla13", MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
				catch (System::Exception^ e) {
					MessageBox::Show("MSG-(GeneralException - enableAttributeRelevanceAdvisorySwitch):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RtiAmb_Portico_Hla13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
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
					MessageBox::Show("MSG-(FederateNotExecutionMember - enableAttributeScopeAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RtiAmb_Portico_Hla13", MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
				catch (RTI::ConcurrentAccessAttempted& e) {
					MessageBox::Show("MSG-(ConcurrentAccessAttempted - enableAttributeScopeAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RtiAmb_Portico_Hla13", MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
				catch (RTI::SaveInProgress& e) {
					MessageBox::Show("MSG-(SaveInProgress - enableAttributeScopeAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RtiAmb_Portico_Hla13", MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
				catch (RTI::RestoreInProgress& e) {
					MessageBox::Show("MSG-(RestoreInProgress - enableAttributeScopeAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RtiAmb_Portico_Hla13", MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
				catch (RTI::RTIinternalError& e) {
					MessageBox::Show("MSG-(RTIinternalError - enableAttributeScopeAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RtiAmb_Portico_Hla13", MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
				catch (System::Exception^ e) {
					MessageBox::Show("MSG-(GeneralException - enableAttributeScopeAdvisorySwitch):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RtiAmb_Portico_Hla13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
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
					MessageBox::Show("MSG-(FederateNotExecutionMember - enableInteractionRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RtiAmb_Portico_Hla13", MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
				catch (RTI::ConcurrentAccessAttempted& e) {
					MessageBox::Show("MSG-(ConcurrentAccessAttempted - enableInteractionRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RtiAmb_Portico_Hla13", MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
				catch (RTI::SaveInProgress& e) {
					MessageBox::Show("MSG-(SaveInProgress - enableInteractionRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RtiAmb_Portico_Hla13", MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
				catch (RTI::RestoreInProgress& e) {
					MessageBox::Show("MSG-(RestoreInProgress - enableInteractionRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RtiAmb_Portico_Hla13", MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
				catch (RTI::RTIinternalError& e) {
					MessageBox::Show("MSG-(RTIinternalError - enableInteractionRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RtiAmb_Portico_Hla13", MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
				catch (System::Exception^ e) {
					MessageBox::Show("MSG-(GeneralException - enableInteractionRelevanceAdvisorySwitch):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RtiAmb_Portico_Hla13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
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
					MessageBox::Show("MSG-(FederateNotExecutionMember - enableClassRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RtiAmb_Portico_Hla13", MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
				catch (RTI::ConcurrentAccessAttempted& e) {
					MessageBox::Show("MSG-(ConcurrentAccessAttempted - enableClassRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RtiAmb_Portico_Hla13", MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
				catch (RTI::SaveInProgress& e) {
					MessageBox::Show("MSG-(SaveInProgress - enableClassRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RtiAmb_Portico_Hla13", MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
				catch (RTI::RestoreInProgress& e) {
					MessageBox::Show("MSG-(RestoreInProgress - enableClassRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RtiAmb_Portico_Hla13", MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
				catch (RTI::RTIinternalError& e) {
					MessageBox::Show("MSG-(RTIinternalError - enableClassRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RtiAmb_Portico_Hla13", MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
				catch (System::Exception^ e) {
					MessageBox::Show("MSG-(GeneralException - enableClassRelevanceAdvisorySwitch):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RtiAmb_Portico_Hla13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
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
					MessageBox::Show("MSG-(FederateNotExecutionMember - disableAttributeRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RtiAmb_Portico_Hla13", MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
				catch (RTI::ConcurrentAccessAttempted& e) {
					MessageBox::Show("MSG-(ConcurrentAccessAttempted - disableAttributeRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RtiAmb_Portico_Hla13", MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
				catch (RTI::SaveInProgress& e) {
					MessageBox::Show("MSG-(SaveInProgress - disableAttributeRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RtiAmb_Portico_Hla13", MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
				catch (RTI::RestoreInProgress& e) {
					MessageBox::Show("MSG-(RestoreInProgress - disableAttributeRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RtiAmb_Portico_Hla13", MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
				catch (RTI::RTIinternalError& e) {
					MessageBox::Show("MSG-(RTIinternalError - disableAttributeRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RtiAmb_Portico_Hla13", MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
				catch (System::Exception^ e) {
					MessageBox::Show("MSG-(GeneralException - disableAttributeRelevanceAdvisorySwitch):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RtiAmb_Portico_Hla13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
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
					MessageBox::Show("MSG-(FederateNotExecutionMember - disableAttributeScopeAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RtiAmb_Portico_Hla13", MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
				catch (RTI::ConcurrentAccessAttempted& e) {
					MessageBox::Show("MSG-(ConcurrentAccessAttempted - disableAttributeScopeAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RtiAmb_Portico_Hla13", MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
				catch (RTI::SaveInProgress& e) {
					MessageBox::Show("MSG-(SaveInProgress - disableAttributeScopeAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RtiAmb_Portico_Hla13", MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
				catch (RTI::RestoreInProgress& e) {
					MessageBox::Show("MSG-(RestoreInProgress - disableAttributeScopeAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RtiAmb_Portico_Hla13", MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
				catch (RTI::RTIinternalError& e) {
					MessageBox::Show("MSG-(RTIinternalError - disableAttributeScopeAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RtiAmb_Portico_Hla13", MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
				catch (System::Exception^ e) {
					MessageBox::Show("MSG-(GeneralException - disableAttributeScopeAdvisorySwitch):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RtiAmb_Portico_Hla13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
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
					MessageBox::Show("MSG-(FederateNotExecutionMember - disableClassRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RtiAmb_Portico_Hla13", MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
				catch (RTI::ConcurrentAccessAttempted& e) {
					MessageBox::Show("MSG-(ConcurrentAccessAttempted - disableClassRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RtiAmb_Portico_Hla13", MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
				catch (RTI::SaveInProgress& e) {
					MessageBox::Show("MSG-(SaveInProgress - disableClassRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RtiAmb_Portico_Hla13", MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
				catch (RTI::RestoreInProgress& e) {
					MessageBox::Show("MSG-(RestoreInProgress - disableClassRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RtiAmb_Portico_Hla13", MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
				catch (RTI::RTIinternalError& e) {
					MessageBox::Show("MSG-(RTIinternalError - disableClassRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RtiAmb_Portico_Hla13", MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
				catch (System::Exception^ e) {
					MessageBox::Show("MSG-(GeneralException - disableClassRelevanceAdvisorySwitch):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RtiAmb_Portico_Hla13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
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
					MessageBox::Show("MSG-(FederateNotExecutionMember - disableInteractionRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RtiAmb_Portico_Hla13", MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
				catch (RTI::ConcurrentAccessAttempted& e) {
					MessageBox::Show("MSG-(ConcurrentAccessAttempted - disableInteractionRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RtiAmb_Portico_Hla13", MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
				catch (RTI::SaveInProgress& e) {
					MessageBox::Show("MSG-(SaveInProgress - disableInteractionRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RtiAmb_Portico_Hla13", MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
				catch (RTI::RestoreInProgress& e) {
					MessageBox::Show("MSG-(RestoreInProgress - disableInteractionRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RtiAmb_Portico_Hla13", MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
				catch (RTI::RTIinternalError& e) {
					MessageBox::Show("MSG-(RTIinternalError - disableInteractionRelevanceAdvisorySwitch):" + Environment::NewLine + gcnew String(e._reason) + Environment::NewLine, "RtiAmb_Portico_Hla13", MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
				catch (System::Exception^ e) {
					MessageBox::Show("MSG-(GeneralException - disableInteractionRelevanceAdvisorySwitch):" + Environment::NewLine + e->ToString() + Environment::NewLine, "RtiAmb_Portico_Hla13", MessageBoxButtons::OK, MessageBoxIcon::Warning);
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

