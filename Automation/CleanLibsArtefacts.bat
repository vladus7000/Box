echo "Cleaning Libs..."
pushd ..\\Artifacts\\
rmdir /s /q dxut11
rmdir /s /q tinyxml2
rmdir /s /q zlib
popd
exit /b 0