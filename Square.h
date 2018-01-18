#pragma once
#
class Square
{
public:

	float left;
	float top;
	float width;
	float height;
	bool isSelected;
	float DrawWireframe(float r, float g, float b);
	float DrawSolid(float r, float g, float b);
	void HandleLButtonDown(float xpos,float ypos);
	void HandleButtonMove(float xpos, float ypos);


	Square();
	~Square();
};

