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
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Racon.RtiLayer
{
  /// <summary>
  /// Abstract class for HLA class
  /// </summary>
  public abstract class HlaClass
  {
    #region Properties
    /// <summary>
    /// Name of the class
    /// </summary>
    public string Name { get; set; }
    /// <summary>
    /// Handle of the class
    /// </summary>
    public uint Handle { get; set; }
    /// <summary>
    /// PS of the class
    /// </summary>
    public PSKind ClassPS { get; set; }
    /// <summary>
    ///  Class tag
    /// </summary>
    public string Tag { get; set; }
    #endregion

    #region Constructors
    /// <summary>
    /// Constructor
    /// </summary>
    protected HlaClass()
    {
      Name = "";
      Handle = 0;
      Tag = "";
      ClassPS = PSKind.Neither;
    }
    /// <summary>
    /// Constructor
    /// </summary>
    /// <param name="name"></param>
    /// <param name="ps"></param>
    protected HlaClass(string name, PSKind ps) : this()
    {
      Name = name;
      ClassPS = ps;
    }
    #endregion
  }
}
