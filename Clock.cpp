#include "stdafx.h"
#include "Clock.h"
#include <math.h>
#include <stdio.h>

//for igra - openGL
#include <gl\GL.h>
#include<gl\GLU.h>


Clock::Clock()
{
	startTimeInCounts = 0;
	previousTimeInCounts = 0;
	countsPerSecond;

	Start();
	TimePassedSinceStartTime();
	TimePassedSincePreviousTime();
}

Clock::~Clock()
{
}

void Clock::Start() {
	__int64 currentTimeInCounts;
	// countsPerSecond depends on your PC
	if (!QueryPerformanceFrequency(
		(LARGE_INTEGER *)&countsPerSecond)) {
		MessageBox(NULL, L"QueryPerformanceFrequency Failed.", L"ERROR", MB_OK | MB_ICONINFORMATION);
		return;
	}
	QueryPerformanceCounter((LARGE_INTEGER *)&currentTimeInCounts);
	startTimeInCounts = currentTimeInCounts;
	previousTimeInCounts = currentTimeInCounts;
}


// Return absolute time in seconds -since start
double Clock::TimePassedSinceStartTime() {
	__int64 currentTimeInCounts;
	double timePassedSeconds;

	// Calculate time passed in seconds since timer was started
	QueryPerformanceCounter((LARGE_INTEGER *)&currentTimeInCounts);
	timePassedSeconds = (currentTimeInCounts - startTimeInCounts) / (double)countsPerSecond;
	return timePassedSeconds;
}

// Return relative time in seconds - since last measurement
double Clock::TimePassedSincePreviousTime() {
	__int64 currentTimeInCounts, timePassedSincepreviousTimeInCounts;

	// Calculate time passed in seconds since last call to
	// GetTimePassedSinceLastTime
	QueryPerformanceCounter((LARGE_INTEGER *)&currentTimeInCounts);
	timePassedSincepreviousTimeInCounts = currentTimeInCounts - previousTimeInCounts;
	double timePassedSinceLastTimeInSeconds = (currentTimeInCounts - previousTimeInCounts) / (double)countsPerSecond;

	previousTimeInCounts = currentTimeInCounts;
	return timePassedSinceLastTimeInSeconds;
}