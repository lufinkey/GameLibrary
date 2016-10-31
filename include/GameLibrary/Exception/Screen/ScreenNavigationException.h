
#pragma once

#include <GameLibrary/Exception/Exception.h>

namespace fgl
{
	/*! Thrown to indicate an error when pushing, presenting, popping, or dismissing a Screen*/
	class ScreenNavigationException : public Exception
	{
	public:
		/*! \copydoc fgl::Exception::Exception(const fgl::String&)*/
		explicit ScreenNavigationException(const String&message);
		
		/*! \copydoc fgl::Exception::getClassName()const*/
		virtual String getClassName() const override;
	};
}
