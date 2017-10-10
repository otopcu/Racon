RACoN - RTI abstraction component for MS.NET (RACoN)
https://sites.google.com/site/okantopcu/racon
=============================================


Copyright @ Okan Topçu, 2009-2016
otot.support@outlook.com

The RTI abstraction component for MS.NET (RACoN) deals with the HLA runtime infrastructure (RTI) level communication 
in order to access the federation-wide data (actually, the objects and interactions exchanged in federation). 
RTI is middleware broker software that manages the federation execution and object exchange through a federation execution.
The RACoN provides the .NET wrapper classes for the RTI and RTI-specific data structures. 
As it offers more maintainable, robust, and portable methods, developing an abstraction layer (wrapper) over RTI 
is a popular approach in many HLA-based projects.

What's New?
0.0.2.4 (07.06.17):
Support for all user-supplied tags. Now, RACoN can encode/decode all tags in various datatypes.
Encoding/decoding mechanism is restructured.

0.0.2.3 (29.04.17):
HLA1516e Ownership management services are implemented.
DDM services are re-implemented for HLA Evolved.
GetDimensionHandle() and GetDimensionName() services are implemented.
GetFederateHandle() and GetFederateName() services are implemented.
Multiple FOM modules are supported.
Some overloads for Create and Join federation execution services are added.
System.Windows.Forms reference is removed.

Please note that this is a research and an academic library that is not intended for a production environment. 
No support is guaranteed. Use it at your own risk. Also note that the versions do not follow backward compatibility policy. 
This means that a new version may not work with an older version of the library, which may cause loss of work.

You can influence this project by sending your feedback. Please, contact project owner(s) and feel free 
for any comments, offers, remarks, ideas, etc. 
