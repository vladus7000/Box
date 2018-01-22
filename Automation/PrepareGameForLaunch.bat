echo off
echo "Preparing game for launch..."
Setlocal EnableDelayedExpansion
call CleanProjects.bat
if %ERRORLEVEL% == 0 goto step2
goto err
:step2
call GenProjects.bat
if %ERRORLEVEL% == 0 goto step3
goto err
:step3
call BuildGame.bat
if %ERRORLEVEL% == 0 goto step4
goto err
:step4
call CopyGameBuild.bat
if %ERRORLEVEL% == 0 goto step5
goto err
:step5
call BakeResources.bat
if %ERRORLEVEL% == 0 goto step6
goto err
:step6
mshta "about:<script>alert('Game should be able to run from <Root>/Game folder');close()</script>"
exit /b 0

err:
mshta "about:<script>alert('Something went wrong');close()</script>"
exit /b 1