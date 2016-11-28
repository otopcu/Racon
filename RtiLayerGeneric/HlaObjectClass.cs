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

namespace Racon.RtiLayer
{
  public class HlaObjectClass : HlaClass
  {
    #region Properties
    /// <summary>
    ///  PrivilegeToDelete Attribute
    /// </summary>
    public HlaAttribute PrivilegeToDelete { get; set; }

    /// <summary>
    ///  PrivilegeToDelete Attribute
    /// </summary>
    public BindingList<HlaAttribute> Attributes { get; set; }
    #endregion

    #region Constructors
    /// <summary>
    /// Constructor
    /// </summary>
    public HlaObjectClass() : base()
    {
      Attributes = new BindingList<HlaAttribute>();
      PrivilegeToDelete = new HlaAttribute("HLAprivilegeToDeleteObject", PSKind.Neither);// !!! Naming only conforms to HLA1516-2010
      //PrivilegeToDelete = new HlaAttribute("privilegeToDelete", PSKind.Neither);// !!! Naming only conforms to HLA13
      Attributes.Add(PrivilegeToDelete);
    }
    public HlaObjectClass(string name, PSKind ps) : base(name, ps)
    {
    }
    #endregion
  }
}
