echo off
echo "Preparing Editor for launch"
Setlocal EnableDelayedExpansion
call CleanProjects.bat
if %ERRORLEVEL% == 0 goto step2
goto err
:step2
call GenProjects.bat
if %ERRORLEVEL% == 0 goto step3
goto err
:step3
call BuildEditor.bat
if %ERRORLEVEL% == 0 goto step4
goto err
:step4
call CopyEditorBuild.bat
if %ERRORLEVEL% == 0 goto step5
goto err
:step5
mshta "about:<script>alert('You can find editor in the <root>IDEs\\Editor.exe');close()</script>"
exit /b 0

err:
mshta "about:<script>alert('Something went wrong');close()</script>"
exit /b 1