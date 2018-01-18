#pragma once

// for IGRA - OPENGL
#include <gl\gl.h> // Header File For The OpenGL32 Library
#include <gl\glu.h> // Header File For The GLu32 Library
class Shape
{
public:

	float xPosCircle;
	float yPosCircle;
	float angularStep;
	float radius;
	float x, y, xc, yc;

	bool Square;
	bool Circle;
	bool Triangle;
	bool Red;
	bool Green;
	bool Blue;
	bool Wireframe;
	bool Solid;

	enum shape
	{
		SQUARE_SHAPE, TRIANGLE_SHAPE, CIRCLE_SHAPE
	};
	enum color
	{
		RED_COLOR, GREEN_COLOR, BLUE_COLOR
	};

	enum mode {
		DRAW_WIREFRAME_MODE, DRAW_SOLID_MODE
	};

	color shapeColor;
	mode shapeMode;
	shape shapeShape;

	Shape(float mousePosX, float mousePosY, color Color, mode Mode, shape Shape);
	~Shape();
	void DrawShape();
	float degToRad(float degAngle);
};

