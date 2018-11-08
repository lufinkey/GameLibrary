
#pragma once

#include <GameLibrary/Exception/Exception.hpp>
#include <GameLibrary/Exception/ExceptionPtr.hpp>
#include "ArrayList.hpp"
#include "Tools.hpp"
#include <functional>
#include <future>
#include <list>
#include <memory>
#include <mutex>

namespace fgl
{
	template<typename ARG, typename RETURN>
	struct PromiseHelperTypes
	{
		using Func = std::function<RETURN(ARG)>;
	};
	
	template<typename RETURN>
	struct PromiseHelperTypes<void, RETURN>
	{
		using Func = std::function<RETURN()>;
	};
	
	
	
	template<typename RESULT>
	class Promise
	{
	public:
		typedef RESULT Result;
		
		using Resolver = typename PromiseHelperTypes<RESULT,void>::Func;
		using Rejecter = std::function<void(ExceptionPtr)>;
		template<typename RETURN>
		using Then = typename PromiseHelperTypes<RESULT,RETURN>::Func;
		template<typename ERROR, typename RETURN>
		using Catch = std::function<RETURN(ERROR)>;
		
		
		explicit Promise(const std::function<void(Resolver, Rejecter)>& executor)
			: continuer(std::make_shared<Continuer>()) {
			ASSERT(executor != nullptr, "promise executor cannot be null");
			auto _continuer = continuer;
			if constexpr(std::is_same<RESULT,void>::value) {
				executor([=]() {
					_continuer->resolve();
				}, [=](ExceptionPtr error) {
					_continuer->reject(error);
				});
			}
			else {
				executor([=](RESULT result) {
					_continuer->resolve(result);
				}, [=](ExceptionPtr error) {
					_continuer->reject(error);
				});
			}
		}
		
		
		Promise<void> then(Then<void> onresolve, Catch<std::exception_ptr,void> onreject) {
			return continuer->handle(onresolve, onreject);
		}
		
		
		template<typename NEXT_RESULT>
		Promise<NEXT_RESULT> then(Then<Promise<NEXT_RESULT>> onresolve, Catch<std::exception_ptr,Promise<NEXT_RESULT>> onreject) {
			return continuer->template handle<NEXT_RESULT>(onresolve, onreject);
		}
		
		
		Promise<void> then(Then<void> onresolve) {
			return continuer->handle(onresolve, nullptr);
		}
		
		
		template<typename NEXT_RESULT>
		Promise<NEXT_RESULT> then(Then<Promise<NEXT_RESULT>> onresolve) {
			return continuer->template handle<NEXT_RESULT>(onresolve, nullptr);
		}
		
		
		template<typename ERROR>
		Promise<RESULT> fail(Catch<ERROR,void> onreject) {
			Then<Promise<RESULT>> resultForwarder = nullptr;
			if constexpr(std::is_same<RESULT,void>::value) {
				resultForwarder = [=]() -> Promise<RESULT> {
					return Promise<RESULT>::resolve();
				};
			}
			else {
				resultForwarder = [=](RESULT result) -> Promise<RESULT> {
					return Promise<RESULT>::resolve(result);
				};
			}
			return continuer->template handle<RESULT>(resultForwarder, [=](std::exception_ptr exceptionPtr) -> Promise<RESULT> {
				if constexpr(std::is_same<std::exception_ptr,ERROR>::value) {
					onreject(exceptionPtr);
					return Promise<RESULT>::nothing();
				}
				else {
					try {
						std::rethrow_exception(exceptionPtr);
					}
					catch(const ERROR& error) {
						onreject(error);
						return Promise<RESULT>::nothing();
					}
					catch(...) {
						return Promise<RESULT>::reject(exceptionPtr);
					}
				}
			});
		}
		
		
		template<typename ERROR>
		Promise<RESULT> fail(Catch<ERROR,Promise<RESULT>> onreject) {
			Then<Promise<RESULT>> resultForwarder = nullptr;
			if constexpr(std::is_same<RESULT,void>::value) {
				resultForwarder = [=]() -> Promise<RESULT> {
					return Promise<RESULT>::resolve();
				};
			}
			else {
				resultForwarder = [=](RESULT result) -> Promise<RESULT> {
					return Promise<RESULT>::resolve(result);
				};
			}
			return continuer->template handle<RESULT>(resultForwarder, [=](std::exception_ptr exceptionPtr) {
				if constexpr(std::is_same<std::exception_ptr,ERROR>::value) {
					return onreject(exceptionPtr);
				}
				else {
					try {
						std::rethrow_exception(exceptionPtr);
					}
					catch(const ERROR& error) {
						return onreject(error);
					}
					catch(...) {
						return Promise<RESULT>::reject(exceptionPtr);
					}
				}
			});
		}
		
		
		Promise<void> finally(std::function<void()> onfinish) {
			Then<Promise<void>> thenFinally = nullptr;
			if constexpr(std::is_same<RESULT,void>::value) {
				thenFinally = [=]() -> Promise<void> {
					onfinish();
					return Promise<void>::resolve();
				};
			}
			else {
				thenFinally = [=](RESULT result) -> Promise<void> {
					onfinish();
					return Promise<void>::resolve();
				};
			}
			return continuer->template handle<void>(thenFinally, [=](std::exception_ptr exceptionPtr) {
				onfinish();
				return Promise<void>::resolve();
			});
		}
		
		
		template<typename NEXT_RESULT>
		Promise<NEXT_RESULT> finally(std::function<Promise<NEXT_RESULT>()> onfinish) {
			Then<Promise<NEXT_RESULT>> thenFinally = nullptr;
			if constexpr(std::is_same<RESULT,void>::value) {
				thenFinally = [=]() -> Promise<NEXT_RESULT> {
					return onfinish();
				};
			}
			else {
				thenFinally = [=](RESULT result) -> Promise<NEXT_RESULT> {
					return onfinish();
				};
			}
			return continuer->template handle<void>(thenFinally, [=](std::exception_ptr exceptionPtr) {
				return onfinish();
			});
		}
		
		
		template<typename _RESULT=RESULT,
			typename std::enable_if<!std::is_same<_RESULT,void>::value, std::nullptr_t>::type = nullptr>
		static Promise<_RESULT> resolve(_RESULT result) {
			return Promise<_RESULT>([&](auto resolve, auto reject) {
				resolve(result);
			});
		}
		
		
		template<typename _RESULT=RESULT,
			typename std::enable_if<std::is_same<_RESULT,void>::value, std::nullptr_t>::type = nullptr>
		static Promise<_RESULT> resolve() {
			return Promise<_RESULT>([&](auto resolve, auto reject) {
				resolve();
			});
		}
		
		
		static Promise<RESULT> reject(ExceptionPtr error) {
			return Promise<RESULT>([&](auto resolve, auto reject) {
				reject(error);
			});
		}
		
		
		static Promise<RESULT> nothing() {
			return Promise<RESULT>([](auto resolve, auto reject) {
				// do nothing
			});
		}
		
		
		template<typename _RESULT=RESULT,
			typename std::enable_if<!std::is_same<_RESULT,void>::value, std::nullptr_t>::type = nullptr>
		static Promise<ArrayList<_RESULT>> all(ArrayList<Promise<_RESULT>> promises) {
			return Promise<ArrayList<_RESULT>>([&](auto resolve, auto reject) {
				size_t promiseCount = promises.size();
				if(promiseCount == 0) {
					resolve({});
					return;
				}
				
				struct SharedInfo {
					std::mutex mutex;
					bool rejected = false;
					ArrayList<RESULT> results;
					ArrayList<size_t> addIndexes;
				};
				
				auto sharedInfoPtr = std::make_shared<SharedInfo>();
				sharedInfoPtr->results.reserve(promiseCount);
				sharedInfoPtr->addIndexes.resize(promiseCount, 0);
				
				auto resolveIndex = [=](size_t index, _RESULT result) {
					auto& sharedInfo = *sharedInfoPtr;
					std::unique_lock<std::mutex> lock(sharedInfo.mutex);
					if(sharedInfo.rejected) {
						return;
					}
					size_t addIndex = sharedInfo.addIndexes[index];
					for(size_t i=(index+1); i<promiseCount; i++) {
						sharedInfo.addIndexes[i] += 1;
					}
					sharedInfo.results.add(addIndex, result);
					bool finished = (sharedInfo.results.size() == promiseCount);
					lock.unlock();
					if(finished) {
						resolve(std::move(sharedInfo.results));
					}
				};
				
				auto rejectAll = [=](std::exception_ptr error) {
					auto& sharedInfo = *sharedInfoPtr;
					std::unique_lock<std::mutex> lock(sharedInfo.mutex);
					if(sharedInfo.rejected) {
						return;
					}
					sharedInfo.rejected = true;
					lock.unlock();
					reject(error);
				};
				
				for(size_t i=0; i<promiseCount; i++) {
					auto& promise = promises[i];
					promise.then([=](_RESULT result) {
						resolveIndex(i, result);
					}, [=](std::exception_ptr error) {
						rejectAll(error);
					});
				}
			});
		}
		
		
		template<typename _RESULT=RESULT,
			typename std::enable_if<std::is_same<_RESULT,void>::value, std::nullptr_t>::type = nullptr>
		static Promise<void> all(ArrayList<Promise<_RESULT>> promises) {
			return Promise<void>([&](auto resolve, auto reject) {
				size_t promiseCount = promises.size();
				if(promiseCount == 0) {
					resolve();
					return;
				}
				
				struct SharedInfo {
					std::mutex mutex;
					bool rejected = false;
					size_t counter = 0;
				};
				
				auto sharedInfoPtr = std::make_shared<SharedInfo>();
				
				auto resolveIndex = [=](size_t index) {
					auto& sharedInfo = *sharedInfoPtr;
					std::unique_lock<std::mutex> lock(sharedInfo.mutex);
					if(sharedInfo.rejected) {
						return;
					}
					sharedInfo.counter += 1;
					bool finished = (sharedInfo.counter == promiseCount);
					lock.unlock();
					if(finished) {
						resolve();
					}
				};
				
				auto rejectAll = [=](std::exception_ptr error) {
					auto& sharedInfo = *sharedInfoPtr;
					std::unique_lock<std::mutex> lock(sharedInfo.mutex);
					if(sharedInfo.rejected) {
						return;
					}
					sharedInfo.rejected = true;
					lock.unlock();
					reject(error);
				};
				
				for(size_t i=0; i<promiseCount; i++) {
					auto& promise = promises[i];
					promise.then([=]() {
						resolveIndex(i);
					}, [=](std::exception_ptr error) {
						rejectAll(error);
					});
				}
			});
		}
		
		
		template<typename _RESULT=RESULT,
			typename std::enable_if<!std::is_same<_RESULT,void>::value, std::nullptr_t>::type = nullptr>
		static Promise<_RESULT> race(ArrayList<Promise<_RESULT>> promises) {
			return Promise<_RESULT>([&](auto resolve, auto reject) {
				size_t promiseCount = promises.size();
				
				struct SharedInfo {
					std::mutex mutex;
					bool finished;
				};
				
				auto sharedInfoPtr = std::make_shared<SharedInfo>();
				
				auto resolveIndex = [=](size_t index, auto result) {
					auto& sharedInfo = *sharedInfoPtr;
					std::unique_lock<std::mutex> lock(sharedInfo.mutex);
					if(sharedInfo.finished) {
						return;
					}
					sharedInfo.finished = true;
					lock.unlock();
					resolve(result);
				};
				
				auto rejectAll = [=](std::exception_ptr error) {
					auto& sharedInfo = *sharedInfoPtr;
					std::unique_lock<std::mutex> lock(sharedInfo.mutex);
					if(sharedInfo.finished) {
						return;
					}
					sharedInfo.finished = true;
					lock.unlock();
					reject(error);
				};
				
				for(size_t i=0; i<promiseCount; i++) {
					auto& promise = promises[i];
					promise.then([=](auto result) {
						resolveIndex(i, result);
					}, [=](std::exception_ptr error) {
						rejectAll(error);
					});
				}
			});
		}
		
		
		template<typename _RESULT=RESULT,
			typename std::enable_if<std::is_same<_RESULT,void>::value, std::nullptr_t>::type = nullptr>
		static Promise<void> race(ArrayList<Promise<_RESULT>> promises) {
			return Promise<void>([&](auto resolve, auto reject) {
				size_t promiseCount = promises.size();
				
				struct SharedInfo {
					std::mutex mutex;
					bool finished;
				};
				
				auto sharedInfoPtr = std::make_shared<SharedInfo>();
				
				auto resolveIndex = [=](size_t index) {
					auto& sharedInfo = *sharedInfoPtr;
					std::unique_lock<std::mutex> lock(sharedInfo.mutex);
					if(sharedInfo.finished) {
						return;
					}
					sharedInfo.finished = true;
					lock.unlock();
					resolve();
				};
				
				auto rejectAll = [=](std::exception_ptr error) {
					auto& sharedInfo = *sharedInfoPtr;
					std::unique_lock<std::mutex> lock(sharedInfo.mutex);
					if(sharedInfo.finished) {
						return;
					}
					sharedInfo.finished = true;
					lock.unlock();
					reject(error);
				};
				
				for(size_t i=0; i<promiseCount; i++) {
					auto& promise = promises[i];
					promise.then([=]() {
						resolveIndex(i);
					}, [=](std::exception_ptr error) {
						rejectAll(error);
					});
				}
			});
		}
		
		
	private:
		enum class State
		{
			EXECUTING = 0,
			RESOLVED = 1,
			REJECTED = 2
		};
		
		
		class Continuer
		{
		public:
			Continuer()
				: future(promise.get_future().share()),
				state(State::EXECUTING) {
				//
			}
			
			
			template<typename _RESULT=RESULT,
				typename std::enable_if<!std::is_same<_RESULT,void>::value, std::nullptr_t>::type = nullptr>
			void resolve(_RESULT result) {
				std::unique_lock<std::mutex> lock(sync);
				ASSERT(state == State::EXECUTING, "Cannot resolve or reject a promise multiple times");
				promise.set_value(result);
				state = State::RESOLVED;
				auto callbacks = std::list<Resolver>();
				callbacks.swap(resolvers);
				resolvers.clear();
				rejecters.clear();
				lock.unlock();
				for(auto& callback : callbacks) {
					callback(result);
				}
			}
			
			
			template<typename _RESULT=RESULT,
				typename std::enable_if<std::is_same<_RESULT,void>::value, std::nullptr_t>::type = nullptr>
			void resolve() {
				std::unique_lock<std::mutex> lock(sync);
				ASSERT(state == State::EXECUTING, "Cannot resolve or reject a promise multiple times");
				promise.set_value();
				state = State::RESOLVED;
				auto callbacks = std::list<Resolver>();
				callbacks.swap(resolvers);
				resolvers.clear();
				rejecters.clear();
				lock.unlock();
				for(auto& callback : callbacks) {
					callback();
				}
			}
			
			
			void reject(ExceptionPtr error) {
				std::unique_lock<std::mutex> lock(sync);
				ASSERT(state == State::EXECUTING, "Cannot resolve or reject a promise multiple times");
				promise.set_exception(error.ptr());
				state = State::REJECTED;
				auto callbacks = std::list<Rejecter>();
				callbacks.swap(rejecters);
				resolvers.clear();
				rejecters.clear();
				lock.unlock();
				for(auto& callback : callbacks) {
					callback(error);
				}
			}
			
			
			Promise<void> handle(Then<void> onresolve, Catch<std::exception_ptr,void> onreject) {
				return Promise<void>([&](auto resolve, auto reject) {
					std::unique_lock<std::mutex> lock(sync);
					switch(state) {
						case State::EXECUTING: {
							if constexpr(std::is_same<RESULT,void>::value) {
								resolvers.push_back([=]() {
									// TODO wrap this in a DispatchQueue call
									if(onresolve) {
										onresolve();
									}
									resolve();
								});
							}
							else {
								resolvers.push_back([=](auto result) {
									// TODO wrap this in a DispatchQueue call
									if(onresolve) {
										onresolve(result);
									}
									resolve();
								});
							}
							rejecters.push_back([=](auto exceptionPtr) {
								// TODO wrap this in a DispatchQueue call
								if(onreject) {
									onreject(exceptionPtr.ptr());
								}
								else {
									reject(exceptionPtr);
								}
							});
						}
						break;
						
						case State::RESOLVED: {
							lock.unlock();
							// TODO wrap this in a DispatchQueue call
							if(onresolve) {
								if constexpr(std::is_same<RESULT,void>::value) {
									future.get();
									onresolve();
								}
								else {
									onresolve(future.get());
								}
							}
							resolve();
						}
						break;
						
						case State::REJECTED: {
							lock.unlock();
							// TODO wrap this in a DispatchQueue call
							if(onreject) {
								try {
									future.get();
								}
								catch(...) {
									onreject(std::current_exception());
								}
							}
							else {
								try {
									future.get();
								}
								catch(...) {
									reject(std::current_exception());
								}
							}
						}
						break;
					}
				});
			}
			
			
			template<typename NEXT_RESULT>
			Promise<NEXT_RESULT> handle(Then<Promise<NEXT_RESULT>> onresolve, Catch<std::exception_ptr,Promise<NEXT_RESULT>> onreject) {
				ASSERT(onresolve != nullptr, "onresolve cannot be null");
				return Promise<NEXT_RESULT>([&](auto resolve, auto reject) {
					std::unique_lock<std::mutex> lock(sync);
					switch(state) {
						case State::EXECUTING: {
							if constexpr(std::is_same<RESULT,void>::value) {
								resolvers.push_back([=]() {
									// TODO wrap this in a DispatchQueue call
									auto nextPromise = onresolve();
									if constexpr(std::is_same<NEXT_RESULT,void>::value) {
										nextPromise.then([=]() {
											resolve();
										},[=](std::exception_ptr error) {
											reject(error);
										});
									}
									else {
										nextPromise.then([=](auto nextResult) {
											resolve(nextResult);
										},[=](std::exception_ptr error) {
											reject(error);
										});
									}
								});
							}
							else {
								resolvers.push_back([=](auto result) {
									// TODO wrap this in a DispatchQueue call
									auto nextPromise = onresolve(result);
									if constexpr(std::is_same<NEXT_RESULT,void>::value) {
										nextPromise.then([=]() {
											resolve();
										},[=](std::exception_ptr error) {
											reject(error);
										});
									}
									else {
										nextPromise.then([=](auto nextResult) {
											resolve(nextResult);
										},[=](std::exception_ptr error) {
											reject(error);
										});
									}
								});
							}
							rejecters.push_back([=](auto exceptionPtr) {
								// TODO wrap this in a DispatchQueue call
								if(onreject) {
									auto nextPromise = onreject(exceptionPtr.ptr());
									if constexpr(std::is_same<NEXT_RESULT,void>::value) {
										nextPromise.then([=]() {
											resolve();
										}, [=](std::exception_ptr error) {
											reject(error);
										});
									}
									else {
										nextPromise.then([=](auto nextResult) {
											resolve(nextResult);
										}, [=](std::exception_ptr error) {
											reject(error);
										});
									}
								}
								else {
									reject(exceptionPtr);
								}
							});
						}
						break;
						
						case State::RESOLVED: {
							lock.unlock();
							// TODO wrap this in a DispatchQueue call
							if constexpr(std::is_same<RESULT,void>::value) {
								future.get();
								auto nextPromise = onresolve();
								if constexpr(std::is_same<NEXT_RESULT,void>::value) {
									nextPromise.then([=]() {
										resolve();
									}, [=](std::exception_ptr error) {
										reject(error);
									});
								}
								else {
									nextPromise.then([=](auto nextResult) {
										resolve(nextResult);
									}, [=](std::exception_ptr error) {
										reject(error);
									});
								}
							}
							else {
								auto nextPromise = onresolve(future.get());
								if constexpr(std::is_same<NEXT_RESULT,void>::value) {
									nextPromise.then([=]() {
										resolve();
									}, [=](std::exception_ptr error) {
										reject(error);
									});
								}
								else {
									nextPromise.then([=](auto nextResult) {
										resolve(nextResult);
									}, [=](std::exception_ptr error) {
										reject(error);
									});
								}
							}
						}
						break;
						
						case State::REJECTED: {
							lock.unlock();
							// TODO wrap this in a DispatchQueue call
							if(onreject) {
								try {
									future.get();
								}
								catch(...) {
									auto nextPromise = onreject(std::current_exception());
									if constexpr(std::is_same<NEXT_RESULT,void>::value) {
										nextPromise.then([=]() {
											resolve();
										}, [=](std::exception_ptr error) {
											reject(error);
										});
									}
									else {
										nextPromise.then([=](auto nextResult) {
											resolve(nextResult);
										}, [=](std::exception_ptr error) {
											reject(error);
										});
									}
								}
							}
							else {
								try {
									future.get();
								}
								catch(...) {
									reject(std::current_exception());
								}
							}
						}
						break;
					}
				});
			}
			
			
		private:
			std::promise<Result> promise;
			std::shared_future<Result> future;
			std::list<Resolver> resolvers;
			std::list<Rejecter> rejecters;
			std::mutex sync;
			State state;
		};
		
		std::shared_ptr<Continuer> continuer;
	};
	
	
	
	
	template<typename RESULT>
	RESULT await(Promise<RESULT> promise) {
		std::condition_variable cv;
		std::mutex mutex;
		std::unique_lock<std::mutex> lock(mutex);
		
		bool resolved = false;
		bool rejected = false;
		std::unique_ptr<RESULT> result_ptr = nullptr;
		std::exception_ptr error_ptr = nullptr;
		
		promise.then([&](RESULT result) {
			result_ptr = std::make_unique<RESULT>(result);
			resolved = true;
			cv.notify_one();
		}, [&](std::exception_ptr error) {
			error_ptr = error;
			rejected = true;
			cv.notify_one();
		});
		
		cv.wait(lock, [&]() {
			return rejected || resolved;
		});
		
		if(rejected) {
			std::rethrow_exception(error_ptr);
		}
		return std::move(*result_ptr.get());
	}
	
	
	
	
	template<typename RESULT>
	Promise<RESULT> async(std::function<RESULT()> func) {
		return Promise<RESULT>([=](auto resolve, auto reject) {
			std::thread([=](){
				try {
					if constexpr(std::is_same<RESULT,void>::value) {
						func();
						resolve();
					}
					else {
						auto result = func();
						resolve(result);
					}
				}
				catch(...) {
					reject(std::current_exception());
				}
			});
		});
	}
}
