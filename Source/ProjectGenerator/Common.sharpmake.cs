using System;
using System.IO;
using Sharpmake;

[Fragment, Flags]
public enum BuildType
{
    Editor = 0x01,
    Game = 0x02
}

public class BoxTarget : ITarget
{
    public Optimization Optimization;
    public Platform Platform;
    public BuildSystem BuildSystem;
    public DevEnv DevEnv;
    public OutputType OutputType;
    public DotNetFramework Framework;
    public Blob Blob;
    public BuildType BuildType;
    public override string Name
    {
        get { return Optimization.ToString(); }
    }

    public BoxTarget() { }

    public BoxTarget(
        BuildType buildType,
        Platform platform,
        DevEnv devEnv,
        Optimization optimization,
        OutputType outputType = OutputType.Lib,
        Blob blob = Blob.NoBlob,
        BuildSystem buildSystem = BuildSystem.MSBuild,
        DotNetFramework framework = DotNetFramework.v3_5
    )
    {
        BuildType = buildType;
        Platform = platform;
        DevEnv = devEnv;
        Optimization = optimization;
        OutputType = outputType;
        Framework = framework;
        BuildSystem = buildSystem;
        Blob = blob;
    }
}