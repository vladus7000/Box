using System.IO;
using Sharpmake;

[Generate]
class Engine : Project
{
    public Engine()
    {
        Name = "Engine";

        SourceRootPath = @"[project.SharpmakeCsPath]" + "\\..\\Engine";

        AddTargets(new Target(
            Platform.win64,
            DevEnv.vs2015,
            Optimization.Debug | Optimization.Release,  OutputType.Lib) );
    }

	[Configure]
	public void ConfigureAll(Project.Configuration conf, Target target)
	{
        conf.ProjectPath = Path.Combine("[project.SharpmakeCsPath]", "\\..\\projects");

        if (Globals.isEditor)
        {
            conf.Defines.Add("EDITOR_BUILD");
        }
        else
        {
            conf.Defines.Add("GAME_BUILD");
        }
        conf.IncludePaths.Add(SourceRootPath + "\\Include");
        conf.Output = Project.Configuration.OutputType.Lib;
        //conf.IncludePaths.Add("[project.BasePath]");
        //conf.TargetLibraryPath = "[project.BasePath]/lib";
        conf.IntermediatePath = Path.Combine("[project.SharpmakeCsPath]", "\\..\\..\\Artifacts\\Engine\\intermediate");
        conf.TargetPath = Path.Combine("[project.SharpmakeCsPath]", "\\..\\..\\Artifacts\\Engine\\output");
    }
}