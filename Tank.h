#pragma once

#include "resource.h"
#include <windows.h>	
// for IGRA - OPENGL
#include <gl\gl.h> // Header File For The OpenGL32 Library
#include <gl\glu.h> // Header File For The GLu32 Library

typedef struct TreeNode {
	float matrix[16] = {};
	int drawFunctionID;
	struct TreeNode* child;
};

typedef struct Material {
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat shininess[4];
};
#define DRAW_BASE_FUNCTION_ID  0 
#define DRAW_UPPERBASE_FUNCTION_ID  1
#define DRAW_TURRENT_FUNCTION_ID 2 

TreeNode;
Material;

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

	//functions and methods
	Tank();
	void BuildTree();
	void DrawTree(TreeNode* root);
	void DrawFunction(int id);
	void Draw();
	void DrawBase();
	void DrawUpperBase();
	void DrawTurrent();
	void MoveForward();
	void Rotate(double angle, float x, float y, float z);
	void TankRotate(double angle);
	float degToRad(float degAngle);
	void HandleKeyDown(WPARAM wParam);
	void HandleKeyUp(WPARAM wParam);
	~Tank();
};

