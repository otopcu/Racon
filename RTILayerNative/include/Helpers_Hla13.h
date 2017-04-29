/*
Racon - RTI abstraction component for MS.NET (Racon)
https://sites.google.com/site/okantopcu/racon

Copyright © Okan Topçu, 2009-2017
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
#include <map>
#include <string>
#include<RTI.hh>

using namespace System;

namespace Racon {
	namespace RtiLayer {
		namespace Native {
      // Native RTI object model tracker (NomTracker)
      public class NomHla13
      {
      public:
        // Maps RaconObjectModel (handle) -> NativeObjectModel (handle)
        std::map<char*, RTI::Region*> Regions;
      public:
        NomHla13(void) {
        };
      };
      
      typedef unsigned long  ULong; // c++ unsigned long = C# uint, UInt32
			typedef ULong Handle;
			//typedef Handle ParameterHandle;
			typedef Handle ObjectHandle;
			typedef Handle DimensionHandle;
			typedef Handle OrderingHandle;
			typedef ULong UniqueID;
			typedef ULong FederateHandle;
			typedef ULong InteractionClassHandle;
			typedef ULong ObjectClassHandle;

			// CHlaRegion
			public ref class CHlaRegion
			{
			private:
				String^ _name;
				RTI::Region* _handle;
			public:
				// Handle - RTI Region pointer
				property RTI::Region* Handle {
					void set(RTI::Region* val) {
						_handle = val;
					}
					RTI::Region* get() {
						return _handle;
					};
				};
				// Name
				property String^ Name {
					void set(String^ val) {
						_name = val;
					}
					String^ get() {
						return _name;
					};
				};

			public:
				CHlaRegion(void) {
					Name = "";
				};
			protected: void setRangeLowerBound(unsigned long extentIndex, unsigned long dimensionHandle, unsigned long lowerBound) {
				this->Handle->setRangeLowerBound(extentIndex, dimensionHandle, lowerBound);
			}
			protected: void setRangeUpperBound(unsigned long extentIndex, unsigned long dimensionHandle, unsigned long upperBound) {
				this->Handle->setRangeUpperBound(extentIndex, dimensionHandle, upperBound);
			}
			};

		}
	}
}