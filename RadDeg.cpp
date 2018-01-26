#include "stdafx.h"
#include "RadDeg.h"

float pi = 3.1415; // You could be more precise!

double degToRad(double degAngle) {
	return degAngle / 180 * pi;
}

double radToDeg(double radAngle) {
	return radAngle / pi * 180;
}