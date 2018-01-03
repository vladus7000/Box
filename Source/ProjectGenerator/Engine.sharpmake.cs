using System.IO;
using Sharpmake;
[module: Sharpmake.Include("Common.sharpmake.cs")]

[Generate]
class Zlib : Project
{
    public Zlib()
        : base(typeof(BoxTarget))
    {
        AddTargets(new BoxTarget(BuildType.Game,
        Platform.win64,
        DevEnv.vs2015,
        Optimization.Debug | Optimization.Release));
        SourceFiles.Add(@"[project.SharpmakeCsPath]\\..\\..\\Lib\\zlib\\adler32.c");
        SourceFiles.Add(@"[project.SharpmakeCsPath]\\..\\..\\Lib\\zlib\\compress.c");
        SourceFiles.Add(@"[project.SharpmakeCsPath]\\..\\..\\Lib\\zlib\\crc32.c");
        SourceFiles.Add(@"[project.SharpmakeCsPath]\\..\\..\\Lib\\zlib\\deflate.c");
        SourceFiles.Add(@"[project.SharpmakeCsPath]\\..\\..\\Lib\\zlib\\gzclose.c");
        SourceFiles.Add(@"[project.SharpmakeCsPath]\\..\\..\\Lib\\zlib\\gzlib.c");
        SourceFiles.Add(@"[project.SharpmakeCsPath]\\..\\..\\Lib\\zlib\\gzread.c");
        SourceFiles.Add(@"[project.SharpmakeCsPath]\\..\\..\\Lib\\zlib\\gzwrite.c");
        SourceFiles.Add(@"[project.SharpmakeCsPath]\\..\\..\\Lib\\zlib\\inflate.c");
        SourceFiles.Add(@"[project.SharpmakeCsPath]\\..\\..\\Lib\\zlib\\infback.c");
        SourceFiles.Add(@"[project.SharpmakeCsPath]\\..\\..\\Lib\\zlib\\inftrees.c");
        SourceFiles.Add(@"[project.SharpmakeCsPath]\\..\\..\\Lib\\zlib\\inffast.c");
        SourceFiles.Add(@"[project.SharpmakeCsPath]\\..\\..\\Lib\\zlib\\trees.c");
        SourceFiles.Add(@"[project.SharpmakeCsPath]\\..\\..\\Lib\\zlib\\uncompr.c");
        SourceFiles.Add(@"[project.SharpmakeCsPath]\\..\\..\\Lib\\zlib\\zutil.c");
    }

    [Configure]
    public void ConfigureAll(Project.Configuration conf, BoxTarget target)
    {
        conf.ProjectPath = Path.Combine("[project.SharpmakeCsPath]", "\\..\\projects\\Zlib");

        conf.IncludePaths.Add(@"[project.SharpmakeCsPath]\\..\\..\\Lib\\zlib");
        conf.Output = Project.Configuration.OutputType.Lib;
        //conf.IncludePaths.Add("[project.BasePath]");
        //conf.TargetLibraryPath = "[project.BasePath]/lib";
        conf.IntermediatePath = Path.Combine("[project.SharpmakeCsPath]", "\\..\\..\\Artifacts\\Zlib\\intermediate");
        conf.TargetPath = Path.Combine("[project.SharpmakeCsPath]", "\\..\\..\\Artifacts\\Zlib\\output");
    }
}

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
        conf.IncludePaths.Add(@"[project.SharpmakeCsPath]\\..\\..\\Lib");

        conf.Output = Project.Configuration.OutputType.Lib;
        conf.IntermediatePath = Path.Combine("[project.SharpmakeCsPath]", "\\..\\..\\Artifacts\\Engine\\intermediate");
        conf.TargetPath = Path.Combine("[project.SharpmakeCsPath]", "\\..\\..\\Artifacts\\Engine\\output");
        //conf.IncludePaths.Add("[project.BasePath]");
        //conf.TargetLibraryPath = "[project.BasePath]/lib";
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
        //conf.IntermediatePath = Path.Combine("[project.SharpmakeCsPath]", "\\..\\..\\Artifacts\\Game\\intermediate"); test it
        //conf.TargetPath = Path.Combine("[project.SharpmakeCsPath]", "\\..\\..\\Artifacts\\Game\\output");
        conf.Defines.Add("GAME_BUILD");

        conf.AddPublicDependency<Zlib>(target);
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
        conf.IntermediatePath = Path.Combine("[project.SharpmakeCsPath]", "\\..\\..\\Artifacts\\Editor\\intermediate");
        conf.TargetPath = Path.Combine("[project.SharpmakeCsPath]", "\\..\\..\\Artifacts\\Editor\\output");
        conf.Defines.Add("EDITOR_BUILD");

        conf.AddPublicDependency<Zlib>(target);
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