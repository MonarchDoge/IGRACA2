#include "stdafx.h"
#include "RadDeg.h"

float pi = 3.1415f; // You could be more precise!

double degToRad(float degAngle) {
	return degAngle / 180 * pi;
}

double radToDeg(float radAngle) {
	return radAngle / pi * 180;
}