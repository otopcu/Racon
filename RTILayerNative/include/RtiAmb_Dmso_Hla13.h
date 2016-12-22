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

// Managed Wrapper Class for HLA13 - DMSO
#pragma once
#include "RtiAmb_Hla13.h"

namespace Racon
{
	namespace RtiLayer {
		namespace Native {
			public ref class RtiAmb_Dmso_Hla13 : public RtiAmb_Hla13 {

#pragma region Fields
			private:
#pragma endregion			

#pragma region Ctor/Dtor
			public:
				RtiAmb_Dmso_Hla13(CallbackManager^ eventManager) : RtiAmb_Hla13(eventManager) {
					RtiVersion = "DMSO RTI 1.3NG v6";
				};

				~RtiAmb_Dmso_Hla13()
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
#pragma endregion

#pragma region Helpers
			protected:
#pragma endregion

#pragma endregion

			};
		}
	}
}

