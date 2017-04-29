// ***************************************************************************
//                          LogManager
//		begin                : Nov 27, 2016
// ***************************************************************************

/* 
RACoN - RTI abstraction component for MS.NET (RACoN)
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

using Racon.RtiLayer;
using System;
using System.Collections.ObjectModel; // ObservableCollection
using System.ComponentModel;
using System.Text;
using System.Threading.Tasks;

namespace Racon.Logger
{
  /// <summary>
  /// RACoN log manager
  /// </summary>
  public class LogManager
  {
    #region Fields
    private LogLevel _LogLevel;
    #endregion

    #region Properties
    /// <summary>
    /// Current Log Level. When it is set, it reports version, RTI native library, and current log level via <see cref="Racon.CGenericFederate.StatusMessage"/>.
    /// </summary>
    public LogLevel LogLevel
    {
      get { return _LogLevel; }
      set
      {
        _LogLevel = value;
        // report initialization state
        Add(Properties.Resources.LibName + " v" + Properties.Resources.LibVersion, LogLevel.INFO);
        Add("Log Level: " + value.ToString(), LogLevel.INFO);
      }
    }
    /// <summary>
    /// Keeps all the log entries
    /// </summary>
    internal ObservableCollection<LogEntry> Log;
    #endregion

    #region Constructors
    /// <summary>
    /// LogManager constructor
    /// </summary>
    public LogManager()
    {
      Log = new ObservableCollection<LogEntry>();
      _LogLevel = LogLevel.OFF;
    }
    #endregion

    #region Methods
    /// <summary>
    /// Adds a log entry to the log.
    /// </summary>
    public void Add(string message, LogLevel level = LogLevel.TRACE)
    {
      LogEntry entry = new LogEntry(level, message);
      Log.Add(entry);
    }
   
    /// <summary>
    /// Dumbs all log entries in form of a string
    /// </summary>
    public override string ToString()
    {
      StringBuilder str = new StringBuilder();
      foreach (LogEntry item in Log)
      {
        // convert every message in log format
        if (str.Capacity <= str.MaxCapacity) // check the max capacity
          str.Append(item.TimeStamp + ":\t[" + item.Level.ToString() + "]\t" + item.Message + Environment.NewLine);
      }
      str.Append("Length:" + str.Length + " chars");
      return str.ToString();
    }
    #endregion
  }
}
