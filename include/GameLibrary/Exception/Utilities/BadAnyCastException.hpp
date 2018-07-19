
#pragma once

#include <GameLibrary/Exception/BadCastException.hpp>

namespace fgl
{
	/*! Thrown to indicate that casting an Any object to some other type failed*/
	class BadAnyCastException : public BadCastException
	{
	public:
		/*! Constructs an exception with a casted type string.
			\param casttype the type that the Any object was cast as*/
		explicit BadAnyCastException(const String& casttype);
		
		/*! \copydoc fgl::Exception::getClassName()const*/
		virtual String getClassName() const override;
	};
}
