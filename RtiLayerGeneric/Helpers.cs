using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Racon.RtiLayer
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
  /// Enum Object Class Publication and Subscription Capabilities
  /// </summary>
  public enum PSKind
  {
    Publish,
    Subscribe,
    PublishSubscribe,
    Neither
  };
}
