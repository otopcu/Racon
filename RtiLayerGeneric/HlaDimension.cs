/*
RACoN - RTI abstraction component for MS.NET (RACoN)
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

namespace Racon.RtiLayer
{
  /// <summary>
  /// Dimension
  /// </summary>
  public class HlaDimension
  {
    #region Fields
    #endregion

    #region Properties
    /// <summary>
    /// Region name.
    /// </summary>
    public string Name { get; set; }

    /// <summary>
    /// Handle.
    /// </summary>
    public uint Handle { get; set; }
    #endregion

    #region Constructors
    /// <summary>
    /// CDimension constructor. Dimension name must be the same with the one specified in FED/FDD.
    /// </summary>
    public HlaDimension(string name)
    {
      Name = name;
    }
    #endregion

    #region Methods
    #endregion

  }
}
