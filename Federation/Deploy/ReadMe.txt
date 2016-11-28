To modify package metadata:
In project, modify RACoN.nuspec

To create and update manually the NuGet package:
1. Go to Deploy library
2. in CMD: nuget pack ../FederationLayer.csproj -IncludeReferencedProjects -Properties Configuration=Release

To create package (.nupkg) in each built:
Add DeCo.csproj

  <PropertyGroup>
    <PostBuildEvent>
      $(ProjectDir)Deploy/NuGet.exe pack $(ProjectPath) -OutputDirectory $(ProjectDir)Deploy -IncludeReferencedProjects -Properties Configuration=$(Configuration)
    </PostBuildEvent>
  </PropertyGroup>

To Check the package:
1. Open .nupkg file with 7Zip

Upload to Nuget.org:
www.nuget.org

To install RACoN:
1. in VS solution, open Tools/NuGet Package Manager/Package Manager Console
2. PM> Install-Package RACoN

