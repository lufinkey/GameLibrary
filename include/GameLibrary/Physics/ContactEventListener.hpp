
#pragma once

#include "ContactEvent.hpp"

namespace fgl
{
	class ContactEventListener
	{
		friend class CollisionManager;
	public:
		virtual ~ContactEventListener() = default;
		
	protected:
		virtual void onContact(const ContactEvent& event) {}
		virtual void onContactUpdate(const ContactEvent& event) {}
		virtual void onContactFinish(const ContactEvent& event) {}
	};
}
