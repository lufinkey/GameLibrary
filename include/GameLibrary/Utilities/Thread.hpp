
#pragma once

#include "ArrayList.hpp"
#include <functional>

namespace fgl
{
	class Thread
	{
	private:
		void* threaddata;

	public:
		Thread();
		virtual ~Thread();

		/*! Event called when thread runs (override this with any processing you want to do in this thread) */
		virtual void run();

		/*! Starts the thread. */
		void start();
		/*! Call this function to join the thread with the calling thread and wait until the thread finishes. Thread::join() can only be called from one thread.
			\returns true after a successful join, or false if it failed to join */
		bool join();

		/*! Returns true if the Thread is currently running */
		bool isAlive();

		/*! Causes the calling thread to wait for a specified number of milliseconds */
		static void sleep(unsigned long long milliseconds);
		/*! Causes the calling thread to wait for a specified number of milliseconds + nanoseconds */
		static void sleep(unsigned long long milliseconds, unsigned long nanoseconds);

		/*! Runs a given function on the main thread
			\param func the function to run on the main thread
			\param wait true to wait until the function finishes, or false to continue */
		static void runOnMainThread(const std::function<void()>& func, bool wait=false);
		/*! Runs a given function in a new thread.
			\param func the function to run */
		static void runOnThread(const std::function<void()>& func);
		/*! Tells if the current thread is the main thread
			\returns true if the current thread is the main thread */
		static bool isMainThread();

		/*! Calls all main thread callbacks (if calling thread is main thread) */
		static void update();
	};
}
