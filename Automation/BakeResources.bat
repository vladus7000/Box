echo "Baking resoures..."
echo "Baking Textures"
call "../Lib/7zip/7z.exe" a -tzip "../Game/Textures.zip" "../Assets/Textures/*"
call "../Lib/7zip/7z.exe" a -tzip "../Game/Shaders.zip" "../Assets/Shaders/*"
exit /b 0