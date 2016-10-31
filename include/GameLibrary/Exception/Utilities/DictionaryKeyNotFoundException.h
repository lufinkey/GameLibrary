
#pragma once

#include <GameLibrary/Exception/OutOfBoundsException.h>

namespace fgl
{
	/*! Thrown to indicate that a specified key is not set in a Dictionary*/
	class DictionaryKeyNotFoundException : public OutOfBoundsException
	{
	public:
		/*! a String representation of the key that was not found in the Dictionary object*/
		String keyString;
		
		/*! Constructs an exception with a String representation of the key, which may or may not be a String
			\param keyString a String representation of the key that was not found in the Dictionary object*/
		DictionaryKeyNotFoundException(const String& keyString);
		
		/*! \copydoc fgl::Exception::getClassName()const*/
		virtual String getClassName() const override;
	};
}
