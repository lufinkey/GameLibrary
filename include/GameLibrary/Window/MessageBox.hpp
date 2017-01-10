
#pragma once

#include "Window.hpp"

namespace fgl
{
	class MessageBox
	{
	public:
		static unsigned int show(Window*parent, const String&title, const String&message);
		static unsigned int show(Window*parent, const String&title, const String&message, const ArrayList<String>&options);
	};
}
