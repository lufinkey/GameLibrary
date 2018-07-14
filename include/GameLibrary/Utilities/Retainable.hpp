
#pragma once

#include "TypeRegistry.hpp"
#include <stddef.h>

namespace fgl
{
	class Retainable
	{
	public:
		Retainable();
		virtual ~Retainable() = default;
		
		size_t retain();
		size_t release();
		size_t getRetainCount() const;
		
	private:
		size_t retainCount;
	};
	
	REGISTER_TYPE(Retainable)
}
