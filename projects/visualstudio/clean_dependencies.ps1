#!C:/WINDOWS/System32/WindowsPowerShell/v1.0/powershell -file
param([string]$GameLibraryPath="")

$msbuild_path=(Resolve-Path HKLM:\SOFTWARE\MICROSOFT\MSBuild\ToolsVersions\14.0 | Get-ItemProperty -Name MSBuildToolsPath).MSBuildToolsPath
$msbuild_path="$msbuild_path\msbuild.exe"

echo "cleaning SDL"
$SDL_path="$GameLibraryPath\external\SDL\SDL\VisualC"
& "$msbuild_path" "$SDL_path\SDL.sln" /t:Clean

echo "cleaning SDL_image"
$SDL_image_path="$GameLibraryPath\external\SDL\SDL_image\VisualC"
& "$msbuild_path" "$SDL_image_path\SDL_image.sln" /t:Clean

echo "cleaning SDL_ttf"
$SDL_ttf_path="$GameLibraryPath\external\SDL\SDL_ttf\VisualC"
& "$msbuild_path" "$SDL_ttf_path\SDL_ttf.sln" /t:Clean

echo "Windows is a garbage OS" | Out-File "$GameLibraryPath\projects\visualstudio\dependency_clean.log"
