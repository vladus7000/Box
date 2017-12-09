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
using System.CodeDom.Compiler;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text.RegularExpressions;
using Microsoft.Build.Utilities;

namespace Sharpmake
{
    public class Assembler
    {
        /// <summary>
        /// Extra user directory to load assembly from using statement detection
        /// </summary>
        public List<string> AssemblyDirectory { get { return _assemblyDirectory; } }

        /// <summary>
        /// Extra user assembly to use while compiling
        /// </summary>
        public List<Assembly> Assemblies { get { return _assemblies; } }

        /// <summary>
        /// Extra user assembly file name to use while compiling
        /// </summary>
        public List<string> References { get { return _references; } }

        public bool UseDefaultReferences = true;

        public static readonly string[] DefaultReferences = { "System.dll", "System.Core.dll" };

        public Assembly BuildAssembly(params string[] sourceFiles)
        {
            // Alway compile to a physic dll to be able to debug
            string tmpFile = GetTmpAssemblyFile();
            return Build(tmpFile, sourceFiles);
        }

        public static TDelegate BuildDelegate<TDelegate>(string sourceFilePath, string fullFunctionName, Assembly[] assemblies)
            where TDelegate : class
        {
            FileInfo fileInfo = new FileInfo(sourceFilePath);
            if (!fileInfo.Exists)
                throw new Error("source file name not found: {0}", sourceFilePath);

            Type delegateType = typeof(TDelegate);
            Error.Valid(IsDelegate(delegateType), "BuildDelegate<FUNC_TYPE>(), FUNC_TYPE is not a delegate");
            MethodInfo delegateMethodInfo = GetDelegateMethodInfo(delegateType);


            ParameterInfo[] delegateParameterInfos = delegateMethodInfo.GetParameters();
            ParameterInfo delegateReturnInfos = delegateMethodInfo.ReturnParameter;

            Assembly assembly;

            Assembler assembler = new Assembler();
            assembler.UseDefaultReferences = false;
            assembler.Assemblies.AddRange(assemblies);

            assembly = assembler.BuildAssembly(fileInfo.FullName);

            List<MethodInfo> matchMethods = new List<MethodInfo>();

            foreach (Type type in assembly.GetTypes())
            {
                MethodInfo[] methodInfos = type.GetMethods();

                foreach (MethodInfo methodInfo in methodInfos)
                {
                    string fullName = methodInfo.DeclaringType.FullName + "." + methodInfo.Name;
                    if (fullFunctionName == fullName &&
                        methodInfo.IsStatic && methodInfo.GetParameters().Length == delegateMethodInfo.GetParameters().Length)
                    {
                        ParameterInfo[] parameterInfos = methodInfo.GetParameters();
                        ParameterInfo returnInfos = methodInfo.ReturnParameter;

                        bool equal = (returnInfos.GetType() == delegateReturnInfos.GetType() &&
                                       parameterInfos.Length == delegateParameterInfos.Length);

                        if (equal)
                        {
                            for (int i = 0; i < parameterInfos.Length; ++i)
                            {
                                if (parameterInfos[i].GetType() != delegateParameterInfos[i].GetType())
                                {
                                    equal = false;
                                    break;
                                }
                            }
                        }
                        if (equal)
                            matchMethods.Add(methodInfo);
                    }
                }
            }

            if (matchMethods.Count != 1)
                throw new Error("Cannot find method name {0} that match {1} in {2}", fullFunctionName, delegateMethodInfo.ToString(), sourceFilePath);

            MethodInfo method = matchMethods[0];

            // bind the method
            Delegate returnDelegate;
            try
            {
                returnDelegate = method.CreateDelegate(delegateType);
                InternalError.Valid(returnDelegate != null);
            }
            catch (Exception e)
            {
                throw new InternalError(e);
            }

            TDelegate result = returnDelegate as TDelegate;
            InternalError.Valid(result != null, "Cannot cast built delegate into user delegate");

            return result;
        }

        public static TDelegate BuildDelegate<TDelegate>(string functionBody, string functionNamespace, string[] usingNamespaces, Assembly[] assemblies)
            where TDelegate : class
        {
            Assembler assembler = new Assembler();
            assembler.UseDefaultReferences = false;
            assembler.Assemblies.AddRange(assemblies);

            const string className = "AssemblerBuildFunction_Class";
            const string methodName = "AssemblerBuildFunction_Method";

            // Fix : Bug with -> Path.GetTempFileName
            // http://msdn.microsoft.com/en-ca/library/windows/desktop/aa364991(v=vs.85).aspx
            // Limit of 65535 limit on files when generating the temp file. New temp file will use
            // a new Guid as filename and Sharpmake will clean the temporary files when done by aggregating
            // the temp files and deleting them.
            // eg. "C:\\fastbuild-work\\85f7d472c25d494ca09f2ea7fe282d50"
            //string sourceTmpFile = Path.GetTempFileName();
            string sourceTmpFile = Path.Combine(Path.GetTempPath(), (Guid.NewGuid().ToString("N") + ".tmp"));

            Type delegateType = typeof(TDelegate);

            Error.Valid(IsDelegate(delegateType), "BuildDelegate<TDelegate>(), TDelegate is not a delegate");

            MethodInfo methodInfo = GetDelegateMethodInfo(delegateType);

            using (StreamWriter writer = new StreamWriter(sourceTmpFile))
            {
                // add using namespace...
                foreach (string usingNamespace in usingNamespaces)
                    writer.WriteLine("using {0};", usingNamespace);
                writer.WriteLine();

                // namespace name
                writer.WriteLine("namespace {0}", functionNamespace);
                writer.WriteLine("{");
                writer.WriteLine("    public static class {0}", className);
                writer.WriteLine("    {");

                // write method signature
                string returnTypeName = methodInfo.ReturnType == typeof(void) ? "void" : methodInfo.ReturnType.FullName;
                writer.Write("        public static {0} {1}(", returnTypeName, methodName);
                ParameterInfo[] parameters = methodInfo.GetParameters();
                for (int i = 0; i < parameters.Length; i++)
                {
                    string parametersName = parameters[i].Name;
                    string parametersType = (parameters[i].ParameterType == typeof(Object)) ? "Object" : parameters[i].ParameterType.FullName;

                    writer.Write("{0}{1} {2}", i == 0 ? "" : ", ", parametersType, parametersName);
                }
                writer.WriteLine(")");
                // write method body
                writer.WriteLine("        {");
                writer.WriteLine("            {0}" + Environment.NewLine, functionBody.Replace("\n", "\n            "));
                writer.WriteLine("        }");
                writer.WriteLine("    }");
                writer.WriteLine("}");
            }

            // build in memory
            Assembly assembly = assembler.Build(null, sourceTmpFile);
            InternalError.Valid(assembly != null);

            // Try to delete tmp file to prevent polution, but usefull while debugging
            //if (!System.Diagnostics.Debugger.IsAttached)
            Util.TryDeleteFile(sourceTmpFile);

            // Scan assembly to find our tmp class
            string fullClassName = functionNamespace + "." + className;
            Type buildedType = assembly.GetType(fullClassName);

            // get out method to bind into the delegate
            MethodInfo builtMethod = buildedType.GetMethod(methodName);
            InternalError.Valid(builtMethod != null);

            // bind the method
            Delegate returnDelegate;
            try
            {
                returnDelegate = builtMethod.CreateDelegate(delegateType);
                InternalError.Valid(returnDelegate != null);
            }
            catch (Exception e)
            {
                throw new InternalError(e);
            }

            TDelegate result = returnDelegate as TDelegate;
            InternalError.Valid(result != null, "Cannot cast built delegate into user delegate");

            return result;
        }

        #region Private

        private List<string> _assemblyDirectory = new List<string>();
        private List<Assembly> _assemblies = new List<Assembly>();
        public List<string> _references = new List<string>();
        private static Regex s_includeRegex = new Regex(@"^\s*\[module:\s*Sharpmake.Include([^\""])*\""((?<INCLUDE>([^\""]*)+))", RegexOptions.Compiled | RegexOptions.Singleline | RegexOptions.CultureInvariant);
        private static Regex s_referenceRegex = new Regex(@"^\s*\[module:\s*Sharpmake.Reference([^\""])*\""((?<REFERENCE>([^\""]*)+))", RegexOptions.Compiled | RegexOptions.Singleline | RegexOptions.CultureInvariant);

        private static bool IsDelegate(Type delegateType)
        {
            if (delegateType.BaseType != typeof(MulticastDelegate))
                return false;
            MethodInfo invoke = delegateType.GetMethod("Invoke");
            return (invoke != null);
        }

        private static MethodInfo GetDelegateMethodInfo(Type delegateType)
        {
            if (!IsDelegate(delegateType))
                throw new Error("not a delegate: {0}", delegateType);
            return delegateType.GetMethod("Invoke");
        }

        private Assembly Build(string libraryFile, params string[] sources)
        {
            List<string> sourceFiles = GetSourceFiles(sources);

            HashSet<string> references = new HashSet<string>();

            Dictionary<string, string> providerOptions = new Dictionary<string, string>();
            providerOptions.Add("CompilerVersion", "v4.0");
            CodeDomProvider provider = new Microsoft.CSharp.CSharpCodeProvider(providerOptions);

            CompilerParameters cp = new CompilerParameters();

            if (UseDefaultReferences)
            {
                foreach (string defaultReference in DefaultReferences)
                    references.Add(GetAssemblyDllPath(defaultReference));
            }

            foreach (string assemblyFile in _references)
                references.Add(assemblyFile);

            foreach (Assembly assembly in _assemblies)
            {
                if (!assembly.IsDynamic)
                    references.Add(assembly.Location);
            }

            cp.ReferencedAssemblies.AddRange(references.ToArray());

            // Generate an library
            cp.GenerateExecutable = false;

            // Set the level at which the compiler  
            // should start displaying warnings.
            cp.WarningLevel = 4;

            // Set whether to treat all warnings as errors.
            cp.TreatWarningsAsErrors = false;

            // Set compiler argument to optimize output.
            // TODO : figure out why it does not work when uncommenting the following line
            // cp.CompilerOptions = "/optimize";

            // Specify the assembly file name to generate
            if (libraryFile == null)
            {
                cp.GenerateInMemory = true;
                cp.IncludeDebugInformation = false;
            }
            else
            {
                cp.GenerateInMemory = false;
                cp.IncludeDebugInformation = true;
                cp.OutputAssembly = libraryFile;
            }

            // Notes:
            // Avoid getting spoiled by environment variables. 
            // C# will give compilation errors if a LIB variable contains non-existing directories.
            Environment.SetEnvironmentVariable("LIB", null);

            // Invoke compilation of the source file.
            CompilerResults cr = provider.CompileAssemblyFromFile(cp, sourceFiles.ToArray());

            if (cr.Errors.HasErrors)
            {
                string errorMessage = "";
                foreach (CompilerError ce in cr.Errors)
                    errorMessage += ce + Environment.NewLine;
                throw new Error(errorMessage);
            }

            return cr.CompiledAssembly;
        }

        internal List<string> GetSourceFiles(string[] sources)
        {
            List<string> sourceFiles = new List<string>(sources);

            // Get all using namespace from sourceFiles
            for (int i = 0; i < sourceFiles.Count; ++i)
            {
                string sourceFile = sourceFiles[i];
                if (File.Exists(sourceFile))
                {
                    List<string> includes = new List<string>();

                    AnalyseSourceFile(sourceFile, includes);

                    foreach (string include in includes)
                    {
                        if (!sourceFiles.Contains(include))
                            sourceFiles.Add(include);
                    }
                }
                else
                {
                    throw new Error("source file not found: " + sourceFile);
                }
            }

            return sourceFiles;
        }

        private void AnalyseSourceFile(string sourceFile, List<string> includes)
        {
            using (StreamReader reader = new StreamReader(sourceFile))
            {
                FileInfo sourceFilePath = new FileInfo(sourceFile);

                int lineNumber = 0;
                string line = reader.ReadLine();
                while (line != null)
                {
                    ++lineNumber;

                    Match match = s_includeRegex.Match(line);
                    for (; match.Success; match = match.NextMatch())
                    {
                        string includeFilename = match.Groups["INCLUDE"].ToString();
                        string resolvedIncludeFilename = "";

                        if (!Path.IsPathRooted(includeFilename))
                            resolvedIncludeFilename = Util.PathGetAbsolute(sourceFilePath.DirectoryName, includeFilename);

                        if (!File.Exists(resolvedIncludeFilename))
                            resolvedIncludeFilename = Util.GetCapitalizedPath(resolvedIncludeFilename);
                        if (!File.Exists(resolvedIncludeFilename))
                            throw new Error("\t" + sourceFilePath.FullName + "(" + lineNumber + "): error: Sharpmake.Include file not found {0}", includeFilename);

                        includes.Add(resolvedIncludeFilename);
                    }

                    match = s_referenceRegex.Match(line);
                    for (; match.Success; match = match.NextMatch())
                    {
                        string referenceRelativePath = match.Groups["REFERENCE"].ToString();
                        string referencePath = "";

                        if (!Path.IsPathRooted(referenceRelativePath))
                            referencePath = Util.PathGetAbsolute(sourceFilePath.DirectoryName, referenceRelativePath);

                        if (!File.Exists(referencePath))
                        {
                            referencePath = Util.PathGetAbsolute(Path.GetDirectoryName(Assembly.GetEntryAssembly().Location), referenceRelativePath);
                            if (!File.Exists(referencePath))
                            {
                                // try using .net framework locations
                                referencePath = GetAssemblyDllPath(referenceRelativePath);

                                if (referencePath == null)
                                    throw new Error("\t" + sourceFilePath.FullName + "(" + lineNumber + "): error: Sharpmake.Reference file not found: {0}", referenceRelativePath);
                            }
                        }

                        _references.Add(referencePath);
                    }
                    line = reader.ReadLine();
                }
            }
        }

        private static string GetAssemblyDllPath(string fileName)
        {
            for (int i = (int)TargetDotNetFrameworkVersion.VersionLatest; i >= 0; --i)
            {
                string frameworkDirectory = null;
#if !__MonoCS__
                try
                {
                    frameworkDirectory = ToolLocationHelper.GetPathToDotNetFrameworkReferenceAssemblies((TargetDotNetFrameworkVersion)i);
                }
                catch (ArgumentException)
#endif // if !__MonoCS__
                {
                    frameworkDirectory = ToolLocationHelper.GetPathToDotNetFramework((TargetDotNetFrameworkVersion)i);
                }
                if (frameworkDirectory != null)
                {
                    string result = Path.Combine(frameworkDirectory, fileName);
                    if (File.Exists(result))
                        return result;
                }
            }
            return null;
        }

        private static int s_nextTempFile = 0;

        [System.Diagnostics.DebuggerNonUserCode]
        private string GetTmpAssemblyFile()
        {
            // try to re use the same file name to not pollute tmp directory
            string tmpFilePrefix = GetType().FullName + "_";
            string tmpFileSuffix = ".tmp.dll";

            while (s_nextTempFile < int.MaxValue)
            {
                int currentTempFile = s_nextTempFile;
                ++s_nextTempFile;
                string tmpFile = Path.Combine(Path.GetTempPath(), tmpFilePrefix + currentTempFile + tmpFileSuffix);
                if (!File.Exists(tmpFile) || Util.TryDeleteFile(tmpFile))
                {
                    return tmpFile;
                }
            }
            return null;
        }

        #endregion
    }
}
