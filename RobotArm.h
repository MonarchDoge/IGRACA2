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
#define DRAW_LOWERARM_FUNCTION_ID  1
#define DRAW_UPPERARM_FUNCTION_ID  2 
#define DRAW_JOINT_FUNCTION_ID  3

TreeNode;
Material;

class RobotArm {
public:
	// Texture image      // Following Nodes need to available in HandleKeyDown        
	TreeNode * base;
	TreeNode* lowerarmjoint;
	TreeNode* lowerarmnode;
	TreeNode* upperarmjoint;
	TreeNode* upperarmnode;

	Material redPlasticMaterial;
	Material yellowPlasticMaterial;
	Material greenPlasticMaterial;

	float ArmHeight;
	RobotArm();
	void BuildTree();
	void DrawTree(TreeNode* root);
	void DrawFunction(int id);
	void Draw();
	void DrawBase();
	void DrawLowerArm();
	void DrawUpperArm();
	void DrawJoint();
	void HandleKeyDown(WPARAM wParam);
};

