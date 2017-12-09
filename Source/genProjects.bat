pushd ProjectGenerator
SET DEVENV="%ProgramFiles(x86)%\Microsoft Visual Studio 14.0\Common7\IDE\devenv.com"
%DEVENV% generator.sln /build Release
generator\bin\Release\Sharpmake.Application.exe /sources(@"main.sharpmake.cs")
rmdir /s /q generator\\bin
rmdir /s /q generator\\obj
rmdir /s /q .vs
popd
exit /b 0