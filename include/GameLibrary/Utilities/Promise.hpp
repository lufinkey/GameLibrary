
#pragma once

#include <GameLibrary/Exception/Exception.hpp>
#include <GameLibrary/Exception/ExceptionPtr.hpp>
#include "ArrayList.hpp"
#include "FunctionTraits.hpp"
#include <functional>
#include <future>
#include <memory>

namespace fgl
{
	template<typename RESULT_TYPE>
	class Promise;
	
	template<typename RESULT_TYPE>
	struct PromiseHelperTypes
	{
		using Callback = std::function<void(RESULT_TYPE)>;
		template<typename NEXT_RESULT_TYPE>
		using NextCallback = std::function<Promise<NEXT_RESULT_TYPE>(RESULT_TYPE)>;
	};
	
	template<>
	struct PromiseHelperTypes<void>
	{
		using Callback = std::function<void()>;
		template<typename NEXT_RESULT_TYPE>
		using NextCallback = std::function<Promise<NEXT_RESULT_TYPE>()>;
	};
	
	template<typename RESULT_TYPE, typename ERROR_TYPE>
	using PromiseExecutor = std::function<void(typename PromiseHelperTypes<RESULT_TYPE>::Callback, typename PromiseHelperTypes<ERROR_TYPE>::Callback)>;
	
	template<typename RESULT_TYPE=void>
	class Promise
	{
	public:
		typedef RESULT_TYPE result_type;
		typedef Promise<RESULT_TYPE> promise_type;
		
		using Resolver = typename PromiseHelperTypes<RESULT_TYPE>::Callback;
		template<typename ERROR_TYPE>
		using Rejecter = typename PromiseHelperTypes<ERROR_TYPE>::Callback;
		template<typename NEXT_RESULT_TYPE>
		using NextResolver = typename PromiseHelperTypes<RESULT_TYPE>::template NextCallback<NEXT_RESULT_TYPE>;
		template<typename NEXT_RESULT_TYPE, typename ERROR_TYPE>
		using NextRejecter = typename PromiseHelperTypes<ERROR_TYPE>::template NextCallback<NEXT_RESULT_TYPE>;
		
		// constructor<any>
		template<
			typename _RESULT_TYPE=RESULT_TYPE,
			typename std::enable_if<!std::is_same<_RESULT_TYPE, void>::value, std::nullptr_t>::type = nullptr>
		explicit Promise(const PromiseExecutor<_RESULT_TYPE, ExceptionPtr>& executor)
		{
			if(!executor) {
				throw IllegalArgumentException("executor", "cannot be null");
			}
			// create continuer
			auto _continuer = std::shared_ptr<Continuer>(new Continuer());
			continuer = _continuer;
			// execute
			printf("calling executor\n");
			executor([=](const _RESULT_TYPE& result) {
				_continuer->resolve(result);
			}, [=](ExceptionPtr error) {
				_continuer->reject(error.ptr());
			});
		}
		
		// constructor<void>
		template<
			typename _RESULT_TYPE=RESULT_TYPE,
			typename std::enable_if<std::is_same<_RESULT_TYPE, void>::value, std::nullptr_t>::type = nullptr>
		explicit Promise(const PromiseExecutor<_RESULT_TYPE, ExceptionPtr>& executor)
		{
			if(!executor) {
				throw IllegalArgumentException("executor", "cannot be null");
			}
			// create continuer
			auto _continuer = std::shared_ptr<Continuer>(new Continuer());
			continuer = _continuer;
			// execute
			executor([=]() {
				_continuer->resolve();
			}, [=](ExceptionPtr error) {
				_continuer->reject(error.ptr());
			});
		}
		
		// resolve<any>
		template<
			typename _RESULT_TYPE=RESULT_TYPE,
			typename std::enable_if<!std::is_same<_RESULT_TYPE, void>::value, std::nullptr_t>::type = nullptr>
		static Promise<_RESULT_TYPE> resolve(const _RESULT_TYPE& result)
		{
			return Promise<_RESULT_TYPE>([&](const Resolver& resolve, const Rejecter<ExceptionPtr>& reject) {
				resolve(result);
			});
		}
		
		// resolve<void>
		template<
			typename _RESULT_TYPE=RESULT_TYPE,
			typename std::enable_if<std::is_same<_RESULT_TYPE, void>::value, std::nullptr_t>::type = nullptr>
		static Promise<_RESULT_TYPE> resolve()
		{
			return Promise<_RESULT_TYPE>([&](const Resolver& resolve, const Rejecter<ExceptionPtr>& reject) {
				resolve();
			});
		}
		
		// reject
		template<typename ERROR_TYPE>
		static Promise<RESULT_TYPE> reject(const ERROR_TYPE& error)
		{
			return Promise<RESULT_TYPE>([&](const Resolver& resolve, const Rejecter<ExceptionPtr>& reject) {
				reject(error);
			});
		}
		
		// then<any>(onResolve -> promise, onReject -> promise)
		template<
			typename NEXT_RESULT_TYPE,
			typename ERROR_TYPE,
			typename _RESULT_TYPE=RESULT_TYPE,
			typename RESOLVER = typename PromiseHelperTypes<_RESULT_TYPE>::template NextCallback<NEXT_RESULT_TYPE>,
			typename REJECTER = typename PromiseHelperTypes<ERROR_TYPE>::template NextCallback<NEXT_RESULT_TYPE>,
			typename RESOLVER_TRAITS = function_traits<RESOLVER>,
			typename REJECTER_TRAITS = function_traits<REJECTER>,
			typename std::enable_if<(
				!std::is_same<_RESULT_TYPE, void>::value &&
				std::is_same<Promise<NEXT_RESULT_TYPE>, typename RESOLVER_TRAITS::result_type>::value &&
				std::is_same<Promise<NEXT_RESULT_TYPE>, typename REJECTER_TRAITS::result_type>::value
			), std::nullptr_t>::type = nullptr>
		Promise<NEXT_RESULT_TYPE> then(
			const RESOLVER& onResolve,
			const REJECTER& onReject)
		{
			if(!onResolve && !onReject) {
				throw fgl::IllegalArgumentException("onResolve", "cannot be null if onReject is also null");
			}
			auto _continuer = continuer;
			return Promise<NEXT_RESULT_TYPE>([=](const typename Promise<NEXT_RESULT_TYPE>::Resolver& resolve, const typename Promise<NEXT_RESULT_TYPE>::template Rejecter<ExceptionPtr>& reject) {
				if(onResolve) {
					_continuer->setResolver([=](const _RESULT_TYPE& result) {
						try {
							auto nextPromise = onResolve(result);
							nextPromise.template then<void, ExceptionPtr>(resolve, reject);
						}
						catch(...) {
							reject(std::current_exception());
						}
					});
				}
				if(onReject) {
					_continuer->template setRejecter([=](std::exception_ptr error) {
						callRejector<NEXT_RESULT_TYPE, ERROR_TYPE>(onReject, error, resolve, reject);
					});
				}
			});
		}
		
		// then<void>(onResolve -> promise, onReject -> promise)
		template<
			typename NEXT_RESULT_TYPE,
			typename ERROR_TYPE,
			typename _RESULT_TYPE=RESULT_TYPE,
			typename RESOLVER = typename PromiseHelperTypes<_RESULT_TYPE>::template NextCallback<NEXT_RESULT_TYPE>,
			typename REJECTER = typename PromiseHelperTypes<ERROR_TYPE>::template NextCallback<NEXT_RESULT_TYPE>,
			typename RESOLVER_TRAITS = function_traits<RESOLVER>,
			typename REJECTER_TRAITS = function_traits<REJECTER>,
			typename std::enable_if<(
				std::is_same<_RESULT_TYPE, void>::value &&
				std::is_same<Promise<NEXT_RESULT_TYPE>, typename RESOLVER_TRAITS::result_type>::value &&
				std::is_same<Promise<NEXT_RESULT_TYPE>, typename REJECTER_TRAITS::result_type>::value
			), std::nullptr_t>::type = nullptr>
		Promise<NEXT_RESULT_TYPE> then(
			const RESOLVER& onResolve,
			const REJECTER& onReject)
		{
			if(!onResolve && !onReject) {
				throw fgl::IllegalArgumentException("onResolve", "cannot be null if onReject is also null");
			}
			auto _continuer = continuer;
			return Promise<NEXT_RESULT_TYPE>([=](const typename Promise<NEXT_RESULT_TYPE>::Resolver& resolve, const typename Promise<NEXT_RESULT_TYPE>::template Rejecter<ExceptionPtr>& reject) {
				if(onResolve) {
					_continuer->setResolver([=]() {
						try {
							auto nextPromise = onResolve();
							nextPromise.template then<void, ExceptionPtr>(resolve, reject);
						}
						catch(...) {
							reject(std::current_exception());
						}
					});
				}
				if(onReject) {
					_continuer->template setRejecter<ERROR_TYPE>([=](std::exception_ptr error) {
						callRejector<NEXT_RESULT_TYPE, ERROR_TYPE>(onReject, error, resolve, reject);
					});
				}
			});
		}
		
		// then<>(onResolve -> promise)
		template<
			typename NEXT_RESULT_TYPE,
			typename _RESULT_TYPE=RESULT_TYPE,
			typename RESOLVER = typename PromiseHelperTypes<_RESULT_TYPE>::template NextCallback<NEXT_RESULT_TYPE>,
			typename RESOLVER_TRAITS = function_traits<RESOLVER>,
			typename std::enable_if<(
				std::is_same<Promise<NEXT_RESULT_TYPE>, typename RESOLVER_TRAITS::result_type>::value
			), std::nullptr_t>::type = nullptr>
		Promise<NEXT_RESULT_TYPE> then(const RESOLVER& onResolve)
		{
			return then<NEXT_RESULT_TYPE, std::exception_ptr>(onResolve, std::function<Promise<NEXT_RESULT_TYPE>(std::exception_ptr)>(nullptr));
		}
		
		// fail<>(onReject -> promise)
		template<
			typename NEXT_RESULT_TYPE,
			typename ERROR_TYPE,
			typename REJECTER = typename PromiseHelperTypes<ERROR_TYPE>::template NextCallback<NEXT_RESULT_TYPE>,
			typename REJECTER_TRAITS = function_traits<REJECTER>,
			typename std::enable_if<(
				std::is_same<Promise<NEXT_RESULT_TYPE>, typename REJECTER_TRAITS::result_type>::value
			), std::nullptr_t>::type = nullptr>
		Promise<NEXT_RESULT_TYPE> fail(const REJECTER& onReject)
		{
			return then<NEXT_RESULT_TYPE, ERROR_TYPE>(std::function<Promise<NEXT_RESULT_TYPE>(std::exception_ptr)>(nullptr), onReject);
		}
		
		// then<any>(onResolve -> void, onReject -> void)
		template<
			typename NEXT_RESULT_TYPE,
			typename ERROR_TYPE,
			typename _RESULT_TYPE=RESULT_TYPE,
			typename RESOLVER = typename PromiseHelperTypes<_RESULT_TYPE>::Callback,
			typename REJECTER = typename PromiseHelperTypes<ERROR_TYPE>::Callback,
			typename RESOLVER_TRAITS = function_traits<RESOLVER>,
			typename REJECTER_TRAITS = function_traits<REJECTER>,
			typename std::enable_if<(
				!std::is_same<_RESULT_TYPE, void>::value &&
				std::is_same<NEXT_RESULT_TYPE, void>::value &&
				std::is_same<void, typename RESOLVER_TRAITS::result_type>::value &&
				std::is_same<void, typename REJECTER_TRAITS::result_type>::value
			), std::nullptr_t>::type = nullptr>
		Promise<NEXT_RESULT_TYPE> then(
			const RESOLVER& onResolve,
			const REJECTER& onReject)
		{
			if(!onResolve && !onReject) {
				throw fgl::IllegalArgumentException("onResolve", "cannot be null if onReject is also null");
			}
			auto _continuer = continuer;
			return Promise<void>([=](const typename Promise<void>::Resolver& resolve, const typename Promise<void>::template Rejecter<ExceptionPtr>& reject) {
				if(onResolve) {
					_continuer->setResolver([=](const RESULT_TYPE& result) {
						try {
							onResolve(result);
							resolve();
						}
						catch(...) {
							reject(std::current_exception());
						}
					});
				}
				if(onReject) {
					_continuer->template setRejecter([=](std::exception_ptr error) {
						callRejector<NEXT_RESULT_TYPE, ERROR_TYPE>(onReject, error, resolve, reject);
					});
				}
			});
		}
		
		// then<void>(onResolve -> void, onReject -> void)
		template<
			typename NEXT_RESULT_TYPE,
			typename ERROR_TYPE,
			typename _RESULT_TYPE=RESULT_TYPE,
			typename RESOLVER = typename PromiseHelperTypes<_RESULT_TYPE>::Callback,
			typename REJECTER = typename PromiseHelperTypes<ERROR_TYPE>::Callback,
			typename RESOLVER_TRAITS = function_traits<RESOLVER>,
			typename REJECTER_TRAITS = function_traits<REJECTER>,
			typename std::enable_if<(
				std::is_same<_RESULT_TYPE, void>::value &&
				std::is_same<NEXT_RESULT_TYPE, void>::value &&
				std::is_same<void, typename RESOLVER_TRAITS::result_type>::value &&
				std::is_same<void, typename REJECTER_TRAITS::result_type>::value
			), std::nullptr_t>::type = nullptr>
		Promise<NEXT_RESULT_TYPE> then(
			const RESOLVER& onResolve,
			const REJECTER& onReject)
		{
			if(!onResolve && !onReject) {
				throw fgl::IllegalArgumentException("onResolve", "cannot be null if onReject is also null");
			}
			auto _continuer = continuer;
			return Promise<void>([=](const typename Promise<void>::Resolver& resolve, const typename Promise<void>::template Rejecter<ExceptionPtr>& reject) {
				if(onResolve) {
					_continuer->setResolver([=]() {
						try {
							onResolve();
							resolve();
						}
						catch(...) {
							reject(std::current_exception());
						}
					});
				}
				if(onReject) {
					_continuer->template setRejecter([=](const std::exception_ptr error) {
						callRejector<NEXT_RESULT_TYPE, ERROR_TYPE>(onReject, error, resolve, reject);
					});
				}
			});
		}
		
		// then<>(onResolve -> promise)
		template<
			typename NEXT_RESULT_TYPE,
			typename _RESULT_TYPE=RESULT_TYPE,
			typename RESOLVER = typename PromiseHelperTypes<_RESULT_TYPE>::Callback,
			typename RESOLVER_TRAITS = function_traits<RESOLVER>,
			typename std::enable_if<(
				std::is_same<NEXT_RESULT_TYPE, void>::value &&
				std::is_same<void, typename RESOLVER_TRAITS::result_type>::value
			), std::nullptr_t>::type = nullptr>
		Promise<NEXT_RESULT_TYPE> then(const RESOLVER& onResolve)
		{
			return then<void, ExceptionPtr>(onResolve, std::function<void(ExceptionPtr)>(nullptr));
		}
		
		// fail<>(onReject -> void)
		template<
			typename NEXT_RESULT_TYPE,
			typename ERROR_TYPE,
			typename REJECTER = typename PromiseHelperTypes<ERROR_TYPE>::Callback,
			typename REJECTER_TRAITS = function_traits<REJECTER>,
			typename std::enable_if<(
				std::is_same<NEXT_RESULT_TYPE, void>::value &&
				std::is_same<void, typename REJECTER_TRAITS::result_type>::value
			), std::nullptr_t>::type = nullptr>
		Promise<NEXT_RESULT_TYPE> fail(const REJECTER& onReject)
		{
			return then<void, ERROR_TYPE>(Resolver(nullptr), onReject);
		}
		
	private:
		template<
			typename NEXT_RESULT_TYPE,
			typename ERROR_TYPE,
			typename REJECTER = typename PromiseHelperTypes<ERROR_TYPE>::Callback,
			typename NEXT_RESOLVER = typename Promise<NEXT_RESULT_TYPE>::Resolver,
			typename NEXT_REJECTER = typename Promise<NEXT_RESULT_TYPE>::template Rejecter<ExceptionPtr>,
			typename REJECTER_TRAITS = function_traits<REJECTER>,
			typename std::enable_if<(
				(std::is_same<ERROR_TYPE, ExceptionPtr>::value || std::is_same<ERROR_TYPE, std::exception_ptr>::value) &&
				std::is_same<Promise<NEXT_RESULT_TYPE>, typename REJECTER_TRAITS::result_type>::value
			), std::nullptr_t>::type = nullptr>
		void callRejector(REJECTER onReject, ExceptionPtr error, NEXT_RESOLVER resolve, NEXT_REJECTER reject) {
			try {
				auto nextPromise = onReject(error);
				nextPromise.template then<void, ExceptionPtr>(resolve, reject);
			}
			catch(...) {
				reject(std::current_exception());
			}
		}
		
		template<
			typename NEXT_RESULT_TYPE,
			typename ERROR_TYPE,
			typename REJECTER = typename PromiseHelperTypes<ERROR_TYPE>::Callback,
			typename NEXT_RESOLVER = typename Promise<NEXT_RESULT_TYPE>::Resolver,
			typename NEXT_REJECTER = typename Promise<NEXT_RESULT_TYPE>::template Rejecter<ExceptionPtr>,
			typename REJECTER_TRAITS = function_traits<REJECTER>,
			typename std::enable_if<(
				(std::is_same<ERROR_TYPE, ExceptionPtr>::value || std::is_same<ERROR_TYPE, std::exception_ptr>::value) &&
				std::is_same<NEXT_RESULT_TYPE, void>::value &&
				std::is_same<void, typename REJECTER_TRAITS::result_type>::value
			), std::nullptr_t>::type = nullptr>
		void callRejector(REJECTER onReject, ExceptionPtr error, NEXT_RESOLVER resolve, NEXT_REJECTER reject) {
			try {
				onReject(error);
				resolve();
			}
			catch(...) {
				reject(std::current_exception());
			}
		}
		
		template<
			typename NEXT_RESULT_TYPE,
			typename ERROR_TYPE,
			typename REJECTER = typename PromiseHelperTypes<ERROR_TYPE>::Callback,
			typename NEXT_RESOLVER = typename Promise<NEXT_RESULT_TYPE>::Resolver,
			typename NEXT_REJECTER = typename Promise<NEXT_RESULT_TYPE>::template Rejecter<ExceptionPtr>,
			typename REJECTER_TRAITS = function_traits<REJECTER>,
			typename std::enable_if<(
				!(std::is_same<ERROR_TYPE, ExceptionPtr>::value || std::is_same<ERROR_TYPE, std::exception_ptr>::value) &&
				std::is_same<Promise<NEXT_RESULT_TYPE>, typename REJECTER_TRAITS::result_type>::value
			), std::nullptr_t>::type = nullptr>
		void callRejector(REJECTER onReject, ExceptionPtr error, NEXT_RESOLVER resolve, NEXT_REJECTER reject) {
			try {
				std::rethrow_exception(error);
			}
			catch(ERROR_TYPE exception) {
				auto nextPromise = onReject(exception);
				nextPromise.template then<void, ExceptionPtr>(resolve, reject);
			}
			catch(...) {
				reject(std::current_exception());
			}
		}
		
		template<
			typename NEXT_RESULT_TYPE,
			typename ERROR_TYPE,
			typename REJECTER = typename PromiseHelperTypes<ERROR_TYPE>::Callback,
			typename NEXT_RESOLVER = typename Promise<NEXT_RESULT_TYPE>::Resolver,
			typename NEXT_REJECTER = typename Promise<NEXT_RESULT_TYPE>::template Rejecter<ExceptionPtr>,
			typename REJECTER_TRAITS = function_traits<REJECTER>,
			typename std::enable_if<(
				!(std::is_same<ERROR_TYPE, ExceptionPtr>::value || std::is_same<ERROR_TYPE, std::exception_ptr>::value) &&
				std::is_same<NEXT_RESULT_TYPE, void>::value &&
				std::is_same<void, typename REJECTER_TRAITS::result_type>::value
			), std::nullptr_t>::type = nullptr>
		void callRejector(REJECTER onReject, ExceptionPtr error, NEXT_RESOLVER resolve, NEXT_REJECTER reject) {
			try {
				std::rethrow_exception(error);
			}
			catch(ERROR_TYPE exception) {
				onReject(exception);
				resolve();
			}
			catch(...) {
				reject(std::current_exception());
			}
		}
		
		class Continuer
		{
		public:
			Continuer()
				: resolved(false),
				rejected(false)
			{
				//
			}
			
			~Continuer()
			{
				if(rejected && !rejecter) {
					printf("unhandled promise rejection\n");
				}
			}
			
			Continuer(const Continuer&) = delete;
			
			template<
				typename _RESULT_TYPE=RESULT_TYPE,
				typename std::enable_if<!std::is_same<_RESULT_TYPE, void>::value, std::nullptr_t>::type = nullptr>
			void resolve(const _RESULT_TYPE& result)
			{
				mut.lock();
				if(resolved || rejected) {
					mut.unlock();
					throw fgl::IllegalStateException("cannot resolve or reject promise multiple times");
				}
				resolved = true;
				promise.set_value(result);
				if(resolver) {
					mut.unlock();
					resolver(promise.get_future().get());
				}
				else {
					mut.unlock();
				}
			}
			
			template<
				typename _RESULT_TYPE=RESULT_TYPE,
				typename std::enable_if<std::is_same<_RESULT_TYPE, void>::value, std::nullptr_t>::type = nullptr>
			void resolve()
			{
				mut.lock();
				if(resolved || rejected) {
					mut.unlock();
					throw fgl::IllegalStateException("cannot resolve or reject promise multiple times");
				}
				resolved = true;
				promise.set_value();
				if(resolver) {
					mut.unlock();
					promise.get_future().get();
					resolver();
				}
				else {
					mut.unlock();
				}
			}
			
			void reject(std::exception_ptr error)
			{
				mut.lock();
				if(resolved || rejected) {
					mut.unlock();
					throw fgl::IllegalStateException("cannot resolve or reject promise multiple times");
				}
				rejected = true;
				promise.set_exception(error);
				if(rejecter) {
					mut.unlock();
					try {
						promise.get_future().get();
					}
					catch(...) {
						rejecter(std::current_exception());
					}
				}
				else {
					mut.unlock();
				}
			}
			
			template<typename ERROR_TYPE>
			void reject(const ERROR_TYPE& error) 
			{
				reject(std::make_exception_ptr(error));
			}
			
			template<
				typename _RESULT_TYPE=RESULT_TYPE,
				typename std::enable_if<!std::is_same<_RESULT_TYPE, void>::value, std::nullptr_t>::type = nullptr>
			void setResolver(const Resolver& onResolve)
			{
				if(!onResolve) {
					throw fgl::IllegalArgumentException("onResolve", "cannot be null");
				}
				mut.lock();
				if(resolver) {
					mut.unlock();
					throw fgl::IllegalStateException("resolver has already been set");
				}
				resolver = onResolve;
				bool callNow = resolved;
				mut.unlock();
				if(callNow) {
					resolver(promise.get_future().get());
				}
			}
			
			template<
				typename _RESULT_TYPE=RESULT_TYPE,
				typename std::enable_if<std::is_same<_RESULT_TYPE, void>::value, std::nullptr_t>::type = nullptr>
			void setResolver(const Resolver& onResolve)
			{
				if(!onResolve) {
					throw fgl::IllegalArgumentException("onResolve", "cannot be null");
				}
				mut.lock();
				if(resolver) {
					mut.unlock();
					throw fgl::IllegalStateException("resolver has already been set");
				}
				resolver = onResolve;
				bool callNow = resolved;
				mut.unlock();
				if(callNow) {
					promise.get_future().get();
					resolver();
				}
			}
			
			void setRejecter(const std::function<void(std::exception_ptr)>& onReject)
			{
				if(!onReject) {
					throw fgl::IllegalArgumentException("onReject", "cannot be null");
				}
				mut.lock();
				if(rejecter) {
					mut.unlock();
					throw fgl::IllegalStateException("rejecter has already been set");
				}
				rejecter = onReject;
				bool callNow = rejected;
				mut.unlock();
				if(callNow) {
					try {
						promise.get_future().get();
					}
					catch(...) {
						rejecter(std::current_exception());
					}
				}
			}
			
			bool hasResolver() const {
				return resolver;
			}
			
			bool hasRejecter() const {
				return rejecter;
			}
			
		private:
			std::mutex mut;
			std::promise<RESULT_TYPE> promise;
			Resolver resolver;
			std::function<void(std::exception_ptr)> rejecter;
			bool resolved;
			bool rejected;
		};
		
		std::shared_ptr<Continuer> continuer;
	};
}
