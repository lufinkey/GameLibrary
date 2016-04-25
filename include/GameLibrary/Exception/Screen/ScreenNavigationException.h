
#pragma once

#include <GameLibrary/Exception/Exception.h>

namespace GameLibrary
{
	/*! Thrown to indicate an error when pushing, presenting, popping, or dismissing a Screen*/
	class ScreenNavigationException : public Exception
	{
	public:
		/*! \copydoc GameLibrary::Exception::Exception(const GameLibrary::String&)*/
		explicit ScreenNavigationException(const String&message);
		
		/*! \copydoc GameLibrary::Exception::getClassName()const*/
		virtual String getClassName() const override;
	};
}
