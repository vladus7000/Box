using System.IO;
using Sharpmake;

[module: Sharpmake.Include("Game.sharpmake.cs")]
[module: Sharpmake.Include("Editor.sharpmake.cs")]
[module: Sharpmake.Include("Engine.sharpmake.cs")]

public static class Globals
{
    public static bool isEditor = false;
}

[Generate]
class BasicsSolution : Solution
{
    public BasicsSolution()
    {
        Name = "Game";

        AddTargets(new Target(
            Platform.win64,
            DevEnv.vs2015,
            Optimization.Debug | Optimization.Release));
    }

    [Configure]
    public void ConfigureAll(Solution.Configuration conf, Target target)
    {
        conf.SolutionPath = @"[solution.SharpmakeCsPath]\\..";

        conf.AddProject<Game>(target);
		conf.AddProject<Engine>(target);
    }
}

[Generate]
class EditorSolution : Solution
{
    public EditorSolution()
    {
        Name = "Editor";

        AddTargets(new Target(
            Platform.win64,
            DevEnv.vs2015,
            Optimization.Debug | Optimization.Release, OutputType.Lib, Blob.NoBlob, BuildSystem.MSBuild/*, DotNetFramework.v4_0*/));
    }

    [Configure]
    public void ConfigureAll(Solution.Configuration conf, Target target)
    {
        Globals.isEditor = true;
        conf.SolutionPath = @"[solution.SharpmakeCsPath]\\..";
		conf.AddProject<Editor>(target);
        conf.AddProject<Engine>(target);
        Globals.isEditor = false;
    }

	[Main]
	public static void SharpmakeMain(Arguments sharpmakeArgs)
	{
		sharpmakeArgs.Generate<BasicsSolution>();
		sharpmakeArgs.Generate<EditorSolution>();
	}
}