#include "stdafx.h"
#include "Square.h"
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

Square::Square()
{
	left = -0.2;
	top = 0.2;
	width = 0.4;
	height = 0.4;
	isSelected = false;
}

float Square::DrawSolid(float r, float g, float b) {
	glColor3f(r, g, b);

	//draw a square with a line loop
	glBegin(GL_POLYGON);
	glVertex2f(left, top);//LT
	glVertex2f(left + width, top);//LT
	glVertex2f(left + width, top - height);//LT
	glVertex2f(left, top - height);//LT
	glEnd();

	return r;
}

float Square::DrawWireframe(float r, float g, float b) {
	glColor3f(r, g, b);

	//draw a square with a line loop
	glBegin(GL_LINE_LOOP);
	glVertex2f(left, top);//LT
	glVertex2f(left + width, top);//LT
	glVertex2f(left + width, top - height);//LT
	glVertex2f(left, top - height);//LT
	glEnd();

	return r;
}

void Square::HandleLButtonDown(float xpos, float ypos)
{
	isSelected = ptInRect(xpos, ypos, left, top, width, height);
}

void Square::HandleButtonMove(float xpos, float ypos)
{
	left = xpos;
	top = ypos;
}

Square::~Square()
{
}
