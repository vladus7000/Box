﻿// Copyright (c) 2017 Ubisoft Entertainment
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using Sharpmake.Generators.FastBuild;

namespace Sharpmake.Generators.VisualStudio
{
    public class ProjectOptionsGenerator
    {
        class ProjectOptionsGenerationContext
        {
            private readonly Project.Configuration _projectConfiguration;

            public string OutputDirectoryRelative { get; set; }
            public string OutputLibraryDirectoryRelative { get; set; }
            public string IntermediateDirectoryRelative { get; set; }
            public string TargetName { get; set; }
            public bool HasClrSupport { get; set; }
            public EnvironmentVariableResolver Resolver { get; }
            public string PlatformLibraryExtension { get; }
            public string PlatformOutputLibraryExtension { get; }
            public string PlatformPrefixExtension { get; }
            public string BaseProjectPath { get; }
            public IPlatformDescriptor PlatformDescriptor { get; }
            public IPlatformVcxproj PlatformVcxproj { get; }

            public ProjectOptionsGenerationContext(Project.Configuration conf, params VariableAssignment[] resolverParams)
            {
                _projectConfiguration = conf;
                Resolver = PlatformRegistry.Get<IPlatformDescriptor>(conf.Platform).GetPlatformEnvironmentResolver(resolverParams);

                PlatformDescriptor = PlatformRegistry.Get<IPlatformDescriptor>(conf.Platform);
                PlatformVcxproj = PlatformRegistry.Get<IPlatformVcxproj>(conf.Platform);

                string platformLibraryExtension = ".lib";
                string platformOutputLibraryExtension = ".lib";
                string platformPrefixExtension = string.Empty;
                PlatformVcxproj.SetupPlatformLibraryOptions(ref platformLibraryExtension, ref platformOutputLibraryExtension, ref platformPrefixExtension);

                PlatformLibraryExtension = platformLibraryExtension;
                PlatformOutputLibraryExtension = platformOutputLibraryExtension;
                PlatformPrefixExtension = platformPrefixExtension;

                BaseProjectPath = Bff.GetMasterBffPath(conf);
            }
        }

        internal class VcxprojCmdLineOptions : Dictionary<string, string>
        {
        }

        private static string GetPlatformStringDefineQuot(Platform platform)
        {
            return @"&quot;";
        }

        private string GetPlatformStringResourceDefineQuote(Platform platform)
        {
            return @"\&quot;";
        }

        internal void GenerateOptions(IGenerationContext context)
        {
            var optionsContext = new ProjectOptionsGenerationContext(context.Configuration,
                new VariableAssignment("project", context.Project),
                new VariableAssignment("target", context.Configuration),
                new VariableAssignment("conf", context.Configuration));

            GenerateGeneralOptions(context, optionsContext);
            GenerateCompilerOptions(context, optionsContext);
            GenerateLinkerOptions(context, optionsContext);
            GenerateManifestToolOptions(context, optionsContext);
            GeneratePostBuildOptions(context, optionsContext);
        }

        private void GenerateGeneralOptions(IGenerationContext context, ProjectOptionsGenerationContext optionsContext)
        {
            DevEnv visualVersion = context.Configuration.Target.GetFragment<DevEnv>();

            // Default defines, includes, libraries...
            context.Options.ExplicitDefines.AddRange(optionsContext.PlatformVcxproj.GetImplicitlyDefinedSymbols(context));

            // Set whatever VS needs to delete when you run the Clean command.
            optionsContext.PlatformVcxproj.SetupDeleteExtensionsOnCleanOptions(context);

            if (context.Configuration.DefaultOption == Options.DefaultTarget.Debug)
            {
                context.SelectOption
                (
                    Options.Option(Options.Vc.Compiler.RuntimeLibrary.MultiThreaded, () => { }),
                    Options.Option(Options.Vc.Compiler.RuntimeLibrary.MultiThreadedDebug, () => context.Options.ExplicitDefines.Add("_DEBUG")),
                    Options.Option(Options.Vc.Compiler.RuntimeLibrary.MultiThreadedDLL, () => { }),
                    Options.Option(Options.Vc.Compiler.RuntimeLibrary.MultiThreadedDebugDLL, () => context.Options.ExplicitDefines.Add("_DEBUG"))
                );
            }
            else // Release
            {
                context.Options.ExplicitDefines.Add("NDEBUG");
            }

            //Output
            //    Application                             Project.ProjectConfiguration.ConfigurationType="1"
            //    Dll                                     Project.ProjectConfiguration.ConfigurationType="2"             /D "_WINDLL"                            /DLL
            //    Lib                                     Project.ProjectConfiguration.ConfigurationType="4"
            SelectConfigurationTypeOption(context);

            context.Options.ExplicitDefines.AddRange(optionsContext.PlatformVcxproj.GetImplicitlyDefinedSymbols(context));

            optionsContext.OutputDirectoryRelative = Util.PathGetRelative(context.ProjectDirectory, context.Configuration.TargetPath);
            optionsContext.OutputLibraryDirectoryRelative = Util.PathGetRelative(context.ProjectDirectory, context.Configuration.TargetLibraryPath);
            if (context.Configuration.Output == Project.Configuration.OutputType.Lib)
                context.Options["OutputDirectory"] = optionsContext.OutputLibraryDirectoryRelative;
            else
                context.Options["OutputDirectory"] = optionsContext.OutputDirectoryRelative;


            //IntermediateDirectory
            optionsContext.IntermediateDirectoryRelative = Util.PathGetRelative(context.ProjectDirectory, context.Configuration.IntermediatePath);
            context.Options["IntermediateDirectory"] = optionsContext.IntermediateDirectoryRelative;
            context.CommandLineOptions["IntermediateDirectory"] = Util.GetConvertedRelativePath(context.ProjectDirectory, optionsContext.IntermediateDirectoryRelative, optionsContext.BaseProjectPath, true, context.Project.RootPath);

            optionsContext.TargetName = context.Configuration.TargetFileFullName;
            context.Options["LayoutDir"] = !string.IsNullOrEmpty(context.Configuration.LayoutDir) ? context.Configuration.LayoutDir : FileGeneratorUtilities.RemoveLineTag;
            context.Options["PullMappingFile"] = !string.IsNullOrEmpty(context.Configuration.PullMappingFile) ? context.Configuration.PullMappingFile : FileGeneratorUtilities.RemoveLineTag;
            context.Options["PullTemporaryFolder"] = !string.IsNullOrEmpty(context.Configuration.PullTemporaryFolder) ? context.Configuration.PullTemporaryFolder : FileGeneratorUtilities.RemoveLineTag;

            if (!string.IsNullOrEmpty(context.Configuration.LayoutExtensionFilter))
                context.Options["LayoutExtensionFilter"] = context.Configuration.LayoutExtensionFilter;
            else
                context.Options["LayoutExtensionFilter"] = context.Configuration.IsFastBuild ? "*.pdb;*.ilk;*.exp;*.lib;*.winmd;*.appxrecipe;*.map;*.pl;*.bat;*.txt;*.log;*.pel2;*.zip;*.etl;*.vspx" : FileGeneratorUtilities.RemoveLineTag;

            // This should normally be set with the KitsRootPaths class, but this allows the coder to force a platform version.
            context.SelectOptionWithFallback(
                () => { context.Options["WindowsTargetPlatformVersion"] = FileGeneratorUtilities.RemoveLineTag; },
                Options.Option(Options.Vc.General.WindowsTargetPlatformVersion.v8_1, () => { context.Options["WindowsTargetPlatformVersion"] = "8.1"; }),
                Options.Option(Options.Vc.General.WindowsTargetPlatformVersion.v10_0_10240_0, () => { context.Options["WindowsTargetPlatformVersion"] = "10.0.10240.0"; }),
                Options.Option(Options.Vc.General.WindowsTargetPlatformVersion.v10_0_10586_0, () => { context.Options["WindowsTargetPlatformVersion"] = "10.0.10586.0"; }),
                Options.Option(Options.Vc.General.WindowsTargetPlatformVersion.v10_0_14393_0, () => { context.Options["WindowsTargetPlatformVersion"] = "10.0.14393.0"; }),
                Options.Option(Options.Vc.General.WindowsTargetPlatformVersion.v10_0_15063_0, () => { context.Options["WindowsTargetPlatformVersion"] = "10.0.15063.0"; })
                );
        }

        private static void SelectConfigurationTypeOption(IGenerationContext context)
        {
            context.CommandLineOptions["ConfigurationType"] = FileGeneratorUtilities.RemoveLineTag;
            switch (context.Configuration.Output)
            {
                case Project.Configuration.OutputType.Exe:
                case Project.Configuration.OutputType.DotNetConsoleApp:
                case Project.Configuration.OutputType.DotNetWindowsApp:
                    {
                        context.Options["ConfigurationType"] = context.Configuration.IsFastBuild ? "Makefile" : "Application";
                    }
                    break;
                case Project.Configuration.OutputType.Dll:
                case Project.Configuration.OutputType.DotNetClassLibrary:
                    {
                        if (!PlatformRegistry.Get<IPlatformDescriptor>(context.Configuration.Platform).HasSharedLibrarySupport)
                        {
                            throw new Error($"Current platform {context.Configuration.Platform} doesn't support shared lib output type: Project {context.Project.GetType()} conf {context.Configuration.Target}");
                        }
                        context.Options["ConfigurationType"] = context.Configuration.IsFastBuild ? "Makefile" : "DynamicLibrary";
                        context.CommandLineOptions["ConfigurationType"] = @"/D""_WINDLL""";
                    }
                    break;
                case Project.Configuration.OutputType.Lib:
                    context.Options["ConfigurationType"] = context.Configuration.IsFastBuild ? "Makefile" : "StaticLibrary";
                    break;
                case Project.Configuration.OutputType.Utility:
                    context.Options["ConfigurationType"] = "Utility";
                    break;
            }
        }

        private void GenerateCompilerOptions(IGenerationContext context, ProjectOptionsGenerationContext optionsContext)
        {
            if (context.Configuration.ForcedIncludes.Count > 0)
            {
                context.Options["ForcedIncludeFiles"] = context.Configuration.ForcedIncludes.JoinStrings(";");
                StringBuilder result = new StringBuilder();
                foreach (var forcedInclude in context.Configuration.ForcedIncludes)
                    result.Append(@"/FI""" + forcedInclude + @""" ");
                result.Remove(result.Length - 1, 1);
                context.CommandLineOptions["ForcedIncludeFiles"] = result.ToString();
            }
            else
            {
                context.Options["ForcedIncludeFiles"] = FileGeneratorUtilities.RemoveLineTag;
                context.CommandLineOptions["ForcedIncludeFiles"] = FileGeneratorUtilities.RemoveLineTag;
            }

            if (optionsContext.PlatformDescriptor.IsUsingClang)
            {
                context.Options["CharacterSet"] = FileGeneratorUtilities.RemoveLineTag;
                context.CommandLineOptions["CharacterSet"] = FileGeneratorUtilities.RemoveLineTag;
            }
            else
            {
                //Options.Vc.General.CharacterSet.
                //    NotSet                                  CharacterSet="0"
                //    UseUnicodeCharaterSet                   Project.ProjectConfiguration.CharacterSet="1"                  /D "_UNICODE" /D "UNICODE"
                //    UseMultiByteCharaterSet                 Project.ProjectConfiguration.CharacterSet="2"                  /D "_MBCS"
                context.SelectOption
                (
                Options.Option(Options.Vc.General.CharacterSet.Default, () => { context.Options["CharacterSet"] = "NotSet"; context.CommandLineOptions["CharacterSet"] = FileGeneratorUtilities.RemoveLineTag; }),
                Options.Option(Options.Vc.General.CharacterSet.Unicode, () => { context.Options["CharacterSet"] = "Unicode"; context.CommandLineOptions["CharacterSet"] = @"/D""_UNICODE"" /D""UNICODE"""; }),
                Options.Option(Options.Vc.General.CharacterSet.MultiByte, () => { context.Options["CharacterSet"] = "MultiByte"; context.CommandLineOptions["CharacterSet"] = @"/D""_MBCS"""; })
                );
            }

            // Compiler section

            //Options.Vc.General.CommonLanguageRuntimeSupport.
            context.SelectOption
            (
            Options.Option(Options.Vc.General.CommonLanguageRuntimeSupport.NoClrSupport, () => { context.Options["CLRSupport"] = FileGeneratorUtilities.RemoveLineTag; context.CommandLineOptions["CLRSupport"] = FileGeneratorUtilities.RemoveLineTag; }),
            Options.Option(Options.Vc.General.CommonLanguageRuntimeSupport.ClrSupport, () => { context.Options["CLRSupport"] = "true"; context.CommandLineOptions["CLRSupport"] = "/clr"; }),
            Options.Option(Options.Vc.General.CommonLanguageRuntimeSupport.PureMsilClrSupport, () => { context.Options["CLRSupport"] = "Pure"; context.CommandLineOptions["CLRSupport"] = "/clr:pure"; }),
            Options.Option(Options.Vc.General.CommonLanguageRuntimeSupport.SafeMsilClrSupport, () => { context.Options["CLRSupport"] = "Safe"; context.CommandLineOptions["CLRSupport"] = "/clr:safe"; }),
            Options.Option(Options.Vc.General.CommonLanguageRuntimeSupport.SafeMsilClrSupportOldSyntax, () => { context.Options["CLRSupport"] = "OldSyntax"; context.CommandLineOptions["CLRSupport"] = "/clr:oldSyntax"; })
            );

            context.SelectOption
            (
            Options.Option(Options.Vc.General.MfcSupport.UseMfcStdWin, () => { context.Options["UseOfMfc"] = FileGeneratorUtilities.RemoveLineTag; context.CommandLineOptions["UseOfMfc"] = FileGeneratorUtilities.RemoveLineTag; }),
            Options.Option(Options.Vc.General.MfcSupport.UseMfcStatic, () => { context.Options["UseOfMfc"] = "Static"; context.CommandLineOptions["UseOfMfc"] = FileGeneratorUtilities.RemoveLineTag; }),
            Options.Option(Options.Vc.General.MfcSupport.UseMfcDynamic, () => { context.Options["UseOfMfc"] = "Dynamic"; context.CommandLineOptions["UseOfMfc"] = FileGeneratorUtilities.RemoveLineTag; })
            );

            //Options.Vc.General.WholeProgramOptimization.
            //    NoWholeProgramOptimization              WholeProgramOptimization="0"
            //    UseLinkTimeCodeGeneration               WholeProgramOptimization="1"                    /GL                                 /LTCG
            //    ProfileGuidedOptimizationInstrument     WholeProgramOptimization="2"                    /GL                                 /LTCG:PGINSTRUMENT
            //    ProfileGuidedOptimizationOptimize       WholeProgramOptimization="3"                    /GL                                 /LTCG:PGOPTIMIZE /PGD:"f:\coding\helloworld\helloworld\Debug\hellochange.pgd"
            //    ProfileGuidedOptimizationUpdate         WholeProgramOptimization="3"                    /GL                                 /LTCG:PGUPDATE /PGD:"f:\coding\helloworld\helloworld\Debug\hellochange.pgd"
            context.SelectOption
            (
            Options.Option(Options.Vc.General.WholeProgramOptimization.Disable, () => { context.Options["WholeProgramOptimization"] = "false"; context.Options["CompilerWholeProgramOptimization"] = "false"; context.CommandLineOptions["CompilerWholeProgramOptimization"] = FileGeneratorUtilities.RemoveLineTag; }),
            Options.Option(Options.Vc.General.WholeProgramOptimization.LinkTime, () => { context.Options["WholeProgramOptimization"] = "true"; context.Options["CompilerWholeProgramOptimization"] = "true"; context.CommandLineOptions["CompilerWholeProgramOptimization"] = "/GL"; }),
            Options.Option(Options.Vc.General.WholeProgramOptimization.Instrument, () => { context.Options["WholeProgramOptimization"] = "PGInstrument"; context.Options["CompilerWholeProgramOptimization"] = "true"; context.CommandLineOptions["CompilerWholeProgramOptimization"] = "/GL"; }),
            Options.Option(Options.Vc.General.WholeProgramOptimization.Optimize, () => { context.Options["WholeProgramOptimization"] = "PGOptimize"; context.Options["CompilerWholeProgramOptimization"] = "true"; context.CommandLineOptions["CompilerWholeProgramOptimization"] = "/GL"; }),
            Options.Option(Options.Vc.General.WholeProgramOptimization.Update, () => { context.Options["WholeProgramOptimization"] = "PGUpdate"; context.Options["CompilerWholeProgramOptimization"] = "true"; context.CommandLineOptions["CompilerWholeProgramOptimization"] = "/GL"; })
            );

            optionsContext.PlatformVcxproj.SelectApplicationFormatOptions(context);
            optionsContext.PlatformVcxproj.SelectBuildType(context);

            context.Options["ExecutablePath"] = FileGeneratorUtilities.RemoveLineTag;
            context.Options["IncludePath"] = FileGeneratorUtilities.RemoveLineTag;
            context.Options["LibraryPath"] = FileGeneratorUtilities.RemoveLineTag;
            context.Options["ExcludePath"] = FileGeneratorUtilities.RemoveLineTag;

            optionsContext.PlatformVcxproj.SetupSdkOptions(context);

            // AdditionalIncludeDirectories
            SelectAdditionalIncludeDirectoriesOption(context, optionsContext);

            // Options.Vc.Compiler.AdditionalUsingDirectories
            Strings additionalUsingDirectories = Options.GetStrings<Options.Vc.Compiler.AdditionalUsingDirectories>(context.Configuration);
            additionalUsingDirectories.AddRange(context.Configuration.AdditionalUsingDirectories);


            bool writeResourceCompileTag = false;
            // Options.Vc.ResourceCompiler.AdditionalIncludeDirectories
            Strings resourceIncludePath = Options.GetStrings<Options.Vc.ResourceCompiler.AdditionalIncludeDirectories>(context.Configuration);
            if (resourceIncludePath.Any())
            {
                context.Options["ResourceAdditionalIncludeDirectories"] = new Strings(Util.PathGetRelative(context.ProjectDirectory, resourceIncludePath)).JoinStrings(";");
                writeResourceCompileTag = true;
            }
            else
            {
                context.Options["ResourceAdditionalIncludeDirectories"] = FileGeneratorUtilities.RemoveLineTag;
            }

            //Resource Compiler ShowProgress
            //    No                                      ShowProgress="false"
            //    Yes                                     ShowProgress="true"
            context.SelectOption
            (
            Options.Option(Options.Vc.ResourceCompiler.ShowProgress.No, () => { context.Options["ResourceCompilerShowProgress"] = FileGeneratorUtilities.RemoveLineTag; }),
            Options.Option(Options.Vc.ResourceCompiler.ShowProgress.Yes, () => { context.Options["ResourceCompilerShowProgress"] = "true"; writeResourceCompileTag = true; })
            );

            // Options.Vc.ResourceCompiler.PreprocessorDefinitions
            Strings resourcedefines = Options.GetStrings<Options.Vc.ResourceCompiler.PreprocessorDefinitions>(context.Configuration);
            if (resourcedefines.Any())
            {
                context.Options["ResourcePreprocessorDefinitions"] = resourcedefines.JoinStrings(";").Replace(@"""", GetPlatformStringResourceDefineQuote(context.Configuration.Platform));
                writeResourceCompileTag = true;
            }
            else
            {
                context.Options["ResourcePreprocessorDefinitions"] = FileGeneratorUtilities.RemoveLineTag;
            }

            context.Options["ResourceCompileTag"] = writeResourceCompileTag ? string.Empty : FileGeneratorUtilities.RemoveLineTag;

            context.Options["AdditionalUsingDirectories"] = additionalUsingDirectories.Count > 0 ? string.Join(";", additionalUsingDirectories.Select(s => Util.PathGetRelative(context.ProjectDirectory, s))) : FileGeneratorUtilities.RemoveLineTag;

            additionalUsingDirectories.AddRange(optionsContext.PlatformVcxproj.GetCxUsingPath(context));
            if (additionalUsingDirectories.Count > 0)
            {
                StringBuilder result = new StringBuilder();
                foreach (string additionalUsingDirectory in additionalUsingDirectories)
                    result.Append(@"/AI""" + additionalUsingDirectory + @""" ");
                result.Remove(result.Length - 1, 1);
                context.CommandLineOptions["AdditionalUsingDirectories"] = result.ToString();
            }
            else
            {
                context.CommandLineOptions["AdditionalUsingDirectories"] = FileGeneratorUtilities.RemoveLineTag;
            }

            //Options.Vc.General.DebugInformation.   
            //    Disabled                                Project.ProjectConfiguration.Tool.DebugInformationFormat="0"   
            //    C7Compatible                            Project.ProjectConfiguration.Tool.DebugInformationFormat="1"   /Z7
            //    ProgramDatabase                         Project.ProjectConfiguration.Tool.DebugInformationFormat="3"   /Zi
            //    ProgramDatabaseForEditAndContinue       Project.ProjectConfiguration.Tool.DebugInformationFormat="4"   /ZI

            SelectDebugInformationOption(context, optionsContext);

            //Options.Vc.General.UseDebugLibraries.
            //    Disable                                 WarnAsError="false"
            //    Enable                                  WarnAsError="true"                              /WX
            context.SelectOption
            (
            Options.Option(Options.Vc.General.UseDebugLibraries.Disabled, () => { context.Options["UseDebugLibraries"] = "false"; }),
            Options.Option(Options.Vc.General.UseDebugLibraries.Enabled, () => { context.Options["UseDebugLibraries"] = "true"; })
            );

            //Options.Vc.General.WarningLevel.
            //    Level0                                  Project.ProjectConfiguration.Tool.WarningLevel="0"             /W0
            //    Level1                                  Project.ProjectConfiguration.Tool.WarningLevel="1"             /W1
            //    Level2                                  Project.ProjectConfiguration.Tool.WarningLevel="2"             /W2
            //    Level3                                  Project.ProjectConfiguration.Tool.WarningLevel="3"             /W3
            //    Level4                                  Project.ProjectConfiguration.Tool.WarningLevel="4"             /W4
            context.SelectOption
            (
            Options.Option(Options.Vc.General.WarningLevel.Level0, () => { context.Options["WarningLevel"] = "TurnOffAllWarnings"; context.CommandLineOptions["WarningLevel"] = "/W0"; }),
            Options.Option(Options.Vc.General.WarningLevel.Level1, () => { context.Options["WarningLevel"] = "Level1"; context.CommandLineOptions["WarningLevel"] = "/W1"; }),
            Options.Option(Options.Vc.General.WarningLevel.Level2, () => { context.Options["WarningLevel"] = "Level2"; context.CommandLineOptions["WarningLevel"] = "/W2"; }),
            Options.Option(Options.Vc.General.WarningLevel.Level3, () => { context.Options["WarningLevel"] = "Level3"; context.CommandLineOptions["WarningLevel"] = "/W3"; }),
            Options.Option(Options.Vc.General.WarningLevel.Level4, () => { context.Options["WarningLevel"] = "Level4"; context.CommandLineOptions["WarningLevel"] = "/W4"; }),
            Options.Option(Options.Vc.General.WarningLevel.EnableAllWarnings, () => { context.Options["WarningLevel"] = "EnableAllWarnings"; context.CommandLineOptions["WarningLevel"] = "/Wall"; })
            );

            //Options.Vc.General.TreatWarnigAsError.
            //    Disable                                 WarnAsError="false"
            //    Enable                                  WarnAsError="true"                              /WX
            context.SelectOption
            (
            Options.Option(Options.Vc.General.TreatWarningsAsErrors.Disable, () => { context.Options["TreatWarningAsError"] = "false"; context.CommandLineOptions["TreatWarningAsError"] = "/WX-"; }),
            Options.Option(Options.Vc.General.TreatWarningsAsErrors.Enable, () => { context.Options["TreatWarningAsError"] = "true"; context.CommandLineOptions["TreatWarningAsError"] = "/WX"; })
            );

            SelectNativeVsEvironmentOption(context);

            context.Options["TrackFileAccess"] = FileGeneratorUtilities.RemoveLineTag;

            SelectPlatformToolsetOption(context, optionsContext);

            // Compiler.SuppressStartupBanner
            context.CommandLineOptions["SuppressStartupBanner"] = "/nologo";

            //Options.Vc.Compiler.MultiProcessorCompilation.
            context.SelectOption
            (
            Options.Option(Options.Vc.Compiler.MultiProcessorCompilation.Enable, () => { context.Options["MultiProcessorCompilation"] = "true"; context.CommandLineOptions["MultiProcessorCompilation"] = "/MP"; }),
            Options.Option(Options.Vc.Compiler.MultiProcessorCompilation.Disable, () => { context.Options["MultiProcessorCompilation"] = "false"; context.CommandLineOptions["MultiProcessorCompilation"] = FileGeneratorUtilities.RemoveLineTag; })
            );


            //Options.Vc.Compiler.Optimization.
            //    Disable                                 Project.ProjectConfiguration.Tool.Optimization="0"             /Od
            //    MinimizeSize                            Project.ProjectConfiguration.Tool.Optimization="1"             /O1
            //    MaximizeSpeed                           Project.ProjectConfiguration.Tool.Optimization="2"             /O2
            //    FullOptimization                        Project.ProjectConfiguration.Tool.Optimization="3"             /Ox
            context.SelectOption
            (
            Options.Option(Options.Vc.Compiler.Optimization.Disable, () => { context.Options["Optimization"] = "Disabled"; context.CommandLineOptions["Optimization"] = "/Od"; }),
            Options.Option(Options.Vc.Compiler.Optimization.MinimizeSize, () => { context.Options["Optimization"] = "MinSpace"; context.CommandLineOptions["Optimization"] = "/O1"; }),
            Options.Option(Options.Vc.Compiler.Optimization.MaximizeSpeed, () => { context.Options["Optimization"] = "MaxSpeed"; context.CommandLineOptions["Optimization"] = "/O2"; }),
            Options.Option(Options.Vc.Compiler.Optimization.FullOptimization, () => { context.Options["Optimization"] = "Full"; context.CommandLineOptions["Optimization"] = "/Ox"; })
            );

            //Options.Vc.Compiler.Inline.
            //    Default                                 InlineFunctionExpansion="0"
            //    OnlyInline                              InlineFunctionExpansion="1"                     /Ob1
            //    AnySuitable                             InlineFunctionExpansion="2"                     /Ob2
            //    Disable                                 InlineFunctionExpansion="3"                     /Ob0
            context.SelectOption
            (
            Options.Option(Options.Vc.Compiler.Inline.Default, () => { context.Options["InlineFunctionExpansion"] = "Default"; context.CommandLineOptions["InlineFunctionExpansion"] = FileGeneratorUtilities.RemoveLineTag; }),
            Options.Option(Options.Vc.Compiler.Inline.OnlyInline, () => { context.Options["InlineFunctionExpansion"] = "OnlyExplicitInline"; context.CommandLineOptions["InlineFunctionExpansion"] = "/Ob1"; }),
            Options.Option(Options.Vc.Compiler.Inline.AnySuitable, () => { context.Options["InlineFunctionExpansion"] = "AnySuitable"; context.CommandLineOptions["InlineFunctionExpansion"] = "/Ob2"; }),
            Options.Option(Options.Vc.Compiler.Inline.Disable, () => { context.Options["InlineFunctionExpansion"] = "Disable"; context.CommandLineOptions["InlineFunctionExpansion"] = "/Ob0"; })
            );

            //Options.Vc.Compiler.Intrinsic.
            //    Disable                                 EnableIntrinsicFunctions="false"
            //    Enable                                  EnableIntrinsicFunctions="true"                 /Oi
            context.SelectOption
            (
            Options.Option(Options.Vc.Compiler.Intrinsic.Disable, () => { context.Options["EnableIntrinsicFunctions"] = "false"; context.CommandLineOptions["EnableIntrinsicFunctions"] = FileGeneratorUtilities.RemoveLineTag; }),
            Options.Option(Options.Vc.Compiler.Intrinsic.Enable, () => { context.Options["EnableIntrinsicFunctions"] = "true"; context.CommandLineOptions["EnableIntrinsicFunctions"] = "/Oi"; })
            );

            //Compiler.Optimization.FavorSizeOrSpeed
            //    Neither                                 FavorSizeOrSpeed="0"
            //    FavorFastCode                           FavorSizeOrSpeed="1"                            /Ot
            //    FavorSmallCode                          FavorSizeOrSpeed="2"                            /Os
            context.SelectOption
            (
            Options.Option(Options.Vc.Compiler.FavorSizeOrSpeed.Neither, () => { context.Options["FavorSizeOrSpeed"] = "Neither"; context.CommandLineOptions["FavorSizeOrSpeed"] = FileGeneratorUtilities.RemoveLineTag; }),
            Options.Option(Options.Vc.Compiler.FavorSizeOrSpeed.FastCode, () => { context.Options["FavorSizeOrSpeed"] = "Speed"; context.CommandLineOptions["FavorSizeOrSpeed"] = "/Ot"; }),
            Options.Option(Options.Vc.Compiler.FavorSizeOrSpeed.SmallCode, () => { context.Options["FavorSizeOrSpeed"] = "Size"; context.CommandLineOptions["FavorSizeOrSpeed"] = "/Os"; })
            );

            //Compiler.Optimization.OmitFramePointers
            //    Disable                                 OmitFramePointers="false"
            //    Enable                                  OmitFramePointers="true"                        /Oy
            context.SelectOption
            (
            Options.Option(Options.Vc.Compiler.OmitFramePointers.Disable, () => { context.Options["OmitFramePointers"] = "false"; context.CommandLineOptions["OmitFramePointers"] = "/Oy-"; }),
            Options.Option(Options.Vc.Compiler.OmitFramePointers.Enable, () => { context.Options["OmitFramePointers"] = "true"; context.CommandLineOptions["OmitFramePointers"] = "/Oy"; })
            );

            //Compiler.Optimization.FiberSafe
            //    Disable                                 EnableFiberSafeOptimizations="false"
            //    Enable                                  EnableFiberSafeOptimizations="true"             /GT
            context.SelectOption
            (
            Options.Option(Options.Vc.Compiler.FiberSafe.Disable, () => { context.Options["EnableFiberSafeOptimizations"] = "false"; context.CommandLineOptions["EnableFiberSafeOptimizations"] = FileGeneratorUtilities.RemoveLineTag; }),
            Options.Option(Options.Vc.Compiler.FiberSafe.Enable, () => { context.Options["EnableFiberSafeOptimizations"] = "true"; context.CommandLineOptions["EnableFiberSafeOptimizations"] = "/GT"; })
            );

            //Compiler.IgnoreStandardIncludePath.
            //    Disable                                 IgnoreStandardIncludePath="false"
            //    Enable                                  IgnoreStandardIncludePath="true"                /X
            context.SelectOption
            (
            Options.Option(Options.Vc.Compiler.IgnoreStandardIncludePath.Disable, () => { context.Options["IgnoreStandardIncludePath"] = "false"; context.CommandLineOptions["IgnoreStandardIncludePath"] = FileGeneratorUtilities.RemoveLineTag; }),
            Options.Option(Options.Vc.Compiler.IgnoreStandardIncludePath.Enable, () => { context.Options["IgnoreStandardIncludePath"] = "true"; context.CommandLineOptions["IgnoreStandardIncludePath"] = "/X"; })
            );

            //Compiler.Proprocessor.GenerateProcessorFile
            //    Disable                                 GeneratePreprocessedFile="0"
            //    WithLineNumbers                         GeneratePreprocessedFile="1"                    /P
            //    WithoutLineNumbers                      GeneratePreprocessedFile="2"                    /EP /P
            context.SelectOption
            (
            Options.Option(Options.Vc.Compiler.GenerateProcessorFile.Disable, () => { context.Options["GeneratePreprocessedFile"] = "false"; context.CommandLineOptions["GeneratePreprocessedFile"] = FileGeneratorUtilities.RemoveLineTag; }),
            Options.Option(Options.Vc.Compiler.GenerateProcessorFile.WithLineNumbers, () => { context.Options["GeneratePreprocessedFile"] = "true"; context.CommandLineOptions["GeneratePreprocessedFile"] = "/P"; }),
            Options.Option(Options.Vc.Compiler.GenerateProcessorFile.WithoutLineNumbers, () => { context.Options["GeneratePreprocessedFile"] = "true"; context.CommandLineOptions["GeneratePreprocessedFile"] = "/EP /P"; })
            );

            //Options.Vc.Compiler.KeepComment.
            //    Disable                                 KeepComments="false"
            //    Enable                                  KeepComments="true"                             /C
            context.SelectOption
            (
            Options.Option(Options.Vc.Compiler.KeepComment.Disable, () => { context.Options["KeepComments"] = "false"; context.CommandLineOptions["KeepComments"] = FileGeneratorUtilities.RemoveLineTag; }),
            Options.Option(Options.Vc.Compiler.KeepComment.Enable, () => { context.Options["KeepComments"] = "true"; context.CommandLineOptions["KeepComments"] = "/C"; })
            );

            //Options.Vc.Compiler.StringPooling.
            //    Disable                                 StringPooling="false"
            //    Enable                                  StringPooling="true"                            /GF
            context.SelectOption
            (
            Options.Option(Options.Vc.Compiler.StringPooling.Disable, () => { context.Options["StringPooling"] = "false"; context.CommandLineOptions["StringPooling"] = "/GF-"; }),
            Options.Option(Options.Vc.Compiler.StringPooling.Enable, () => { context.Options["StringPooling"] = "true"; context.CommandLineOptions["StringPooling"] = "/GF"; })
            );

            //Options.Vc.Compiler.Exceptions.
            //    Disable                                 ExceptionHandling="false"
            //    Enable                                  ExceptionHandling="Sync"                        /EHsc
            //    EnableWithExternC                       ExceptionHandling="SyncCThrow"                  /EHs
            //    EnableWithSEH                           ExceptionHandling="Async"                       /EHa
            context.SelectOption
            (
            Options.Option(Options.Vc.Compiler.Exceptions.Disable, () => { context.Options["ExceptionHandling"] = "false"; context.CommandLineOptions["ExceptionHandling"] = FileGeneratorUtilities.RemoveLineTag; }),
            Options.Option(Options.Vc.Compiler.Exceptions.Enable, () => { context.Options["ExceptionHandling"] = "Sync"; context.CommandLineOptions["ExceptionHandling"] = "/EHsc"; }),
            Options.Option(Options.Vc.Compiler.Exceptions.EnableWithExternC, () => { context.Options["ExceptionHandling"] = "SyncCThrow"; context.CommandLineOptions["ExceptionHandling"] = "/EHs"; }),
            Options.Option(Options.Vc.Compiler.Exceptions.EnableWithSEH, () => { context.Options["ExceptionHandling"] = "Async"; context.CommandLineOptions["ExceptionHandling"] = "/EHa"; })
            );

            context.Options["ForcedUsingFiles"] = FileGeneratorUtilities.RemoveLineTag;
            if (context.Configuration.ForceUsingFiles.Any())
            {
                context.Options["ForcedUsingFiles"] = context.Configuration.ForceUsingFiles.JoinStrings(";", true);
            }

            //Options.Vc.Compiler.CompileAsWinRT.     
            //    Disable                                 CompileAsWinRT="false"
            //    Enable                                  CompileAsWinRT="true"
            context.SelectOption
            (
            Options.Option(Options.Vc.Compiler.CompileAsWinRT.Default, () => { context.Options["CompileAsWinRT"] = FileGeneratorUtilities.RemoveLineTag; }),
            Options.Option(Options.Vc.Compiler.CompileAsWinRT.Disable, () => { context.Options["CompileAsWinRT"] = "false"; }),
            Options.Option(Options.Vc.Compiler.CompileAsWinRT.Enable, () => { context.Options["CompileAsWinRT"] = "true"; })
            );

            //Options.Vc.Compiler.TypeChecks.
            //    Disable                                 SmallerTypeCheck="true"                         /RTCc
            context.SelectOption
            (
            Options.Option(Options.Vc.Compiler.TypeChecks.Disable, () => { context.Options["SmallerTypeCheck"] = "false"; context.CommandLineOptions["SmallerTypeCheck"] = FileGeneratorUtilities.RemoveLineTag; }),
            Options.Option(Options.Vc.Compiler.TypeChecks.Enable, () => { context.Options["SmallerTypeCheck"] = "true"; context.CommandLineOptions["SmallerTypeCheck"] = "/RTCc"; })
            );

            //Options.Vc.Compiler.RuntimeChecks.
            //    Default                                 BasicRuntimeChecks="0"
            //    StackFrames                             BasicRuntimeChecks="1"                          /RTCs
            //    UninitializedVariables                  BasicRuntimeChecks="2"                          /RTCu
            //    Both                                    BasicRuntimeChecks="3"                          /RTC1
            context.SelectOption
            (
            Options.Option(Options.Vc.Compiler.RuntimeChecks.Default, () => { context.Options["BasicRuntimeChecks"] = "Default"; context.CommandLineOptions["BasicRuntimeChecks"] = FileGeneratorUtilities.RemoveLineTag; }),
            Options.Option(Options.Vc.Compiler.RuntimeChecks.StackFrames, () => { context.Options["BasicRuntimeChecks"] = "StackFrameRuntimeCheck"; context.CommandLineOptions["BasicRuntimeChecks"] = "/RTCs"; }),
            Options.Option(Options.Vc.Compiler.RuntimeChecks.UninitializedVariables, () => { context.Options["BasicRuntimeChecks"] = "UninitializedLocalUsageCheck"; context.CommandLineOptions["BasicRuntimeChecks"] = "/RTCu"; }),
            Options.Option(Options.Vc.Compiler.RuntimeChecks.Both, () => { context.Options["BasicRuntimeChecks"] = "EnableFastChecks"; context.CommandLineOptions["BasicRuntimeChecks"] = "/RTC1"; })
            );

            if (Util.IsCpp(context.Configuration))
            {
                //Options.Vc.Compiler.RuntimeLibrary.
                //    MultiThreaded                           RuntimeLibrary="0"                              /MT
                //    MultiThreadedDebug                      RuntimeLibrary="1"                              /MTd
                //    MultiThreadedDLL                        RuntimeLibrary="2"                              /MD
                //    MultiThreadedDebugDLL                   RuntimeLibrary="3"                              /MDd
                context.SelectOption
                (
                Options.Option(Options.Vc.Compiler.RuntimeLibrary.MultiThreaded, () => { context.Options["RuntimeLibrary"] = "MultiThreaded"; context.CommandLineOptions["RuntimeLibrary"] = "/MT"; }),
                Options.Option(Options.Vc.Compiler.RuntimeLibrary.MultiThreadedDebug, () => { context.Options["RuntimeLibrary"] = "MultiThreadedDebug"; context.CommandLineOptions["RuntimeLibrary"] = "/MTd"; }),
                Options.Option(Options.Vc.Compiler.RuntimeLibrary.MultiThreadedDLL, () => { context.Options["RuntimeLibrary"] = "MultiThreadedDLL"; context.CommandLineOptions["RuntimeLibrary"] = "/MD"; }),
                Options.Option(Options.Vc.Compiler.RuntimeLibrary.MultiThreadedDebugDLL, () => { context.Options["RuntimeLibrary"] = "MultiThreadedDebugDLL"; context.CommandLineOptions["RuntimeLibrary"] = "/MDd"; })
                );
            }
            else
            {
                context.Options["RuntimeLibrary"] = FileGeneratorUtilities.RemoveLineTag;
            }

            bool clrSupport = Util.IsDotNet(context.Configuration);
            if (!clrSupport)
            {
                //Options.Vc.Compiler.MinimalRebuild.
                //    Disable                                 MinimalRebuild="false"
                //    Enable                                  MinimalRebuild="true"                           /Gm
                context.SelectOption
                (
                Options.Option(Options.Vc.Compiler.MinimalRebuild.Disable, () => { context.Options["MinimalRebuild"] = "false"; context.CommandLineOptions["MinimalRebuild"] = "/Gm-"; }),
                Options.Option(Options.Vc.Compiler.MinimalRebuild.Enable, () => { context.Options["MinimalRebuild"] = FileGeneratorUtilities.RemoveLineTag; context.CommandLineOptions["MinimalRebuild"] = "/Gm"; })
                );

                //Options.Vc.Compiler.RTTI.
                //    Disable                                 RuntimeTypeInfo="false"                         /GR-
                //    Enable                                  RuntimeTypeInfo="true"
                context.SelectOption
                (
                Options.Option(Options.Vc.Compiler.RTTI.Disable, () => { context.Options["RuntimeTypeInfo"] = "false"; context.CommandLineOptions["RuntimeTypeInfo"] = "/GR-"; }),
                Options.Option(Options.Vc.Compiler.RTTI.Enable, () => { context.Options["RuntimeTypeInfo"] = "true"; context.CommandLineOptions["RuntimeTypeInfo"] = "/GR"; })
                );
            }
            else
            {
                context.Options["MinimalRebuild"] = FileGeneratorUtilities.RemoveLineTag;
                context.Options["RuntimeTypeInfo"] = FileGeneratorUtilities.RemoveLineTag;
                context.CommandLineOptions["MinimalRebuild"] = FileGeneratorUtilities.RemoveLineTag;
                context.CommandLineOptions["RuntimeTypeInfo"] = FileGeneratorUtilities.RemoveLineTag;
            }

            //Options.Vc.Compiler.StructAlignment.
            //    Default                                 StructMemberAlignment="0"
            //    Alignment1                              StructMemberAlignment="1"                       /Zp1
            //    Alignment2                              StructMemberAlignment="2"                       /Zp2
            //    Alignment4                              StructMemberAlignment="3"                       /Zp4
            //    Alignment8                              StructMemberAlignment="4"                       /Zp8
            //    Alignment16                             StructMemberAlignment="5"                       /Zp16
            context.SelectOption
            (
            Options.Option(Options.Vc.Compiler.StructAlignment.Default, () => { context.Options["StructMemberAlignment"] = "Default"; context.CommandLineOptions["StructMemberAlignment"] = FileGeneratorUtilities.RemoveLineTag; }),
            Options.Option(Options.Vc.Compiler.StructAlignment.Alignment1, () => { context.Options["StructMemberAlignment"] = "1Byte"; context.CommandLineOptions["StructMemberAlignment"] = "/Zp1"; }),
            Options.Option(Options.Vc.Compiler.StructAlignment.Alignment2, () => { context.Options["StructMemberAlignment"] = "2Bytes"; context.CommandLineOptions["StructMemberAlignment"] = "/Zp2"; }),
            Options.Option(Options.Vc.Compiler.StructAlignment.Alignment4, () => { context.Options["StructMemberAlignment"] = "4Bytes"; context.CommandLineOptions["StructMemberAlignment"] = "/Zp4"; }),
            Options.Option(Options.Vc.Compiler.StructAlignment.Alignment8, () => { context.Options["StructMemberAlignment"] = "8Bytes"; context.CommandLineOptions["StructMemberAlignment"] = "/Zp8"; }),
            Options.Option(Options.Vc.Compiler.StructAlignment.Alignment16, () => { context.Options["StructMemberAlignment"] = "16Bytes"; context.CommandLineOptions["StructMemberAlignment"] = "/Zp16"; })
            );

            //Options.Vc.Compiler.BufferSecurityCheck.
            //    Enable                                  BufferSecurityCheck="true"
            //    Disable                                 BufferSecurityCheck="false"                     /GS-
            context.SelectOption
            (
            Options.Option(Options.Vc.Compiler.BufferSecurityCheck.Enable, () => { context.Options["BufferSecurityCheck"] = "true"; context.CommandLineOptions["BufferSecurityCheck"] = "/GS"; }),
            Options.Option(Options.Vc.Compiler.BufferSecurityCheck.Disable, () => { context.Options["BufferSecurityCheck"] = "false"; context.CommandLineOptions["BufferSecurityCheck"] = "/GS-"; })
            );

            //Options.Vc.Compiler.OptimizeGlobalData.
            //    Disable                                 /Gw- in AdditionalOptions
            //    Enable                                  /Gw in AdditionalOptions
            if (context.Configuration.Platform.IsMicrosoft())
            {
                context.SelectOption
                (
                Options.Option(Options.Vc.Compiler.OptimizeGlobalData.Disable, () =>
                { /* do nothing */
                }),
                Options.Option(Options.Vc.Compiler.OptimizeGlobalData.Enable, () => { context.Configuration.AdditionalCompilerOptions.Add("/Gw"); })
                );
            }

            //Options.Vc.Compiler.FunctionLevelLinking.
            //    Disable                                 EnableFunctionLevelLinking="false"
            //    Enable                                  EnableFunctionLevelLinking="true"               /Gy
            context.SelectOption
            (
            Options.Option(Options.Vc.Compiler.FunctionLevelLinking.Disable, () => { context.Options["EnableFunctionLevelLinking"] = "false"; context.CommandLineOptions["EnableFunctionLevelLinking"] = "/Gy-"; }),
            Options.Option(Options.Vc.Compiler.FunctionLevelLinking.Enable, () => { context.Options["EnableFunctionLevelLinking"] = "true"; context.CommandLineOptions["EnableFunctionLevelLinking"] = "/Gy"; })
            );

            //Options.Vc.Compiler.EnhancedInstructionSet.
            //    Disable                                 EnableEnhancedInstructionSet
            //    SIMD                                    EnableEnhancedInstructionSet                /arch:SSE
            //    SIMD2                                   EnableEnhancedInstructionSet                /arch:SSE2
            //    AdvancedVectorExtensions                EnableEnhancedInstructionSet                /arch:AVX
            //    NoEnhancedInstructions                  EnableEnhancedInstructionSet                /arch:IA32
            context.SelectOption
            (
            Options.Option(Options.Vc.Compiler.EnhancedInstructionSet.Disable, () => { context.Options["EnableEnhancedInstructionSet"] = "NotSet"; context.CommandLineOptions["EnableEnhancedInstructionSet"] = FileGeneratorUtilities.RemoveLineTag; }),
            Options.Option(Options.Vc.Compiler.EnhancedInstructionSet.SIMD, () => { context.Options["EnableEnhancedInstructionSet"] = "StreamingSIMDExtensions"; context.CommandLineOptions["EnableEnhancedInstructionSet"] = "/arch:SSE"; }),
            Options.Option(Options.Vc.Compiler.EnhancedInstructionSet.SIMD2, () => { context.Options["EnableEnhancedInstructionSet"] = "StreamingSIMDExtensions2"; context.CommandLineOptions["EnableEnhancedInstructionSet"] = "/arch:SSE2"; }),
            Options.Option(Options.Vc.Compiler.EnhancedInstructionSet.AdvancedVectorExtensions, () => { context.Options["EnableEnhancedInstructionSet"] = "AdvancedVectorExtensions"; context.CommandLineOptions["EnableEnhancedInstructionSet"] = "/arch:AVX"; }),
            Options.Option(Options.Vc.Compiler.EnhancedInstructionSet.AdvancedVectorExtensions2, () => { context.Options["EnableEnhancedInstructionSet"] = "AdvancedVectorExtensions2"; context.CommandLineOptions["EnableEnhancedInstructionSet"] = "/arch:AVX2"; }),
            Options.Option(Options.Vc.Compiler.EnhancedInstructionSet.NoEnhancedInstructions, () => { context.Options["EnableEnhancedInstructionSet"] = "NoExtensions"; context.CommandLineOptions["EnableEnhancedInstructionSet"] = "/arch:IA32"; })
            );

            //Options.Vc.Compiler.FloatingPointModel.
            //    Precise                                 FloatingPointModel="0"                          /fp:precise
            //    Strict                                  FloatingPointModel="1"                          /fp:strict
            //    Fast                                    FloatingPointModel="2"                          /fp:fast
            context.SelectOption
            (
            Options.Option(Options.Vc.Compiler.FloatingPointModel.Precise, () => { context.Options["FloatingPointModel"] = "Precise"; context.CommandLineOptions["FloatingPointModel"] = "/fp:precise"; }),
            Options.Option(Options.Vc.Compiler.FloatingPointModel.Strict, () => { context.Options["FloatingPointModel"] = "Strict"; context.CommandLineOptions["FloatingPointModel"] = "/fp:strict"; }),
            Options.Option(Options.Vc.Compiler.FloatingPointModel.Fast, () => { context.Options["FloatingPointModel"] = "Fast"; context.CommandLineOptions["FloatingPointModel"] = "/fp:fast"; })
            );

            //Options.Vc.Compiler.FloatingPointExceptions.
            //    Disable                                 FloatingPointExceptions="false"
            //    Enable                                  FloatingPointExceptions="true"                  /fp:except
            context.SelectOption
            (
            Options.Option(Options.Vc.Compiler.FloatingPointExceptions.Disable, () => { context.Options["FloatingPointExceptions"] = "false"; context.CommandLineOptions["FloatingPointExceptions"] = "/fp:except-"; }),
            Options.Option(Options.Vc.Compiler.FloatingPointExceptions.Enable, () => { context.Options["FloatingPointExceptions"] = "true"; context.CommandLineOptions["FloatingPointExceptions"] = "/fp:except"; })
            );

            // CreateHotpatchableImage = "false"
            // CreateHotpatchableImage = "true"     /hotpatch
            context.CommandLineOptions["CreateHotpatchableImage"] = FileGeneratorUtilities.RemoveLineTag;

            //Options.Vc.Compiler.DisableLanguageExtensions.    
            //    Disable                                 DisableLanguageExtensions="false"               
            //    Enable                                  DisableLanguageExtensions="true"                /Za
            context.SelectOption
            (
            Options.Option(Options.Vc.Compiler.DisableLanguageExtensions.Disable, () => { context.Options["DisableLanguageExtensions"] = "false"; context.CommandLineOptions["DisableLanguageExtensions"] = FileGeneratorUtilities.RemoveLineTag; }),
            Options.Option(Options.Vc.Compiler.DisableLanguageExtensions.Enable, () => { context.Options["DisableLanguageExtensions"] = "true"; context.CommandLineOptions["DisableLanguageExtensions"] = "/Za"; })
            );

            //Options.Vc.Compiler.BuiltInWChartType.
            //    Disable                                 TreatWChar_tAsBuiltInType="false"               /Zc:wchar_t-
            //    Enable                                  TreatWChar_tAsBuiltInType="true"                /Zc:wchar_t
            context.SelectOption
            (
            Options.Option(Options.Vc.Compiler.BuiltInWChartType.Disable, () => { context.Options["TreatWChar_tAsBuiltInType"] = "false"; context.CommandLineOptions["TreatWChar_tAsBuiltInType"] = "/Zc:wchar_t-"; }),
            Options.Option(Options.Vc.Compiler.BuiltInWChartType.Enable, () => { context.Options["TreatWChar_tAsBuiltInType"] = "true"; context.CommandLineOptions["TreatWChar_tAsBuiltInType"] = "/Zc:wchar_t"; })
            );

            //    Disable                                 Removed_unreferenced_COMDAT="false"               
            //    Enable                                  Removed_unreferenced_COMDAT="true"                /Zc:inline[-]
            context.SelectOption
            (
            Options.Option(Options.Vc.Compiler.RemovedUnreferencedCOMDAT.Disable, () => { context.Options["Removed_unreferenced_COMDAT"] = "false"; }),
            Options.Option(Options.Vc.Compiler.RemovedUnreferencedCOMDAT.Enable, () => { context.Options["Removed_unreferenced_COMDAT"] = "true"; })
            );

            //Options.Vc.Compiler.ForceLoopScope.
            //    Disable                                 ForceConformanceInForLoopScope="false"          /Zc:forScope-
            //    Enable                                  ForceConformanceInForLoopScope="true"           /Zc:forScope
            context.SelectOption
            (
            Options.Option(Options.Vc.Compiler.ForceLoopScope.Disable, () => { context.Options["ForceConformanceInForLoopScope"] = "false"; context.CommandLineOptions["ForceConformanceInForLoopScope"] = "/Zc:forScope-"; }),
            Options.Option(Options.Vc.Compiler.ForceLoopScope.Enable, () => { context.Options["ForceConformanceInForLoopScope"] = "true"; context.CommandLineOptions["ForceConformanceInForLoopScope"] = "/Zc:forScope"; })
            );

            //Options.Vc.Compiler.OpenMP.   
            //    Disable                                 OpenMP="false"                                  /openmp-
            //    Enable                                  OpenMP="true"                                   /openmp
            context.SelectOption
            (
            Options.Option(Options.Vc.Compiler.OpenMP.Disable, () => { context.Options["OpenMP"] = "false"; context.CommandLineOptions["OpenMP"] = "/openmp-"; }),
            Options.Option(Options.Vc.Compiler.OpenMP.Enable, () => { context.Options["OpenMP"] = "true"; context.CommandLineOptions["OpenMP"] = "/openmp"; })
            );

            //Options.Vc.Compiler.GenerateXMLDocumentation.   
            //    Disable                                 GenerateXMLDocumentation="false"                                  
            //    Enable                                  GenerateXMLDocumentation="true"                                   /openmp
            context.SelectOption
            (
            Options.Option(Options.Vc.Compiler.GenerateXMLDocumentation.Disable, () => { context.Options["GenerateXMLDocumentation"] = "false"; context.CommandLineOptions["GenerateXMLDocumentation"] = FileGeneratorUtilities.RemoveLineTag; }),
            Options.Option(Options.Vc.Compiler.GenerateXMLDocumentation.Enable, () => { context.Options["GenerateXMLDocumentation"] = "true"; context.CommandLineOptions["GenerateXMLDocumentation"] = @"/doc""[project.RootPath]"""; })
            );

            //Options.Vc.Compiler.PrecompiledHeader
            //      NotUsingPrecompiledHeaders          UsePrecompiledHeader="0"
            //      CreatePrecompiledHeader             UsePrecompiledHeader="1"                            /Yc
            //      UsePrecompiledHeader                UsePrecompiledHeader="2"                            /Yu
            SelectPrecompiledHeaderOption(context, optionsContext);

            //Options.Vc.Compiler.CallingConvention.
            //    cdecl                                   CallingConvention="0"                           /Gd
            //    fastcall                                CallingConvention="1"                           /Gr
            //    stdcall                                 CallingConvention="2"                           /Gz
            context.SelectOption
            (
            Options.Option(Options.Vc.Compiler.CallingConvention.cdecl, () => { context.Options["CallingConvention"] = "Cdecl"; context.CommandLineOptions["CallingConvention"] = "/Gd"; }),
            Options.Option(Options.Vc.Compiler.CallingConvention.fastcall, () => { context.Options["CallingConvention"] = "FastCall"; context.CommandLineOptions["CallingConvention"] = "/Gr"; }),
            Options.Option(Options.Vc.Compiler.CallingConvention.stdcall, () => { context.Options["CallingConvention"] = "StdCall"; context.CommandLineOptions["CallingConvention"] = "/Gz"; })
            );

            //Options.Vc.Compiler.ShowIncludes.
            //    Disable                               ShowIncludes="false"
            //    Enable                                ShowIncludes="true"                           /showIncludes
            context.SelectOption
            (
            Options.Option(Options.Vc.Compiler.ShowIncludes.Disable, () => { context.Options["ShowIncludes"] = "false"; }),
            Options.Option(Options.Vc.Compiler.ShowIncludes.Enable, () => { context.Options["ShowIncludes"] = "true"; })
            );

            // Options.Vc.Compiler.DisableSpecificWarnings
            Strings disableWarnings = Options.GetStrings<Options.Vc.Compiler.DisableSpecificWarnings>(context.Configuration);
            context.Options["DisableSpecificWarnings"] = disableWarnings.JoinStrings(";");
            context.CommandLineOptions["DisableSpecificWarnings"] = FileGeneratorUtilities.RemoveLineTag;
            if (disableWarnings.Count > 0)
            {
                StringBuilder result = new StringBuilder();
                foreach (string disableWarning in disableWarnings)
                    result.Append(@"/wd""" + disableWarning + @""" ");
                result.Remove(result.Length - 1, 1);
                context.CommandLineOptions["DisableSpecificWarnings"] = result.ToString();
            }

            // Options.Vc.Compiler.UndefinePreprocessorDefinitions
            Strings undefinePreprocessors = Options.GetStrings<Options.Vc.Compiler.UndefinePreprocessorDefinitions>(context.Configuration);
            context.Options["UndefinePreprocessorDefinitions"] = undefinePreprocessors.JoinStrings(";");
            context.CommandLineOptions["UndefinePreprocessorDefinitions"] = FileGeneratorUtilities.RemoveLineTag;
            if (undefinePreprocessors.Count > 0)
            {
                StringBuilder result = new StringBuilder();
                foreach (string undefine in undefinePreprocessors)
                    result.Append(@"/U""" + undefine + @""" ");
                result.Remove(result.Length - 1, 1);
                context.CommandLineOptions["UndefinePreprocessorDefinitions"] = result.ToString();
            }

            // concat defines, don't add options.Defines since they are automatically added by VS
            Strings defines = new Strings();
            defines.AddRange(context.Options.ExplicitDefines);
            defines.AddRange(context.Configuration.Defines);

            context.Options["PreprocessorDefinitions"] = defines.JoinStrings(";");

            context.CommandLineOptions["PreprocessorDefinitions"] = FileGeneratorUtilities.RemoveLineTag;
            if (defines.Count > 0)
            {
                StringBuilder fastBuildDefines = new StringBuilder();
                string platformDefineSwitch = optionsContext.PlatformDescriptor.IsUsingClang ? "-D" : "/D";

                bool first = true;
                foreach (string define in defines)
                {
                    if (string.IsNullOrWhiteSpace(define))
                        continue;

                    if (!first)
                        fastBuildDefines.Append(" ");
                    else
                        first = false;
                    fastBuildDefines.AppendFormat(@"{0}""{1}""", platformDefineSwitch, define.Replace(@"""", @"\"""));
                }
                context.CommandLineOptions["PreprocessorDefinitions"] = fastBuildDefines.ToString();
            }

            // UndefineAllPreprocessorDefinitions
            context.CommandLineOptions["UndefineAllPreprocessorDefinitions"] = FileGeneratorUtilities.RemoveLineTag;

            // Default defines...
            optionsContext.PlatformVcxproj.SelectCompilerOptions(context);

            if (context.Configuration.Target.GetFragment<DevEnv>() == DevEnv.vs2012 && context.Configuration.Target.GetPlatform().IsPC())
            {
                // Disable all the unsupported features of LLVM-clang on MSVC2012
                if (context.Options["PlatformToolset"] == "LLVM-vs2012")
                {
                    // Full list is in this file:
                    // http://llvm.org/viewvc/llvm-project/cfe/trunk/include/clang/Driver/CLCompatOptions.td?view=markup
                    context.Options["NativeEnvironmentVS2012"] = FileGeneratorUtilities.RemoveLineTag; // Native environment makes the compiler fallback to MSVC

                    // reset the overriden path
                    context.Options["ExecutablePath"] = FileGeneratorUtilities.RemoveLineTag;
                    context.Options["IncludePath"] = FileGeneratorUtilities.RemoveLineTag;
                    context.Options["LibraryPath"] = FileGeneratorUtilities.RemoveLineTag;
                    context.Options["ExcludePath"] = FileGeneratorUtilities.RemoveLineTag;

                    // Uncomment those when LLVM 3.9 is released
                    // The new version will add support from PCH, but requires them to be set as ForceIncludes
                    // List<string> forceIncludes = new List<string>();
                    // forceIncludes.AddRange(options["ForcedIncludeFiles"].Split(new[] { ';' }, StringSplitOptions.RemoveEmptyEntries));
                    // forceIncludes.Add(conf.PrecompHeader);
                    // forceIncludes.Remove(RemoveLineTag);
                    // options["ForcedIncludeFiles"] = Util.JoinStrings(forceIncludes, ";");
                }
            }

            // Options.Vc.Compiler.AdditionalOptions
            context.Configuration.AdditionalCompilerOptions.Sort();
            string additionalCompilerOptions = context.Configuration.AdditionalCompilerOptions.JoinStrings(" ");
            context.Options["AdditionalCompilerOptions"] = additionalCompilerOptions;

            optionsContext.HasClrSupport = clrSupport;
        }

        private static void SelectAdditionalIncludeDirectoriesOption(IGenerationContext context, ProjectOptionsGenerationContext optionsContext)
        {
            var includePaths = new OrderableStrings(optionsContext.PlatformVcxproj.GetIncludePaths(context));
            context.Options["AdditionalIncludeDirectories"] = includePaths.Count > 0 ? includePaths.JoinStrings(";") : FileGeneratorUtilities.RemoveLineTag;

            var platformIncludePaths = optionsContext.PlatformVcxproj.GetPlatformIncludePaths(context);

            context.CommandLineOptions["AdditionalIncludeDirectories"] = FileGeneratorUtilities.RemoveLineTag;
            if (optionsContext.Resolver != null)
            {
                var dirs = new List<string>();
                dirs.AddRange(includePaths);
                dirs.AddRange(platformIncludePaths);
                
                if (dirs.Any())
                {
                    StringBuilder result = new StringBuilder();
                    foreach (string additionalIncludeDirectory in dirs)
                    {
                        string path = Util.GetConvertedRelativePath(context.ProjectDirectory, additionalIncludeDirectory, optionsContext.BaseProjectPath, true, context.Project.RootPath);
                        if (optionsContext.PlatformDescriptor.IsUsingClang)
                            result.Append($@"-I""{path}"" ");
                        else
                            result.Append($@"/I""{path}"" ");
                    }

                    result.Remove(result.Length - 1, 1);
                    context.CommandLineOptions["AdditionalIncludeDirectories"] = result.ToString();
                }
            }

            context.CommandLineOptions["AdditionalResourceIncludeDirectories"] = FileGeneratorUtilities.RemoveLineTag;
            if (optionsContext.Resolver != null)
            {
                if (platformIncludePaths.Any())
                {
                    StringBuilder result = new StringBuilder();
                    foreach (string additionalIncludeDirectory in platformIncludePaths)
                    {
                        string path = Util.GetConvertedRelativePath(context.ProjectDirectory, optionsContext.Resolver.Resolve(additionalIncludeDirectory), optionsContext.BaseProjectPath, true, context.Project.RootPath);
                        if (optionsContext.PlatformDescriptor.IsUsingClang)
                            result.Append($@"-I""{path}"" ");
                        else
                            result.Append($@"/I""{path}"" ");
                    }

                    result.Remove(result.Length - 1, 1);
                    context.CommandLineOptions["AdditionalResourceIncludeDirectories"] = result.ToString();
                }
            }
        }

        private static void SelectDebugInformationOption(IGenerationContext context, ProjectOptionsGenerationContext optionsContext)
        {
            // win64 don't support /ZI which is the default one, forward it to /Zi
            if (optionsContext.PlatformVcxproj.HasEditAndContinueDebuggingSupport)
            {
                context.SelectOption
                (
                Options.Option(Options.Vc.General.DebugInformation.Disable, () => { context.Options["DebugInformationFormat"] = "None"; context.CommandLineOptions["DebugInformationFormat"] = FileGeneratorUtilities.RemoveLineTag; }),
                Options.Option(Options.Vc.General.DebugInformation.C7Compatible, () => { context.Options["DebugInformationFormat"] = "OldStyle"; context.CommandLineOptions["DebugInformationFormat"] = "/Z7"; }),
                Options.Option(Options.Vc.General.DebugInformation.ProgramDatabase, () => { context.Options["DebugInformationFormat"] = "ProgramDatabase"; context.CommandLineOptions["DebugInformationFormat"] = "/Zi"; }),
                Options.Option(Options.Vc.General.DebugInformation.ProgramDatabaseEnC, () => { context.Options["DebugInformationFormat"] = "EditAndContinue"; context.CommandLineOptions["DebugInformationFormat"] = "/ZI"; })
                );
            }
            else
            {
                context.SelectOption
                (
                Options.Option(Options.Vc.General.DebugInformation.Disable, () => { context.Options["DebugInformationFormat"] = "None"; context.CommandLineOptions["DebugInformationFormat"] = FileGeneratorUtilities.RemoveLineTag; }),
                Options.Option(Options.Vc.General.DebugInformation.C7Compatible, () => { context.Options["DebugInformationFormat"] = "OldStyle"; context.CommandLineOptions["DebugInformationFormat"] = "/Z7"; }),
                Options.Option(Options.Vc.General.DebugInformation.ProgramDatabase, () => { context.Options["DebugInformationFormat"] = "ProgramDatabase"; context.CommandLineOptions["DebugInformationFormat"] = "/Zi"; }),
                Options.Option(Options.Vc.General.DebugInformation.ProgramDatabaseEnC, () => { context.Options["DebugInformationFormat"] = "ProgramDatabase"; context.CommandLineOptions["DebugInformationFormat"] = "/Zi"; })
                );
            }
        }

        private static void SelectNativeVsEvironmentOption(IGenerationContext context)
        {
            switch (context.DevelopmentEnvironment)
            {
                case DevEnv.vs2010:
                    {
                        context.Options["NativeEnvironmentVS2012"] = FileGeneratorUtilities.RemoveLineTag;
                        context.Options["NativeEnvironmentVS2013"] = FileGeneratorUtilities.RemoveLineTag;
                    }
                    break;
                case DevEnv.vs2012:
                    {
                        //Options.Vc.General.NativeEnvironment.
                        context.SelectOption
                        (
                        Options.Option(Options.Vc.General.NativeEnvironment.Enable, () => { context.Options["NativeEnvironmentVS2012"] = "true"; }),
                        Options.Option(Options.Vc.General.NativeEnvironment.Disable, () => { context.Options["NativeEnvironmentVS2012"] = FileGeneratorUtilities.RemoveLineTag; })
                        );

                        context.Options["NativeEnvironmentVS2013"] = FileGeneratorUtilities.RemoveLineTag;
                    }
                    break;
                case DevEnv.vs2013:
                case DevEnv.vs2015: // For the moment and until proven otherwise, use the native env. flag of VS2013
                case DevEnv.vs2017:
                    {
                        //Options.Vc.General.NativeEnvironment.
                        context.SelectOption
                        (
                        Options.Option(Options.Vc.General.NativeEnvironment.Enable, () => { context.Options["NativeEnvironmentVS2013"] = "true"; }),
                        Options.Option(Options.Vc.General.NativeEnvironment.Disable, () => { context.Options["NativeEnvironmentVS2013"] = FileGeneratorUtilities.RemoveLineTag; })
                        );

                        context.Options["NativeEnvironmentVS2012"] = FileGeneratorUtilities.RemoveLineTag;
                    }
                    break;
            }
        }

        private static void SelectPlatformToolsetOption(IGenerationContext context, ProjectOptionsGenerationContext optionsContext)
        {
            context.SelectOption
            (
                Options.Option(Options.Vc.General.PlatformToolset.Default, () => { context.Options["PlatformToolset"] = context.DevelopmentEnvironment.GetDefaultPlatformToolset(); }),
                Options.Option(Options.Vc.General.PlatformToolset.v100, () => { context.Options["PlatformToolset"] = "v100"; }),
                Options.Option(Options.Vc.General.PlatformToolset.v110, () => { context.Options["PlatformToolset"] = "v110"; }),
                Options.Option(Options.Vc.General.PlatformToolset.v120, () => { context.Options["PlatformToolset"] = "v120"; }),
                Options.Option(Options.Vc.General.PlatformToolset.v140, () => { context.Options["PlatformToolset"] = "v140"; }),
                Options.Option(Options.Vc.General.PlatformToolset.LLVM_vs2012, () => { context.Options["PlatformToolset"] = "LLVM-vs2012"; context.Options["TrackFileAccess"] = "false"; })
            );
            optionsContext.PlatformVcxproj.SetupPlatformToolsetOptions(context);
        }

        private static void SelectPrecompiledHeaderOption(IGenerationContext context, ProjectOptionsGenerationContext optionsContext)
        {
            if (string.IsNullOrEmpty(context.Configuration.PrecompHeader) || string.IsNullOrEmpty(context.Configuration.PrecompSource))
            {
                context.Options["UsePrecompiledHeader"] = "NotUsing";
                context.Options["PrecompiledHeaderThrough"] = FileGeneratorUtilities.RemoveLineTag;
                context.Options["PrecompiledHeaderFile"] = FileGeneratorUtilities.RemoveLineTag;
                context.CommandLineOptions["PrecompiledHeaderThrough"] = FileGeneratorUtilities.RemoveLineTag;
                context.CommandLineOptions["PrecompiledHeaderFile"] = FileGeneratorUtilities.RemoveLineTag;
            }
            else
            {
                context.Options["UsePrecompiledHeader"] = "Use";
                context.Options["PrecompiledHeaderThrough"] = context.Configuration.PrecompHeader;
                string outputFolder = string.IsNullOrEmpty(context.Configuration.PrecompHeaderOutputFolder) ? optionsContext.IntermediateDirectoryRelative : Util.PathGetRelative(context.ProjectDirectory, context.Configuration.PrecompHeaderOutputFolder);
                context.Options["PrecompiledHeaderFile"] = outputFolder + Util.WindowsSeparator + context.Configuration.Project.Name + ".pch";
                context.CommandLineOptions["PrecompiledHeaderThrough"] = context.Options["PrecompiledHeaderThrough"];
                context.CommandLineOptions["PrecompiledHeaderFile"] = Util.GetConvertedRelativePath(context.ProjectDirectory, context.Options["PrecompiledHeaderFile"], optionsContext.BaseProjectPath, true, context.Project.RootPath);

                if (!optionsContext.PlatformDescriptor.HasPrecompiledHeaderSupport)
                    throw new Error("Precompiled header not supported for spu configuration: {0}", context.Configuration);
            }
        }

        private void GenerateLinkerOptions(IGenerationContext context, ProjectOptionsGenerationContext optionsContext)
        {
            string outputExtension = context.Configuration.OutputExtension;
            string outputFileName = $"{optionsContext.PlatformVcxproj.GetOutputFileNamePrefix(context, context.Configuration.Output)}{optionsContext.TargetName}";

            context.Options["ImportLibrary"] = FileGeneratorUtilities.RemoveLineTag;
            context.CommandLineOptions["ImportLibrary"] = FileGeneratorUtilities.RemoveLineTag;
            context.Options["OutputFileName"] = outputFileName;
            context.Options["OutputFileExtension"] = "." + outputExtension;

            context.Options["AdditionalDeploymentFolders"] = "";

            switch (context.Configuration.Output)
            {
                case Project.Configuration.OutputType.Dll:
                case Project.Configuration.OutputType.DotNetClassLibrary:
                case Project.Configuration.OutputType.Exe:
                case Project.Configuration.OutputType.DotNetConsoleApp:
                case Project.Configuration.OutputType.DotNetWindowsApp:
                    context.Options["OutputFile"] = optionsContext.OutputDirectoryRelative + Util.WindowsSeparator + outputFileName + "." + outputExtension;
                    if (context.Configuration.Output == Project.Configuration.OutputType.Dll)
                    {
                        context.Options["ImportLibrary"] = optionsContext.OutputLibraryDirectoryRelative + Util.WindowsSeparator + optionsContext.TargetName + ".lib";
                        context.CommandLineOptions["ImportLibrary"] = "/IMPLIB:" + Util.GetConvertedRelativePath(context.ProjectDirectory, context.Options["ImportLibrary"], optionsContext.BaseProjectPath, true, context.Project.RootPath);
                    }
                    break;
                case Project.Configuration.OutputType.Lib:
                    context.Options["OutputFile"] = optionsContext.OutputLibraryDirectoryRelative + Util.WindowsSeparator + outputFileName + "." + outputExtension;
                    break;
                case Project.Configuration.OutputType.Utility:
                    context.Options["OutputFile"] = FileGeneratorUtilities.RemoveLineTag;
                    context.Options["OutputFileExtension"] = FileGeneratorUtilities.RemoveLineTag;
                    break;
                case Project.Configuration.OutputType.None:
                    break;
                default:
                    throw new ArgumentOutOfRangeException();
            }

            //ShowProgress
            //    NotSet                                  ShowProgress="0"
            //    DisplayAllProgressMessages              ShowProgress="1"                            /VERBOSE
            //    DisplaysSomeProgressMessages            ShowProgress="2"                            /VERBOSE:LIB
            context.SelectOption
            (
            Options.Option(Options.Vc.Linker.ShowProgress.NotSet, () => { context.Options["ShowProgress"] = "NotSet"; context.CommandLineOptions["ShowProgress"] = FileGeneratorUtilities.RemoveLineTag; }),
            Options.Option(Options.Vc.Linker.ShowProgress.DisplayAllProgressMessages, () => { context.Options["ShowProgress"] = "LinkVerbose"; context.CommandLineOptions["ShowProgress"] = "/VERBOSE"; }),
            Options.Option(Options.Vc.Linker.ShowProgress.DisplaysSomeProgressMessages, () => { context.Options["ShowProgress"] = "LinkVerboseLib"; context.CommandLineOptions["ShowProgress"] = "/VERBOSE:Lib"; })
            );

            //Incremental
            //    Default                                 LinkIncremental="0"
            //    Disable                                 LinkIncremental="1"                         /INCREMENTAL:NO
            //    Enable                                  LinkIncremental="2"                         /INCREMENTAL
            context.SelectOption
            (
            Options.Option(Options.Vc.Linker.Incremental.Default, () => { context.Options["LinkIncremental"] = FileGeneratorUtilities.RemoveLineTag; context.CommandLineOptions["LinkIncremental"] = FileGeneratorUtilities.RemoveLineTag; }),
            Options.Option(Options.Vc.Linker.Incremental.Disable, () => { context.Options["LinkIncremental"] = "false"; context.CommandLineOptions["LinkIncremental"] = "/INCREMENTAL:NO"; }),
            Options.Option(Options.Vc.Linker.Incremental.Enable, () => { context.Options["LinkIncremental"] = "true"; context.CommandLineOptions["LinkIncremental"] = "/INCREMENTAL"; })
            );

            //EmbedManifest
            //    Yes                                 EmbedManifest="true"
            //    No                                  EmbedManifest="false"
            context.SelectOption
            (
            Options.Option(Options.Vc.Linker.EmbedManifest.Default, () => { context.Options["EmbedManifest"] = FileGeneratorUtilities.RemoveLineTag; }),
            Options.Option(Options.Vc.Linker.EmbedManifest.Yes, () => { context.Options["EmbedManifest"] = "true"; }),
            Options.Option(Options.Vc.Linker.EmbedManifest.No, () => { context.Options["EmbedManifest"] = "false"; })
            );

            //SuppressStartupBanner
            //    Disable                                 SuppressStartupBanner="false"
            //    Enable                                  SuppressStartupBanner="true"                /NOLOGO
            context.SelectOption
            (
            Options.Option(Options.Vc.Linker.SuppressStartupBanner.Disable, () => { context.Options["SuppressStartupBanner"] = "false"; context.CommandLineOptions["LinkerSuppressStartupBanner"] = FileGeneratorUtilities.RemoveLineTag; }),
            Options.Option(Options.Vc.Linker.SuppressStartupBanner.Enable, () => { context.Options["SuppressStartupBanner"] = "true"; context.CommandLineOptions["LinkerSuppressStartupBanner"] = "/NOLOGO"; })
            );

            //LinkLibraryDependencies
            //    Enable                                  LinkLibraryDependencies="true"
            //    Disable                                 LinkLibraryDependencies="false"
            context.SelectOption
            (
            Options.Option(Options.Vc.Linker.LinkLibraryDependencies.Default, () => { context.Options["LinkLibraryDependencies"] = FileGeneratorUtilities.RemoveLineTag; }),
            Options.Option(Options.Vc.Linker.LinkLibraryDependencies.Enable, () => { context.Options["LinkLibraryDependencies"] = "true"; }),
            Options.Option(Options.Vc.Linker.LinkLibraryDependencies.Disable, () => { context.Options["LinkLibraryDependencies"] = "false"; })
            );

            //ReferenceOutputAssembly
            //    Enable                                  ReferenceOutputAssembly="true"
            //    Disable                                 ReferenceOutputAssembly="false"
            context.SelectOption
            (
            Options.Option(Options.Vc.Linker.ReferenceOutputAssembly.Default, () => { context.Options["ReferenceOutputAssembly"] = FileGeneratorUtilities.RemoveLineTag; }),
            Options.Option(Options.Vc.Linker.ReferenceOutputAssembly.Enable, () => { context.Options["ReferenceOutputAssembly"] = "true"; }),
            Options.Option(Options.Vc.Linker.ReferenceOutputAssembly.Disable, () => { context.Options["ReferenceOutputAssembly"] = "false"; })
            );

            //CopyLocalSatelliteAssemblies
            //    Enable                                  CopyLocalSatelliteAssemblies="true"
            //    Disable                                 CopyLocalSatelliteAssemblies="false"
            context.SelectOption
            (
            Options.Option(Options.Vc.Linker.CopyLocalSatelliteAssemblies.Default, () => { context.Options["CopyLocalSatelliteAssemblies"] = FileGeneratorUtilities.RemoveLineTag; }),
            Options.Option(Options.Vc.Linker.CopyLocalSatelliteAssemblies.Enable, () => { context.Options["CopyLocalSatelliteAssemblies"] = "true"; }),
            Options.Option(Options.Vc.Linker.CopyLocalSatelliteAssemblies.Disable, () => { context.Options["CopyLocalSatelliteAssemblies"] = "false"; })
            );

            //IgnoreImportLibrary
            //    Enable                                  IgnoreImportLibrary="true"
            //    Disable                                 IgnoreImportLibrary="false"
            if (context.Configuration.Target.GetFragment<DevEnv>() == DevEnv.vs2010)
            {
                context.Options["IgnoreImportLibrary"] = FileGeneratorUtilities.RemoveLineTag;
            }
            else
            {
                context.SelectOption
                (
                Options.Option(Options.Vc.Linker.IgnoreImportLibrary.Enable, () => { context.Options["IgnoreImportLibrary"] = "true"; }),
                Options.Option(Options.Vc.Linker.IgnoreImportLibrary.Disable, () => { context.Options["IgnoreImportLibrary"] = "false"; })
                );
            }

            //RunCodeAnalysis
            //    Enable                                  RunCodeAnalysis="true"
            //    Disable                                 RunCodeAnalysis="false"
            if (context.Configuration.Target.GetFragment<DevEnv>() == DevEnv.vs2010)
            {
                context.Options["RunCodeAnalysis"] = FileGeneratorUtilities.RemoveLineTag;
            }
            else
            {
                context.SelectOption
                (
                Options.Option(Options.Vc.CodeAnalysis.RunCodeAnalysis.Enable, () => { context.Options["RunCodeAnalysis"] = "true"; }),
                Options.Option(Options.Vc.CodeAnalysis.RunCodeAnalysis.Disable, () => { context.Options["RunCodeAnalysis"] = FileGeneratorUtilities.RemoveLineTag; })
                );
            }

            //UseLibraryDependencyInputs
            //    Enable                                  UseLibraryDependencyInputs="true"
            //    Disable                                 UseLibraryDependencyInputs="false"
            context.SelectOption
            (
            Options.Option(Options.Vc.Linker.UseLibraryDependencyInputs.Default, () => { context.Options["UseLibraryDependencyInputs"] = FileGeneratorUtilities.RemoveLineTag; }),
            Options.Option(Options.Vc.Linker.UseLibraryDependencyInputs.Enable, () => { context.Options["UseLibraryDependencyInputs"] = "true"; }),
            Options.Option(Options.Vc.Linker.UseLibraryDependencyInputs.Disable, () => { context.Options["UseLibraryDependencyInputs"] = "false"; })
            );

            //DisableFastUpToDateCheck
            //    Enable                                  DisableFastUpToDateCheck="true"
            //    Disable                                 DisableFastUpToDateCheck="false"
            context.SelectOption
            (
            Options.Option(Options.Vc.General.DisableFastUpToDateCheck.Enable, () => { context.Options["DisableFastUpToDateCheck"] = "true"; }),
            Options.Option(Options.Vc.General.DisableFastUpToDateCheck.Disable, () => { context.Options["DisableFastUpToDateCheck"] = FileGeneratorUtilities.RemoveLineTag; })
            );

            //RandomizedBaseAddress
            context.SelectOption
            (
            Options.Option(Options.Vc.Linker.RandomizedBaseAddress.Default, () => { context.Options["RandomizedBaseAddress"] = FileGeneratorUtilities.RemoveLineTag; context.CommandLineOptions["RandomizedBaseAddress"] = "/DYNAMICBASE"; }),
            Options.Option(Options.Vc.Linker.RandomizedBaseAddress.Enable, () => { context.Options["RandomizedBaseAddress"] = "true"; context.CommandLineOptions["RandomizedBaseAddress"] = "/DYNAMICBASE"; }),
            Options.Option(Options.Vc.Linker.RandomizedBaseAddress.Disable, () => { context.Options["RandomizedBaseAddress"] = "false"; context.CommandLineOptions["RandomizedBaseAddress"] = "/DYNAMICBASE:NO"; })
            );

            // Delay Loaded DLLs
            var libFiles = new OrderableStrings(context.Configuration.LibraryFiles);
            libFiles.AddRange(context.Configuration.DependenciesLibraryFiles);
            libFiles.AddRange(optionsContext.PlatformVcxproj.GetLibraryFiles(context));

            Strings delayedDLLs = Options.GetStrings<Options.Vc.Linker.DelayLoadDLLs>(context.Configuration);
            if (delayedDLLs.Count() > 0)
            {
                context.Options["DelayLoadedDLLs"] = delayedDLLs.JoinStrings(";");

                StringBuilder result = new StringBuilder();
                foreach (string delayedDLL in delayedDLLs)
                    result.Append(@"/DELAYLOAD:""" + delayedDLL + @""" ");
                result.Remove(result.Length - 1, 1);
                context.CommandLineOptions["DelayLoadedDLLs"] = result.ToString();

                if (context.Configuration.IsFastBuild)
                    libFiles.Add("Delayimp.lib");
            }
            else
            {
                context.Options["DelayLoadedDLLs"] = FileGeneratorUtilities.RemoveLineTag;
                context.CommandLineOptions["DelayLoadedDLLs"] = FileGeneratorUtilities.RemoveLineTag;
            }

            Strings ignoreSpecificLibraryNames = Options.GetStrings<Options.Vc.Linker.IgnoreSpecificLibraryNames>(context.Configuration);
            ignoreSpecificLibraryNames.ToLower();
            ignoreSpecificLibraryNames.InsertSuffix(optionsContext.PlatformLibraryExtension, true);

            context.Options["AdditionalDependencies"] = FileGeneratorUtilities.RemoveLineTag;
            context.Options["AdditionalLibraryDirectories"] = FileGeneratorUtilities.RemoveLineTag;
            context.CommandLineOptions["AdditionalDependencies"] = FileGeneratorUtilities.RemoveLineTag;
            context.CommandLineOptions["AdditionalLibraryDirectories"] = FileGeneratorUtilities.RemoveLineTag;

            if (!(context.Configuration.Output == Project.Configuration.OutputType.None ||
                context.Configuration.Output == Project.Configuration.OutputType.Lib && !context.Configuration.ExportAdditionalLibrariesEvenForStaticLib))
            {
                //AdditionalLibraryDirectories
                //                                            AdditionalLibraryDirectories="dir1;dir2"    /LIBPATH:"dir1" /LIBPATH:"dir2"
                SelectAdditionalLibraryDirectoriesOption(context, optionsContext);

                //AdditionalDependencies                      
                //                                            AdditionalDependencies="lib1;lib2"      "lib1;lib2" 
                SelectAdditionalDependenciesOption(context, optionsContext, libFiles, ignoreSpecificLibraryNames);
            }

            // Set module definition
            if (!string.IsNullOrEmpty(context.Configuration.ModuleDefinitionFile))
            {
                var filePath = Util.PathGetRelative(context.ProjectDirectory, context.Configuration.ModuleDefinitionFile);
                context.Options["ModuleDefinitionFile"] = filePath;
                context.CommandLineOptions["ModuleDefinitionFile"] = "/DEF:" + Util.GetConvertedRelativePath(context.ProjectDirectory, context.Configuration.ModuleDefinitionFile, optionsContext.BaseProjectPath, true, context.Project.RootPath);
            }
            else
            {
                context.Options["ModuleDefinitionFile"] = FileGeneratorUtilities.RemoveLineTag;
                context.CommandLineOptions["ModuleDefinitionFile"] = FileGeneratorUtilities.RemoveLineTag;
            }

            //IgnoreAllDefaultLibraries
            //    Enable                                  IgnoreAllDefaultLibraries="true"        /NODEFAULTLIB
            //    Disable                                 IgnoreAllDefaultLibraries="false"
            context.SelectOption
            (
            Options.Option(Options.Vc.Linker.IgnoreAllDefaultLibraries.Enable, () => { context.Options["IgnoreAllDefaultLibraries"] = "true"; context.CommandLineOptions["IgnoreAllDefaultLibraries"] = "/NODEFAULTLIB"; }),
            Options.Option(Options.Vc.Linker.IgnoreAllDefaultLibraries.Disable, () => { context.Options["IgnoreAllDefaultLibraries"] = "false"; context.CommandLineOptions["IgnoreAllDefaultLibraries"] = FileGeneratorUtilities.RemoveLineTag; })
            );

            //IgnorSpecificLibraryNames
            //                                            IgnoreDefaultLibraryNames=[lib]         /NODEFAULTLIB:[lib]
            context.Options["IgnoreDefaultLibraryNames"] = ignoreSpecificLibraryNames.JoinStrings(";");
            context.CommandLineOptions["IgnoreDefaultLibraryNames"] = FileGeneratorUtilities.RemoveLineTag;
            if (optionsContext.Resolver != null)
            {
                string[] ignoreLibs = optionsContext.Resolver.Resolve(context.Options["IgnoreDefaultLibraryNames"]).Split(new char[] { ';' }, StringSplitOptions.RemoveEmptyEntries);
                if (ignoreLibs.GetLength(0) > 0)
                {
                    StringBuilder result = new StringBuilder();
                    foreach (string ignoreLib in ignoreLibs)
                        result.Append(@"/NODEFAULTLIB:""" + ignoreLib + @""" ");
                    result.Remove(result.Length - 1, 1);
                    context.CommandLineOptions["IgnoreDefaultLibraryNames"] = result.ToString();
                }
            }

            //GenerateManifest
            //    Enable                                  GenerateManifest="true"                 /MANIFEST
            //    Disable                                 GenerateManifest="false"
            SelectGenerateManifestOption(context, optionsContext);

            //GenerateDebugInformation="false"
            //    VS2012-VS2013
            //    Enable                                  GenerateDebugInformation="true"           /DEBUG
            //    Disable                                 GenerateDebugInformation="false"
            //
            //    VS2015
            //    Enable                                  GenerateDebugInformation="Debug"          /DEBUG
            //    EnableFastLink                          GenerateDebugInformation="DebugFastLink"  /DEBUG:FASTLINK
            //    Disable                                 GenerateDebugInformation="No"
            SelectGenerateDebugInformationOption(context, optionsContext);

            // GenerateMapFile
            SelectGenerateMapFileOption(context, optionsContext);

            //MapExports
            //    Enable                                  MapExports="true"                       /MAPINFO:EXPORTS
            //    Disable                                 MapExports="false"
            context.SelectOption
            (
            Options.Option(Options.Vc.Linker.MapExports.Enable, () => { context.Options["MapExports"] = "true"; context.CommandLineOptions["MapExports"] = "/MAPINFO:EXPORTS"; }),
            Options.Option(Options.Vc.Linker.MapExports.Disable, () => { context.Options["MapExports"] = "false"; context.CommandLineOptions["MapExports"] = FileGeneratorUtilities.RemoveLineTag; })
            );

            //AssemblyDebug
            //    NoDebuggableAttributeEmitted            AssemblyDebug="0"
            //    RuntimeTrackingAndDisableOptimizations  AssemblyDebug="1"                       /ASSEMBLYDEBUG
            //    NoRuntimeTrackingAndEnableOptimizations  AssemblyDebug="2"                       /ASSEMBLYDEBUG:DISABLE
            context.SelectOption
            (
            Options.Option(Options.Vc.Linker.AssemblyDebug.NoDebuggableAttributeEmitted, () => { context.Options["AssemblyDebug"] = FileGeneratorUtilities.RemoveLineTag; context.CommandLineOptions["AssemblyDebug"] = FileGeneratorUtilities.RemoveLineTag; }),
            Options.Option(Options.Vc.Linker.AssemblyDebug.RuntimeTrackingAndDisableOptimizations, () => { context.Options["AssemblyDebug"] = "true"; context.CommandLineOptions["AssemblyDebug"] = "/ASSEMBLYDEBUG"; }),
            Options.Option(Options.Vc.Linker.AssemblyDebug.NoRuntimeTrackingAndEnableOptimizations, () => { context.Options["AssemblyDebug"] = "false"; context.CommandLineOptions["AssemblyDebug"] = "/ASSEMBLYDEBUG:DISABLE"; })
            );

            //SubSystem
            //    Console                                 SubSystem="1"                           /SUBSYSTEM:CONSOLE
            //    Windows                                 SubSystem="2"                           /SUBSYSTEM:WINDOWS
            //    Native                                  SubSystem="3"                           /SUBSYSTEM:NATIVE
            context.SelectOption
            (
            Options.Option(Options.Vc.Linker.SubSystem.Console, () => { context.Options["SubSystem"] = "Console"; context.CommandLineOptions["SubSystem"] = "/SUBSYSTEM:CONSOLE"; }),
            Options.Option(Options.Vc.Linker.SubSystem.Application, () => { context.Options["SubSystem"] = "Windows"; context.CommandLineOptions["SubSystem"] = "/SUBSYSTEM:WINDOWS"; }),
            Options.Option(Options.Vc.Linker.SubSystem.Native, () => { context.Options["SubSystem"] = "Native"; context.CommandLineOptions["SubSystem"] = "/SUBSYSTEM:NATIVE"; })
            );


            //HeapSize
            //HeapReserveSize
            //                                            HeapReserveSize="0"                     /HEAP:reserve
            //HeapCommitSize
            //                                            HeapCommitSize="0"                      /HEAP:reserve,commit
            Options.Vc.Linker.HeapSize heap = Options.GetObject<Options.Vc.Linker.HeapSize>(context.Configuration);
            if (heap == null)
            {
                context.Options["HeapReserveSize"] = FileGeneratorUtilities.RemoveLineTag;
                context.Options["HeapCommitSize"] = FileGeneratorUtilities.RemoveLineTag;
                context.CommandLineOptions["HeapReserveSize"] = FileGeneratorUtilities.RemoveLineTag;
                context.CommandLineOptions["HeapCommitSize"] = FileGeneratorUtilities.RemoveLineTag;
            }
            else
            {
                context.Options["HeapReserveSize"] = heap.ReserveSize.ToString();
                context.Options["HeapCommitSize"] = heap.CommintSize.ToString();
                context.CommandLineOptions["HeapReserveSize"] = "/HEAP:reserve";
                context.CommandLineOptions["HeapCommitSize"] = "/HEAP:reserve,commit";
            }

            //StackSize
            //StackReserveSize
            //                                            StackReserveSize="0"                    /STACK:reserve
            //StackCommitSize
            //                                            StackCommitSize="0"                     /STACK:reserve,commit
            Options.Vc.Linker.StackSize stack = Options.GetObject<Options.Vc.Linker.StackSize>(context.Configuration);
            if (stack == null)
            {
                context.Options["StackReserveSize"] = FileGeneratorUtilities.RemoveLineTag;
                context.Options["StackCommitSize"] = FileGeneratorUtilities.RemoveLineTag;
                context.CommandLineOptions["StackReserveSize"] = FileGeneratorUtilities.RemoveLineTag;
                context.CommandLineOptions["StackCommitSize"] = FileGeneratorUtilities.RemoveLineTag;
            }
            else
            {
                context.Options["StackReserveSize"] = stack.ReserveSize.ToString();
                context.Options["StackCommitSize"] = stack.CommintSize.ToString();
                if (stack.CommintSize > 0)
                {
                    context.CommandLineOptions["StackReserveSize"] = FileGeneratorUtilities.RemoveLineTag;
                    context.CommandLineOptions["StackCommitSize"] = "/STACK:" + stack.ReserveSize + "," + stack.CommintSize;
                }
                else
                {
                    context.CommandLineOptions["StackReserveSize"] = "/STACK:" + stack.ReserveSize;
                    context.CommandLineOptions["StackCommitSize"] = FileGeneratorUtilities.RemoveLineTag;
                }
            }

            //LargeAddress
            //    Default                                 LargeAddressAware="0"
            //    NotSupportLargerThan2Gb                 LargeAddressAware="1"                   /LARGEADDRESSAWARE:NO
            //    SupportLargerThan2Gb                    LargeAddressAware="2"                   /LARGEADDRESSAWARE
            context.SelectOption
            (
            Options.Option(Options.Vc.Linker.LargeAddress.Default, () => { context.Options["LargeAddressAware"] = "true"; context.CommandLineOptions["LargeAddressAware"] = "/LARGEADDRESSAWARE"; }),
            Options.Option(Options.Vc.Linker.LargeAddress.NotSupportLargerThan2Gb, () => { context.Options["LargeAddressAware"] = "false"; context.CommandLineOptions["LargeAddressAware"] = "/LARGEADDRESSAWARE:NO"; }),
            Options.Option(Options.Vc.Linker.LargeAddress.SupportLargerThan2Gb, () => { context.Options["LargeAddressAware"] = "true"; context.CommandLineOptions["LargeAddressAware"] = "/LARGEADDRESSAWARE"; })
            );

            Options.Vc.Linker.BaseAddress baseAddress = Options.GetObject<Options.Vc.Linker.BaseAddress>(context.Configuration);
            context.Options["BaseAddress"] = (baseAddress != null && baseAddress.Value.Length > 0) ? (baseAddress.Value) : "";
            context.CommandLineOptions["BaseAddress"] = (baseAddress != null && baseAddress.Value.Length > 0) ? @"/BASE:""" + (baseAddress.Value) + @"""" : FileGeneratorUtilities.RemoveLineTag;

            //Reference
            //    Default                                 OptimizeReferences="0"
            //    KeepUnreferencedData                    OptimizeReferences="1"                  /OPT:NOREF
            //    EliminateUnreferencedData               OptimizeReferences="2"                  /OPT:REF
            context.SelectOption
            (
            Options.Option(Options.Vc.Linker.Reference.KeepUnreferencedData, () => { context.Options["OptimizeReferences"] = "false"; context.CommandLineOptions["OptimizeReference"] = "/OPT:NOREF"; }),
            Options.Option(Options.Vc.Linker.Reference.EliminateUnreferencedData, () => { context.Options["OptimizeReferences"] = "true"; context.CommandLineOptions["OptimizeReference"] = "/OPT:REF"; })
            );

            //EnableCOMDATFolding
            //    Default                                 EnableCOMDATFolding="0"
            //    DoNotRemoveRedundantCOMDATs             EnableCOMDATFolding="1"                 /OPT:NOICF
            //    RemoveRedundantCOMDATs                  EnableCOMDATFolding="2"                 /OPT:ICF
            context.SelectOption
            (
            Options.Option(Options.Vc.Linker.EnableCOMDATFolding.DoNotRemoveRedundantCOMDATs, () => { context.Options["EnableCOMDATFolding"] = "false"; context.CommandLineOptions["EnableCOMDATFolding"] = "/OPT:NOICF"; }),
            Options.Option(Options.Vc.Linker.EnableCOMDATFolding.RemoveRedundantCOMDATs, () => { context.Options["EnableCOMDATFolding"] = "true"; context.CommandLineOptions["EnableCOMDATFolding"] = "/OPT:ICF"; })
            );

            //FixedBaseAddress
            //    Default                                 FixedBaseAddress="0"
            //    Enable                                  FixedBaseAddress="1"                  /FIXED
            //    Disable                                 FixedBaseAddress="2"                  /FIXED:NO
            context.SelectOption
            (
            Options.Option(Options.Vc.Linker.FixedBaseAddress.Default, () => { context.Options["FixedBaseAddress"] = FileGeneratorUtilities.RemoveLineTag; context.CommandLineOptions["FixedBaseAddress"] = FileGeneratorUtilities.RemoveLineTag; }),
            Options.Option(Options.Vc.Linker.FixedBaseAddress.Enable, () => { context.Options["FixedBaseAddress"] = "true"; context.CommandLineOptions["FixedBaseAddress"] = "/FIXED"; }),
            Options.Option(Options.Vc.Linker.FixedBaseAddress.Disable, () => { context.Options["FixedBaseAddress"] = "false"; context.CommandLineOptions["FixedBaseAddress"] = "/FIXED:NO"; })
            );

            //GenerateWindowsMetadata
            //    Default                                 GenerateWindowsMetadata="0"
            //    Enable                                  GenerateWindowsMetadata="1"                  /WINMD
            //    Disable                                 GenerateWindowsMetadata="2"                  /WINMD:NO
            context.SelectOption
            (
            Options.Option(Options.Vc.Linker.GenerateWindowsMetadata.Default, () =>
            {
                context.Options["GenerateWindowsMetadata"] = FileGeneratorUtilities.RemoveLineTag;
                context.CommandLineOptions["GenerateWindowsMetadata"] = FileGeneratorUtilities.RemoveLineTag;
                context.CommandLineOptions["WindowsMetadataFile"] = FileGeneratorUtilities.RemoveLineTag;
            }),
            Options.Option(Options.Vc.Linker.GenerateWindowsMetadata.Enable, () =>
            {
                context.Options["GenerateWindowsMetadata"] = "true";
                string windowsMetadataFile = @"$(OutDir)\$(RootNamespace).winmd";
                context.CommandLineOptions["GenerateWindowsMetadata"] = "/WINMD";

                // TODO: fix this! the $ variables above will never be resolved by fastbuild
                context.CommandLineOptions["WindowsMetadataFile"] = @"/WINMDFILE:""" + Util.GetConvertedRelativePath(context.ProjectDirectory, windowsMetadataFile, optionsContext.BaseProjectPath, true, context.Project.RootPath) + @"""";
            }),
            Options.Option(Options.Vc.Linker.GenerateWindowsMetadata.Disable, () =>
            {
                context.Options["GenerateWindowsMetadata"] = "false";
                context.CommandLineOptions["GenerateWindowsMetadata"] = "/WINMD:NO";
                context.CommandLineOptions["WindowsMetadataFile"] = FileGeneratorUtilities.RemoveLineTag;
            })
            );

            //LinkTimeCodeGeneration
            //    Default                                 LinkTimeCodeGeneration="0"
            //    UseLinkTimeCodeGeneration               LinkTimeCodeGeneration="1"              /ltcg
            //    ProfileGuidedOptimizationInstrument     LinkTimeCodeGeneration="2"              /ltcg:pginstrument
            //    ProfileGuidedOptimizationOptimize       LinkTimeCodeGeneration="3"              /ltcg:pgoptimize
            //    ProfileGuidedOptimizationUpdate         LinkTimeCodeGeneration="4"              /ltcg:pgupdate
            bool profileGuideOptimization = false;

            if (context.Configuration.Output == Project.Configuration.OutputType.Lib)
            {
                context.SelectOption
                (
                Options.Option(Options.Vc.Linker.LinkTimeCodeGeneration.Default, () => { context.Options["LinkTimeCodeGeneration"] = "false"; context.CommandLineOptions["LinkTimeCodeGeneration"] = FileGeneratorUtilities.RemoveLineTag; }),
                Options.Option(Options.Vc.Linker.LinkTimeCodeGeneration.UseLinkTimeCodeGeneration, () => { context.Options["LinkTimeCodeGeneration"] = "true"; context.CommandLineOptions["LinkTimeCodeGeneration"] = context.CommandLineOptions["LinkTimeCodeGeneration"] = "/LTCG"; }),
                Options.Option(Options.Vc.Linker.LinkTimeCodeGeneration.ProfileGuidedOptimizationInstrument, () => { context.Options["LinkTimeCodeGeneration"] = "true"; context.CommandLineOptions["LinkTimeCodeGeneration"] = context.CommandLineOptions["LinkTimeCodeGeneration"] = "/LTCG"; }),
                Options.Option(Options.Vc.Linker.LinkTimeCodeGeneration.ProfileGuidedOptimizationOptimize, () => { context.Options["LinkTimeCodeGeneration"] = "true"; context.CommandLineOptions["LinkTimeCodeGeneration"] = context.CommandLineOptions["LinkTimeCodeGeneration"] = "/LTCG"; }),
                Options.Option(Options.Vc.Linker.LinkTimeCodeGeneration.ProfileGuidedOptimizationUpdate, () => { context.Options["LinkTimeCodeGeneration"] = "true"; context.CommandLineOptions["LinkTimeCodeGeneration"] = context.CommandLineOptions["LinkTimeCodeGeneration"] = "/LTCG"; })
                );
            }
            else
            {
                context.SelectOption
                (
                Options.Option(Options.Vc.Linker.LinkTimeCodeGeneration.Default, () => { context.Options["LinkTimeCodeGeneration"] = "Default"; context.CommandLineOptions["LinkTimeCodeGeneration"] = FileGeneratorUtilities.RemoveLineTag; }),
                Options.Option(Options.Vc.Linker.LinkTimeCodeGeneration.UseLinkTimeCodeGeneration, () => { context.Options["LinkTimeCodeGeneration"] = "UseLinkTimeCodeGeneration"; context.CommandLineOptions["LinkTimeCodeGeneration"] = "/LTCG"; }),
                Options.Option(Options.Vc.Linker.LinkTimeCodeGeneration.ProfileGuidedOptimizationInstrument, () => { context.Options["LinkTimeCodeGeneration"] = "PGInstrument"; profileGuideOptimization = true; context.CommandLineOptions["LinkTimeCodeGeneration"] = "/LTCG:PGInstrument"; }),
                Options.Option(Options.Vc.Linker.LinkTimeCodeGeneration.ProfileGuidedOptimizationOptimize, () => { context.Options["LinkTimeCodeGeneration"] = "PGOptimization"; profileGuideOptimization = true; context.CommandLineOptions["LinkTimeCodeGeneration"] = "/LTCG:PGOptimize"; }),
                Options.Option(Options.Vc.Linker.LinkTimeCodeGeneration.ProfileGuidedOptimizationUpdate, () => { context.Options["LinkTimeCodeGeneration"] = "PGUpdate"; profileGuideOptimization = true; context.CommandLineOptions["LinkTimeCodeGeneration"] = "/LTCG:PGUpdate"; })
                );
            }


            if (profileGuideOptimization)
            {
                string profileGuidedDatabase = optionsContext.OutputDirectoryRelative + Util.WindowsSeparator + optionsContext.TargetName + ".pgd";
                context.Options["ProfileGuidedDatabase"] = profileGuidedDatabase;
                context.CommandLineOptions["ProfileGuidedDatabase"] = @"/PGD:""" + profileGuidedDatabase + @"""";
            }
            else
            {
                context.Options["ProfileGuidedDatabase"] = "";
                context.CommandLineOptions["ProfileGuidedDatabase"] = FileGeneratorUtilities.RemoveLineTag;
            }

            // FunctionOrder
            // FunctionOrder="@..\path_to\order.txt"             /ORDER:"@..\path_to\order.txt"
            Options.Vc.Linker.FunctionOrder fctOrder = Options.GetObject<Options.Vc.Linker.FunctionOrder>(context.Configuration);
            context.Options["FunctionOrder"] = (fctOrder != null) ? fctOrder.Order : FileGeneratorUtilities.RemoveLineTag;
            context.CommandLineOptions["FunctionOrder"] = (fctOrder != null) ? @"/ORDER:@""" + fctOrder.Order + @"""" : FileGeneratorUtilities.RemoveLineTag;

            // ForceFileOutput
            context.SelectOption
            (
            Options.Option(Options.Vc.Linker.ForceFileOutput.Default, () => { context.Options["ForceFileOutput"] = FileGeneratorUtilities.RemoveLineTag; context.CommandLineOptions["ForceFileOutput"] = FileGeneratorUtilities.RemoveLineTag; }),
            Options.Option(Options.Vc.Linker.ForceFileOutput.MultiplyDefinedSymbolOnly, () => { context.Options["ForceFileOutput"] = "MultiplyDefinedSymbolOnly"; context.CommandLineOptions["ForceFileOutput"] = "/FORCE:MULTIPLE"; })
            );

            // Target Machine
            optionsContext.PlatformVcxproj.SetupPlatformTargetOptions(context);
            optionsContext.PlatformVcxproj.SelectLinkerOptions(context);

            // Options.Vc.Linker.AdditionalOptions
            context.Configuration.AdditionalLinkerOptions.Sort();
            string linkerAdditionalOptions = context.Configuration.AdditionalLinkerOptions.JoinStrings(" ");

            // Treat Options.Vc.Linker.DisableSpecificWarnings here because
            // it does not have a specific line in the vcxproj
            Strings disableLinkerWarnings = Options.GetStrings<Options.Vc.Linker.DisableSpecificWarnings>(context.Configuration);
            if (disableLinkerWarnings.Count > 0)
                linkerAdditionalOptions += " /ignore:" + disableLinkerWarnings.JoinStrings(",");

            context.Options["AdditionalLinkerOptions"] = linkerAdditionalOptions;
        }

        private void GenerateManifestToolOptions(IGenerationContext context, ProjectOptionsGenerationContext optionsContext)
        {
            Strings manifestInputs = new Strings();

            string vsManifestFilesPath = Util.SimplifyPath(Path.Combine(context.DevelopmentEnvironment.GetVisualStudioVCRootPath(), "Include", "Manifest"));

            //EnableDpiAwareness
            context.SelectOption
            (
            Options.Option(Options.Vc.ManifestTool.EnableDpiAwareness.Default, () => { context.Options["EnableDpiAwareness"] = FileGeneratorUtilities.RemoveLineTag; }),
            Options.Option(Options.Vc.ManifestTool.EnableDpiAwareness.Yes, () => { context.Options["EnableDpiAwareness"] = "true"; manifestInputs.Add(Path.Combine(vsManifestFilesPath, "dpiaware.manifest")); }),
            Options.Option(Options.Vc.ManifestTool.EnableDpiAwareness.PerMonitor, () => { context.Options["EnableDpiAwareness"] = "PerMonitorHighDPIAware"; manifestInputs.Add(Path.Combine(vsManifestFilesPath, "PerMonitorHighDPIAware.manifest")); }),
            Options.Option(Options.Vc.ManifestTool.EnableDpiAwareness.No, () => { context.Options["EnableDpiAwareness"] = "false"; })
            );

            if (context.Configuration.AdditionalManifestFiles.Count > 0)
            {
                context.Options["AdditionalManifestFiles"] = string.Join(";", Util.PathGetRelative(context.ProjectDirectory, context.Configuration.AdditionalManifestFiles));
                foreach (string additionalManifestFile in context.Configuration.AdditionalManifestFiles)
                    manifestInputs.Add(Util.GetConvertedRelativePath(context.ProjectDirectory, additionalManifestFile, optionsContext.BaseProjectPath, true, context.Project.RootPath));
            }
            else
                context.Options["AdditionalManifestFiles"] = FileGeneratorUtilities.RemoveLineTag;

            if (manifestInputs.Count > 0)
            {
                Options.Vc.Linker.EmbedManifest embedManifest = Options.GetObject<Options.Vc.Linker.EmbedManifest>(context.Configuration);
                if (embedManifest == Options.Vc.Linker.EmbedManifest.No)
                    throw new NotImplementedException("Sharpmake does not support manifestinputs without embedding the manifest!");

                StringBuilder result = new StringBuilder();
                foreach (string manifest in manifestInputs)
                {
                    result.Append(@"/manifestinput:""" + manifest + @""" ");
                }
                result.Remove(result.Length - 1, 1);
                context.CommandLineOptions["ManifestInputs"] = result.ToString();
            }
            else
            {
                context.CommandLineOptions["ManifestInputs"] = FileGeneratorUtilities.RemoveLineTag;
            }
        }

        private void GeneratePostBuildOptions(IGenerationContext context, ProjectOptionsGenerationContext optionsContext)
        {
            string eventSeparator = "&#x0D;&#x0A;";

            context.Options["PreBuildEvent"] = context.Configuration.EventPreBuild.Count == 0 ? FileGeneratorUtilities.RemoveLineTag : (context.Configuration.EventPreBuild.JoinStrings(eventSeparator) + eventSeparator).Replace(@"""", @"&quot;");
            context.Options["PreBuildEventDescription"] = context.Configuration.EventPreBuildDescription != String.Empty ? context.Configuration.EventPreBuildDescription : FileGeneratorUtilities.RemoveLineTag;
            context.Options["PreBuildEventEnable"] = context.Configuration.EventPreBuildExcludedFromBuild ? "false" : "true";

            context.Options["PreLinkEvent"] = context.Configuration.EventPreLink.Count == 0 ? FileGeneratorUtilities.RemoveLineTag : (context.Configuration.EventPreLink.JoinStrings(eventSeparator) + eventSeparator).Replace(@"""", @"&quot;");
            context.Options["PreLinkEventDescription"] = context.Configuration.EventPreLinkDescription != String.Empty ? context.Configuration.EventPreLinkDescription : FileGeneratorUtilities.RemoveLineTag;
            context.Options["PreLinkEventEnable"] = context.Configuration.EventPreLinkExcludedFromBuild ? "false" : "true";

            context.Options["PrePostLinkEvent"] = context.Configuration.EventPrePostLink.Count == 0 ? FileGeneratorUtilities.RemoveLineTag : (context.Configuration.EventPrePostLink.JoinStrings(eventSeparator) + eventSeparator).Replace(@"""", @"&quot;");
            context.Options["PrePostLinkEventDescription"] = context.Configuration.EventPrePostLinkDescription != String.Empty ? context.Configuration.EventPrePostLinkDescription : FileGeneratorUtilities.RemoveLineTag;
            context.Options["PrePostLinkEventEnable"] = context.Configuration.EventPrePostLinkExcludedFromBuild ? "false" : "true";

            if (context.Configuration.Output == Project.Configuration.OutputType.Exe || context.Configuration.ExecuteTargetCopy)
            {
                foreach (string copyFile in context.Configuration.ResolvedTargetCopyFiles)
                {
                    string copyFileDirectory = Path.GetDirectoryName(copyFile);
                    if (String.Compare(copyFileDirectory, context.Configuration.TargetPath, StringComparison.OrdinalIgnoreCase) != 0)
                    {
                        string relativeCopyFile = Util.PathGetRelative(context.ProjectDirectory, copyFile);
                        if (context.Configuration.IsFastBuild)
                        {
                            string targetFileName = Path.Combine(optionsContext.OutputDirectoryRelative, Path.GetFileName(copyFile));
                            relativeCopyFile = Util.GetConvertedRelativePath(context.ProjectDirectory, relativeCopyFile, optionsContext.BaseProjectPath, true, context.Project.RootPath);
                            targetFileName = Util.GetConvertedRelativePath(context.ProjectDirectory, targetFileName, optionsContext.BaseProjectPath, true, context.Project.RootPath);
                            context.Configuration.EventPostBuildCopies.Add(new KeyValuePair<string, string>(optionsContext.Resolver.Resolve(relativeCopyFile), optionsContext.Resolver.Resolve(targetFileName)));
                        }
                        else
                        {
                            if (context.Configuration.CopyDependenciesBuildStep == null)
                            {
                                context.Configuration.CopyDependenciesBuildStep = new Project.Configuration.FileCustomBuild();
                                context.Configuration.CopyDependenciesBuildStep.Description = "Copy files to output paths...";
                            }

                            context.Configuration.CopyDependenciesBuildStep.CommandLines.Add(context.Configuration.CreateTargetCopyCommand(relativeCopyFile, optionsContext.OutputDirectoryRelative, context.ProjectDirectory));
                            context.Configuration.CopyDependenciesBuildStep.Inputs.Add(relativeCopyFile);
                            context.Configuration.CopyDependenciesBuildStep.Outputs.Add(Path.Combine(optionsContext.OutputDirectoryRelative, Path.GetFileName(copyFile)));
                        }
                    }
                }

                foreach (var customEvent in context.Configuration.ResolvedEventPreBuildExe)
                {
                    if (context.Configuration.IsFastBuild && optionsContext.Resolver != null)
                    {
                        if (customEvent is Project.Configuration.BuildStepExecutable)
                        {
                            ResolveExecutable(context, optionsContext, customEvent, Vcxproj.BuildStep.PreBuild);
                        }
                        else if (customEvent is Project.Configuration.BuildStepCopy)
                        {
                            ResolveCopy(context, optionsContext, customEvent, Vcxproj.BuildStep.PreBuild);
                        }
                    }
                    else
                    {
                        if (customEvent is Project.Configuration.BuildStepExecutable)
                        {
                            var execEvent = (Project.Configuration.BuildStepExecutable)customEvent;

                            string relativeExecutableFile = Util.PathGetRelative(context.ProjectDirectory, execEvent.ExecutableFile);
                            context.Configuration.EventPreBuild.Add(String.Format(relativeExecutableFile + @" {0}", execEvent.ExecutableOtherArguments));
                        }
                        else if (customEvent is Project.Configuration.BuildStepCopy)
                        {
                            var copyEvent = (Project.Configuration.BuildStepCopy)customEvent;
                            context.Configuration.EventPreBuild.Add(copyEvent.GetCopyCommand(context.ProjectDirectory, optionsContext.Resolver));
                        }
                        else
                        {
                            throw new Error("Invalid type in Prebuild steps");
                        }
                    }
                }

                foreach (var customEvent in context.Configuration.ResolvedEventPostBuildExe)
                {
                    if (context.Configuration.IsFastBuild && optionsContext.Resolver != null)
                    {
                        if (customEvent is Project.Configuration.BuildStepExecutable)
                        {
                            ResolveExecutable(context, optionsContext, customEvent, Vcxproj.BuildStep.PostBuild);
                        }
                        else if (customEvent is Project.Configuration.BuildStepCopy)
                        {
                            ResolveCopy(context, optionsContext, customEvent, Vcxproj.BuildStep.PostBuild);
                        }
                    }
                    else
                    {
                        if (customEvent is Project.Configuration.BuildStepExecutable)
                        {
                            var execEvent = (Project.Configuration.BuildStepExecutable)customEvent;

                            string relativeExecutableFile = Util.PathGetRelative(context.ProjectDirectory, execEvent.ExecutableFile);
                            string eventString = string.Format(
                                "{0} {1}",
                                Util.SimplifyPath(optionsContext.Resolver.Resolve(relativeExecutableFile)),
                                optionsContext.Resolver.Resolve(execEvent.ExecutableOtherArguments)
                            );

                            if (!context.Configuration.EventPostBuild.Contains(eventString))
                                context.Configuration.EventPostBuild.Add(eventString);
                        }
                        else if (customEvent is Project.Configuration.BuildStepCopy)
                        {
                            var copyEvent = (Project.Configuration.BuildStepCopy)customEvent;
                            string eventString = copyEvent.GetCopyCommand(context.ProjectDirectory, optionsContext.Resolver);

                            if (!context.Configuration.EventPostBuild.Contains(eventString))
                                context.Configuration.EventPostBuild.Add(eventString);
                        }
                        else
                        {
                            throw new Error("Invalid type in PostBuild steps");
                        }
                    }
                }

                foreach (var customEvent in context.Configuration.ResolvedEventCustomPreBuildExe)
                {
                    if (context.Configuration.IsFastBuild && optionsContext.Resolver != null)
                    {
                        if (customEvent is Project.Configuration.BuildStepExecutable)
                        {
                            ResolveExecutable(context, optionsContext, customEvent, Vcxproj.BuildStep.PreBuildCustomAction);
                        }
                        else if (customEvent is Project.Configuration.BuildStepCopy)
                        {
                            ResolveCopy(context, optionsContext, customEvent, Vcxproj.BuildStep.PreBuildCustomAction);
                        }
                    }
                }

                foreach (var customEvent in context.Configuration.ResolvedEventCustomPostBuildExe)
                {
                    if (context.Configuration.IsFastBuild && optionsContext.Resolver != null)
                    {
                        if (customEvent is Project.Configuration.BuildStepExecutable)
                        {
                            ResolveExecutable(context, optionsContext, customEvent, Vcxproj.BuildStep.PostBuildCustomAction);
                        }
                        else if (customEvent is Project.Configuration.BuildStepCopy)
                        {
                            ResolveCopy(context, optionsContext, customEvent, Vcxproj.BuildStep.PostBuildCustomAction);
                        }
                    }
                }
            }

            if (context.Configuration.Output == Project.Configuration.OutputType.Exe || context.Configuration.Output == Project.Configuration.OutputType.Dll)
            {
                if (context.Configuration.PostBuildStampExe != null && !context.Configuration.IsFastBuild)
                {
                    context.Configuration.EventPostBuild.Insert(0,
                        string.Format(
                            "{0} {1} {2} {3}",
                            Util.SimplifyPath(Util.PathGetRelative(context.ProjectDirectory, optionsContext.Resolver.Resolve(context.Configuration.PostBuildStampExe.ExecutableFile))),
                            context.Configuration.PostBuildStampExe.ExecutableInputFileArgumentOption,
                            context.Configuration.PostBuildStampExe.ExecutableOutputFileArgumentOption,
                            context.Configuration.PostBuildStampExe.ExecutableOtherArguments
                        )
                    );
                }
            }

            context.Options["PreBuildEvent"] = context.Configuration.EventPreBuild.Count == 0 ? FileGeneratorUtilities.RemoveLineTag : (context.Configuration.EventPreBuild.JoinStrings(eventSeparator, escapeXml: true) + eventSeparator);
            context.Options["PreBuildEventDescription"] = context.Configuration.EventPreBuildDescription != String.Empty ? context.Configuration.EventPreBuildDescription : FileGeneratorUtilities.RemoveLineTag;
            context.Options["PreBuildEventEnable"] = context.Configuration.EventPreBuildExcludedFromBuild ? "false" : "true";

            context.Options["PostBuildEvent"] = context.Configuration.EventPostBuild.Count == 0 ? FileGeneratorUtilities.RemoveLineTag : (Util.JoinStrings(context.Configuration.EventPostBuild, eventSeparator, escapeXml: true) + eventSeparator);
            context.Options["PostBuildEventDescription"] = context.Configuration.EventPostBuildDescription != String.Empty ? context.Configuration.EventPostBuildDescription : FileGeneratorUtilities.RemoveLineTag;
            context.Options["PostBuildEventEnable"] = context.Configuration.EventPostBuildExcludedFromBuild ? "false" : "true";

            context.Options["CustomBuildEvent"] = context.Configuration.EventCustomBuild.Count == 0 ? FileGeneratorUtilities.RemoveLineTag : (context.Configuration.EventCustomBuild.JoinStrings(eventSeparator, escapeXml: true) + eventSeparator);
            context.Options["CustomBuildEventDescription"] = context.Configuration.EventCustomBuildDescription != String.Empty ? context.Configuration.EventCustomBuildDescription : FileGeneratorUtilities.RemoveLineTag;
            context.Options["CustomBuildEventOutputs"] = context.Configuration.EventCustomBuildOutputs != String.Empty ? context.Configuration.EventCustomBuildOutputs : FileGeneratorUtilities.RemoveLineTag;

            context.Options["CustomBuildStep"] = context.Configuration.CustomBuildStep.Count == 0 ? FileGeneratorUtilities.RemoveLineTag : (Util.JoinStrings(context.Configuration.CustomBuildStep, eventSeparator, escapeXml: true) + eventSeparator);
            context.Options["CustomBuildStepDescription"] = context.Configuration.CustomBuildStepDescription != String.Empty ? context.Configuration.CustomBuildStepDescription : FileGeneratorUtilities.RemoveLineTag;
            context.Options["CustomBuildStepOutputs"] = context.Configuration.CustomBuildStepOutputs.Count == 0 ? FileGeneratorUtilities.RemoveLineTag : (context.Configuration.CustomBuildStepOutputs.JoinStrings(";", escapeXml: true));
            context.Options["CustomBuildStepInputs"] = context.Configuration.CustomBuildStepInputs.Count == 0 ? FileGeneratorUtilities.RemoveLineTag : (context.Configuration.CustomBuildStepInputs.JoinStrings(";", escapeXml: true));
            context.Options["CustomBuildStepBeforeTargets"] = context.Configuration.CustomBuildStepBeforeTargets != String.Empty ? context.Configuration.CustomBuildStepBeforeTargets : FileGeneratorUtilities.RemoveLineTag;
            context.Options["CustomBuildStepAfterTargets"] = context.Configuration.CustomBuildStepAfterTargets != String.Empty ? context.Configuration.CustomBuildStepAfterTargets : FileGeneratorUtilities.RemoveLineTag;
            context.Options["CustomBuildStepTreatOutputAsContent"] = context.Configuration.CustomBuildStepTreatOutputAsContent != String.Empty ? context.Configuration.CustomBuildStepTreatOutputAsContent : FileGeneratorUtilities.RemoveLineTag;
        }

        private void ResolveCopy(IGenerationContext context, ProjectOptionsGenerationContext optionsContext, Project.Configuration.BuildStepBase copyBuildStep, Vcxproj.BuildStep buildStep)
        {
            var customEvent = (Project.Configuration.BuildStepCopy)copyBuildStep;
            var copyCustomEvent = new Project.Configuration.BuildStepCopy(customEvent);

            var isFolder = !copyCustomEvent.DestinationPath.Substring(copyCustomEvent.DestinationPath.LastIndexOf(@"\", System.StringComparison.Ordinal)).Contains(".");

            if (isFolder || Directory.Exists(copyCustomEvent.SourcePath) && (Directory.Exists(copyCustomEvent.DestinationPath)))
                IntermediateCopyStep(context, optionsContext, copyCustomEvent, buildStep, true);
            else
                IntermediateCopyStep(context, optionsContext, copyCustomEvent, buildStep);
        }

        private void IntermediateCopyStep(IGenerationContext context, ProjectOptionsGenerationContext optionsContext, Project.Configuration.BuildStepCopy customEvent, Vcxproj.BuildStep buildStep, bool isDirectory = false)
        {
            customEvent.DestinationPath = Util.GetConvertedRelativePath(context.ProjectDirectory, optionsContext.Resolver.Resolve(customEvent.DestinationPath), optionsContext.BaseProjectPath, true);
            customEvent.SourcePath = Util.GetConvertedRelativePath(context.ProjectDirectory, optionsContext.Resolver.Resolve(customEvent.SourcePath), optionsContext.BaseProjectPath, true);

            var newCustomEvent = new Project.Configuration.BuildStepCopy(customEvent);

            if (isDirectory)
                newCustomEvent.IsFileCopy = false;

            var copyName = GetBuildStepName(context, optionsContext, newCustomEvent, buildStep);
            AddEventToBuildStep(context, newCustomEvent, buildStep, copyName);
        }

        private void ResolveExecutable(IGenerationContext context, ProjectOptionsGenerationContext optionsContext, Project.Configuration.BuildStepBase executableBuildStep, Vcxproj.BuildStep buildStep)
        {
            var customEvent = (Project.Configuration.BuildStepExecutable)executableBuildStep;

            var newCustomEvent = new Project.Configuration.BuildStepExecutable(
                Util.SimplifyPath(Util.GetConvertedRelativePath(context.ProjectDirectory, optionsContext.Resolver.Resolve(customEvent.ExecutableFile), optionsContext.BaseProjectPath, true)),
                Util.SimplifyPath(Util.GetConvertedRelativePath(context.ProjectDirectory, optionsContext.Resolver.Resolve(customEvent.ExecutableInputFileArgumentOption), optionsContext.BaseProjectPath, true)),
                Util.SimplifyPath(Util.GetConvertedRelativePath(context.ProjectDirectory, optionsContext.Resolver.Resolve(customEvent.ExecutableOutputFileArgumentOption), optionsContext.BaseProjectPath, true)),
                optionsContext.Resolver.Resolve(customEvent.ExecutableOtherArguments),
                customEvent.ExecutableWorkingDirectory == String.Empty ? FileGeneratorUtilities.RemoveLineTag : Util.GetConvertedRelativePath(context.ProjectDirectory, optionsContext.Resolver.Resolve(customEvent.ExecutableWorkingDirectory), optionsContext.BaseProjectPath, true));

            newCustomEvent.FastBuildUseStdOutAsOutput = customEvent.FastBuildUseStdOutAsOutput;

            var execName = GetBuildStepName(context, optionsContext, newCustomEvent, buildStep);
            AddEventToBuildStep(context, newCustomEvent, buildStep, execName);
        }

        private void AddEventToBuildStep(IGenerationContext context, Project.Configuration.BuildStepBase eventBuildStep, Vcxproj.BuildStep buildStep, string eventName)
        {
            switch (buildStep)
            {
                case Vcxproj.BuildStep.PostBuild:
                    if (!context.Configuration.EventPostBuildExecute.ContainsKey(eventName))
                        context.Configuration.EventPostBuildExecute.Add(eventName, eventBuildStep);
                    break;

                case Vcxproj.BuildStep.PostBuildCustomAction:
                    if (!context.Configuration.EventCustomPostBuildExecute.ContainsKey(eventName))
                        context.Configuration.EventCustomPostBuildExecute.Add(eventName, eventBuildStep);
                    break;

                case Vcxproj.BuildStep.PreBuild:
                    if (!context.Configuration.EventPreBuildExecute.ContainsKey(eventName))
                        context.Configuration.EventPreBuildExecute.Add(eventName, eventBuildStep);
                    break;

                case Vcxproj.BuildStep.PreBuildCustomAction:
                    if (!context.Configuration.EventCustomPrebuildExecute.ContainsKey(eventName))
                        context.Configuration.EventCustomPrebuildExecute.Add(eventName, eventBuildStep);
                    break;
            }
        }

        private string GetBuildStepName(IGenerationContext context, ProjectOptionsGenerationContext optionsContext, Project.Configuration.BuildStepBase eventBuildStep, Vcxproj.BuildStep buildStep)
        {
            if (eventBuildStep is Project.Configuration.BuildStepExecutable)
            {
                var cEvent = eventBuildStep as Project.Configuration.BuildStepExecutable;
                string execName;

                if (buildStep == Vcxproj.BuildStep.PostBuild || buildStep == Vcxproj.BuildStep.PostBuildCustomAction || cEvent.IsNameSpecific)
                {
                    execName = @"Exec_"
                               + cEvent.ExecutableFile.Substring(cEvent.ExecutableFile.LastIndexOf(@"\") + 1);
                    execName += "_" + (context.Configuration.TargetPath + context.Configuration.TargetFileFullName).GetHashCode().ToString("X8");
                }
                else
                {
                    execName = @"Exec_"
                           + cEvent.ExecutableFile.Substring(cEvent.ExecutableFile.LastIndexOf(@"\") + 1);
                    execName += "_" + (execName).GetHashCode().ToString("X8");
                }
                execName = execName.Replace('.', '_');

                return execName;
            }

            if (eventBuildStep is Project.Configuration.BuildStepCopy)
            {
                var cEvent = eventBuildStep as Project.Configuration.BuildStepCopy;
                string copyName;

                if (buildStep == Vcxproj.BuildStep.PostBuild || buildStep == Vcxproj.BuildStep.PostBuildCustomAction || cEvent.IsNameSpecific)
                {
                    copyName = "Copy_" + (optionsContext.Resolver.Resolve(context.Configuration.TargetFileFullName) + cEvent.SourcePath + cEvent.DestinationPath).GetHashCode().ToString("X8");
                }
                else
                {
                    copyName = "Copy_" + (cEvent.SourcePath + cEvent.DestinationPath).GetHashCode().ToString("X8");
                }

                return copyName;
            }

            return null;
        }

        private static void SelectAdditionalLibraryDirectoriesOption(IGenerationContext context, ProjectOptionsGenerationContext optionsContext)
        {
            var libDirs = new List<string>(context.Configuration.LibraryPaths);
            libDirs.AddRange(context.Configuration.DependenciesLibraryPaths);
            libDirs.AddRange(optionsContext.PlatformVcxproj.GetLibraryPaths(context));
            var relativeAdditionalLibraryDirectories = Util.PathGetRelative(context.ProjectDirectory, libDirs).ToList();

            if (relativeAdditionalLibraryDirectories.Any())
            {
                string addLibDirsStr = string.Join(";", relativeAdditionalLibraryDirectories);
                context.Options["AdditionalLibraryDirectories"] = addLibDirsStr;
            }
            else
            {
                context.Options["AdditionalLibraryDirectories"] = FileGeneratorUtilities.RemoveLineTag;
            }

            relativeAdditionalLibraryDirectories.AddRange(optionsContext.PlatformVcxproj.GetPlatformLibraryPaths(context));

            if (optionsContext.Resolver != null)
            {
                var addLibDirs = relativeAdditionalLibraryDirectories.Select(path => optionsContext.Resolver.Resolve(path)).ToArray();
                if (addLibDirs.Length > 0)
                {
                    StringBuilder result = new StringBuilder();

                    string linkOption = "";
                    if (!PlatformRegistry.Get<IPlatformDescriptor>(context.Configuration.Platform).IsUsingClang)
                        linkOption = @"/LIBPATH:";
                    else
                        linkOption = @"-L";

                    bool first = true;
                    foreach (string additionalLibraryDirectory in addLibDirs)
                    {
                        if (!first)
                            result.Append(" ");
                        else
                            first = false;

                        string convertedRelativePath = Util.GetConvertedRelativePath(context.ProjectDirectory, additionalLibraryDirectory, Bff.GetMasterBffPath(context.Configuration), true, context.Project.RootPath);
                        result.Append($@"{linkOption}""{convertedRelativePath}""");

                    }

                    context.CommandLineOptions["AdditionalLibraryDirectories"] = result.ToString();
                }
            }
        }

        private static void SelectAdditionalDependenciesOption(IGenerationContext context, ProjectOptionsGenerationContext optionsContext, OrderableStrings libraryFiles, Strings ignoreSpecificLibraryNames)
        {
            // convert all root paths to be relative to the vcxproj
            var convertedPaths = new HashSet<string>();
            for (int i = 0; i < libraryFiles.Count; ++i)
            {
                string libraryFile = libraryFiles[i];
                if (Path.IsPathRooted(libraryFile))
                {
                    libraryFiles[i] = Util.GetConvertedRelativePath(context.ProjectDirectory, libraryFile, context.ProjectDirectory, true, context.Project.RootPath);

                    // keep track of the converted paths, we'll need it later
                    convertedPaths.Add(libraryFiles[i]);
                }
            }
            libraryFiles.Sort();

            List<string> additionalDependencies = new List<string>(libraryFiles.Count);
            List<string> cmdLineAdditionalDependencies = new List<string>(libraryFiles.Count);
            for (int i = 0; i < libraryFiles.Count; ++i)
            {
                string libname = libraryFiles[i];

                // We've got two kinds of way of listing a library:
                // - With a filename without extension we must add the potential prefix and potential extension.
                //      Ex:  On clang we add -l (supposedly because the exact file is named lib<library>.a)
                // - With a filename with the lib extension (.a or .lib), we shouldn't touch it as it's already set by the script.
                if (Path.GetExtension(libname) != optionsContext.PlatformLibraryExtension)
                    libname = optionsContext.PlatformPrefixExtension + libname + optionsContext.PlatformOutputLibraryExtension;

                if (!ignoreSpecificLibraryNames.Contains(libname))
                {
                    additionalDependencies.Add(libname);

                    // Rebase any full path for FastBuild.
                    if (Path.IsPathRooted(libname))
                        libname = Util.GetConvertedRelativePath(context.ProjectDirectory, libname, optionsContext.BaseProjectPath, true, context.Project.RootPath);

                    cmdLineAdditionalDependencies.Add(libname);
                }
                else
                {
                    ignoreSpecificLibraryNames.Remove(libname);
                }
            }

            context.Options["AdditionalDependencies"] = string.Join(";", additionalDependencies);

            if (optionsContext.Resolver != null)
            {
                var platformAdditionalDependencies = optionsContext.PlatformVcxproj.GetPlatformLibraryFiles(context);

                // Joins the list of dependencies with a ; and then re-split them after a resolve.
                // We have to do it that way because a token can be resolved into a
                // semicolon -separated list of dependencies.
                string[] addDeps = optionsContext.Resolver.Resolve(string.Join(";", cmdLineAdditionalDependencies.Concat(platformAdditionalDependencies))).Split(new char[] { ';' }, StringSplitOptions.RemoveEmptyEntries);
                if (addDeps.Any())
                {
                    StringBuilder result = new StringBuilder();
                    foreach (string additionalDependency in addDeps)
                    {
                        string dependencyPath = additionalDependency;

                        // if the path to this dependency was converted
                        // convert it again to make it relative to the master bff
                        if (convertedPaths.Contains(additionalDependency))
                            dependencyPath = Util.GetConvertedRelativePath(context.ProjectDirectory, additionalDependency, optionsContext.BaseProjectPath, true, context.Project.RootPath);

                        result.Append(@"""" + dependencyPath + @""" ");
                    }

                    result.Remove(result.Length - 1, 1);
                    context.CommandLineOptions["AdditionalDependencies"] = result.ToString();
                }
            }

            optionsContext.PlatformVcxproj.SelectPlatformAdditionalDependenciesOptions(context);
        }

        private static void SelectGenerateManifestOption(IGenerationContext context, ProjectOptionsGenerationContext optionsContext)
        {
            context.SelectOption
            (
            Options.Option(Options.Vc.Linker.GenerateManifest.Enable, () =>
            {
                context.Options["GenerateManifest"] = "true";

                if (optionsContext.PlatformVcxproj.HasUserAccountControlSupport)
                {
                    context.CommandLineOptions["GenerateManifest"] = string.Format(@"/MANIFEST /MANIFESTUAC:""level=^'{0}^' uiAccess=^'false^'""", context.Configuration.ApplicationPermissions);

                    switch (context.Configuration.ApplicationPermissions)
                    {
                        case Project.Configuration.UACExecutionLevel.asInvoker:
                            context.Options["UACExecutionLevel"] = FileGeneratorUtilities.RemoveLineTag;
                            break;
                        case Project.Configuration.UACExecutionLevel.highestAvailable:
                        case Project.Configuration.UACExecutionLevel.requireAdministrator:
                            context.Options["UACExecutionLevel"] = context.Configuration.ApplicationPermissions.ToString();
                            break;
                    }
                }
                else
                {
                    context.CommandLineOptions["GenerateManifest"] = @"/MANIFEST /MANIFESTUAC:NO";
                    context.Options["UACExecutionLevel"] = FileGeneratorUtilities.RemoveLineTag;
                }

                if (context.Options["EmbedManifest"] == "false")
                {
                    string manifestFile = optionsContext.IntermediateDirectoryRelative + Util.WindowsSeparator + optionsContext.TargetName + context.Configuration.ManifestFileSuffix;
                    context.Options["ManifestFile"] = manifestFile;
                    context.CommandLineOptions["ManifestFile"] = @"/ManifestFile:""" + Util.GetConvertedRelativePath(context.ProjectDirectory, manifestFile, Bff.GetMasterBffPath(context.Configuration), true, context.Project.RootPath) + @"""";
                }
                else
                {
                    context.Options["ManifestFile"] = FileGeneratorUtilities.RemoveLineTag;
                    context.CommandLineOptions["ManifestFile"] = "/MANIFEST:EMBED";
                }
            }),
            Options.Option(Options.Vc.Linker.GenerateManifest.Disable, () =>
            {
                context.Options["GenerateManifest"] = "false";
                context.Options["ManifestFile"] = FileGeneratorUtilities.RemoveLineTag;
                context.CommandLineOptions["GenerateManifest"] = FileGeneratorUtilities.RemoveLineTag;
                context.CommandLineOptions["ManifestFile"] = FileGeneratorUtilities.RemoveLineTag;
                context.Options["UACExecutionLevel"] = FileGeneratorUtilities.RemoveLineTag;
            })
            );
        }

        private static void SelectGenerateDebugInformationOption(IGenerationContext context, ProjectOptionsGenerationContext optionsContext)
        {
            Action<bool> enableDebugInformation = (isFastLink) =>
            {
                if (isFastLink && context.Configuration.IsFastBuild)
                {
                    var generateFullPDB = Options.GetObject<Options.Vc.Linker.GenerateFullProgramDatabaseFile>(context.Configuration);
                    if (generateFullPDB == Options.Vc.Linker.GenerateFullProgramDatabaseFile.Enable)
                    {
                        isFastLink = false;
                        context.Builder.LogWarningLine("FastBuild and generateFullPDB will disable FastLink");
                    }
                }

                context.Options["GenerateDebugInformation"] = isFastLink ? "DebugFastLink" : "true";

                context.SelectOption
                (
                Options.Option(Options.Vc.Linker.GenerateFullProgramDatabaseFile.Enable, () => { context.Options["FullProgramDatabaseFile"] = "true"; }),
                Options.Option(Options.Vc.Linker.GenerateFullProgramDatabaseFile.Disable, () => { context.Options["FullProgramDatabaseFile"] = FileGeneratorUtilities.RemoveLineTag; })
                );

                string optionsCompilerProgramDatabaseFile = context.Configuration.CompilerPdbFilePath;
                string optionsLinkerProgramDatabaseFile = context.Configuration.LinkerPdbFilePath;
                string cmdLineOptionsCompilerProgramDatabaseFile = context.Configuration.CompilerPdbFilePath;
                string cmdLineOptionsLinkerProgramDatabaseFile = context.Configuration.LinkerPdbFilePath;

                if (context.Configuration.UseRelativePdbPath)
                {
                    optionsCompilerProgramDatabaseFile = Util.PathGetRelative(context.ProjectDirectory, optionsCompilerProgramDatabaseFile, true);
                    optionsLinkerProgramDatabaseFile = Util.PathGetRelative(context.ProjectDirectory, optionsLinkerProgramDatabaseFile, true);

                    string masterBffPath = Bff.GetMasterBffPath(context.Configuration);
                    cmdLineOptionsCompilerProgramDatabaseFile = Util.GetConvertedRelativePath(context.ProjectDirectory, cmdLineOptionsCompilerProgramDatabaseFile, masterBffPath, true, context.Project.RootPath);
                    cmdLineOptionsLinkerProgramDatabaseFile = Util.GetConvertedRelativePath(context.ProjectDirectory, cmdLineOptionsLinkerProgramDatabaseFile, masterBffPath, true, context.Project.RootPath);
                }

                context.Options["CompilerProgramDatabaseFile"] = string.IsNullOrEmpty(optionsCompilerProgramDatabaseFile)
                    ? FileGeneratorUtilities.RemoveLineTag
                    : optionsCompilerProgramDatabaseFile;
                context.Options["LinkerProgramDatabaseFile"] = string.IsNullOrEmpty(optionsLinkerProgramDatabaseFile)
                    ? FileGeneratorUtilities.RemoveLineTag
                    : optionsLinkerProgramDatabaseFile;
                context.CommandLineOptions["GenerateDebugInformation"] = isFastLink ? "/DEBUG:FASTLINK" : "/DEBUG";

                // %2 is converted by FastBuild
                // Output name of object being compiled, as specified by CompilerOutputPath and the name of discovered objects depending on the Compiler input options (extension is also replace with CompilerOutputExtension).
                if (FastBuildSettings.EnableFastLinkPDBSupport && isFastLink)
                    context.CommandLineOptions["CompilerProgramDatabaseFile"] = @"/Fd""%2.pdb""";
                else if (!string.IsNullOrEmpty(cmdLineOptionsCompilerProgramDatabaseFile))
                    context.CommandLineOptions["CompilerProgramDatabaseFile"] = $@"/Fd""{cmdLineOptionsCompilerProgramDatabaseFile}""";
                else
                    context.CommandLineOptions["CompilerProgramDatabaseFile"] = FileGeneratorUtilities.RemoveLineTag;

                if (!string.IsNullOrEmpty(cmdLineOptionsLinkerProgramDatabaseFile))
                    context.CommandLineOptions["LinkerProgramDatabaseFile"] = $@"/PDB:""{cmdLineOptionsLinkerProgramDatabaseFile}""";
                else
                    context.CommandLineOptions["LinkerProgramDatabaseFile"] = FileGeneratorUtilities.RemoveLineTag;
            };

            context.SelectOption
            (
            Options.Option(Options.Vc.Linker.GenerateDebugInformation.Enable, () => { enableDebugInformation(false); }),
            Options.Option(Options.Vc.Linker.GenerateDebugInformation.EnableFastLink, () =>
            {
                if (optionsContext.HasClrSupport)
                    context.Builder.LogWarningLine("GenerateDebugInformation.EnableFastLink is not supported with CLR/dot net (project: " + context.Project.Name + "), fallback to GenerateDebugInformation.Enable");
                enableDebugInformation(!optionsContext.HasClrSupport);
            }),
            Options.Option(Options.Vc.Linker.GenerateDebugInformation.Disable, () =>
            {
                context.Options["GenerateDebugInformation"] = "false";
                context.Options["CompilerProgramDatabaseFile"] = FileGeneratorUtilities.RemoveLineTag;
                context.Options["LinkerProgramDatabaseFile"] = FileGeneratorUtilities.RemoveLineTag;
                context.Options["FullProgramDatabaseFile"] = FileGeneratorUtilities.RemoveLineTag;

                context.CommandLineOptions["GenerateDebugInformation"] = FileGeneratorUtilities.RemoveLineTag;
                context.CommandLineOptions["CompilerProgramDatabaseFile"] = FileGeneratorUtilities.RemoveLineTag;
                context.CommandLineOptions["LinkerProgramDatabaseFile"] = FileGeneratorUtilities.RemoveLineTag;
            })
            );
        }

        private static void SelectGenerateMapFileOption(IGenerationContext context, ProjectOptionsGenerationContext optionsContext)
        {
            Action enableMapOption = () =>
            {
                context.Options["GenerateMapFile"] = "true";
                string mapFile = optionsContext.OutputDirectoryRelative + Util.WindowsSeparator + optionsContext.TargetName + ".map";
                context.Options["MapFileName"] = mapFile;
                mapFile = Util.GetConvertedRelativePath(context.ProjectDirectory, mapFile, Bff.GetMasterBffPath(context.Configuration), true, context.Project.RootPath);

                if (PlatformRegistry.Get<IPlatformDescriptor>(context.Configuration.Platform).IsUsingClang)
                {
                    context.CommandLineOptions["GenerateMapFile"] = @"-Wl,-Map=""" + mapFile + @"""";
                }
                else
                {
                    context.CommandLineOptions["GenerateMapFile"] = @"/MAP"":" + mapFile + @"""";
                }
            };

            context.SelectOption
            (
            Options.Option(Options.Vc.Linker.GenerateMapFile.Disable, () =>
            {
                context.Options["GenerateMapFile"] = "false";
                context.Options["MapFileName"] = "";
                context.CommandLineOptions["GenerateMapFile"] = FileGeneratorUtilities.RemoveLineTag;
            }),
            Options.Option(Options.Vc.Linker.GenerateMapFile.Normal, enableMapOption),
            Options.Option(Options.Vc.Linker.GenerateMapFile.Full, enableMapOption)
            );
        }
    }
}
