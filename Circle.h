#pragma once
class Circle
{
public:

	float radius;
	float xc;
	float yc;
	float nrOfPoints;
	bool isSelected;
	float DrawSolid(float r, float g, float b);
	float DrawWireframe(float r, float g, float b);
	void HandleLButtonDown(float xpos, float ypos);
	void HandleButtonMove(float xpos, float ypos);

	Circle();
	~Circle();
};

