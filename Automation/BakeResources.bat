echo "Baking resoures..."
echo "Baking Textures"
call "../Lib/7zip/7z.exe" a -tzip "../Game/Textures.zip" "../Assets/Textures/*"
exit /b 0