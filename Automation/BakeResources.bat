echo "Baking resoures..."
echo "Baking Textures"
call "../Lib/7zip/7z.exe" a -tzip "../Game/Textures.zip" "../Assets/Textures/*"
call "../Lib/7zip/7z.exe" a -tzip "../Game/Shaders.zip" "../Assets/Shaders/*"
call "../Lib/7zip/7z.exe" a -tzip "../Game/Models.zip" "../Assets/Models/*"
exit /b 0