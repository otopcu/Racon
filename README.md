RACoN - RTI abstraction component for MS.NET (RACoN)
https://sites.google.com/view/okantopcu/racon
=============================================

Copyright @ Okan Topçu, 2009-2024
otot.support@outlook.com

The RTI abstraction component for MS.NET (RACoN) deals with the HLA runtime infrastructure (RTI) level communication 
in order to access the federation-wide data (actually, the objects and interactions exchanged in federation). 
RTI is middleware broker software that manages the federation execution and object exchange through a federation execution.
The RACoN provides the .NET wrapper classes for the RTI and RTI-specific data structures. 
As it offers more maintainable, robust, and portable methods, developing an abstraction layer (wrapper) over RTI is a popular approach in many HLA-based projects.

## Programmer's Guide
Guide to Distributed Simulation with HLA: https://link.springer.com/book/10.1007/978-3-319-61267-6

## Sample Federations:
Chat Federation (Basic): https://github.com/otopcu/Chat
Chat Federation (DDM): https://github.com/otopcu/Chat/tree/DDM
Strait Traffic Monitoring Simulation: https://github.com/otopcu/STMS

## What's New?
### 0.0.2.5 (20231116):
* Compiled with VS 2022 Enterprise and .NET Framework (Racon: 4.8, Racon.RtiLayer:	4.7.2).
* Added some missing support services: get/set automatic resign directive, enable/disable callbacks, normalize federate handle
•	Added missing declaration management services: unpublish/unsubscribe.
* NotConnected exception is now correctly sets the federate state as NOTCONNECTED.
* Update rate designator is implemented. Now, it is possible to employ smart update reduction. 
* Supports OpenRTI v0.10.0
* Bug fixes.

### 0.0.2.4 (20171010):
* Binary distribution is win64/x64 now.
* Support for all user-supplied tags. Now, RACoN can encode/decode all tags in various datatypes.
* Encoding/decoding mechanism is restructured.

### 0.0.2.3 (20170417):
* HLA1516e Ownership management services are implemented.
* DDM services are re-implemented for HLA Evolved.
* GetDimensionHandle() and GetDimensionName() services are implemented.
* GetFederateHandle() and GetFederateName() services are implemented.
* Multiple FOM modules are supported.
* Some overloads for Create and Join federation execution services are added.
* System.Windows.Forms reference is removed.

## Remarks
Please note that this is a research and an academic library that is not intended for a production environment. 
No support is guaranteed. Use it at your own risk. Also note that the versions do not follow backward compatibility policy. 
This means that a new version may not work with an older version of the library, which may cause loss of work.

You can influence this project by sending your feedback. Please, contact project owner(s) and feel free for any comments, offers, remarks, ideas, etc. 
