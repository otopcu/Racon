// ******************************************************************************
//                          RTIAmbBase.h
//		begin                : Dec 19, 2009
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
using namespace System;
using namespace System::Collections::Generic;
using namespace System::ComponentModel; // BindingList
// Racon
using namespace Racon::RtiLayer;

namespace Racon
{
	namespace RtiLayer {
		namespace Native {
			public ref class CRtiAmb : Racon::RtiLayer::RtiAmb
			{
#pragma region Fields
			private:
#pragma endregion

#pragma region Constructor
			protected:
				CRtiAmb::CRtiAmb() {};
#pragma endregion			

#pragma region Methods

#pragma region Data Distribution Management
			public:
				// CHlaRegion is defined in C++
				virtual bool associateRegionForUpdates(CHlaRegion ^, HlaObject ^, List<HlaAttribute^>^) = 0;
				virtual bool changeAttributeOrderType(HlaObject ^, List<HlaAttribute^>^, OrderingHandle) = 0;
				virtual bool changeInteractionOrderType(HlaInteractionClass ^, OrderingHandle) = 0;
				virtual bool createRegion(CHlaRegion^, unsigned long, long) = 0;
				virtual bool deleteRegion(CHlaRegion^) = 0;
				virtual bool registerObjectInstanceWithRegion(HlaObject ^, List<HlaAttribute^>^, List<CHlaRegion^>^) = 0;
				virtual bool requestClassAttributeValueUpdateWithRegion(HlaObjectClass ^, List<HlaAttribute^>^, CHlaRegion ^) = 0;
				virtual bool sendInteraction(HlaInteraction ^, CHlaRegion ^) = 0;
				virtual bool subscribeObjectClassAttributesWithRegion(HlaObjectClass ^, List<HlaAttribute^>^, CHlaRegion ^, Boolean) = 0;
				virtual bool subscribeInteractionClass(HlaInteractionClass ^, CHlaRegion ^) = 0;
				virtual bool unassociateRegionForUpdates(CHlaRegion ^, HlaObject ^) = 0;
				virtual void unsubscribeInteractionClass(HlaInteractionClass ^, CHlaRegion ^) = 0;
				virtual void unsubscribeObjectClassWithRegion(HlaObjectClass ^, CHlaRegion ^) = 0;
#pragma endregion

#pragma endregion
			};
		}
	}
}