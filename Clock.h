#pragma once

// for IGRA - OPENGL
#include <gl\gl.h> // Header File For The OpenGL32 Library
#include <gl\glu.h> // Header File For The GLu32 Library
class Clock {
public:
	__int64 startTimeInCounts;
	__int64 previousTimeInCounts;
	__int64 countsPerSecond;
	Clock();
	void Start();
	double TimePassedSinceStartTime();
	double TimePassedSincePreviousTime();
	~Clock();
};