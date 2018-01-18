#include "stdafx.h"
#include "Circle.h"
#include <math.h>
#include <iostream>
#include <iomanip>
#include <stdio.h>

//for igra - openGL
#include <gl\GL.h>
#include<gl\GLU.h>

bool ptInRect(float x, float y, float left, float top, float width, float height)
{
	return (x >= left && x <= left + width && y >= top - height && y < top);
}

Circle::Circle()
{
	nrOfPoints = 30;
	radius = 0.4;
}


float degToRad(float degAngle) {
	double pi = 3.1415926535; 
	return degAngle / 180.0 * pi;
}

float Circle::DrawSolid(float r, float g, float b) {
	glColor3f(r, g, b);

	float angularStep = 360.0 / nrOfPoints;
	float x, y = 0;
	glColor3f(0, 0, 1);
	glBegin(GL_POLYGON);
	for (int i = 0; i < nrOfPoints; i++) {
		x = xc + radius * cos(degToRad(angularStep*i));
		y = yc + radius * sin(degToRad(angularStep*i));
		glVertex2f(x, y);
	}
	glEnd();

	return r;
}

float Circle::DrawWireframe(float r, float g, float b) {
	glColor3f(r, g, b);

	float angularStep = 360.0 / nrOfPoints;
	float x, y = 0; 
	glColor3f(0, 0, 1);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < nrOfPoints; i++) {
		x = xc + radius * cos(degToRad(angularStep*i)); 
		y = yc + radius * sin(degToRad(angularStep*i));
		glVertex2f(x, y);
	}
	glEnd();

	return r;
}

void Circle::HandleLButtonDown(float xpos, float ypos)
{
	//isSelected = ptInRect(xpos, ypos, left, top, width, height);
	xc = xpos;
	yc = ypos;
}

void Circle::HandleButtonMove(float xpos, float ypos)
{
	//left = xpos;
	//top = ypos;
}

Circle::~Circle()
{
}

