
#include <GameLibrary/Exception/Graphics/ImageOutOfBoundsException.hpp>
#include "../ExceptionMacros.hpp"

namespace fgl
{
	ImageOutOfBoundsException::ImageOutOfBoundsException(size_t ptx, size_t pty, size_t w, size_t h)
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

	ImageOutOfBoundsException::ImageOutOfBoundsException(size_t i, size_t w, size_t h)
		: OutOfBoundsException((String)"(" + (i%w) + ", " + (i/w) + ") is out of bounds in Image with a size of (" + w + ", " + h + ")"),
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
