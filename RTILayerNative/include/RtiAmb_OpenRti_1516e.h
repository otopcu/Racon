// ******************************************************************************
//                          RtiAmb_OpenRti_1516e.h
//		begin                : Feb 03, 2016
// ******************************************************************************

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
//#include "Helpers.h"
//#include "Helpers_Hla1516e.h"
//#include "RTI/RTI1516.h"
#include "RtiAmb_Hla1516e.h"
//#include "FdAmb_Hla1516e.h"

//using namespace System;
//using namespace System::Windows::Forms;
//using namespace System::ComponentModel;
//using namespace rti1516e;

namespace Racon
{
	namespace RtiLayer {
		namespace Native {
			ref class CHlaRegion;
			ref class HlaInteraction;

			public ref class RtiAmb_OpenRti_1516e : public RtiAmb_Hla1516e {

#pragma region Fields
#pragma endregion			

#pragma region Constructors
			public:
				RtiAmb_OpenRti_1516e(CallbackManager^);
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
#pragma endregion			

#pragma region Declaration Management
			public:
#pragma endregion	// Declaration Management		

#pragma region Object Management
			public:
				bool registerObject(HlaObject ^) override;
				bool registerObject(HlaObject ^, String^) override;
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
				virtual void getAttributeHandlesFromRti(HlaObjectClass ^) override;// deprecated
				virtual void getClassHandleFromRti(HlaInteractionClass ^) override; // deprecated
				virtual void getClassHandleFromRti(HlaObjectClass ^) override;// deprecated
				void getParameterHandlesFromRti(HlaInteractionClass ^) override;// deprecated
#pragma endregion

#pragma region Helpers
			protected:
				rti1516e::ObjectClassHandle getObjectClassHandle(HlaObjectClass ^) override;
				rti1516e::InteractionClassHandle getInteractionClassHandle(HlaInteractionClass ^) override;
				rti1516e::AttributeHandle getAttributeHandle(HlaAttribute^, rti1516e::ObjectClassHandle) override;
				rti1516e::ParameterHandle getParameterHandle(HlaParameter^, rti1516e::InteractionClassHandle) override;
				// Conversions for handle - OpenRTI specific 
				ULong Handle2Long(rti1516e::ObjectClassHandle handle) override {
					return toULong(gcnew String(handle.toString().c_str()));
				}
				ULong Handle2Long(rti1516e::InteractionClassHandle handle) override {
					return toULong(gcnew String(handle.toString().c_str()));
				}
				ULong Handle2Long(rti1516e::AttributeHandle handle) override {
					return toULong(gcnew String(handle.toString().c_str()));
				}
				ULong Handle2Long(rti1516e::ParameterHandle handle) override {
					return toULong(gcnew String(handle.toString().c_str()));
				}
				ULong Handle2Long(rti1516e::ObjectInstanceHandle handle) override {
					return toULong(gcnew String(handle.toString().c_str()));
				}
				ULong Handle2Long(rti1516e::FederateHandle handle) override {
					return toULong(gcnew String(handle.toString().c_str()));
				}
				ULong Handle2Long(rti1516e::RegionHandle handle) override {
					return toULong(gcnew String(handle.toString().c_str()));
				}
#pragma endregion

#pragma endregion

			};
		}
	}
}

