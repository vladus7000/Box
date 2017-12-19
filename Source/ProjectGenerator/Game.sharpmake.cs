using System.IO;
using Sharpmake;
[module: Sharpmake.Include("Common.sharpmake.cs")]

[Generate]
class Game : Project
{
    public Game()
        : base(typeof(BoxTarget))
    {
        Name = "Game";

        SourceRootPath = @"[project.SharpmakeCsPath]" + "\\..\\Game";
		
        AddTargets(new BoxTarget(BuildType.Game,
            Platform.win64,
            DevEnv.vs2015,
            Optimization.Debug | Optimization.Release));
    }

	[Configure]
	public void ConfigureAll(Project.Configuration conf, BoxTarget target)
	{
        conf.PrecompHeader = "StdAfx.hpp";
        conf.PrecompSource = "StdAfx.cpp";

        conf.Defines.Add("EDITOR_BUILD");

        conf.ProjectPath = Path.Combine("[project.SharpmakeCsPath]", "\\..\\projects\\Game");
        conf.IntermediatePath = Path.Combine("[project.SharpmakeCsPath]", "\\..\\..\\Artifacts\\Game\\intermediate");
        conf.TargetPath = Path.Combine("[project.SharpmakeCsPath]", "\\..\\..\\Artifacts\\Game\\output");

        //TODO: test it
        if (conf.VcxprojUserFile == null)
        {
            conf.VcxprojUserFile = new Project.Configuration.VcxprojUserFileSettings();
        }
        conf.VcxprojUserFile.LocalDebuggerWorkingDirectory = @"[project.SharpmakeCsPath]\..\..\Game";


        conf.AddPublicDependency<EngineForGame>(target);

    }
}