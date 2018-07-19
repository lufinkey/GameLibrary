
#pragma once

#include <typeinfo>
#include <GameLibrary/Exception/Utilities/BadAnyCastException.hpp>
#include "String.hpp"

namespace fgl
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
			virtual const std::type_info& getTypeInfo() const = 0;
		};
		
		template<typename T>
		class Derived : public Base
		{
		public:
			T value;
			
			Derived(const T& val) : value(val) {}
			Derived(T&& val) : value(val) {}
			virtual Base* clone() const override { return new Derived<T>(value); }
			virtual void* getPtr() const override { return (void*)(&value); }
			virtual String toString() const override { return fgl::stringify<T>(value); }
			virtual const std::type_info& getTypeInfo() const override { return typeid(T); }
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
		
		Any(Any& any) : ptr(any.cloneBase())
		{
			//
		}
		
		Any(Any&& any) : ptr(any.ptr)
		{
			any.ptr = nullptr;
		}
		
		Any(const Any& any) : ptr(any.cloneBase())
		{
			//
		}
		
		Any(const Any&& any) : ptr(any.cloneBase())
		{
			//
		}
		
		template<typename U>
		Any(U&& value) : ptr(new Derived<typename std::decay<U>::type>(value))
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
		U& as()
		{
			typedef typename std::decay<U>::type T;
			if(ptr==nullptr)
			{
				throw BadAnyCastException(typeid(T).name());
			}
			if(typeid(T)==ptr->getTypeInfo())
			{
				Derived<T>* derived = static_cast<Derived<T>*>(ptr);
				return derived->value;
			}
			else
			{
				throw BadAnyCastException(typeid(T).name());
			}
		}
		
		template<class U>
		const U& as() const
		{
			typedef typename std::decay<U>::type T;
			if(ptr==nullptr)
			{
				throw BadAnyCastException(typeid(T).name());
			}
			if(typeid(T)==ptr->getTypeInfo())
			{
				Derived<T>* derived = static_cast<Derived<T>*>(ptr);
				return derived->value;
			}
			else
			{
				throw BadAnyCastException(typeid(T).name());
			}
		}
		
		template<class U>
		bool is() const
		{
			typedef typename std::decay<U>::type T;
			if(ptr==nullptr)
			{
				return false;
			}
			if(typeid(T)==ptr->getTypeInfo())
			{
				return true;
			}
			return false;
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
		
		bool isEmpty() const
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
		
		const std::type_info& getTypeInfo() const
		{
			if(ptr==nullptr)
			{
				return typeid(std::nullptr_t);
			}
			return ptr->getTypeInfo();
		}
	};
}
