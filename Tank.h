#pragma once

#include "resource.h"
#include <windows.h>	
// for IGRA - OPENGL
#include <gl\gl.h> // Header File For The OpenGL32 Library
#include <gl\glu.h> // Header File For The GLu32 Library
#include "Clock.h"

struct TreeNode {
	float matrix[16] = {};
	int drawFunctionID;
	struct TreeNode* child;
};

struct Material{
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat shininess[4];
};

#define DRAW_BASE_FUNCTION_ID  0 
#define DRAW_UPPERBASE_FUNCTION_ID  1
#define DRAW_TURRENT_FUNCTION_ID 2 

class Tank {
public:
	//Drawing the tank
	TreeNode* base;
	TreeNode* upperbase;
	TreeNode* turrent;

	//Materials
	Material redPlasticMaterial;
	Material yellowPlasticMaterial;
	Material greenPlasticMaterial;

	//Movement
	double xPos = 0;
	double yPos = 0; 
	double zPos = 0;
	double Rotationlimit = -45;
	double yRotation = 0;

	//Misc
	float TurrentHeight;

	double dist = 1;
	double rot = 1;

	//for timer/clock
	__int64 startTimeInCounts = 0;
	__int64 lastTimeInCounts = 0;
	__int64 countsPerSecond;

	//for the tank accleration speeds
	bool WisPressed = false;
	bool SisPressed = false;
	Clock* clock;

	//functions and methods
	Tank();
	void BuildTree();
	void DrawTree(TreeNode* root);
	void DrawFunction(int id);
	void Draw();
	void DrawBase();
	void DrawUpperBase();
	void DrawTurrent();
	void TurrentRotate(float angle);
	void Update();
	float degToRad(float degAngle);
	void HandleKeyDown(WPARAM wParam);
	void HandleKeyUp(WPARAM wParam);
	~Tank();
};

