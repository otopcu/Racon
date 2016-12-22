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

/// <summary>
/// General types and helper classes used by the RtiLayer
/// </summary>

#pragma once
#include "RTI/RTI1516.h"

namespace Racon {
	namespace RtiLayer {
		namespace Native {
			// Convert to ULong
			static System::UInt32 toULong(String^ handle) {
				return System::UInt32::Parse(handle->Split('(', ')')[1]);
			};
			// Convert to ULong
			static System::UInt32 toULong(std::wstring handle) {
				String^ hand = gcnew String(handle.c_str());
				return System::UInt32::Parse(hand->Split('(', ')')[1]);
			};
			// Convert rti1516e enums to string
			static const char* SynchronizationPointFailureReasonStrings[] = { "SYNCHRONIZATION_POINT_LABEL_NOT_UNIQUE", "SYNCHRONIZATION_SET_MEMBER_NOT_JOINED" };
			static String^ getTextForSynchronizationPointFailureReason(int enumVal)
			{
				return gcnew String(SynchronizationPointFailureReasonStrings[enumVal]);
			};
			// Native RTI object model tracker (NomTracker)
			public class OmHla1516e
			{
			public:
				// Maps RaconObjectModel (handle) -> NativeObjectModel (handle)
				std::map<unsigned int, rti1516e::ObjectClassHandle> ObjectClasses;
				std::map<unsigned int, rti1516e::InteractionClassHandle> InteractionClasses;
				std::map<unsigned int, rti1516e::AttributeHandle> Attributes;
				std::map<unsigned int, rti1516e::ParameterHandle> Parameters;
				std::map<unsigned int, rti1516e::ObjectInstanceHandle> Objects;
			public:
				OmHla1516e(void) {
				};
			};

		}
	}
}