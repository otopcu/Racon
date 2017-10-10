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

using System;
using System.Runtime.InteropServices;

namespace Racon.RtiLayer
{
  /// <summary>
  /// Base class for HlaAttribute and HlaParameter
  /// </summary>
  public abstract class HlaProperty
  {
    #region Properties
    /// <summary>
    /// Name must be the same with the attribute name in the FOM
    /// </summary>
    public string Name { get; set; }
    /// <summary>
    /// Handle
    /// </summary>
    public uint Handle { get; set; }
    /// <summary>
    /// Value - a generic pointer for to data (char *). Getting and setting is done via AddValue() and GetValue() methods respectively.
    /// </summary>
    public IntPtr Value { get; set; }
    /// <summary>
    /// Memory length of parameter value
    /// </summary>
    public int Length;
    #endregion

    #region Constructors
    /// <summary>
    /// Constructor
    /// </summary>
    public HlaProperty()
    {
      Name = "";
      Length = 0;
    }
    #endregion

    #region Methods
    /// <summary>
    /// Encode data
    /// </summary>
    /// <typeparam name="_type"></typeparam>
    /// <param name="value"></param>
    public void AddValue<_type>(_type value)
    {
      Value = Encoder.Encode(value, out Length);// encodes data and gives its size
    }

    ///// <summary>
    ///// Data Unmarshaling
    ///// IntPtr - pointer for unmanaged memory (void *)
    ///// </summary>
    //public void AddValue(IntPtr value)
    //{
    //  Value = value;
    //}
    ///// <summary>
    ///// Data Marshaling
    ///// String
    ///// </summary>
    //public void AddValue(string value)
    //{
    //  Value = Marshal.StringToHGlobalAnsi(value);
    //  Length = value.Length + 1;
    //}
    ///// <summary>
    ///// Data Marshaling
    ///// DateTime - non-blittable
    ///// </summary>
    //public void AddValue(DateTime value)
    //{
    //  Value = Marshal.StringToHGlobalAnsi(value.ToString("o"));
    //  Length = value.ToString("o").Length + 1;
    //}

    ////public void AddValue(double value)
    ////{
    ////  Value = Marshal.AllocHGlobal(sizeof(double));
    ////  Marshal.StructureToPtr(value, Value, false);
    ////  Length = sizeof(double);

    ////  //  Value = Marshal.StringToHGlobalAnsi(value.ToString());
    ////  //  Length = value.ToString().Length + 1;
    ////}

    ///// <summary>
    ///// Data Marshaling
    ///// 
    ///// Value Types (Only Blittables): 
    ///// (1) Structs (2) Enumerations
    ///// 
    ///// Structs: 
    ///// (1) Numeric Types (2) bool (System.Boolean) (3) User-defined Structs
    ///// 
    ///// Numeric Types: 
    ///// (1) Integral types (2) Floating-point Types (3) decimal (System.Decimal)
    ///// 
    ///// Integral Types: 
    ///// sbyte, byte, char, short, ushort, int, uint, long, ulong 
    ///// 
    ///// Floating-point Types: 
    ///// float (System.Single), double (System.Double) 
    ///// </summary>
    //public void AddValue<_type>(_type value) where _type : struct
    //{
    //  // determine the correct output type:
    //  Type outputType = typeof(_type).IsEnum ? Enum.GetUnderlyingType(typeof(_type)) : typeof(_type);
    //  int outBufferSize = Marshal.SizeOf(outputType);
    //  Value = Marshal.AllocHGlobal(outBufferSize);
    //  Marshal.StructureToPtr(value, Value, false);
    //  Length = outBufferSize;

    //  //MessageBox.Show("Data type (" + outputType + "). Size (" + outBufferSize + ")" + Environment.NewLine, "Racon-HlaInteraction.h", MessageBoxButtons.OK, MessageBoxIcon.Error);
    //}

    /// <summary>
    /// Decodes the attribute value
    /// </summary>
    public _type GetValue<_type>()
    {
      return Encoder.Decode<_type>(Value);
      //try
      //{
      //  _type data;
      //  // String
      //  if (typeof(_type) == typeof(string))
      //  {
      //    data = (_type)Convert.ChangeType(Marshal.PtrToStringAnsi(Value), typeof(_type));
      //  }
      //  // DateTime
      //  else if (typeof(_type) == typeof(DateTime))
      //  {
      //    data = (_type)Convert.ChangeType(DateTime.Parse(Marshal.PtrToStringAnsi(Value)), typeof(_type));
      //  }
      //  //// int/long
      //  //else if ((typeof(_type) == typeof(int)) || (typeof(_type) == typeof(long)))
      //  //{
      //  //  return (_type)Convert.ChangeType(Marshal.ReadInt32(Value), typeof(_type));
      //  //}
      //  //// double
      //  //else if ((typeof(_type) == typeof(double)))
      //  //{
      //  //  return (_type)Marshal.PtrToStructure(Value, typeof(double));
      //  //}

      //  // Value Type for blittables
      //  else
      //  //return (_type)Convert.ChangeType(Marshal.ReadInt32(Value), typeof(_type));
      //  //return (_type)Convert.ChangeType(Marshal.PtrToStructure(Value, typeof(_type)), typeof(_type));
      //  {
      //    Type outputType = typeof(_type).IsEnum ? Enum.GetUnderlyingType(typeof(_type)) : typeof(_type);
      //    data = (_type)(Marshal.PtrToStructure(Value, outputType));
      //  }
      //  return data;
      //}
      //catch (ArgumentException)
      //{
      //  //System.Diagnostics.Debug.WriteLine("Racon-ArgumentException (GetParameterValue): Data type (" + typeof(_type) + ") for decoding is not supported in the current version. Data type must be blittable. For enumerations, cast it to uint." + Environment.NewLine);
      //  throw;
      //}
      //catch (InvalidCastException)
      //{
      //  //MessageBox.Show("Racon-InvalidCastException (GetParameterValue): Data type (" + typeof(_type) + ") for decoding is not supported in the current version. Contact Racon support site" + Environment.NewLine, "Racon-HlaInteraction.h", MessageBoxButtons.OK, MessageBoxIcon.Error);
      //  throw;
      //}
    }
    #endregion

    /// <summary>
    /// Data Marshaling
    /// String
    /// </summary>
    public void FreeIntPtrMemory()
    {
      Marshal.FreeHGlobal(Value);
    }

  }
}
