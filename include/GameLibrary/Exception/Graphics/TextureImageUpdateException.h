
#pragma once

#include <GameLibrary/Exception/Exception.h>

namespace GameLibrary
{
	/*! Thrown to indicate that a call to TextureImage::update failed*/
	class TextureImageUpdateException : public Exception
	{
	public:
		/*! \copydoc GameLibrary::Exception::Exception(const GameLibrary::String&)*/
		explicit TextureImageUpdateException(const String&message);
		
		/*! \copydoc GameLibrary::Exception::getClassName()const*/
		virtual String getClassName() const override;
	};
}
