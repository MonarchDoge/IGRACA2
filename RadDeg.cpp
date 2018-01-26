#include "stdafx.h"
#include "RadDeg.h"

double degToRad(float degAngle) {
	float pi = 3.1415f; // You could be more precise!
	return degAngle / 180 * pi;
}

double radToDeg(float radAngle) {
	float pi = 3.1415f; // You could be more precise!
	return radAngle / pi * 180;
}