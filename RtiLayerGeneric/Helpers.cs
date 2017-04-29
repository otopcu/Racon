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

namespace Racon
{
  /// <summary>
  /// This package presents a generic RTI layer.
  /// </summary>
  /// 
  /// <remarks>
  /// The RTI library, called RtiLayer, is the major portion of the .NET wrapper that abstracts the RTI services and makes transparent the use of both 
  /// •	the various HLA specifications (e.g.DoD HLA 1.3 specification, IEEE 1516-2000 standard, and HLA Evolved and 
  /// •	the various vendor specific RTI implementation for the same specification (e.g. Portico, DMSO RTI 1.3 NG). Another approach to a generic RTI abstraction is to use the dynamic link compatible HLA API standard.
  /// </remarks>
  /// 
  internal class NamespaceDoc
  {
    // Only for documentation of Namespace: Racon.Federation in Sandcastle
  }

  /// <summary>
  /// Callback model - used in connect() service
  /// </summary>
  public enum CallbackModel
  {
    /// <summary>
    /// The RTI shall invoke callbacks immediately when they are available unless the callbacks have been     disabled using the Disable Callbacks service
    /// </summary>
    IMMEDIATE,
    /// <summary>
    /// Callbacks are only made when the federate calls the Evoke Callback/Evoke Multiple Callbacks services, unless callbacks have been disabled using the Disable Callbacks service.
    /// </summary>
    EVOKED
  }
  /// <summary>
  /// Enum Object Class Publication and Subscription Capabilities
  /// </summary>
  public enum PSKind
  {
    /// <summary>
    /// Publish
    /// </summary>
    Publish,
    /// <summary>
    /// Subscribe
    /// </summary>
    Subscribe,
    /// <summary>
    /// Both Publish and Subscribe
    /// </summary>
    PublishSubscribe,
    /// <summary>
    /// Neither Publish or Subscribe
    /// </summary>
    Neither
  };

  /// <summary>
  /// Enum debugging and information level
  /// <![CDATA[ ALL < TRACE < INFO < WARN < ERROR < OFF ]]>
  /// </summary>
  public enum LogLevel
  {
    /// <summary>
    /// All info is provided about RACoN events
    /// </summary>
    ALL = 0,
    /// <summary>
    /// This level provides detailed information about events including handles. Debug Level
    /// </summary>
    TRACE = 1,
    /// <summary>
    /// Only important info is provided.
    /// </summary>
    INFO = 2,
    /// <summary>
    /// Reports incorrect behavior, but RTI can continue. Production level
    /// </summary>
    WARN = 3,
    /// <summary>
    /// Reports application crashes, Racon must stop
    /// </summary>
    ERROR = 4,
    /// <summary>
    /// No information is provided
    /// </summary>
    OFF = 5
  };

}
