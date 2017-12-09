using System.IO;
using Sharpmake;

[Generate]
class Editor : Project
{
    public Editor()
    {
        Name = "Editor";

        SourceRootPath = @"[project.SharpmakeCsPath]" + "\\..\\Editor";
		
        AddTargets(new Target(
            Platform.win64,
            DevEnv.vs2015,
            Optimization.Debug | Optimization.Release, OutputType.Lib, Blob.NoBlob, BuildSystem.MSBuild/*, DotNetFramework.v4_0*/));
    }

	[Configure]
	public void ConfigureAll(Project.Configuration conf, Target target)
	{
        conf.ProjectPath = Path.Combine("[project.SharpmakeCsPath]", "\\..\\projects");
        conf.IntermediatePath = Path.Combine("[project.SharpmakeCsPath]", "\\..\\..\\Artifacts\\Editor\\intermediate\\");
        conf.TargetPath = Path.Combine("[project.SharpmakeCsPath]", "\\..\\..\\Artifacts\\Editor\\output");

        conf.Options.Add(Options.Vc.General.TreatWarningsAsErrors.Disable);
        conf.Options.Add(Options.Vc.Compiler.RTTI.Enable);
        conf.Options.Add(Options.Vc.Compiler.Exceptions.EnableWithSEH);
        conf.Options.Add(Options.Vc.Compiler.RuntimeChecks.Default);
        conf.Options.Add(Options.Vc.General.CommonLanguageRuntimeSupport.ClrSupport);

        conf.ReferencesByName.Add(
                    "Microsoft.VisualC",
                    "PresentationFramework",
                    "PresentationCore",
                    "System",
                    "System.Configuration",
                    "System.Data",
                    "System.Design",
                    "System.Drawing",
                    "System.Drawing.Design",
                    "System.Xml",
                    "System.Xaml",
                    "System.Windows.Forms",
                    "WindowsBase",
                    "WindowsFormsIntegration");

        if (target.Optimization == Optimization.Debug)
        {
            conf.Options.Add(Options.Vc.Compiler.RuntimeLibrary.MultiThreadedDebugDLL);
        }
        else
        {
            conf.Options.Add(Options.Vc.Compiler.RuntimeLibrary.MultiThreadedDLL);
        }

        conf.AddPrivateDependency<Engine>(target);
    }
}