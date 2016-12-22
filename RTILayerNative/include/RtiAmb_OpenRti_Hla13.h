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

// Managed Wrapper Class for HLA13 - OpenRti
#pragma once
#include "RtiAmb_Hla13.h"

namespace Racon
{
	namespace RtiLayer {
		namespace Native {
			public ref class RtiAmb_OpenRti_Hla13 : public RtiAmb_Hla13 {

#pragma region Fields
			private:
				String^ connectionString;
#pragma endregion			

#pragma region Ctor/Dtor
			public:
				RtiAmb_OpenRti_Hla13(CallbackManager^ eventManager) : RtiAmb_Hla13(eventManager) {
					RtiVersion = "OpenRTI v0.8";
					connectionString = "";
				};

				~RtiAmb_OpenRti_Hla13()
				{
					rti->~RTIambassador();
					_nativeFdAmb->~FdAmb_Hla13();
				};
#pragma endregion						

#pragma region Methods
#pragma region Fedaration Management
			public:
				// Connect
				void connect(String ^localSetting) override {
					// For IEEE1516-2010 - Pseudo Connect - Returns always connected
					// OpenRTI HLA1.3 interprets federation execution names as urls: <protocol>://<address>/<path>/<name>
					// Where the protocol overrides the protocol field from the string list.
					connectionString = localSetting;
					String^ msg = "Federate application is connected.";
					if (localSetting != "")
						msg += " The connection protocol and the network address of the RTI server is " + gcnew String(connectionString) + ".";
					else
					{
						msg += "the connection protocol is THREAD.";
					}
					this->OnFederateConnected(gcnew RaconEventArgs(msg));
				};

				// Create Federation
				void createFederation(String ^fedexec, String ^fdd) override {
					fedexec = connectionString + "/" + fedexec;//e.g. fedexec = "rti://127.0.0.1:7777/CHAT";
					RtiAmb_Hla13::createFederation(fedexec, fdd);
				};

				// Join Federation
				unsigned int joinFederation(String ^fedexec, String ^fdName) override {
					fedexec = connectionString + "/" + fedexec;//e.g. fedexec = "rti://127.0.0.1:7777/CHAT";
					return RtiAmb_Hla13::joinFederation(fedexec, fdName);// Call baseclass
				};

				// Destroy Federation
				void destroyFederation(String ^fedexec) override {
					fedexec = connectionString + "/" + fedexec;
					RtiAmb_Hla13::destroyFederation(fedexec);	// Call baseclass
				};
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
#pragma endregion

#pragma region Helpers
			protected:
#pragma endregion

#pragma endregion

			};
		}
	}
}

