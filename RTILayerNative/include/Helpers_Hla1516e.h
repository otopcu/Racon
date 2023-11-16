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

/// <summary>
/// General types and helper classes used by the RtiLayer
/// </summary>

#pragma once
#include "RTI/RTI1516.h"

namespace Racon {
  namespace RtiLayer {
    namespace Native {
      // RTI Native object model (Nom) tracker (NomTracker)
      public class NomHla1516e
      {
      public:
        rti1516e::FederateHandle FederateHandle;
        // Maps RaconObjectModel (handle) -> NativeObjectModel (handle)
        std::map<unsigned int, rti1516e::ObjectClassHandle> ObjectClasses;
        std::map<unsigned int, rti1516e::InteractionClassHandle> InteractionClasses;
        std::map<unsigned int, rti1516e::AttributeHandle> Attributes;
        std::map<unsigned int, rti1516e::ParameterHandle> Parameters;
        std::map<unsigned int, rti1516e::ObjectInstanceHandle> Objects;
        std::map<unsigned int, rti1516e::MessageRetractionHandle> RetractionHandles;
        std::map<unsigned int, rti1516e::RegionHandle> Regions;
        std::map<unsigned int, rti1516e::DimensionHandle> Dimensions;
      public:
        NomHla1516e(void) {
        };
      };

      // Convert to ULong - OpenRTI Specific
      static System::UInt32 toULong(String^ handle) {
        return System::UInt32::Parse(handle->Split('(', ')')[1]);
      };
      static System::UInt32 toULong(std::wstring handle) {
        String^ hand = gcnew String(handle.c_str());
        return System::UInt32::Parse(hand->Split('(', ')')[1]);
      };

      // Convert rti1516e enums to string
      // SynchronizationPointFailure -> String
      static const char* SynchronizationPointFailureReasonStrings[] = { "SYNCHRONIZATION_POINT_LABEL_NOT_UNIQUE", "SYNCHRONIZATION_SET_MEMBER_NOT_JOINED" };
      static String^ getTextForSynchronizationPointFailureReason(int enumVal)
      {
        return gcnew String(SynchronizationPointFailureReasonStrings[enumVal]);
      };
      // rti1516e::SaveFailureReason -> String
      static inline String^ ToString(rti1516e::SaveFailureReason val)
      {
        switch (val)
        {
          case rti1516e::RTI_UNABLE_TO_SAVE: return "RTI_UNABLE_TO_SAVE";
          case rti1516e::FEDERATE_REPORTED_FAILURE_DURING_SAVE: return "FEDERATE_REPORTED_FAILURE_DURING_SAVE";
          case rti1516e::FEDERATE_RESIGNED_DURING_SAVE: return "FEDERATE_RESIGNED_DURING_SAVE";
          case rti1516e::RTI_DETECTED_FAILURE_DURING_SAVE: return "RTI_DETECTED_FAILURE_DURING_SAVE";
          case rti1516e::SAVE_TIME_CANNOT_BE_HONORED: return "SAVE_TIME_CANNOT_BE_HONORED";
          case rti1516e::SAVE_ABORTED: return "SAVE_ABORTED";
          default: return "Unknown";
        }
      }
      // rti1516e::RestoreFailureReason -> String
      static inline String^ ToString(rti1516e::RestoreFailureReason val)
      {
        switch (val)
        {
        case rti1516e::RTI_UNABLE_TO_RESTORE: return "RTI_UNABLE_TO_RESTORE";
        case rti1516e::FEDERATE_REPORTED_FAILURE_DURING_RESTORE: return "FEDERATE_REPORTED_FAILURE_DURING_RESTORE";
        case rti1516e::FEDERATE_RESIGNED_DURING_RESTORE: return "FEDERATE_RESIGNED_DURING_RESTORE";
        case rti1516e::RTI_DETECTED_FAILURE_DURING_RESTORE: return "RTI_DETECTED_FAILURE_DURING_RESTORE";
        case rti1516e::RESTORE_ABORTED: return "RESTORE_ABORTED";
        default: return "Unknown";
        }
      }
      // Convert enum (rti1516e::OrderType) to string
      static inline String^ ToString(rti1516e::OrderType val)
      {
        switch (val)
        {
          case rti1516e::RECEIVE: return "RECEIVE";
          case rti1516e::TIMESTAMP: return "TIMESTAMP";
          default: return "Unknown";
        }
      }
      // Convert enum (rti1516e::TransportationType) to string
      static inline String^ ToString(rti1516e::TransportationType val)
      {
        switch (val)
        {
          case rti1516e::RELIABLE: return "RELIABLE";
          case rti1516e::BEST_EFFORT: return "BEST_EFFORT";
          default: return "Unknown";
        }
      }

      // Conversions for Nom => Rom Handles - this may differentiate between portico and openrti
      static ULong Handle2Long(rti1516e::ObjectClassHandle handle) {
        return toULong(gcnew String(handle.toString().c_str()));
      }
      static ULong Handle2Long(rti1516e::InteractionClassHandle handle) {
        return toULong(gcnew String(handle.toString().c_str()));
      }
      static ULong Handle2Long(rti1516e::AttributeHandle handle) {
        return toULong(gcnew String(handle.toString().c_str()));
      }
      static ULong Handle2Long(rti1516e::ParameterHandle handle) {
        return toULong(gcnew String(handle.toString().c_str()));
      }
      static ULong Handle2Long(rti1516e::ObjectInstanceHandle handle) {
        return toULong(gcnew String(handle.toString().c_str()));
      }
      static ULong Handle2Long(rti1516e::FederateHandle handle) {
        return toULong(gcnew String(handle.toString().c_str()));
      }
      static ULong Handle2Long(rti1516e::RegionHandle handle) {
        return toULong(gcnew String(handle.toString().c_str()));
      }
      static ULong Handle2Long(rti1516e::DimensionHandle handle) {
        return toULong(gcnew String(handle.toString().c_str()));
      }

    }
  }
}