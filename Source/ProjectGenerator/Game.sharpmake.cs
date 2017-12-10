using System.IO;
using Sharpmake;

[Generate]
class Game : Project
{
    public Game()
    {
        Name = "Game";

        SourceRootPath = @"[project.SharpmakeCsPath]" + "\\..\\Game";
		
        AddTargets(new Target(
            Platform.win64,
            DevEnv.vs2015,
            Optimization.Debug | Optimization.Release));
    }

	[Configure]
	public void ConfigureAll(Project.Configuration conf, Target target)
	{
        conf.ProjectPath = Path.Combine("[project.SharpmakeCsPath]", "\\..\\projects");
        conf.IntermediatePath = Path.Combine("[project.SharpmakeCsPath]", "\\..\\..\\Artifacts\\Game\\intermediate");
        conf.TargetPath = Path.Combine("[project.SharpmakeCsPath]", "\\..\\..\\Artifacts\\Game\\output");

        conf.AddPublicDependency<Engine>(target);

    }
}