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
  /// CRegion
  /// </summary>
  public class HlaRegion
  {
    #region Fields
    #endregion

    #region Properties
    /// <summary>
    /// Name 
    /// </summary>
    public string Name { get; set; }
    /// <summary>
    /// Handle
    /// </summary>
    public uint Handle { get; set; }
    #endregion

    #region Constructors
    /// <summary>
    ///  CRegion constructor.
    /// </summary>
    public HlaRegion(string name)
      : base()
    {
      Name = name;
    }
    #endregion

    #region Methods
    #endregion

  }
}
