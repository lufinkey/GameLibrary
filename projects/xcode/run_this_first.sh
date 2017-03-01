#!/bin/bash

# Enter into script folder
base_dir=$(dirname "${BASH_SOURCE[0]}")
cd "$base_dir"

function fix_sdl_project
{
	text_to_find=' \/Library\/Frameworks\/SDL2\.framework\/Headers"'
	text_to_repl=' \/Library\/Frameworks\/SDL2\.framework\/Headers \.\.\/\.\.\/SDL\/include"'
	sed -i '' -e "s/$text_to_find/$text_to_repl/g" "$1"

	text_to_find='	MACOSX_DEPLOYMENT_TARGET = 10\.5;'
	text_to_repl='	MACOSX_DEPLOYMENT_TARGET = 10\.6;'
	sed -i '' -e "s/$text_to_find/$text_to_repl/g" "$1"
}

function fix_sdl_project2
{
	fix_sdl_project "$1"

	text_to_find='	\/Library\/Frameworks\/SDL2\.framework\/Headers,\n	*\);'
	text_to_repl='	\/Library\/Frameworks\/SDL2\.framework\/Headers,\
					\.\.\/\.\.\/SDL\/include,\
				);'

	perl -i -0pe "s/$text_to_find/$text_to_repl/g" "$1"
}

fix_sdl_project "../../external/SDL/SDL_image/Xcode/SDL_image.xcodeproj/project.pbxproj"
fix_sdl_project2 "../../external/SDL/SDL_mixer/Xcode/SDL_mixer.xcodeproj/project.pbxproj"
fix_sdl_project2 "../../external/SDL/SDL_ttf/Xcode/SDL_ttf.xcodeproj/project.pbxproj"
