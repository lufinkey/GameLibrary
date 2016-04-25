
#include <GameLibrary/Exception/BadCastException.h>

namespace GameLibrary
{
	/*! Thrown to indicate that casting a Number object to an arithmetic type failed.*/
	class BadNumberCastException : public BadCastException
	{
	public:
		/*! Constructs an exception with a casted type string.
			\param casttype the type that the Number object was cast as*/
		explicit BadNumberCastException(const String& casttype);
		
		/*! \copydoc GameLibrary::Exception::getClassName()const*/
		virtual String getClassName() const override;
	};
}
