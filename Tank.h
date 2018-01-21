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
	// Texture image  
	// Following Nodes need to available in HandleKeyDown        
	TreeNode* base;
	TreeNode* upperbase;
	TreeNode* turrent;

	Material redPlasticMaterial;
	Material yellowPlasticMaterial;
	Material greenPlasticMaterial;

	double xPos = 0;
	double yPos = 0; 
	double zPos = 0;

	double yRotation = -45;

	float TurrentHeight;
	Tank();
	void BuildTree();
	void DrawTree(TreeNode* root);
	void DrawFunction(int id);
	void Draw();
	void DrawBase();
	void DrawUpperBase();
	void DrawTurrent();
	void MoveForward(double dist);
	void Rotate(double angle, float x, float y, float z);
	float degToRad(float degAngle);
	void HandleKeyDown(WPARAM wParam);
	~Tank();
};

