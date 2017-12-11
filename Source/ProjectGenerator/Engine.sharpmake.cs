using System.IO;
using Sharpmake;
[module: Sharpmake.Include("Common.sharpmake.cs")]

[Generate]
class Engine : Project
{
    public Engine()
        : base(typeof(BoxTarget))
    {
        Name = "Engine";

        SourceRootPath = @"[project.SharpmakeCsPath]" + "\\..\\Engine";

        AddTargets(new BoxTarget(BuildType.Game,
            Platform.win64,
            DevEnv.vs2015,
            Optimization.Debug | Optimization.Release,  OutputType.Lib) );
        AddTargets(new BoxTarget(BuildType.Editor,
            Platform.win64,
            DevEnv.vs2015,
            Optimization.Debug | Optimization.Release, OutputType.Lib));
    }

	[Configure]
	public void ConfigureAll(Project.Configuration conf, BoxTarget target)
	{
        conf.ProjectPath = Path.Combine("[project.SharpmakeCsPath]", "\\..\\projects");

        if (target.BuildType == BuildType.Editor)
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