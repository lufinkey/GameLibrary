
#pragma once

#include "String.hpp"
#include "Math.hpp"
#include <GameLibrary/IO/Console.hpp>

namespace fgl
{
	#define START_PERFORMANCE_TIMING(timerName) \
		auto timerName##_timer_start = std::chrono::high_resolution_clock::now();
		
	#define FINISH_PERFORMANCE_TIMING(timerName) \
		auto timerName##_timer_finish = std::chrono::high_resolution_clock::now();
		
	#define PRINT_PERFORMANCE_MS(timerName, sectionName) { \
		auto timerName##_duration_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(timerName##_timer_finish - timerName##_timer_start).count(); \
		double timerName##_duration_ms = (double)timerName##_duration_ns / 1000000.0; \
		Console::writeLine((String)sectionName + " took " + timerName##_duration_ms + " ms"); \
	}
		
	#define PRINT_PERFORMANCE_MS_ROUNDED(timerName, sectionName) { \
		auto timerName##_duration_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(timerName##_timer_finish - timerName##_timer_start).count(); \
		double timerName##_duration_ms = Math::round((double)timerName##_duration_ns / 1000000.0); \
		Console::writeLine((String)sectionName + " took " + timerName##_duration_ms + " ms"); \
	}
	
	#define LOG_PERFORMANCE(timerName, sectionName, ...) \
		START_PERFORMANCE_TIMING(timerName) \
		__VA_ARGS__ \
		FINISH_PERFORMANCE_TIMING(timerName) \
		PRINT_PERFORMANCE_MS(timerName, sectionName)
	
	#define START_PERFORMANCE_LOG(timerName) \
		START_PERFORMANCE_TIMING(timerName)
		
	#define FINISH_PERFORMANCE_LOG(timerName, sectionName) \
		FINISH_PERFORMANCE_TIMING(timerName) \
		PRINT_PERFORMANCE_MS(timerName, sectionName)
		
	#define FINISH_PERFORMANCE_LOG_ROUNDED(timerName, sectionName) \
		FINISH_PERFORMANCE_TIMING(timerName) \
		PRINT_PERFORMANCE_MS_ROUNDED(timerName, sectionName)
}
