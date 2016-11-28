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
  public class EventRetractionHandle
  {
    #region Properties
    /// <summary>
    ///  Serial number
    /// </summary>
    public uint SerialNumber { get; set; }
    /// <summary>
    ///  Sending federate handle
    /// </summary>
    public uint SendingFederate { get; set; }
    #endregion

    #region Constructors
    /// <summary>
    /// Constructor
    /// </summary>
    public EventRetractionHandle() 
    {
      SerialNumber = 0;
      SendingFederate = 0;
    }
    public EventRetractionHandle(uint serial, uint sender) 
    {
      SerialNumber = serial;
      SendingFederate = sender;
    }
    public EventRetractionHandle(string serial)
    {
      // !!! OpenRTI returns handle numbers greater than uint.MaxValue for the second joined federate
      uint no;
      if (uint.TryParse(serial.Split('(', ')')[1], out no))
        SerialNumber = no;

    }
    #endregion
  }
}
