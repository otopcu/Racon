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

using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.ComponentModel;

namespace Racon.RtiLayer
{
  public class HlaInteraction
  {
    #region Properties
    /// <summary>
    ///  Name of the interaction class
    /// </summary>
    public string ClassName { get; set; }
    /// <summary>
    ///  Handle of the interaction class
    /// </summary>
    public uint ClassHandle { get; set; }
    /// <summary>
    ///  Interaction tag
    /// </summary>
    public string Tag { get; set; }
    ///// <summary>
    /////  Retraction Handle !!! Nerede olmalı - > HlaObjectEventArgs?
    ///// </summary>
    //public EventRetractionHandle RetractionHandle { get; set; }
    /// <summary>
    ///  Parameters
    /// </summary>
    public BindingList<HlaParameter> Parameters { get; set; }
    #endregion

    #region Constructors
    /// <summary>
    /// Constructor
    /// </summary>
    public HlaInteraction()
    {
      Tag = "";
      Parameters = new BindingList<HlaParameter>();
      //RetractionHandle = new EventRetractionHandle();
    }
    public HlaInteraction(HlaInteractionClass ic) : this()
    {
      ClassHandle = ic.Handle;
      ClassName = ic.Name;
    }
    public HlaInteraction(HlaInteractionClass ic, string tag) : this(ic)
    {
      Tag = tag;
    }
    #endregion

    #region Methods
    public void AddParameter(HlaParameter _parameter)
    {
      Parameters.Add(_parameter);
    }
    public void AddParameterValue(HlaParameter _parameter, string value)
    {
      _parameter.AddValue(value);
      Parameters.Add(_parameter);
    }
    public void AddParameterValue(HlaParameter _parameter, DateTime value)
    {
      _parameter.AddValue(value);
      Parameters.Add(_parameter);
    }
    //public void AddParameterValue(HlaParameter _parameter, double value)
    //{
    //  _parameter.AddValue(value);
    //  Parameters.Add(_parameter);
    //}
    public void AddParameterValue<_type>(HlaParameter _parameter, _type value) where _type : struct
    //public void AddParameterValue<_type>(HlaParameter _parameter, _type value) where _type : struct, IConvertible, IComparable, IFormattable
    {
      _parameter.AddValue(value);
      Parameters.Add(_parameter);
    }
    #endregion
  }
}
