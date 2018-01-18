#include "stdafx.h"
#include "Shapes.h"
#include <math.h>
#include <stdio.h>

//for igra - openGL
#include <gl\GL.h>
#include<gl\GLU.h>



Shape::Shape(float mousePosX, float mousePosY, color Color, mode Mode, shape Shape)
{
	radius = 0.1;
	x = mousePosX;
	y = mousePosY;
	angularStep = 360.0 / 24;
	xc, yc = 0; //circle vertices

	shapeColor = Color;
	shapeMode = Mode;
	shapeShape = Shape;
}


Shape::~Shape()
{
}

void Shape::DrawShape() {
	if (shapeColor == color::RED_COLOR) {
		glColor3f(1, 0, 0);
	}
	if (shapeColor == color::GREEN_COLOR) {
		glColor3f(0, 1, 0);
	}
	if (shapeColor == color::BLUE_COLOR) {
		glColor3f(0, 0, 1);
	}
	if (shapeMode == mode::DRAW_WIREFRAME_MODE) {
		glBegin(GL_LINE_LOOP);
	}
	if (shapeMode == mode::DRAW_SOLID_MODE) {
		glBegin(GL_POLYGON);
	}

	if (shapeShape == shape::SQUARE_SHAPE) {
		glVertex2f(x + 0.1, y + 0.1);//TR
		glVertex2f(x - 0.1, y + 0.1);//TL
		glVertex2f(x - 0.1, y - 0.1);//BL
		glVertex2f(x + 0.1, y - 0.1);//BR
		glEnd();
	}
	if (shapeShape == shape::TRIANGLE_SHAPE) {
		glVertex2f(x, y + 0.1);//LT
		glVertex2f(x + 0.1, y - 0.1);//RB
		glVertex2f(x - 0.1, y - 0.1);//LB
		glEnd();
	}
	if (shapeShape == shape::CIRCLE_SHAPE) {
		for (int i = 0; i < 24; i++) {
			//We need to convert from degree to radians!
			xc = x + radius * cos(degToRad(angularStep*i)); //angular step is used to divide the circle
			yc = y + radius * sin(degToRad(angularStep*i));
			glVertex2f(xc, yc);
		}
		glEnd();
	}
}

float Shape::degToRad(float degAngle) {
	double pi = 3.1415926535; // You could be more precise!
	return degAngle / 180.0 * pi;
}