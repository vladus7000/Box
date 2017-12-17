using System.IO;
using Sharpmake;
[module: Sharpmake.Include("Common.sharpmake.cs")]

//[Generate]
class Engine : Project
{
    public Engine()
        : base(typeof(BoxTarget))
    {
        Name = "Engine";
        SourceRootPath = @"[project.SharpmakeCsPath]" + "\\..\\Engine\\Source";
    }

	[Configure]
	public void BasicConfigure(Project.Configuration conf, BoxTarget target)
	{
        conf.PrecompHeader = "StdAfx.hpp";
        conf.PrecompSource = "StdAfx.cpp";

        conf.IncludePaths.Add(SourceRootPath + "\\..\\Include");
        conf.Output = Project.Configuration.OutputType.Lib;
        //conf.IncludePaths.Add("[project.BasePath]");
        //conf.TargetLibraryPath = "[project.BasePath]/lib";
        conf.IntermediatePath = Path.Combine("[project.SharpmakeCsPath]", "\\..\\..\\Artifacts\\Engine\\intermediate");
        conf.TargetPath = Path.Combine("[project.SharpmakeCsPath]", "\\..\\..\\Artifacts\\Engine\\output");
    }
}

[Generate]
class EngineForGame : Engine
{
    public EngineForGame()
    {
        AddTargets(new BoxTarget(BuildType.Game,
        Platform.win64,
        DevEnv.vs2015,
        Optimization.Debug | Optimization.Release, OutputType.Lib));
    }

    [Configure]
    public void ConfigureAll(Project.Configuration conf, BoxTarget target)
    {
        BasicConfigure(conf, target);
        conf.ProjectPath = Path.Combine("[project.SharpmakeCsPath]", "\\..\\projects\\Game");
        conf.Defines.Add("GAME_BUILD");
    }
}

[Generate]
class EngineForEditor : Engine
{
    public EngineForEditor()
    {
        AddTargets(new BoxTarget(BuildType.Editor,
        Platform.win64,
        DevEnv.vs2015,
        Optimization.Debug | Optimization.Release, OutputType.Lib));
    }

    [Configure]
    public void ConfigureAll(Project.Configuration conf, BoxTarget target)
    {
        BasicConfigure(conf, target);
        conf.ProjectPath = Path.Combine("[project.SharpmakeCsPath]", "\\..\\projects\\Editor");
        conf.Defines.Add("EDITOR_BUILD");
    }
}

[Generate]
class TestEngine : Project
{
    public TestEngine()
        : base(typeof(BoxTarget))
    {
        SourceRootPath = @"[project.SharpmakeCsPath]" + "\\..\\Engine\\Tests";
        AddTargets(new BoxTarget(BuildType.Game,
        Platform.win64,
        DevEnv.vs2015,
        Optimization.Debug | Optimization.Release));
        SourceFiles.Add(@"[project.SharpmakeCsPath]\\..\\..\\Lib\\googletest\\googletest\\src\\gtest-all.cc");
    }

    [Configure]
    public void ConfigureAll(Project.Configuration conf, BoxTarget target)
    {
        conf.ProjectPath = Path.Combine("[project.SharpmakeCsPath]", "\\..\\projects\\TestEngine");

        conf.IncludePaths.Add(@"[project.SharpmakeCsPath]\\..\\..\\Lib\\googletest\\googletest\\include");
        conf.IncludePaths.Add(@"[project.SharpmakeCsPath]\\..\\..\\Lib\\googletest\\googletest");
        //conf.Output = Project.Configuration.OutputType.Lib;
        //conf.IncludePaths.Add("[project.BasePath]");
        //conf.TargetLibraryPath = "[project.BasePath]/lib";
        conf.IntermediatePath = Path.Combine("[project.SharpmakeCsPath]", "\\..\\..\\Artifacts\\TestEngine\\intermediate");
        conf.TargetPath = Path.Combine("[project.SharpmakeCsPath]", "\\..\\..\\Artifacts\\TestEngine\\output");

        conf.AddPublicDependency<EngineForGame>(target);
    }
}