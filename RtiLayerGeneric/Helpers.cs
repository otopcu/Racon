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

using System;
using System.Runtime.InteropServices;

namespace Racon
{
  /// <summary>
  /// This package presents a generic RTI layer.
  /// </summary>
  /// 
  /// <remarks>
  /// The RTI library, called RtiLayer, is the major portion of the .NET wrapper that abstracts the RTI services and makes transparent the use of both 
  /// •	the various HLA specifications (e.g.DoD HLA 1.3 specification, IEEE 1516-2000 standard, and HLA Evolved and 
  /// •	the various vendor specific RTI implementation for the same specification (e.g. Portico, DMSO RTI 1.3 NG). Another approach to a generic RTI abstraction is to use the dynamic link compatible HLA API standard.
  /// </remarks>
  /// 
  internal class NamespaceDoc
  {
    // Only for documentation of Namespace: Racon.Federation in Sandcastle
  }

  /// <summary>
  /// Represents resign actions/directives
  /// </summary>
  public enum ResignAction // and Mappings to HLA13
  {
    /// <summary>
    /// Unconditionally divest ownership of all owned instance attributes.
    /// Corresponds to RELEASE_ATTRIBUTES in HLA 13
    /// </summary>
    UNCONDITIONALLY_DIVEST_ATTRIBUTES,
    /// <summary>
    /// Delete all object instances for which the joined federate has the delete privilege.
    /// Corresponds to DELETE_OBJECTS
    /// </summary>
    DELETE_OBJECTS,
    /// <summary>
    /// Cancel all pending instance attribute ownership acquisitions.
    /// Corresponds to NO_ACTION in HLA 13
    /// </summary>
    CANCEL_PENDING_OWNERSHIP_ACQUISITIONS,
    /// <summary>
    /// Perform action (2) and then action (1).
    /// Corresponds to DELETE_OBJECTS_AND_RELEASE_ATTRIBUTES in HLA 13
    /// </summary>
    DELETE_OBJECTS_THEN_DIVEST,
    /// <summary>
    /// Perform action (3), action (2), and then action (1).
    /// Corresponds to N/A - NO_ACTION in HLA 13
    /// </summary>
    CANCEL_THEN_DELETE_THEN_DIVEST,
    /// <summary>
    /// Perform no actions.
    /// Corresponds to NO_ACTION in HLA 13
    /// </summary>
    NO_ACTION
  };


  /// <summary>
  /// Callback model - used in connect() service
  /// </summary>
  public enum CallbackModel
  {
    /// <summary>
    /// The RTI shall invoke callbacks immediately when they are available unless the callbacks have been     disabled using the Disable Callbacks service
    /// </summary>
    IMMEDIATE,
    /// <summary>
    /// Callbacks are only made when the federate calls the Evoke Callback/Evoke Multiple Callbacks services, unless callbacks have been disabled using the Disable Callbacks service.
    /// </summary>
    EVOKED
  }
  /// <summary>
  /// Enum Object Class Publication and Subscription Capabilities
  /// </summary>
  public enum PSKind
  {
    /// <summary>
    /// Publish
    /// </summary>
    Publish,
    /// <summary>
    /// Subscribe
    /// </summary>
    Subscribe,
    /// <summary>
    /// Both Publish and Subscribe
    /// </summary>
    PublishSubscribe,
    /// <summary>
    /// Neither Publish or Subscribe
    /// </summary>
    Neither
  };

  /// <summary>
  /// Enum debugging and information level
  /// <![CDATA[ ALL < TRACE < INFO < WARN < ERROR < OFF ]]>
  /// </summary>
  public enum LogLevel
  {
    /// <summary>
    /// All info is provided about RACoN events
    /// </summary>
    ALL = 0,
    /// <summary>
    /// This level provides detailed information about events including handles. Debug Level
    /// </summary>
    TRACE = 1,
    /// <summary>
    /// Only important info is provided.
    /// </summary>
    INFO = 2,
    /// <summary>
    /// Reports incorrect behavior, but RTI can continue. Production level
    /// </summary>
    WARN = 3,
    /// <summary>
    /// Reports application crashes, Racon must stop
    /// </summary>
    ERROR = 4,
    /// <summary>
    /// No information is provided
    /// </summary>
    OFF = 5
  };

  /// <summary>
  /// Encoder for data exchange through the RTI.
  /// </summary>
  public static class Encoder
  {
    /// <summary>
    /// Data Marshaling: encodes data and gives its size
    /// 
    /// Supported Data types: String, DateTime (non-blittable), Value types (Only Blittables) 
    /// Value Types: (1) Structs (2) Enumerations (as System.Int32)
    /// Structs: (1) Numeric Types (2) bool (System.Boolean) (3) User-defined Structs
    /// Numeric Types: (1) Integral types (2) Floating-point Types (3) decimal (System.Decimal)
    /// Integral Types: sbyte, byte, char, short, ushort, int, uint, long, ulong 
    /// Floating-point Types: float (System.Single), double (System.Double) 
    /// </summary>
    public static IntPtr Encode<_type>(_type value, out int size)
    {
      IntPtr data;

      if (typeof(_type) == typeof(string))
      {
        data = Marshal.StringToHGlobalAnsi((value as string));
        if (value != null) // check string is null or not
          size = (value as string).Length + 1;
        else
          size = 0;
      }
      else if (typeof(_type) == typeof(DateTime))
      {
        size = ((DateTime)(object)value).ToString("o").Length + 1;
        return Marshal.StringToHGlobalAnsi(((DateTime)(object)value).ToString("o"));
      }
      else
      {
        Type outputType = typeof(_type).IsEnum ? Enum.GetUnderlyingType(typeof(_type)) : typeof(_type);
        int outBufferSize = Marshal.SizeOf(typeof(_type));
        data = Marshal.AllocHGlobal(outBufferSize);
        Marshal.StructureToPtr(value, data, false);
        size = outBufferSize;
      }
      return data;
    }

    /// <summary>
    /// Decodes the data
    /// </summary>
    public static _type Decode<_type>(IntPtr value)
    {
      try
      {
        _type data;
        // String
        if (typeof(_type) == typeof(string))
        {
          data = (_type)Convert.ChangeType(Marshal.PtrToStringAnsi(value), typeof(_type));
        }
        // DateTime
        else if (typeof(_type) == typeof(DateTime))
        {
          data = (_type)Convert.ChangeType(DateTime.Parse(Marshal.PtrToStringAnsi(value)), typeof(_type));
        }
        // Value Types (only blittables)
        else
        {
          Type outputType = typeof(_type).IsEnum ? Enum.GetUnderlyingType(typeof(_type)) : typeof(_type);
          data = (_type)(Marshal.PtrToStructure(value, outputType));
        }
        return data;
      }
      catch (ArgumentException)
      {
        //System.Diagnostics.Debug.WriteLine("Racon-ArgumentException (GetParameterValue): Data type (" + typeof(_type) + ") for decoding is not supported in the current version. Data type must be blittable. For enumerations, cast it to uint." + Environment.NewLine);
        throw;
      }
      catch (InvalidCastException)
      {
        //MessageBox.Show("Racon-InvalidCastException (GetParameterValue): Data type (" + typeof(_type) + ") for decoding is not supported in the current version. Contact Racon support site" + Environment.NewLine, "Racon-HlaInteraction.h", MessageBoxButtons.OK, MessageBoxIcon.Error);
        throw;
      }
    }

  }
}
