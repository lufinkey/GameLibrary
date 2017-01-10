
#pragma once

#include <GameLibrary/Utilities/String.hpp>

#define GAMELIB_MEMBERFUNC_GETCLASSNAME(classname) String classname::getClassName() const {return #classname ; }
