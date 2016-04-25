
#pragma once

#include <GameLibrary/Exception/Utilities/BadAnyCastException.h>
#include "Stringifier.h"

namespace GameLibrary
{
	class Any
	{
	private:
		class Base
		{
		public:
			virtual ~Base() {}
			virtual Base* clone() const = 0;
			virtual void* getPtr() const = 0;
			virtual String toString() const = 0;
		};
		
		template<typename T>
		class Derived : public Base
		{
		public:
			T value;
			
			template<typename U>
			Derived(U&& val) : value(std::forward<U>(val)) {};
			virtual Base* clone() const override { return new Derived<T>(value); }
			virtual void* getPtr() const override { return (void*)(&value); }
			virtual String toString() const override { return Stringifier<T>().convertToString(&value); };
		};
		
		Base* cloneBase() const
		{
			if(ptr!=nullptr)
			{
				return ptr->clone();
			}
			return nullptr;
		}
		
		Base* ptr;
		
	public:
		Any() : ptr(nullptr)
		{
			//
		}
		
		Any(std::nullptr_t) : ptr(nullptr)
		{
			//
		}
		
		Any(const Any& any) : ptr(any.cloneBase())
		{
			//
		}
		
		Any(Any&& any) : ptr(any.ptr)
		{
			any.ptr = nullptr;
		}
		
		template<typename U>
		Any(U&& value) : ptr(new Derived<typename std::decay<U>::type>(std::forward<U>(value)))
		{
			//
		}
		
		~Any()
		{
			if(ptr!=nullptr)
			{
				delete ptr;
			}
		}
		
		Any& operator=(std::nullptr_t)
		{
			Base* old_ptr = ptr;
			ptr = nullptr;
			if(old_ptr!=nullptr)
			{
				delete old_ptr;
			}
			return *this;
		}
		
		Any& operator=(const Any& any)
		{
			if(ptr==any.ptr)
			{
				return *this;
			}
			Base* old_ptr = ptr;
			ptr = any.cloneBase();
			if(old_ptr!=nullptr)
			{
				delete old_ptr;
			}
			return *this;
		}
		
		Any& operator=(Any&& any)
		{
			if(ptr==any.ptr)
			{
				return *this;
			}
			Base* old_ptr = ptr;
			Base* new_ptr = any.ptr;
			any.ptr = nullptr;
			ptr = new_ptr;
			if(old_ptr!=nullptr)
			{
				delete old_ptr;
			}
			return *this;
		}
		
		template<class U>
		U& as(bool safe=true)
		{
			if(safe)
			{
				Derived<typename std::decay<U>::type>* derived = dynamic_cast<Derived<typename std::decay<U>::type>*>(ptr);
				if(derived==nullptr)
				{
					throw BadAnyCastException(typeid(U).name());
				}
				return derived->value;
			}
			else
			{
				Derived<typename std::decay<U>::type>* derived = static_cast<Derived<typename std::decay<U>::type>*>(ptr);
				return derived->value;
			}
		}
		
		template<class U>
		const U& as(bool safe=true) const
		{
			if(safe)
			{
				Derived<typename std::decay<U>::type>* derived = dynamic_cast<Derived<typename std::decay<U>::type>*>(ptr);
				if(derived==nullptr)
				{
					throw BadAnyCastException(typeid(U).name());
				}
				return derived->value;
			}
			else
			{
				Derived<typename std::decay<U>::type>* derived = static_cast<Derived<typename std::decay<U>::type>*>(ptr);
				return derived->value;
			}
		}
		
		template<class U>
		bool is() const
		{
			return (dynamic_cast<Derived<typename std::decay<U>::type>*>(ptr)!=nullptr);
		}
		
		template<class U>
		operator U&()
		{
			return as<U>();
		}
		
		template<class U>
		operator const U&() const
		{
			return as<U>();
		}
		
		bool empty() const
		{
			return !ptr;
		}
		
		void* getPtr() const
		{
			if(ptr==nullptr)
			{
				return nullptr;
			}
			return ptr->getPtr();
		}
		
		String toString() const
		{
			if(ptr==nullptr)
			{
				return "";
			}
			return ptr->toString();
		}
	};
}
