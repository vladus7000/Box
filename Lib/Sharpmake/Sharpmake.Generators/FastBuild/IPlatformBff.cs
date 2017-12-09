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
using System.Collections.Generic;

namespace Sharpmake.Generators.FastBuild
{
    //
    // TODO: Streamline this with the Vcxproj interfaces. There is no reason why BFF and VCXPROJ
    //       generation need to specify twice the platform defines, why both have to specify
    //       whether it takes a lib- prefix before libraries, etc. Furthermore, these interfaces
    //       should be about providing the information required to generate the files, and less
    //       about participating in the generation.
    //

    /// <summary>
    /// Interface that exposes the required methods and properties to generate a .bff file for
    /// FastBuild using Sharpmake.
    /// </summary>
    public interface IPlatformBff
    {
        /// <summary>
        /// Gets the main `#define` symbol for that platform in the BFF file.
        /// </summary>
        /// <remarks>
        /// Note that this is *NOT* the C or C++ define symbol. The BFF scripts support the
        /// `#define` instruction, and this property returns a symbol that tells the scripts
        /// whether we're dealing with a given platform.
        /// </remarks>
        string BffPlatformDefine { get; }

        /// <summary>
        /// Gets a configuration name for that platform in the .bff file for the code files that
        /// are written in native C code.
        /// </summary>
        string CConfigName { get; }

        /// <summary>
        /// Gets a configuration name for that platform in the .bff file for the code files that
        /// are written in native C++ code.
        /// </summary>
        string CppConfigName { get; }

        /// <summary>
        /// Gets whether a library prefix (usually `lib`) is required on that platform when
        /// building libraries.
        /// </summary>
        /// <param name="conf">The <see cref="Configuration"/> under which the check is requested.</param>
        /// <returns>`true` if a prefix is required, `false` otherwise.</returns>
        bool AddLibPrefix(Configuration conf);

        /// <summary>
        /// Setups extra linker settings for linking with that platform.
        /// </summary>
        /// <param name="fileGenerator">A <see cref="IFileGenerator"/> for writing the file.</param>
        /// <param name="outputType">The build output.</param>
        /// <param name="fastBuildOutputFile">The file name of the build output.</param>
        void SetupExtraLinkerSettings(IFileGenerator fileGenerator, Project.Configuration.OutputType outputType, string fastBuildOutputFile);

        CompilerSettings GetMasterCompilerSettings(
               IDictionary<string, CompilerSettings> masterCompilerSettings,
               string compilerName,
               string rootPath,
               DevEnv devEnv,
               string projectRootPath,
               bool useCCompiler);

        void AddCompilerSettings(
            IDictionary<string, CompilerSettings> masterCompilerSettings,
            string compilerName,
            string rootPath,
            DevEnv devEnv,
            string projectRootPath);

        void SetConfiguration(
            IDictionary<string, CompilerSettings.Configuration> configurations,
            string compilerName,
            string projectRootPath,
            DevEnv devEnv,
            bool useCCompiler);
    }
}
