
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

namespace Racon.RtiLayer
{
  /// <summary>
  /// a wrapper class for VariableLengthData.
  /// </summary>
  public class VariableLengthDataWrapper
  {
    private Type type = null;

    #region Properties
    /// <summary>
    /// Data - a generic pointer for to data (char *). Getting and setting is done via AddData() and GetData() methods respectively.
    /// </summary>
    public IntPtr Data { get; set; }

    /// <summary>
    /// Memory length of parameter value
    /// </summary>
    public int Size;
    #endregion

    #region Constructors
    /// <summary>
    /// Constructor
    /// </summary>
    public VariableLengthDataWrapper()
    {
      Size = 0;
    }
    #endregion

    #region Methods
    /// <summary>
    /// Encode data
    /// </summary>
    /// <typeparam name="_type">Generic type</typeparam>
    /// <param name="value">Value</param>
    public void AddData<_type>(_type value)
    {
      Data = Encoder.Encode(value, out Size);// encodes data and gives its size
      type = typeof(_type);
      // Debug
      //Console.WriteLine("Data type: " + typeof(_type).ToString() + ". Size: " + Size + Environment.NewLine);
    }

    /// <summary>
    /// Decodes the data
    /// </summary>
    public _type GetData<_type>()
    {
      return Encoder.Decode<_type>(Data);
    }

    /// <summary>
    /// </summary>
    public void FreeIntPtrMemory()
    {
      Marshal.FreeHGlobal(Data);
    }

    /// <summary>
    /// Returns the string of the data if convertable
    /// </summary>
    public override string ToString()
    {
      // String
      if (type == typeof(string))
      {
        return Marshal.PtrToStringAnsi(Data);
      }
      else if (type == typeof(bool))
      {
        return (Marshal.PtrToStructure(Data, typeof(bool))).ToString();
      }
      else if (type == typeof(DateTime))
      {
        return Marshal.PtrToStringAnsi(Data);
      }
      else if (type.IsValueType)
      {
        return Marshal.PtrToStructure(Data, type).ToString();
      }
      else
        return type.ToString();
    }
    #endregion

  }
}
