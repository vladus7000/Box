setlocal EnableDelayedExpansion
echo "Clean projects"
pushd ..\\Source\\
rmdir /s /q projects
rmdir /s /q .vs
del editor.sln
del game.sln
del editor.VC.db
del game.VC.db
pushd ProjectGenerator
rmdir /s /q generator\\bin
rmdir /s /q generator\\obj
rmdir /s /q .vs
popd
popd
exit /b 0