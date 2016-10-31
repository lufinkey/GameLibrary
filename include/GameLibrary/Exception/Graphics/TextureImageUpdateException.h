
#pragma once

#include <GameLibrary/Exception/Exception.h>

namespace fgl
{
	/*! Thrown to indicate that a call to TextureImage::update failed*/
	class TextureImageUpdateException : public Exception
	{
	public:
		/*! \copydoc fgl::Exception::Exception(const fgl::String&)*/
		explicit TextureImageUpdateException(const String&message);
		
		/*! \copydoc fgl::Exception::getClassName()const*/
		virtual String getClassName() const override;
	};
}
