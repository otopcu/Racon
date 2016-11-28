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
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace Racon.RtiLayer
{
  public class HlaAttribute : HlaProperty
  {
    #region Properties
    /// <summary>
    /// PS
    /// </summary>
    public PSKind AttributePS { get; set; }
    #endregion

    #region Constructors
    /// <summary>
    /// Constructor
    /// </summary>
    public HlaAttribute() : base()
    {
      AttributePS = PSKind.Neither;
    }
    public HlaAttribute(string name, PSKind ps) : this()
    {
      Name = name;
      AttributePS = ps;
    }
    #endregion

  }
}
