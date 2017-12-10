echo off
echo "Clean All"
call CleanEditorArtefacts.bat
call CleanEngineArtefacts.bat
call CleanGameArtefacts.bat
call CleanProjects.bat
echo "Done cleaning"
exit /b 0