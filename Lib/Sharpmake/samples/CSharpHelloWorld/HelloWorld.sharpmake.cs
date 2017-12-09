﻿using Sharpmake;
using System;

namespace CSharpHelloWorld
{
    [Sharpmake.Generate]
    public class HelloWorld : CSharpProject
    {
        public HelloWorld()
        {
            AddTargets(new Target(
            Platform.anycpu,
            DevEnv.vs2013,
            Optimization.Debug | Optimization.Release,
            OutputType.Dll,
            Blob.NoBlob,
            BuildSystem.MSBuild,
            DotNetFramework.v4_5));

            RootPath = @"[project.SharpmakeCsPath]\projects\[project.Name]";

            // This Path will be used to get all SourceFiles in this Folder and all subFolders
            SourceRootPath = @"[project.SharpmakeCsPath]\codebase\[project.Name]";
            AssemblyName = "the other name";
        }

        [Configure()]
        public virtual void ConfigureAll(Configuration conf, Target target)
        {
            conf.ProjectFileName = "[project.Name].[target.DevEnv].[target.Framework]";
            conf.ProjectPath = @"[project.RootPath]";

            conf.Options.Add(Sharpmake.Options.CSharp.TreatWarningsAsErrors.Enabled);
        }
    }

    [Sharpmake.Generate]
    public class HelloWorldSolution : CSharpSolution
    {
        public HelloWorldSolution()
        {
            AddTargets(new Target(
            Platform.anycpu,
            DevEnv.vs2013,
            Optimization.Debug | Optimization.Release,
            OutputType.Dll,
            Blob.NoBlob,
            BuildSystem.MSBuild,
            DotNetFramework.v4_5));
        }

        [Configure()]
        public void ConfigureAll(Configuration conf, Target target)
        {
            conf.SolutionFileName = String.Format("{0}.{1}.{2}",
                                                  Name,
                                                  "[target.DevEnv]",
                                                  "[target.Framework]");
            conf.SolutionPath = @"[solution.SharpmakeCsPath]\projects\";

            conf.AddProject<HelloWorld>(target);
        }

        [Sharpmake.Main]
        public static void SharpmakeMain(Sharpmake.Arguments arguments)
        {
            arguments.Generate<HelloWorldSolution>();
        }
    }
}
