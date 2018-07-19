
#pragma once

#include <GameLibrary/Exception/Exception.hpp>

namespace fgl
{
	/*! Thrown to indicate that a call to TextureImage::create failed*/
	class TextureImageCreateException : public Exception
	{
	public:
		/*! \copydoc fgl::Exception::Exception(const fgl::String&)*/
		explicit TextureImageCreateException(const String&message);
		
		/*! \copydoc fgl::Exception::getClassName()const*/
		virtual String getClassName() const override;
	};
}
