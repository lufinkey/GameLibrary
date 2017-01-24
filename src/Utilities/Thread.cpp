
#include <GameLibrary/Utilities/Thread.hpp>
#include <GameLibrary/Types.hpp>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <system_error>
#include <thread>

namespace fgl
{
	std::thread::id main_thread_id = std::this_thread::get_id();
	
	struct MainThreadFunction
	{
		std::function<void()> func;
		std::condition_variable* waitCondition;
	};
	
	static ArrayList<MainThreadFunction> Thread_mainThreadFunctions;
	static std::mutex Thread_mainThreadFunctions_mutex;
	
	static bool Thread_mainthread_updating = false;
	
	typedef struct
	{
		std::thread*thread;
		std::mutex tlock;
		bool alive;
		bool selfdestruct;
	} ThreadData;
	
	void Thread_ThreadHandler(Thread*thread, ThreadData*threaddata)
	{
		threaddata->tlock.lock();
		threaddata->tlock.unlock();
		thread->run();
		threaddata->tlock.lock();
		threaddata->alive = false;
		delete threaddata->thread;
		threaddata->thread = nullptr;
		threaddata->tlock.unlock();
		if(threaddata->selfdestruct)
		{
			delete threaddata;
		}
	}
	
	Thread::Thread()
	{
		ThreadData* tdata = new ThreadData();
		tdata->alive = false;
		tdata->selfdestruct = false;
		tdata->thread = nullptr;
		threaddata = (void*)tdata;
	}
	
	Thread::~Thread()
	{
		((ThreadData*)threaddata)->tlock.lock();
		ThreadData&tdata = *((ThreadData*)threaddata);
		if(tdata.alive)
		{
			if(tdata.thread->get_id() == std::this_thread::get_id())
			{
				try
				{
					tdata.thread->detach();
				}
				catch(const std::system_error&e)
				{
					//detach error
					std::cerr << e.what() << std::endl;
				}
				tdata.selfdestruct = true;
				tdata.tlock.unlock();
			}
			else
			{
				try
				{
					tdata.thread->join();
				}
				catch(const std::system_error&e)
				{
					//join error
					std::cerr << e.what() << std::endl;
				}
				tdata.tlock.unlock();
				delete ((ThreadData*)threaddata);
			}
		}
		else
		{
			tdata.tlock.unlock();
			delete ((ThreadData*)threaddata);
		}
	}
	
	void Thread::run()
	{
		//open for implementation
	}
	
	void Thread::start()
	{
		((ThreadData*)threaddata)->tlock.lock();
		ThreadData&tdata = *((ThreadData*)threaddata);
		if(!tdata.alive)
		{
			tdata.alive = true;
			tdata.thread = new std::thread(Thread_ThreadHandler, this, &tdata);
		}
		tdata.tlock.unlock();
	}
	
	bool Thread::join()
	{
		((ThreadData*)threaddata)->tlock.lock();
		ThreadData&tdata = *((ThreadData*)threaddata);
		if(tdata.alive)
		{
			try
			{
				tdata.thread->join();
			}
			catch(const std::system_error&e)
			{
				//join error
				std::cerr << e.what() << std::endl;
				tdata.tlock.unlock();
				return false;
			}
			tdata.tlock.unlock();
			return true;
		}
		else
		{
			tdata.tlock.unlock();
			return false;
		}
	}
	
	bool Thread::isAlive()
	{
		((ThreadData*)threaddata)->tlock.lock();
		ThreadData&tdata = *((ThreadData*)threaddata);
		bool alive = tdata.alive;
		tdata.tlock.unlock();
		return alive;
	}
	
	void Thread::sleep(unsigned long long milliseconds)
	{
		if(milliseconds>0)
		{
			std::chrono::milliseconds dur((long long)milliseconds);
			std::this_thread::sleep_for(dur);
		}
	}
	
	void Thread::sleep(unsigned long long milliseconds, unsigned long nanoseconds)
	{
		unsigned long long nanoseconds_total = (milliseconds*1000000) + nanoseconds;
		if(nanoseconds_total>0)
		{
			std::chrono::nanoseconds dur(nanoseconds_total);
			std::this_thread::sleep_for(dur);
		}
	}
	
	void Thread::runOnMainThread(std::function<void()> func, bool wait)
	{
		if(wait)
		{
			if(isMainThread())
			{
				func();
			}
			else
			{
				std::mutex mtx;
				std::unique_lock<std::mutex> lck(mtx);
				std::condition_variable cv;
				
				MainThreadFunction functionData;
				functionData.func = func;
				functionData.waitCondition = &cv;
				
				Thread_mainThreadFunctions_mutex.lock();
				Thread_mainThreadFunctions.add(functionData);
				Thread_mainThreadFunctions_mutex.unlock();
				
				cv.wait(lck);
			}
		}
		else
		{
			MainThreadFunction functionData;
			functionData.func = func;
			functionData.waitCondition = nullptr;
			
			Thread_mainThreadFunctions_mutex.lock();
			Thread_mainThreadFunctions.add(functionData);
			Thread_mainThreadFunctions_mutex.unlock();
		}
	}
	
	void Thread::runOnThread(std::function<void()> func)
	{
		std::thread th(func);
		th.detach();
	}
	
	bool Thread::isMainThread()
	{
		if(std::this_thread::get_id() == main_thread_id)
		{
			return true;
		}
		return false;
	}
	
	void Thread::update()
	{
		if(isMainThread() && !Thread_mainthread_updating)
		{
			Thread_mainthread_updating = true;
			
			Thread_mainThreadFunctions_mutex.lock();
			ArrayList<MainThreadFunction> mainthread_functions = Thread_mainThreadFunctions;
			Thread_mainThreadFunctions.clear();
			Thread_mainThreadFunctions_mutex.unlock();
			
			for(size_t i=0; i < mainthread_functions.size(); i++)
			{
				MainThreadFunction& functionData = mainthread_functions[i];
				functionData.func();
				if(functionData.waitCondition != nullptr)
				{
					functionData.waitCondition->notify_all();
				}
			}
			
			Thread_mainthread_updating = false;
		}
	}
}
