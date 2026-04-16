#include "Precompiled.h"
#include "TimeUtil.h"

using namespace GoatedEngine;
using namespace GoatedEngine::Core;


float TimeUtil::GetTime()
{
	// Get the current time in seconds since the application started
	static const auto startTime = std::chrono::high_resolution_clock::now();
	// Get the current time
	const auto currentTime = std::chrono::high_resolution_clock::now();
	// Calculate the elapsed time in milliseconds
	const auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count();
	return milliseconds / 1000.0f; // Convert milliseconds to seconds
}

float TimeUtil::GetDeltaTime()
{
	static auto lastCallTime = std::chrono::high_resolution_clock::now();
	const auto currentTime = std::chrono::high_resolution_clock::now();
	const auto miliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastCallTime).count();
	lastCallTime = currentTime;
	return miliseconds / 1000.0f; // Convert milliseconds to seconds
}
