#!C:/WINDOWS/System32/WindowsPowerShell/v1.0/powershell -file
param([string]$GameLibraryPath="", [string]$Config = "Debug", [string]$Platform = "Win32", [string]$PlatformToolset = "v140", [string]$OutputDir="")

$exitvalue=0

$msbuild_path=(Resolve-Path HKLM:\SOFTWARE\MICROSOFT\MSBuild\ToolsVersions\14.0 | Get-ItemProperty -Name MSBuildToolsPath).MSBuildToolsPath
$msbuild_path="$msbuild_path\msbuild.exe"

$lib_path="$GameLibraryPath\projects\visualstudio\lib\$Platform\$Config"
mkdir -p "$lib_path"
mkdir -p "$OutputDir"

#Compile SDL2
$SDL_path="$GameLibraryPath\external\SDL\SDL\VisualC"
& "$msbuild_path" /t:SDL2 "$SDL_path\SDL.sln" "/p:PlatformToolset=$PlatformToolset" "/p:Configuration=$Config" "/p:Platform=$Platform"
$SDL_output_path="$SDL_path\$Platform\$Config"
if(Test-Path "$SDL_output_path\SDL2.dll")
{
	copy -Path "$SDL_output_path\SDL2.pdb" -Destination "$OutputDir\SDL2.pdb" -Force
	copy -Path "$SDL_output_path\SDL2.dll" -Destination "$OutputDir\SDL2.dll" -Force
	copy -Path "$SDL_output_path\SDL2.lib" -Destination "$lib_path\SDL2.lib" -Force
}
else
{
	echo "Could not find SDL2.dll"
	$exitvalue=1
}

#Compile SDL2_image
$SDL_image_path="$GameLibraryPath\external\SDL\SDL_image\VisualC"
# Fix bug where it wouldn't compile because of spaces in path, and also add additional include directory
(Get-Content "$SDL_image_path\SDL_image.vcxproj").replace(
	' %(FullPath) ', ' "%(FullPath)" ').replace(
	' $(SolutionDir)\$(Platform)\$(Configuration)\ ', ' "$(SolutionDir)\$(Platform)\$(Configuration)\" ').replace(
	' $(SolutionDir)\$(Platform)\$(Configuration)\<', ' "$(SolutionDir)\$(Platform)\$(Configuration)\"<').replace(
	'<AdditionalIncludeDirectories>external\include;%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>', '<AdditionalIncludeDirectories>external\include;..\..\SDL\include;%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>') |
	Set-Content "$SDL_image_path\SDL_image.vcxproj"
copy -Path "$SDL_output_path\SDL2.lib" -Destination "$SDL_image_path\SDL2.lib"
& "$msbuild_path" /t:SDL2_image "$SDL_image_path\SDL_image.sln" "/p:PlatformToolset=$PlatformToolset" "/p:Configuration=$Config" "/p:Platform=$Platform"
$SDL_image_output_path="$SDL_image_path\$Platform\$Config"
if(Test-Path "$SDL_image_output_path\SDL2_image.dll")
{
	copy -Path "$SDL_image_output_path\*.dll" -Destination "$OutputDir" -Force
	copy -Path "$SDL_image_output_path\SDL2_image.pdb" -Destination "$OutputDir\SDL2_image.pdb" -Force
	copy -Path "$SDL_image_output_path\SDL2_image.lib" -Destination "$lib_path\SDL2_image.lib" -Force
}
else
{
	echo "Could not find SDL2_image.dll"
	$exitvalue=1
}

#Compile SDL2_ttf
$SDL_ttf_path="$GameLibraryPath\external\SDL\SDL_ttf\VisualC"
(Get-Content "$SDL_ttf_path\SDL_ttf.vcxproj").replace(
	' %(FullPath) ', ' "%(FullPath)" ').replace(
	' $(SolutionDir)\$(Platform)\$(Configuration)\ ', ' "$(SolutionDir)\$(Platform)\$(Configuration)\" ').replace(
	' $(SolutionDir)\$(Platform)\$(Configuration)\<', ' "$(SolutionDir)\$(Platform)\$(Configuration)\"<').replace(
	'<AdditionalIncludeDirectories>external\include;%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>', '<AdditionalIncludeDirectories>external\include;..\..\SDL\include;%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>') |
	Set-Content "$SDL_ttf_path\SDL_ttf.vcxproj"
copy -Path "$SDL_output_path\SDL2.lib" -Destination "$SDL_ttf_path\SDL2.lib"
& "$msbuild_path" /t:SDL2_ttf "$SDL_ttf_path\SDL_ttf.sln" "/p:PlatformToolset=$PlatformToolset" "/p:Configuration=$Config" "/p:Platform=$Platform"
$SDL_ttf_output_path="$SDL_ttf_path\$Platform\$Config"
if(Test-Path "$SDL_ttf_output_path\SDL2_ttf.dll")
{
	copy -Path "$SDL_ttf_output_path\*.dll" -Destination "$OutputDir" -Force
	copy -Path "$SDL_ttf_output_path\SDL2_ttf.pdb" -Destination "$OutputDir\SDL2_ttf.pdb" -Force
	copy -Path "$SDL_ttf_output_path\SDL2_ttf.lib" -Destination "$lib_path\SDL2_ttf.lib" -Force
}
else
{
	echo "Could not find SDL2_ttf.dll"
	$exitvalue=1
}

#"Fix" SDL_gfx
echo "Fixing SDL_gfx"
$SDL_gfx_path="$GameLibraryPath\external\SDL\SDL_gfx"
$bad_content = @'
/* Windows targets do not have lrint, so provide a local inline version */
#if defined(_MSC_VER)
/* Detect 64bit and use intrinsic version */
#ifdef _M_X64
#include <emmintrin.h>
static __inline long 
	lrint(float f) 
{
	return _mm_cvtss_si32(_mm_load_ss(&f));
}
#elif defined(_M_IX86)
__inline long int
	lrint (double flt)
{	
	int intgr;
	_asm
	{
		fld flt
			fistp intgr
	};
	return intgr;
}
#elif defined(_M_ARM)
#include <armintr.h>
#pragma warning(push)
#pragma warning(disable: 4716)
__declspec(naked) long int
	lrint (double flt)
{
	__emit(0xEC410B10); // fmdrr  d0, r0, r1
	__emit(0xEEBD0B40); // ftosid s0, d0
	__emit(0xEE100A10); // fmrs   r0, s0
	__emit(0xE12FFF1E); // bx     lr
}
#pragma warning(pop)
#else
#error lrint needed for MSVC on non X86/AMD64/ARM targets.
#endif
#endif
'@
$bad_content = $bad_content.replace("`r`n", "`n")
(Get-Content "$SDL_gfx_path\SDL2_gfxPrimitives.c" -Raw).replace("`r`n", "`n").replace($bad_content, '') | Set-Content "$SDL_gfx_path\SDL2_gfxPrimitives.c"

exit $exitvalue
