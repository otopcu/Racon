
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
using System.ComponentModel;
using System.Runtime.InteropServices;

namespace Racon.RtiLayer
{
  /// <summary>
  /// HlaObject class is a wrapper for HLA object. Its instances represent the Hla objects that are exchanged in a federation and therefore it is important to deal with the RTI object management interface (e.g. for updating an attribute value of an HLA object). So, you need to encapsulate “the part” of your local data structure (in your case it is the predefined C# class used for the simulation) in a HlaObject class, where “the part” is the data needed by other federates.
  /// </summary>
  public class HlaObject
  {
    #region Properties
    /// <summary>
    /// Name 
    /// </summary>
    public string Name { get; set; }
    /// <summary>
    /// Handle
    /// </summary>
    public uint Handle { get; set; }
    /// <summary>
    ///  Object tag
    /// </summary>
    public string Tag { get; set; }
    /// <summary>
    ///  Pointer to object class
    /// </summary>
    public HlaObjectClass Type { get; set; }
    ///// <summary>
    /////  Retraction Handle !!! Nerede olmalı - > HlaObjectEventArgs?
    ///// </summary>
    //public EventRetractionHandle RetractionHandle { get; set; }
    /// <summary>
    ///  Attributes
    /// </summary>
    public BindingList<HlaAttribute> Attributes { get; set; }
    #endregion

    #region Constructors
    /// <summary>
    /// Constructor
    /// </summary>
    public HlaObject()
    {
      Tag = "";
      Name = "";
      Attributes = new BindingList<HlaAttribute>();
      //RetractionHandle = new EventRetractionHandle();
    }
    public HlaObject(HlaObjectClass oc) : this()
    {
      Type = oc;
    }
    public HlaObject(HlaObjectClass oc, string name) : this(oc)
    {
      Name = name;
    }
    // Copy Constructor
    public HlaObject(HlaObject obj) : this()
    {
      Name = obj.Name;
      Handle = obj.Handle;
      Tag = obj.Tag;
      //RetractionHandle = obj.RetractionHandle;
      Attributes = obj.Attributes;
      Type = Type;
    }
    #endregion

    #region Methods
    public void AddAttribute(HlaAttribute attribute)
    {
      Attributes.Add(attribute);
    }
    public void AddAttributeValue(HlaAttribute attribute, string value)
    {
      attribute.AddValue(value);
      Attributes.Add(attribute);
    }
    public void AddAttributeValue(HlaAttribute attribute, DateTime value)
    {
      attribute.AddValue(value);
      Attributes.Add(attribute);
    }
    public void AddAttributeValue<_type>(HlaAttribute attribute, _type value) where _type : struct
    {
      attribute.AddValue(value);
      Attributes.Add(attribute);
    }
    #endregion

  }
}
