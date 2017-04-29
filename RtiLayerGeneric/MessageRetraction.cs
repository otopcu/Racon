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

namespace Racon.RtiLayer
{
  /// <summary>
  /// MessageRetraction
  /// </summary>
  public class MessageRetraction
  {
    #region Properties
    /// <summary>
    /// Message retraction handle.
    /// </summary>
    public uint Handle { get; set; }
    /// <summary>
    ///  Serial number - for HLA13 compability
    /// </summary>
    public uint SerialNumber { get { return Handle; } }
    /// <summary>
    ///  Sending federate handle - for HLA13 compability
    /// </summary>
    public uint SendingFederate { get; set; }
    #endregion

    #region Constructors
    /// <summary>
    /// Constructor
    /// </summary>
    public MessageRetraction() 
    {
      //SerialNumber = 0;
      //SendingFederate = 0;
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="serial"></param>
    /// <param name="sender"></param>
    public MessageRetraction(uint serial, uint sender) 
    {
      Handle = serial;
      SendingFederate = sender;
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="handle"></param>
    public MessageRetraction(uint handle)
    {
      Handle = handle;
    }
    //public MessageRetraction(string serial)
    //{
    //  // !!! OpenRTI does not support event retraction. returns handle numbers greater than uint.MaxValue for the second joined federate
    //  uint no;
    //  if (uint.TryParse(serial.Split('(', ')')[1], out no))
    //    Handle = no;
    //}
    #endregion
  }
}
