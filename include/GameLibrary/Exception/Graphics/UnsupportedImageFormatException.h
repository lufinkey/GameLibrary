
#pragma once

#include <GameLibrary/Exception/Exception.h>

namespace fgl
{
	/* Thrown to indicate that Image::saveToFile tried to use an unsupported image format
		\see Image::saveToFile(const String&,String*)const*/
	class UnsupportedImageFormatException : public Exception
	{
	public:
		/*! the unsupported image extension*/
		String format;
		
		/*! Constructs an exception with the unsupported image extensions.
			\param format the file extension that was unsupported by Image::saveToFile*/
		explicit UnsupportedImageFormatException(const String& format);
		
		/*! \copydoc fgl::Exception::getClassName()const*/
		virtual String getClassName() const override;
	};
}
