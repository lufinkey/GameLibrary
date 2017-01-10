
#include <GameLibrary/Exception/Graphics/ImageOutOfBoundsException.hpp>
#include "../ExceptionMacros.hpp"

namespace fgl
{
	ImageOutOfBoundsException::ImageOutOfBoundsException(unsigned int ptx, unsigned int pty, unsigned int w, unsigned int h)
		: OutOfBoundsException((String)"(" + ptx + ", " + pty + ") is out of bounds in Image with a size of (" + w + ", " + h + ")"),
		index((w*pty)+ptx),
		x(ptx),
		y(pty),
		size(w*h),
		width(w),
		height(h)
	{
		//
	}

	ImageOutOfBoundsException::ImageOutOfBoundsException(unsigned int i, unsigned int w, unsigned int h)
		: OutOfBoundsException((String)"(" + (i%w) + ", " + ((int)(i / w)) + ") is out of bounds in Image with a size of (" + w + ", " + h + ")"),
		index(i),
		x(index%w),
		y(i/w),
		size(w*h),
		width(w),
		height(h)
	{
		//
	}
	
	GAMELIB_MEMBERFUNC_GETCLASSNAME(ImageOutOfBoundsException)
}
