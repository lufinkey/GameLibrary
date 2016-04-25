
#pragma once

#include <GameLibrary/Utilities/String.h>

#define GAMELIB_MEMBERFUNC_GETCLASSNAME(classname) String classname::getClassName() const {return #classname ; }
