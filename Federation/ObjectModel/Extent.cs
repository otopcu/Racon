/*
RACoN - RTI abstraction component for MS.NET (RACoN)
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

namespace Racon.ObjectModel
{
  /// <summary>
  /// CExtent
  /// </summary>
  public class CExtent
  {
    #region Fields
    #endregion

    #region Properties
    /// <summary>
    /// Lower bound.
    /// </summary>
    public ulong LowerBound { get; set; }

    /// <summary>
    /// Upper bound.
    /// </summary>
    public ulong UpperBound { get; set; }

    /// <summary>
    /// Dimension.
    /// </summary>
    public CDimension Dimension { get; set; }
    #endregion

    #region Constructors
    /// <summary>
    /// CExtent constructor.
    /// </summary>
    public CExtent(CDimension dimension, ulong lowerBound, ulong upperBound)
    {
      LowerBound = lowerBound;
      UpperBound = upperBound;
      Dimension = dimension;
    }
    #endregion

    #region Methods

    #endregion

  }
}
