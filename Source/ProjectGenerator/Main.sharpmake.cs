using System.IO;
using Sharpmake;

[module: Sharpmake.Include("Game.sharpmake.cs")]
[module: Sharpmake.Include("Editor.sharpmake.cs")]
[module: Sharpmake.Include("Engine.sharpmake.cs")]
[module: Sharpmake.Include("Common.sharpmake.cs")]

public static class Globals
{
    public static bool isEditor = false;
}

[Generate]
class BasicsSolution : Solution
{
    public BasicsSolution()
        : base(typeof(BoxTarget))
    {
        Name = "Game";

        AddTargets(new BoxTarget(BuildType.Game,
            Platform.win64,
            DevEnv.vs2015,
            Optimization.Debug | Optimization.Release));
    }

    [Configure]
    public void ConfigureAll(Solution.Configuration conf, BoxTarget target)
    {
        conf.SolutionPath = @"[solution.SharpmakeCsPath]\\..";

        conf.AddProject<Game>(target);
		conf.AddProject<EngineForGame>(target);
    }
}

[Generate]
class EditorSolution : Solution
{
    public EditorSolution()
        : base(typeof(BoxTarget))
    {
        Name = "Editor";

        AddTargets(new BoxTarget(BuildType.Editor,
            Platform.win64,
            DevEnv.vs2015,
            Optimization.Debug | Optimization.Release, OutputType.Lib, Blob.NoBlob, BuildSystem.MSBuild/*, DotNetFramework.v4_0*/));
    }

    [Configure]
    public void ConfigureAll(Solution.Configuration conf, BoxTarget target)
    {
        Globals.isEditor = true;
        conf.SolutionPath = @"[solution.SharpmakeCsPath]\\..";
		conf.AddProject<Editor>(target);
        conf.AddProject<EngineForEditor>(target);
        Globals.isEditor = false;
    }
}

class EntryPoint
{
    [Main]
    public static void SharpmakeMain(Arguments sharpmakeArgs)
    {
        Builder.Instance.Arguments.AddFragmentMask(BuildType.Editor | BuildType.Game);
        sharpmakeArgs.Generate<BasicsSolution>();
        sharpmakeArgs.Generate<EditorSolution>();
    }
}