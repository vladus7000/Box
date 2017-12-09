pushd ..\\Source\\
SET DEVENV="%ProgramFiles(x86)%\Microsoft Visual Studio 14.0\Common7\IDE\devenv.com"
%DEVENV% Game.sln /build Release
popd
exit /b 0