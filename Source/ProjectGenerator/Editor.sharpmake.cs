using System.IO;
using Sharpmake;
[module: Sharpmake.Include("Common.sharpmake.cs")]

[Generate]
class Editor : Project
{
    public Editor()
        : base(typeof(BoxTarget))
    {
        Name = "Editor";

        SourceRootPath = @"[project.SharpmakeCsPath]" + "\\..\\Editor";
		
        AddTargets(new BoxTarget(BuildType.Editor,
            Platform.win64,
            DevEnv.vs2015,
            Optimization.Debug | Optimization.Release, OutputType.Lib, Blob.NoBlob, BuildSystem.MSBuild/*, DotNetFramework.v4_0*/));
    }

	[Configure]
	public void ConfigureAll(Project.Configuration conf, BoxTarget target)
	{
        conf.ProjectPath = Path.Combine("[project.SharpmakeCsPath]", "\\..\\projects\\Editor");
        conf.IntermediatePath = Path.Combine("[project.SharpmakeCsPath]", "\\..\\..\\Artifacts\\Editor\\intermediate\\");
        conf.TargetPath = Path.Combine("[project.SharpmakeCsPath]", "\\..\\..\\Artifacts\\Editor\\output");

        conf.Options.Add(Options.Vc.General.TreatWarningsAsErrors.Disable);
        conf.Options.Add(Options.Vc.Compiler.RTTI.Enable);
        conf.Options.Add(Options.Vc.Compiler.Exceptions.EnableWithSEH);
        conf.Options.Add(Options.Vc.Compiler.RuntimeChecks.Default);
        conf.Options.Add(Options.Vc.General.CommonLanguageRuntimeSupport.ClrSupport);

        conf.Defines.Add("EDITOR_BUILD");

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

        conf.ReferencesByPath.Add(@"[project.SharpmakeCsPath]\\..\\..\\Lib\\WinFormsUI\\WeifenLuo.WinFormsUI.Docking.dll");

        //TODO: test it
        if (conf.VcxprojUserFile == null)
        {
            conf.VcxprojUserFile = new Project.Configuration.VcxprojUserFileSettings();
        }
        conf.VcxprojUserFile.LocalDebuggerWorkingDirectory = @"[project.SharpmakeCsPath]\..\..\IDEs";


        if (target.Optimization == Optimization.Debug)
        {
            conf.Options.Add(Options.Vc.Compiler.RuntimeLibrary.MultiThreadedDebugDLL);
        }
        else
        {
            conf.Options.Add(Options.Vc.Compiler.RuntimeLibrary.MultiThreadedDLL);
        }

        conf.AddPublicDependency<EngineForEditor>(target);
    }
}